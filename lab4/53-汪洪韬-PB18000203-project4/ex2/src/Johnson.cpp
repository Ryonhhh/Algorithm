#include<stdio.h>
#include<stdlib.h>
#include<cmath>
#include<math.h>
#include <queue>
#include <vector>
#include <functional>
#include<time.h>
#include <ctime>
#include <windows.h>
#include<iostream>
#define NUM 8
#define MAXIMUM 600
#define MAX 100000
using namespace std;

int cost;
int arcin[MAXIMUM];
int scale[4] = { 27, 81, 243, 729 };
char filein[8][35] = { "../input/input11.txt","../input/input21.txt","../input/input31.txt","../input/input41.txt","../input/input12.txt","../input/input22.txt","../input/input32.txt","../input/input42.txt" };
char fileout[8][35] = { "../output/result11.txt","../output/result21.txt","../output/result31.txt","../output/result41.txt","../output/result12.txt","../output/result22.txt","../output/result32.txt","../output/result42.txt" };

typedef struct arc {
	int w;
	int i;
	int j;
	int rank;
}arc;//arc in Graph

typedef struct ver {
	int rank;
	int info;
	int d;
	struct ver* pi;
	struct ver* p;
	int adj[10];
	int adjnum;
	friend bool operator <(ver node1, ver node2)// overloaded function to compare
	{
		return node1.d > node2.d;
	}
	friend bool operator >(ver node1, ver node2)
	{
		return node1.d > node2.d;
	}
}ver;// vertice node

typedef struct Graph {
	int vernum;
	int arcnum;
	struct ver* ver;
	struct arc* arc;
}Graph;// Graph

void buildGraph(Graph* G, int scale, FILE* fp) {//构建有向图
	G->ver = (ver*)malloc(sizeof(ver) * (scale + 10));
	G->arc = (arc*)malloc(sizeof(arc) * (G->arcnum * 2 + 5));
	int i, j, w, k;
	fscanf(fp, "%d %d", &i, &j);
	for (k = 0; k < scale; k++) {
		G->ver[k].rank = k;
		G->ver[k].info = k;
		G->ver[k].p = NULL;
		G->ver[k].adjnum = 0;
	}// make vertice node
	for (k = 0; k < G->arcnum; k++) {
		fscanf(fp, "%d %d %d", &i, &j, &w);
		G->arc[k].w = w;
		G->arc[k].i = i;
		G->arc[k].j = j;
		G->arc[k].rank = k;
		G->ver[G->arc[k].i].adj[G->ver[G->arc[k].i].adjnum++] = j;
	}
}

void generatearc(int scale, int arcscale, FILE* fp) {//随机生成边与权值
	bool* verarc = (bool*)malloc(sizeof(int) * (scale * scale / 2 + 5));
	int i, j, w, rdm;
	fprintf(fp, "%d %d\n", scale, arcscale);
	for (i = 0; i < scale * scale / 2; i++)
		*(verarc + i) = 1;
	for (i = 0; i < scale; i++) {
		rdm = arcscale;
		while (rdm > 0) {
			j = rand() % scale;
			if (*(verarc + i * (scale / 2) + j)) {
				w = rand() % 51;
				*(verarc + i * (scale / 2) + j) = 0;
				fprintf(fp, "%d %d %d\n", i, j, w);
				rdm--;
			}
		}
	}
	free(verarc);
}

void initialSingle(Graph* G, ver *s) {//单源最短路径的初始化
	int k;
	for (k = 0; k < G->vernum; k++) {
		G->ver[k].d = MAX;
		G->ver[k].pi = NULL;
	}
	s->d = 0;
}

void relax(ver* u, ver* v, int** w) {//松弛操作
	if (v->d > u->d + w[u->rank][v->rank]) {
		v->d = u->d + w[u->rank][v->rank];
		v->pi = u;
	}
}

int BellmanFord(Graph* G, ver *s, int **w) {//Bellman-Ford算法，对每条边进行G.V-1次松弛操作
	int k,l;
	initialSingle(G, s);
	for (k = 1; k < G->vernum; k++)
		for (l = 0; l < G->arcnum; l++)
			relax(&G->ver[G->arc[l].i], &G->ver[G->arc[l].j], w);
	for (k = 0; k < G->arcnum; k++)
		if (G->ver[G->arc[k].j].d > G->ver[G->arc[k].i].d + w[G->ver[G->arc[k].i].rank][G->ver[G->arc[k].j].rank])
			return 0;
	return 1;
}

void Dijkstra(Graph* G, ver* s, int **w) {//Dijkstra算法
	int k, l, u, a;
	initialSingle(G, s);
	bool* S;
	S = (bool*)malloc(sizeof(int) * (G->vernum + 5));
	for (k = 0; k < G->vernum; k++) {
		S[k] = 1;
	}
	l = G->vernum;
	while (l--) {
		priority_queue<ver, vector<ver>, less<ver>> pque;//优先队列实现以v.d为标准的排序
		for (k = 0; k < G->vernum; k++) 
			if (S[k]) {
				pque.push(G->ver[k]);
			}
		u = pque.top().rank;//每次弹出v.d最小的一个顶点
		S[u] = 0;
		for (a = G->ver[u].adjnum; a > 0; a--) {
			relax(&G->ver[u], &G->ver[G->ver[u].adj[a - 1]], w);
		}
		while (!pque.empty()) 
			pque.pop();
	}
	free(S);
}

void printGraph(int s, Graph* G, FILE* fp) {//打印路径
	int k, t;
	int* trace;
	trace = (int*)malloc(sizeof(int) * (G->vernum + 5));
	ver* p;
	for (k = 0; k < G->vernum; k++) {
		if (k == s) {
			;
		}
		else if (G->ver[k].pi == NULL)
			fprintf(fp, "(%d, %d no exist)\n", s, G->ver[k].rank);
		else {
			p = &G->ver[k];
			t = 0;
			while (p->rank != s) {
				trace[t++] = p->rank;
				p = p->pi;
			}
			trace[t] = s;
			fprintf(fp, "(");
			for (t = t ; t > 0; t--)
				fprintf(fp, "%d,", trace[t]);
			fprintf(fp, "%d %d)\n", trace[0], G->ver[k].d);
		}
	}
}

int* Johnson(Graph* G, FILE*fp) {//Johnson算法
	int k, l, s;
	int** win, ** win2;

	s = G->vernum;
	Graph *G2=(Graph*)malloc(sizeof(Graph));

	G2->arcnum = G->arcnum;
	G2->vernum = G->vernum;
	
	G2->ver = (ver*)malloc(sizeof(ver) * (G->vernum + 10));
	G2->arc = (arc*)malloc(sizeof(arc) * (G->arcnum * 2 + 5));
	memcpy(G2->ver, G->ver, sizeof(ver) * (G->vernum + 10));
	memcpy(G2->arc, G->arc, sizeof(arc) * (G->arcnum * 2 + 5));
	G2->ver[G2->vernum].info = s;
	G2->ver[G2->vernum].rank = s;
	G2->vernum++;

	for (k = G2->arcnum; k < G2->arcnum + G2->vernum - 1; k++) {
		G2->arc[k].i = s;
		G2->arc[k].j = k - G2->arcnum;
		G2->arc[k].w = 0;
		G2->arc[k].rank = k;
	}

	G2->arcnum += G2->vernum - 1;//对G‘进行初始化
	win = (int**)malloc(sizeof(int) * (G2->vernum + 5));
	for (k = 0; k < G2->vernum; k++)
		win[k] = (int*)malloc(sizeof(int) * (G2->vernum + 5));
	win2 = (int**)malloc(sizeof(int) * (G2->vernum + 5));
	for (k = 0; k < G2->vernum; k++)
		win2[k] = (int*)malloc(sizeof(int) * (G2->vernum + 5));
	for (k = 0; k < G2->vernum; k++)
		for (l = 0; l < G2->vernum; l++) {
			win[k][l] = MAX;
			win2[k][l] = MAX;
		}
	for (k = 0; k < G2->arcnum; k++) 
		win2[G2->arc[k].i][G2->arc[k].j] = G2->arc[k].w;
	
	int* D = (int*)malloc(sizeof(int) * (G2->vernum * G2->vernum + 5));
	int* h = (int*)malloc(sizeof(int) * (G2->vernum + 5));
	if (!BellmanFord(G2, &G2->ver[s], win2)) {//调用Bellman_ford算法来判断是否存在负环
		printf("the input graph contains a negative-weight cycle.\n");
		return 0;
	}
	else {
		for (k = 0; k < G2->vernum; k++) {
			h[k] = G2->ver[k].d;
		}
		for (k = 0; k < G2->arcnum; k++) {
			win[G2->arc[k].i][G2->arc[k].j] = win2[G2->arc[k].i][G2->arc[k].j] + h[G2->arc[k].i] - h[G2->arc[k].j];//对边权重新赋值
		}
		for (k = 0; k < G->vernum * G->vernum; k++)
			*(D + k) = 0;
		for (k = 0; k < G->vernum; k++) {
			Dijkstra(G, &G->ver[k], win);//调用Dijkstra算法来求每个顶点的单源最短路径
			printGraph(k, G, fp);
			for (l = 0; l < G->vernum; l++)
				*(D + k * G->vernum + l) = G->ver[l].d + h[l] - h[k];
		}
	}
	return D;
}

int main() {
	double run_time[NUM];
	_LARGE_INTEGER time_start;
	_LARGE_INTEGER time_over;
	double dqFreq;
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;//timing module

	FILE* fpin, * fpout, * fptime;
	fptime = fopen("../output/time.txt", "w+");
	int timeloop = 0, loop = 0;
	for (loop = 0; loop < NUM; loop++) {
		fpin = fopen(filein[loop], "w+");
		fpout = fopen(fileout[loop], "w+");
		if (fpin == NULL || fpout == NULL) {
			printf("Can't open the files!\n");
			exit(0);
		}//file pointers
		Graph* G;
		G = (Graph*)malloc(sizeof(Graph));
		G->arcnum = scale[loop % 4] * (loop < 4 ? (int)(log(scale[loop % 4])/log(5)) : (int)(log(scale[loop % 4])/log(7)));
		generatearc(scale[loop % 4], G->arcnum / scale[loop % 4], fpin);
		fseek(fpin, 0, SEEK_SET);
		G->vernum = scale[loop % 4];
		buildGraph(G, scale[loop % 4], fpin);
		QueryPerformanceCounter(&time_start);//start timing
		Johnson(G, fpout);
		QueryPerformanceCounter(&time_over);//end timing
		run_time[loop] = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;//caculate the running time
		free(G);
		fclose(fpin);
		fclose(fpout);
	}
	for (loop = 0; loop < NUM; loop++)
		fprintf(fptime, "%d vers with %d arcs cost %lfus\n", scale[loop % 4], loop < 4 ? (int)(log(scale[loop % 4]) / log(5)) : (int)(log(scale[loop % 4]) / log(7)), run_time[loop]);

	fclose(fptime);
	return 0;
}
