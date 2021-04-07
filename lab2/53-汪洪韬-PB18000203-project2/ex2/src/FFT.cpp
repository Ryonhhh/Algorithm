#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <ctime>
#include <windows.h>
#define pi 3.1415926
#define NUM 6

typedef struct complex {//复数结构体 
	double re = 0;
	double im = 0;
}complex;

complex *FFT(int* a, int n) {
	int i;
	if (n == 1) {//当 n == 1时返回当前值 
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
	w.im = 0;//初始化定义 
	int* a1, * a0;
	complex* y0, * y1, *y;
	a0 = (int*)malloc((n / 2) * sizeof(int));
	a1 = (int*)malloc((n / 2) * sizeof(int));
	for(i = 0; i < n / 2; i++){
		a0[i] = a[i * 2]; 
		a1[i] = a[i * 2 + 1]; 
	}//将系数数组进行分治 
	y0 = (complex*)malloc((n / 2) * sizeof(complex));
	y1 = (complex*)malloc((n / 2) * sizeof(complex));
	y = (complex*)malloc(n * sizeof(complex));
	y0 = FFT(a0, n / 2);
	y1 = FFT(a1, n / 2);//分治计算 
	for (i = 0; i < n / 2; i++) {
		y[i].re = y0[i].re + w.re * y1[i].re - w.im * y1[i].im;
		y[i].im = y0[i].im + w.re * y1[i].im + w.im * y1[i].re;
		y[i + n / 2].re = y0[i].re - w.re * y1[i].re + w.im * y1[i].im;
		y[i + n / 2].im = y0[i].im - w.re * y1[i].im - w.im * y1[i].re;
		w.re = w.re * wn.re - w.im * wn.im;
		w.im = w.re * wn.im + w.im * wn.re;//依据公式计算，将分治计算的两部分进行合并 
	}
	return y;
}

int main() {
	double run_time;
	_LARGE_INTEGER time_start;	//开始时间
	_LARGE_INTEGER time_over;	//结束时间
	double dqFreq;		//计时器频率
	LARGE_INTEGER f;	//计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	FILE* fp1, * fp2, * fp3;
	int i, j, ii, n;
	fp1 = fopen("../input/2_2_input.txt", "r");//输入文件
	fp2 = fopen("../output/time.txt", "w+");//时间输出
	fp3 = fopen("../output/result.txt", "w+");//结果输出
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
			fscanf(fp1, "%d", &a[j]);//按不同数据规模读入数据
		QueryPerformanceCounter(&time_start);//开始计时
		y = FFT(a, n);//开始计算 
		QueryPerformanceCounter(&time_over);//结束计时
		run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;//计算所用时间
		if (ii == 0) //当数据规模为2^3时输出 
			for (i = 0; i < n; i++)
				printf("A(W%d) = %lf + i * %lf\n", i, y[i].re, y[i].im);
		fprintf(fp2, "%d data size cost %lfus\n", n, run_time);
		for (i = 0; i < n; i++)
			fprintf(fp3, "%lf ", y[i].re);//将y的实部输出 
		fprintf(fp3,"\n\n");
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
} 
