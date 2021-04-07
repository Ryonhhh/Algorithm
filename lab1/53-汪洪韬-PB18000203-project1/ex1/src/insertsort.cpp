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


void insertsort(int *a, int n) {
	int i, j, key;
	for (i = 1; i < n; i++) {
		key = a[i];
		j = i - 1;
		while ((j >= 0) && (a[j] > key)) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = key;
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
	FILE* fp1,*fp2;
	int i, j;
	int* a;
	a = (int*)malloc(MAX_SIZE * sizeof(int));
	fp1 = fopen("input/input.txt", "r");//�����ļ�
	fp2 = fopen("output/insertsort/time.txt", "w+");//ʱ�����
	FILE* fp[6];
	fp[0] = fopen("output/insertsort/result_3.txt", "w+");
	fp[1] = fopen("output/insertsort/result_6.txt", "w+");
	fp[2] = fopen("output/insertsort/result_9.txt", "w+");
	fp[3] = fopen("output/insertsort/result_12.txt", "w+");
	fp[4] = fopen("output/insertsort/result_15.txt", "w+");
	fp[5] = fopen("output/insertsort/result_18.txt", "w+");//��ͬ���ݹ�ģ���
	for (i = 0; i < 6; i++) {
		fseek(fp1, 0, SEEK_SET);
		for (j = 0; j < test[i]; j++)
			fscanf(fp1, "%d", &a[j]);//����ͬ���ݹ�ģ��������
		QueryPerformanceCounter(&time_start);//��ʼ��ʱ
		insertsort(a, test[i]);//��ʼ����
		QueryPerformanceCounter(&time_over);//������ʱ
		for (j = 0; j < test[i]; j++)
			fprintf(fp[i], "%d ", a[j]);
		run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;//��������ʱ��
		fprintf(fp2, "%d numbers cost %lfus\n", test[i], run_time);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}