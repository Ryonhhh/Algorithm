#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <ctime>
#include <windows.h>
using namespace std;
#define swap(x,y) {x = x + y; y = x - y; x = x - y;}
#define MAX_SIZE 300000
int test[6] = { 8,64,512,4096,32768,262144 };

int Paritition(int *a, int p, int r) {
	int x = a[p];//设置比较基准为x
	while (p < r) {
		while (p < r && a[r] >= x) {
			--r;
		}
		a[p] = a[r];
		while (p < r && a[p] <= x) {
			++p;
		}
		a[r] = a[p];//比较当前p（r）指向的值与x的大小，若大于（小于）x，则调换到r（p）处
	}
	a[p] = x;
	return p;
}

void quicksort(int *a, int p, int r) //快排母函数
{
	if (p < r) {
		int q = Paritition(a, p, r);//分治
		quicksort(a, p, q - 1);
		quicksort(a, q + 1, r);
	}
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
	int* a;
	a = (int*)malloc(MAX_SIZE * sizeof(int));
	fp1 = fopen("input/input.txt", "r");
	fp2 = fopen("output/quicksort/time.txt", "w+");
	FILE* fp[6];
	fp[0] = fopen("output/quicksort/result_3.txt", "w+");
	fp[1] = fopen("output/quicksort/result_6.txt", "w+");
	fp[2] = fopen("output/quicksort/result_9.txt", "w+");
	fp[3] = fopen("output/quicksort/result_12.txt", "w+");
	fp[4] = fopen("output/quicksort/result_15.txt", "w+");
	fp[5] = fopen("output/quicksort/result_18.txt", "w+");
	for (i = 0; i < 6; i++) {
		fseek(fp1, 0, SEEK_SET);
		for (j = 0; j < test[i]; j++)
			fscanf(fp1, "%d", &a[j]);
		QueryPerformanceCounter(&time_start);
		quicksort(a, 0, test[i] - 1);
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