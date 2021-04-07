#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <ctime>
#include <windows.h>
#define MAX_SIZE 120
#define NUM 5
#define RED 0
#define BLACK 1
int size[5] = { 20,40,60,80,100 };
typedef struct RBTree {
	int color;
	int key;
	struct RBTree* left;
	struct RBTree* right;
	struct RBTree* p;
}RBTree, *Tree;

typedef struct RBRoot {
	RBTree* node;
}RBRoot;

RBTree *NIL;

void RB_print(Tree T, FILE* fp) {//print RBTree's nodes in midorder 
	if (T != NIL) {
		RB_print(T->left, fp);
		fprintf(fp, "%2d ", T->key);//print key
		if (T->color == RED)//print color
			fprintf(fp, "RED\n");
		else
			fprintf(fp, "BLACK\n");
		RB_print(T->right, fp);
	}
}

void LEFT_ROTATE(RBRoot* T, RBTree* s) {//left rotate in center of s
	RBTree* y = s->right;
	s->right = y->left;
	if (y->left != NIL)
		y->left->p = s;
	y->p = s->p;
	if (s->p == NIL)
		T->node = y;//if s's parent is NIL, set y as a root
	else {
		if (s->p->left == s)
			s->p->left = y;
		else
			s->p->right = y;
	}
	y->left = s;
	s->p = y;
}

void RIGHT_ROTATE(RBRoot* T, RBTree* s) {//right rotate in center of s
	RBTree* y = s->left;
	s->left = y->right;
	if (y->right != NIL)
		y->right->p = s;
	y->p = s->p;
	if (s->p == NIL)
		T->node = y;//if s's parent is NIL, set y as a root
	else {
		if (s->p->right == s)
			s->p->right = y;
		else
			s->p->left = y;
	}
	y->right = s;
	s->p = y;
}

void RB_insert_fix(RBRoot* T, RBTree* s) {//fixup the color after insert a new node
	RBTree *y, *p, *gp;
	while ((p = s->p) && p->color == RED) {
		gp = p->p;
		if (p == gp->left) {//parent node is the leftchild of grandparent node
			{
				RBTree* u = gp->right;
				if (u && u->color == RED) {//case1: uncle node is red
					u->color = BLACK;
					p->color = BLACK;
					gp->color = RED;
					s = gp;
					continue;
				}
			}
			if (p->right == s) {//case2: uncle node is black, s is the rightchild
				RBTree* t;
				LEFT_ROTATE(T, p);
				t = p;
				p = s;
				s = t;
			}
			//case3: uncle is black, s is the leftchild
			p->color = BLACK;
			gp->color = RED;
			RIGHT_ROTATE(T, gp);
		}
		else {//symmetric with the case above
			{
				RBTree* u = gp->left;
				if (u && u->color == RED) {
					u->color = BLACK;
					p->color = BLACK;
					gp->color = RED;
					s = gp;
					continue;
				}
			}
			if (p->left == s) {
				RBTree* t;
				RIGHT_ROTATE(T, p);
				t = p;
				p = s;
				s = t;
			}
			p->color = BLACK;
			gp->color = RED;
			LEFT_ROTATE(T, gp);
		}
	}
	T->node->color = BLACK;
}

void RB_insert(RBRoot* T, RBTree *s) {
	RBTree* x = T -> node, * y = NIL;
	while (x != NIL) {//find the insert position
		y = x;
		if (s->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	s->p = y;
	if (y == NIL)//if y is NULL, set s as a root node
		T->node = s;
	else if (s->key < y->key)
		y->left = s;
	else
		y->right = s;
	RB_insert_fix(T, s);//fixup the color
}

void Find_n4node(Tree T, int num, RBTree** d, int a[]) {//try to find n/4 random nodes to delete
	int list[100], i, j, x;
	srand(time(NULL));
	for (i = 0; i < num; i++) {
		while (1) {
			x = rand() % (num * 4);
			for (j = 0; j < i; j++)
				if (list[j] == x) break;
			if (j == i) {
				list[i] = x;
				break;
			}
		}
	}
	RBTree* s;
	for (i = 0; i < num; i++) {
		s = T;
		while (s->key != a[list[i]]) {
			if (s->key > a[list[i]])
				s = s->left;
			else 
				s = s->right;
		}
		d[i] = s;
	}
}

void RBTree_delete_fix(RBRoot *T,RBTree *s,RBTree *p) {//fixup the color after delete
	RBTree *x;
	while ((!s || s->color == BLACK) && (s != T->node)) {
		if (p->left == s) {
			x = p->right;
			if (x->color == RED) {//case1: s's brother is red
				x->color = BLACK;
				p->color = RED;
				LEFT_ROTATE(T, p);
				x = p->right;
			}
			if ((!x->left || x->left->color == BLACK) && (!x->right || x->right->color == BLACK)) {
				//case2: s's brother w is black, and w's two children is black
				x->color = RED;
				s = p;
				p = s->p;
			}
			else {
				if (!x->right || x->right->color == BLACK) {
					//case3: s's brother is black,and w's leftchild is red, rightchild is black 
					x->left->color = BLACK;
					x->color = RED;
					RIGHT_ROTATE(T, x);
					x = p->right;
				}
				//case4: s's brother is black,and w's rightchild is red
				x->color = p->color;
				p->color = BLACK;
				x->right->color = BLACK;
				LEFT_ROTATE(T, p);
				s = T->node;
				break;
			}
		}
		else {//symmetric with the case above
			x = p->left;
			if (x->color == RED) {
				x->color = BLACK;
				p->color = RED;
				RIGHT_ROTATE(T, p);
				x = p->left;
			}
			if ((!x->left || x->left->color == BLACK) && (!x->right || x->right->color == BLACK)) {
				x->color = RED;
				s = p;
				p = s->p;
			}
			else {
				if (!x->left || x->left->color == BLACK) {
					x->right->color = BLACK;
					x->color = RED;
					LEFT_ROTATE(T, x);
					x = p->left;
				}
				x->color = p->color;
				p->color = BLACK;
				x->left->color = BLACK;
				RIGHT_ROTATE(T, p);
				s = T->node;
				break;
			}
		}
	}
	if (s)
		s->color = BLACK;
}

void RB_deleteVer(RBRoot *T, RBTree *s) {//delete a node
	RBTree* c, * p;
	int color;
	if ((s->left != NIL) && (s->right != NIL)) {//s has two children
		RBTree* re = s;
		re = re->right;
		while (re->left != NIL)
			re = re->left;
		if (s->p != NIL) {//s is not root
			if (s->p->left == s)
				s->p->left = re;
			else
				s->p->right = re;
		}
		else//s is root
			T->node = re;
		c = re->right;
		p = re->p;
		color = re->color;
		if (p == s)
			p = re;
		else {
			if (c) //child is not NULL
				c->p = p;
			p->left = c;
			re->right = s->right;
			s->right->p = re;
		}
		re->p = s->p;
		re->color = s->color;
		re->left = s->left;
		s->left->p = re;
		if (color == BLACK)
			RBTree_delete_fix(T, c, p);
		free(s);
		return;
	}
	if (s->left != NIL)
		c = s->left;
	else
		c = s->right;
	p = s->p;
	color = s->color;
	if (c)
		c->p = p;
	if (p) {
		if (p->left == s)
			p->left = c;
		else
			p->right = c;
	}
	else
		T->node = c;
	if (color == BLACK)
		RBTree_delete_fix(T, c, p);
	free(s);
}

void generatenumber(FILE* fp) {//gnerate nodes' key
	int list[100], i, j, a;
	srand(time(NULL));
	for (i = 0; i < 100; i++){
		while (1){
			a = rand() % 1000;
			for (j = 0; j < i; j++)
				if (list[j] == a) break;
			if (j == i){
				list[i] = a;
				break;
			}
		}
	}
	for (i = 0; i < 100; i++)
		fprintf(fp, "%d ", list[i]);
}

int main() {
	double run_time;
	_LARGE_INTEGER time_start;	
	_LARGE_INTEGER time_over;	
	double dqFreq;		
	LARGE_INTEGER f;	
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;//timing module

	FILE* fp1, * fp2, * fp3, * fp4, * fp5;
	fp1 = fopen("../input/input.txt", "w+");
	fp2 = fopen("../output/inorder.txt", "w+");
	fp3 = fopen("../output/time1.txt", "w+");
	fp4 = fopen("../output/delete_data.txt", "w+");
	fp5 = fopen("../output/time2.txt", "w+");
	if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL || fp5 == NULL) {
		printf("Can't open the files!\n");
		exit(0);
	}//file pointers

	int ii, i, j, cnt;
	int* a;
	a = (int*)malloc(MAX_SIZE * sizeof(int));
	RBRoot* T;
	RBTree* d[105];
	NIL = (RBTree*)malloc(sizeof(RBTree));
    NIL->color = BLACK;
    NIL->key = 0;
    NIL->left = NIL;
    NIL->right = NIL;
    NIL->p = NIL;
	double time1[10][105], time2[10][105];
	for (cnt = 0; cnt < 100; cnt++) {
		for (ii = 0; ii < NUM; ii++) {
			generatenumber(fp1);
			fseek(fp1, 0, SEEK_SET);
			for (j = 0; j < size[ii]; j++)
				fscanf(fp1, "%d", &a[j]);//data inputing

			//insert RBTree
			RBRoot* T;
			RBTree* s;
			T = (RBRoot*)malloc(sizeof(RBRoot));
			T->node = NIL;
			QueryPerformanceCounter(&time_start);//start timing
			for (i = 0; i < size[ii]; i++) {
				s = (RBTree*)malloc(sizeof(RBTree));
				s->color = RED;
				s->key = a[i];
				s->left = NIL;
				s->p = NIL;
				s->right = NIL;
				RB_insert(T, s);
			}//build the RBTree
			QueryPerformanceCounter(&time_over);//end timing

			fprintf(fp2, "inorder sequence with color info:\n");
			RB_print(T->node, fp2);
			fprintf(fp2, "\n");

			run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;//caculate the running time
			time1[ii][cnt] = run_time;

			//delete n/4 vertex
			Find_n4node(T->node, size[ii] / 4, d, a);//find n/4 nodes to delete
			fprintf(fp4, "delete vers:");
			for (i = 0; i < size[ii] / 4; i++)
				fprintf(fp4, "%d ", d[i]->key);//output the delete sequence 
			fprintf(fp4, "\n");

			QueryPerformanceCounter(&time_start);//start timing
			for (i = 0; i < size[ii] / 4; i++)
				RB_deleteVer(T, d[i]);
			QueryPerformanceCounter(&time_over);//end timing

			fprintf(fp4, "inorder sequence with color info after delete %d nodes:\n", size[ii] / 4);
			RB_print(T->node, fp4);
			fprintf(fp4, "\n");

			run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;//caculate the running time
			time2[ii][cnt] = run_time;
		}
	}
	double run_time1 = 0, run_time2 = 0;
	for (cnt = 0; cnt < 5; cnt++) {//take the average of 100 operations to get a better result
		for (i = 0; i < 100; i++) {
			run_time1 += time1[cnt][i] / 100;
			run_time2 += time2[cnt][i] / 100;
		}
		fprintf(fp3, "%d data's inserting cost %lfus\n", size[cnt], run_time1);
		fprintf(fp5, "%d data's deleting cost %lfus\n", size[cnt] / 4, run_time2);
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	return 0;
} 