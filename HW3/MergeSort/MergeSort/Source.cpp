/*
*				In His Exalted Name
*	Title:	MergeSort Sequential Code
*	Author: Ahmad Siavashi, Email: siavashi@aut.ac.ir
*	Date:	24/11/2015
*/

// Let it be.
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

#define MAX(A, B) ((A)>(B))?(A):(B)
#define MIN(A, B) ((A)<(B))?(A):(B)

void printArray(int *array, int size);
void fillArray(int *array, int size);
void merge(int *a, int n, int m);
void mergeSort(int *a, int n, int threads);
void check_omp() {
	// written by Ahmad Siavashi (siavashi@aut.ac.ir) April 26th, 2018
#ifdef _DEBUG
	printf("[!] Configuration: Debug.\n");
	printf("[#] Change to Release.\n");
#pragma message ("Change configuration to Release for a fast execution.")
#else
	printf("[-] Configuration: Release.\n");
#endif // _DEBUG
#ifdef _M_X64
	printf("[-] Platform: x64\n");
#elif _M_IX86 
	printf("[-] Platform: x86\n");
#pragma message ("Change platform to x64 for more memory.")
#endif // _M_IX86 
#ifdef _OPENMP
	printf("[!] OpenMP is on.\n");
	printf("[-] OpenMP version: %d\n", _OPENMP);
#else
	printf("[!] OpenMP is off.\n");
	printf("[#] Enable OpenMP.\n");
#endif // _OPENMP
	printf("[-] Maximum threads: %d\n", omp_get_max_threads());
	printf("[-] Nested Parallelism: %s\n", omp_get_nested() ? "On" : "Off");
#pragma message("Enable nested parallelism if you wish to have parallel region within parallel region.")
	printf("[#] Enable nested parallelism.\n");
}


int main(int argc, char *argv[]) {
	//Amir
	double start, end, TimeElapsed;
	int *array = NULL;
	int size = 0;
	int Nthrds = 0;
	if (argc < 2) {
		printf("[-] Invalid No. of arguments.\n");
		printf("[-] Try -> <size_of_array>\n");
		printf(">>> ");
		scanf("%d", &size);
	}
	else {
		size = atoi(argv[1]);
	}
	array = (int *)malloc(sizeof(int) * size);

	fillArray(array, size);
	printf("Merge Sort:\n");
	//AMIR :add start time
	omp_set_num_threads(4);
	omp_set_nested(1);
	Nthrds = omp_get_num_threads();
	check_omp();
	start = omp_get_wtime();
	mergeSort(array, size, Nthrds);
	//printArray(array, size);
	end = omp_get_wtime();
	TimeElapsed = end - start;
	printf("\n\nStart Time : %10f\nEnd Time: %10f\nTime Elapsed is: %10f \n\n", start, end, TimeElapsed);
	free(array);
	system("PAUSE");
	return EXIT_SUCCESS;
}

void fillArray(int *array, int size) {
	srand(time(NULL));
	while (size-- > 0) {
		*array++ = rand() % 100;
	}
}

void printArray(int *array, int size) {
	while (size-- > 0) {
		printf("%d, ", *array++);
	}
	printf("\n");
}

void merge(int *a, int n, int m) {
	int i, j, k;
	int *temp = (int *)malloc(n * sizeof(int));
	for (i = 0, j = m, k = 0; k < n; k++) {
		temp[k] = j == n ? a[i++]
			: i == m ? a[j++]
			: a[j] < a[i] ? a[j++]
			: a[i++];
	}
	for (i = 0; i < n; i++) {
		a[i] = temp[i];
	}
	free(temp);
}

void mergeSort_serial(int *a, int n) {
	int m;
	if (n < 2)
		return;
	m = n / 2;
	mergeSort_serial(a, m);
	mergeSort_serial(a + m, n - m);
	merge(a, n, m);
}

//void mergeSort(int *a, int n, int threads) {
//	int m;
//	if (n < 2)
//		return;
//	m = n / 2;
//	if (threads == 1) { mergeSort_serial(a, m); }
//	else if (threads > 1) {
//#pragma omp task
//		mergeSort(a, m, threads / 2);
//#pragma omp task
//		mergeSort(a + m, n - m, threads - threads / 2);
//#pragma omp taskwait
//		merge(a, n, m);
//	}
//}

void mergeSort(int *a, int n, int threads) {
	int m;
	if (n < 2)
		return;
	m = n / 2;
	if (threads == 1) { mergeSort_serial(a, m); }
	else if (threads > 1) {
#pragma omp parallel sections
		{
#pragma omp section 
			mergeSort(a, m, threads / 2);
#pragma omp section
			mergeSort(a + m, n - m, threads - threads / 2);
		}
		merge(a, n, m);
	}
}
