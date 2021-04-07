#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <ctime>
#include <windows.h>
using namespace std;
#define swap(x,y) {x=x+y; y=x-y; x=x-y;}
#define min(x,y) {x > y ? y : x}
#define MAX_SIZE 300000
int test[6] = { 8,64,512,4096,32768,262144 };


void mergesort_recursive(int *a, int *reg, int p, int r) {
	if (p >= r)
		return;
	int len = r - p, mid = len / 2 + p;
	int p1 = p, r1 = mid;
	int p2 = mid + 1, r2 = r;//分为一半
	mergesort_recursive(a, reg, p1, r1);//左边归并
	mergesort_recursive(a, reg, p2, r2);///右边归并
	int k = p;
	while (p1 <= r1 && p2 <= r2)
		reg[k++] = a[p1] < a[p2] ? a[p1++] : a[p2++];//左右两部分归并
	while (p1 <= r1)
		reg[k++] = a[p1++];
	while (p2 <= r2)
		reg[k++] = a[p2++];
	for (k = p; k <= r; k++)//复制回原数组
		a[k] = reg[k];
}

void mergesort(int *a, int len) {
	int* r;
	r = (int*)malloc(len * sizeof(int));
	mergesort_recursive(a, r, 0, len - 1);
	free(r);
}

int main() {
	double run_time;
	_LARGE_INTEGER time_start;	//开始时间
	_LARGE_INTEGER time_over;	//结束时间
	double dqFreq;		//计时器频率
	LARGE_INTEGER f;	//计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;

	FILE* fp1, * fp2;
	int i, j;
	int* a, * b;
	a = (int*)malloc(MAX_SIZE * sizeof(int));
	b = (int*)malloc(MAX_SIZE * sizeof(int));
	fp1 = fopen("input/input.txt", "r");
	fp2 = fopen("output/mergesort/time.txt", "w+");
	FILE* fp[6];
	fp[0] = fopen("output/mergesort/result_3.txt", "w+");
	fp[1] = fopen("output/mergesort/result_6.txt", "w+");
	fp[2] = fopen("output/mergesort/result_9.txt", "w+");
	fp[3] = fopen("output/mergesort/result_12.txt", "w+");
	fp[4] = fopen("output/mergesort/result_15.txt", "w+");
	fp[5] = fopen("output/mergesort/result_18.txt", "w+");
	for (i = 0; i < 6; i++) {
		fseek(fp1, 0, SEEK_SET);
		for (j = 0; j < test[i]; j++)
			fscanf(fp1, "%d", &a[j]);
		QueryPerformanceCounter(&time_start);
		mergesort(a, test[i]);
		QueryPerformanceCounter(&time_over);
		for (j = 0; j < test[i]; j++)
			fprintf(fp[i], "%d ", a[j]);
		run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		fprintf(fp2, "%d numbers cost %lfus\n", test[i], run_time);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}