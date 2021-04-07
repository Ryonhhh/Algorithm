#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <ctime>
#include <windows.h>
using namespace std;
#define swap(x,y) {x=x+y; y=x-y; x=x-y;}
#define MAX_SIZE 300000
int test[6] = { 8,64,512,4096,32768,262144 };

void countingsort(int* a, int* b, int n) {
	int* c = (int*)malloc(sizeof(int) * 35000);
	int i, j, k;
	for (k = 0; k < 35000; k++)
		c[k] = 0;//c置零
	for (i = 0; i < n; i++)
		c[a[i]]++;//循环后c数组元素i表示数据中i的个数
	for (k = 1; k < 35000; k++)
		c[k] += c[k - 1];//循环后c数组元素i表示数据中小于等于i的个数
	for (j = n; j > 0; j--)
		b[--c[a[j - 1]]] = a[j - 1];//将c[a[j]]个a[j]复制到b中
	for (i = 0; i < n; i++)
		a[i] = b[i];
	free(c);
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
	fp2 = fopen("output/countingsort/time.txt", "w+");
	FILE* fp[6];
	fp[0] = fopen("output/countingsort/result_3.txt", "w+");
	fp[1] = fopen("output/countingsort/result_6.txt", "w+");
	fp[2] = fopen("output/countingsort/result_9.txt", "w+");
	fp[3] = fopen("output/countingsort/result_12.txt", "w+");
	fp[4] = fopen("output/countingsort/result_15.txt", "w+");
	fp[5] = fopen("output/countingsort/result_18.txt", "w+");
	for (i = 0; i < 6; i++) {
		fseek(fp1, 0, SEEK_SET);
		for (j = 0; j < test[i]; j++)
			fscanf(fp1, "%d", &a[j]);
		QueryPerformanceCounter(&time_start);
		countingsort(a, b, test[i]);
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