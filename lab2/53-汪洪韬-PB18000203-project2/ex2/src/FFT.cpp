#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <ctime>
#include <windows.h>
#define pi 3.1415926
#define NUM 6

typedef struct complex {//�����ṹ�� 
	double re = 0;
	double im = 0;
}complex;

complex *FFT(int* a, int n) {
	int i;
	if (n == 1) {//�� n == 1ʱ���ص�ǰֵ 
		complex *ca;
		ca = (complex*)malloc(sizeof(complex));
		(*ca).re = *a;
		(*ca).im = 0;
		return ca;
	}
	complex wn, w;
	wn.re = cos(2 * pi / n);
	wn.im = sin(2 * pi / n);
	w.re = 1;
	w.im = 0;//��ʼ������ 
	int* a1, * a0;
	complex* y0, * y1, *y;
	a0 = (int*)malloc((n / 2) * sizeof(int));
	a1 = (int*)malloc((n / 2) * sizeof(int));
	for(i = 0; i < n / 2; i++){
		a0[i] = a[i * 2]; 
		a1[i] = a[i * 2 + 1]; 
	}//��ϵ��������з��� 
	y0 = (complex*)malloc((n / 2) * sizeof(complex));
	y1 = (complex*)malloc((n / 2) * sizeof(complex));
	y = (complex*)malloc(n * sizeof(complex));
	y0 = FFT(a0, n / 2);
	y1 = FFT(a1, n / 2);//���μ��� 
	for (i = 0; i < n / 2; i++) {
		y[i].re = y0[i].re + w.re * y1[i].re - w.im * y1[i].im;
		y[i].im = y0[i].im + w.re * y1[i].im + w.im * y1[i].re;
		y[i + n / 2].re = y0[i].re - w.re * y1[i].re + w.im * y1[i].im;
		y[i + n / 2].im = y0[i].im - w.re * y1[i].im - w.im * y1[i].re;
		w.re = w.re * wn.re - w.im * wn.im;
		w.im = w.re * wn.im + w.im * wn.re;//���ݹ�ʽ���㣬�����μ���������ֽ��кϲ� 
	}
	return y;
}

int main() {
	double run_time;
	_LARGE_INTEGER time_start;	//��ʼʱ��
	_LARGE_INTEGER time_over;	//����ʱ��
	double dqFreq;		//��ʱ��Ƶ��
	LARGE_INTEGER f;	//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	FILE* fp1, * fp2, * fp3;
	int i, j, ii, n;
	fp1 = fopen("../input/2_2_input.txt", "r");//�����ļ�
	fp2 = fopen("../output/time.txt", "w+");//ʱ�����
	fp3 = fopen("../output/result.txt", "w+");//������
	if (fp1 == NULL || fp2 == NULL || fp3 == NULL) {
		printf("Can't open the files!\n");
		exit(0);
	}
	complex* y;
	int* a;
	for (ii = 0; ii < NUM; ii++) {
		fscanf(fp1, "%d", &n);
		a = (int*)malloc(n * sizeof(int));
		y = (complex*)malloc(n * sizeof(complex));
		for (j = 0; j < n; j++)
			fscanf(fp1, "%d", &a[j]);//����ͬ���ݹ�ģ��������
		QueryPerformanceCounter(&time_start);//��ʼ��ʱ
		y = FFT(a, n);//��ʼ���� 
		QueryPerformanceCounter(&time_over);//������ʱ
		run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;//��������ʱ��
		if (ii == 0) //�����ݹ�ģΪ2^3ʱ��� 
			for (i = 0; i < n; i++)
				printf("A(W%d) = %lf + i * %lf\n", i, y[i].re, y[i].im);
		fprintf(fp2, "%d data size cost %lfus\n", n, run_time);
		for (i = 0; i < n; i++)
			fprintf(fp3, "%lf ", y[i].re);//��y��ʵ����� 
		fprintf(fp3,"\n\n");
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
} 
