#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <ctime>
#include <windows.h>
#define MAX_SIZE 35
#define NUM 5
#define l (size + 1)
#define MAX 9223372036854775800 //long long下的无穷大
using namespace std;
int size;
int s[30][30];
long long m[30][30];

void matrix(int *p, long long *best) {
	int i, j, k, ll;
	int  n = size;
	long long q;
	for(i = 1; i <= n; i++) 
	    m[i][i] = 0;//m数组初始化
	for(ll = 2; ll <= n; ll++){
		for(i = 1; i <= n - ll + 1; i++) {//将第ll层遍历求值
			j = i + ll - 1;        
			m[i][j] = MAX;
			for(k = i; k <= j - 1; k++){//将当前可能的组合遍历，找出使得m[i,j]值最小的组合
				q = m[i][k] + m[k + 1][j] + (long long)p[i - 1] * p[k] * p[j];
				if(q < m[i][j])
				{
					m[i][j] = q;
					s[i][j] = k;
				}
			}
		}
	}
	*best = m[1][size];//将最佳结果赋值到best中
}

void print_optimal_parens(FILE* fp, int i, int j) {//配对输出函数
	if (i == j)
		fprintf(fp, "A%d", i);
	else {
		fprintf(fp, "(");
		print_optimal_parens(fp, i, s[i][j]);
		print_optimal_parens(fp, s[i][j] + 1, j);
		fprintf(fp, ")");
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
	FILE* fp1, * fp2, * fp3;
	int i, j, ii;
	long long best;
	int* a;
	
	a = (int*)malloc(MAX_SIZE * sizeof(int));
	fp1 = fopen("../input/2_1_input.txt", "r");//输入文件
	fp2 = fopen("../output/time.txt", "w+");//时间输出
	fp3 = fopen("../output/result.txt", "w+");//结果输出
	if (fp1 == NULL || fp2 == NULL || fp3 == NULL) {
		printf("Can't open the files!\n");
		exit(0);
	}
	for(i = 0; i < 30; i++)
	    for(j = 0; j < 30; j++){
	    	s[i][j] = 0;
	    	m[i][j] = 0;
		}
	for (ii = 0; ii < NUM; ii++) {
		fscanf(fp1, "%d", &size);
		for (j = 0; j <= size; j++)
			fscanf(fp1, "%d", &a[j]);//按不同数据规模读入数据
		QueryPerformanceCounter(&time_start);//开始计时
		matrix(a, &best);//开始计算 
		QueryPerformanceCounter(&time_over);//结束计时
		if(ii == 0){//当n == 5时输出m和s数组
			printf("While is size is 5:\n");
			printf("m-matrix:\n");
			for (j = 1; j < l - 1; j++){
		     	for(i = 1; i < l; i++)
		            printf("%16lld ", m[j][i]);
		        printf("\n");
		    }
		    printf("s-matrix:\n");
		    for (j = 1; j < l - 1; j++){
		     	for(i = 2; i < l; i++)
		            printf("%d ", s[j][i]);
		        printf("\n");
		    }
	    }
		run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;//计算所用时间
		fprintf(fp2, "%d matrixes cost %lfus\n", size, run_time);
		fprintf(fp3, "%d matrixes' best solution needs %lld multiplies\n", size, best);
		print_optimal_parens(fp3, 1, size);
		fprintf(fp3,"\n");
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
} 
