#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <ctime>
#include <windows.h>
#define MAX_SIZE 35
#define NUM 5
#define l (size + 1)
#define MAX 9223372036854775800 //long long�µ������
using namespace std;
int size;
int s[30][30];
long long m[30][30];

void matrix(int *p, long long *best) {
	int i, j, k, ll;
	int  n = size;
	long long q;
	for(i = 1; i <= n; i++) 
	    m[i][i] = 0;//m�����ʼ��
	for(ll = 2; ll <= n; ll++){
		for(i = 1; i <= n - ll + 1; i++) {//����ll�������ֵ
			j = i + ll - 1;        
			m[i][j] = MAX;
			for(k = i; k <= j - 1; k++){//����ǰ���ܵ���ϱ������ҳ�ʹ��m[i,j]ֵ��С�����
				q = m[i][k] + m[k + 1][j] + (long long)p[i - 1] * p[k] * p[j];
				if(q < m[i][j])
				{
					m[i][j] = q;
					s[i][j] = k;
				}
			}
		}
	}
	*best = m[1][size];//����ѽ����ֵ��best��
}

void print_optimal_parens(FILE* fp, int i, int j) {//����������
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
	_LARGE_INTEGER time_start;	//��ʼʱ��
	_LARGE_INTEGER time_over;	//����ʱ��
	double dqFreq;		//��ʱ��Ƶ��
	LARGE_INTEGER f;	//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	FILE* fp1, * fp2, * fp3;
	int i, j, ii;
	long long best;
	int* a;
	
	a = (int*)malloc(MAX_SIZE * sizeof(int));
	fp1 = fopen("../input/2_1_input.txt", "r");//�����ļ�
	fp2 = fopen("../output/time.txt", "w+");//ʱ�����
	fp3 = fopen("../output/result.txt", "w+");//������
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
			fscanf(fp1, "%d", &a[j]);//����ͬ���ݹ�ģ��������
		QueryPerformanceCounter(&time_start);//��ʼ��ʱ
		matrix(a, &best);//��ʼ���� 
		QueryPerformanceCounter(&time_over);//������ʱ
		if(ii == 0){//��n == 5ʱ���m��s����
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
		run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;//��������ʱ��
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
