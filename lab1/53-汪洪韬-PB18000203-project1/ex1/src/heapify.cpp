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

void max_heapify(int* a, int x, int size) {//����Ϊ����Ѷ�
	int l, r, max;
	l = 2 * x;//�����
	r = 2 * x + 1;//�Ҷ���
	if (l <= size && a[l] > a[x])
		max = l;
	else max = x;
	if (r <= size && a[r] > a[max])
		max = r;
	if (max != x) {
		swap(a[x], a[max]);//�����Ҷ��ӽϴ��һ������
		max_heapify(a, max, size);//������������Ϊ�����
	}
}

void build_max_heap(int* a, int n, int size) {//����
	int i;
	for (i = n / 2; i > 0; i--)
		max_heapify(a, i, size);//��ÿ������������Ϊ����Ѷ�
}

void heapify(int* a, int n) {//������
	int i, size = n;
	build_max_heap(a, n, size);//����
	for (i = n; i > 1; i--) {
		swap(a[i], a[1]);
		size--;//ÿ�������ȡ���Ѷ�Ԫ�أ���a[1]��֮����
		max_heapify(a, 1, size);
	}
}

int main() {
	double run_time;
	_LARGE_INTEGER time_start;	//��ʼʱ��
	_LARGE_INTEGER time_over;	//����ʱ��
	double dqFreq;		//��ʱ��Ƶ��
	LARGE_INTEGER f;	//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;

	FILE* fp1, * fp2;
	int i, j;
	int* a;
	a = (int*)malloc(MAX_SIZE * sizeof(int));
	fp1 = fopen("input/input.txt", "r");
	fp2 = fopen("output/heapify/time.txt", "w+");
	FILE* fp[6];
	fp[0] = fopen("output/heapify/result_3.txt", "w+");
	fp[1] = fopen("output/heapify/result_6.txt", "w+");
	fp[2] = fopen("output/heapify/result_9.txt", "w+");
	fp[3] = fopen("output/heapify/result_12.txt", "w+");
	fp[4] = fopen("output/heapify/result_15.txt", "w+");
	fp[5] = fopen("output/heapify/result_18.txt", "w+");
	for (i = 0; i < 6; i++) {
		fseek(fp1, 0, SEEK_SET);
		for (j = 1; j <= test[i]; j++)
			fscanf(fp1, "%d", &a[j]);
		QueryPerformanceCounter(&time_start);
		heapify(a, test[i]);
		QueryPerformanceCounter(&time_over);
		for (j = 1; j <= test[i]; j++)
			fprintf(fp[i], "%d ", a[j]);
		run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		fprintf(fp2, "%d numbers cost %lfus\n", test[i], run_time);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}