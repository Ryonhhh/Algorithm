#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <queue>
#include <vector>
#include <functional>
#include<time.h>
#include <ctime>
#include <windows.h>
#include<iostream>
#define NUM 4
#define MAXIMUM 600
using namespace std;

int cost;
int arcin[MAXIMUM];
int scale[4] = { 8,64,128,512 };
char filein[4][35] = { "../input/input1.txt","../input/input2.txt","../input/input3.txt","../input/input4.txt" };
char fileout[4][35] = { "../output/result1.txt","../output/result2.txt","../output/result3.txt","../output/result4.txt" };

typedef struct arc {
	int w;
	int i;
	int j;
	int rank;
	friend bool operator <(arc node1, arc node2)//overloaded function
	{
		return node1.w > node2.w;
	}
	friend bool operator >(arc node1, arc node2)
	{
		return node1.w > node2.w;
	}
}arc;//arc in Graph


typedef struct ver {
	int rank;
	int info;
	int r;
	struct ver* p;
}ver;// vertice node

typedef struct Graph {
	int vernum;
	int arcnum;
	struct ver* ver;
	struct arc* arc;
}Graph;// Graph

void buildGraph(Graph *G, int scale, FILE* fp) {//构建无向图
	G->ver = (ver*)malloc(sizeof(ver) * (scale + 5));
	G->arc = (arc*)malloc(sizeof(arc) * (G->arcnum + 5));
	ver* p;
	int i, j, w, k;
	for (k = 0; k < scale; k++) {
		G->ver[k].rank = k;
		G->ver[k].info = k;
		G->ver[k].p = NULL;
	}// make vertice node
	for (k = 0; k < G->arcnum; k++) {
		fscanf(fp, "%d %d %d", &i, &j, &w);
		G->arc[k].w = w;
		G->arc[k].i = i;
		G->arc[k].j = j;
		G->arc[k].rank = k;
	}
}

int generatearc(int scale, FILE* fp) {//随机生成边与权值
	int* ver_arcnum = (int*)malloc(sizeof(int) * (scale + 5));
	bool* verarc = (bool*)malloc(sizeof(int) * (scale * scale / 2 + 5));
	int i, j, w , rdm, sum = 0;
	for (i = 0; i < scale * scale / 2; i++)
		*(verarc + i) = 1;
	for (i = 0; i < scale; i++)
		ver_arcnum[i] = 0;
	for (i = 0; i < scale; i++) {
		rdm = 1 + rand() % (scale / 2) - ver_arcnum[i];
		while (rdm > 0) {
			j = rand() % scale;
			if (ver_arcnum[j] < scale / 2 && *(verarc + i * (scale / 2) + j)) {
				w = 1 + rand() % 20;
				*(verarc + i * (scale / 2) + j) = 0;
				*(verarc + j * (scale / 2) + i) = 0;
				fprintf(fp, "%d %d %d\n", i, j, w);
				sum++;
				rdm--;
				ver_arcnum[i]++; 
				if (i != j)
					ver_arcnum[j]++;
			}
		}
	}
	free(ver_arcnum);
	free(verarc);
	return sum;
}

void makeSet(ver *v) {//新建集合
	v->p = v;
	v->r = 0;
}

ver *findSet(ver *v) {//找到集合的根节点
	if (v != v->p)
		v->p = findSet(v->p);
	return v->p;
}

void linkSet(ver *u, ver *v) {//链接两个集合
	if (u->r > v->r)
		v->p = u;
	else {
		u->p = v;
		if (u->r == v->r)
			v->r++;
	}
}

void unionSet(ver *u, ver *v) {//将u,v所在的集合合并
	linkSet(findSet(u), findSet(v));
}

void Kruscal(Graph *G) {
	int k, x, y, curarc, innum = 0;
	cost = 0;
	for (k = 0; k < MAXIMUM; k++)
		arcin[k] = 0;
	priority_queue<arc, vector<arc>, less<arc>> pque;//实现以边权值为比较标准的优先队列
	for (k = 0; k < G->vernum; k++) {
		makeSet(&G->ver[k]);
	}
	for (k = 0; k < G->arcnum; k++)
		pque.push(G->arc[k]);
	for (k = 0; k < G->arcnum; k++) {
		x = pque.top().i;
		y = pque.top().j;
		curarc = pque.top().rank;
		pque.pop();//弹出当前权值最小的边
		if (findSet(&G->ver[x]) != findSet(&G->ver[y])) {//若此边连接了两个不相交的集合，则将它加入最小生成树的边集
			arcin[innum++] = curarc;
			cost += G->arc[curarc].w;
			unionSet(&G->ver[x], &G->ver[y]);
		}
	}
}

void printGraph(Graph *G, int scale, FILE* fp) {
	int k;
	fprintf(fp,"the whole cost is %d\n", cost);
	for (k = 0; k < scale - 1; k++)
		fprintf(fp, "%d %d %d\n", G->arc[arcin[k]].i, G->arc[arcin[k]].j, G->arc[arcin[k]].w);
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
	int timeloop = 0, loop = 0, sum;
	for (loop = 0; loop < NUM; loop++) {
			fpin = fopen(filein[loop], "w+");
			fpout = fopen(fileout[loop], "w+");
			if (fpin == NULL || fpout == NULL) {
				printf("Can't open the files!\n");
				exit(0);
			}//file pointers
			sum = generatearc(scale[loop], fpin);
			fseek(fpin, 0, SEEK_SET);
			Graph *G;
			G = (Graph*)malloc(sizeof(Graph));
			G->arcnum = sum;
			G->vernum = scale[loop];
			buildGraph(G, scale[loop], fpin);
			QueryPerformanceCounter(&time_start);//start timing
			Kruscal(G);
			QueryPerformanceCounter(&time_over);//end timing
			printGraph(G, scale[loop], fpout);
			run_time[loop] = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;//caculate the running time
			fclose(fpin);
			fclose(fpout);
		}
	for (loop = 0; loop < NUM; loop++)
		fprintf(fptime, "%d data's inserting cost %lfus\n", scale[loop], run_time[loop]);
	
	fclose(fptime);
	return 0;
}