#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <ctime>
#include <windows.h>
#define MAX_SIZE 30
#define NUM 30
#define RED 0
#define BLACK 1
int size[5] = { 20,40,60,80,100 };

typedef struct INTree {
	int color;
	int key;
	int high;
	int max;
	struct INTree* left;
	struct INTree* right;
	struct INTree* p;
}INTree, * Tree;

typedef struct INRoot {
	INTree* node;
}INRoot;

INTree* NIL;

int MAX3(int x, int y, int z) {
	return ((x > y ? x : y) > z ? (x > y ? x : y) : z) ;
}

int MAX2(int x, int y) {
	return (x > y ? x : y);
}

void IN_print(Tree T, FILE* fp) {//print INTree's nodes in midorder 
	if (T != NIL) {
		IN_print(T->left, fp);
		fprintf(fp, "[%2d, %2d] %2d\n", T->key, T->high, T->max);//print info
		IN_print(T->right, fp);
	}
}

void LEFT_ROTATE(INRoot* T, INTree* s) {//left rotate in center of s
	INTree* y = s->right;
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

	y->max = s->max;
	s->max = MAX3(s->high, s->left->max, s->right->max);
}

void RIGHT_ROTATE(INRoot* T, INTree* s) {//right rotate in center of s
	INTree* y = s->left;
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

	y->max = s->max;
	s->max = MAX3(s->high, s->left->max, s->right->max);
}

void IN_insert_fix(INRoot* T, INTree* s) {//fixup the color after insert a new node
	INTree* y, * p, * gp;
	while ((p = s->p) && p->color == RED) {
		gp = p->p;
		if (p == gp->left) {//parent node is the leftchild of grandparent node
			{
				INTree* u = gp->right;
				if (u && u->color == RED) {//case1: uncle node is red
					u->color = BLACK;
					p->color = BLACK;
					gp->color = RED;
					s = gp;
					continue;
				}
			}
			if (p->right == s) {//case2: uncle node is black, s is the rightchild
				INTree* t;
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
				INTree* u = gp->left;
				if (u && u->color == RED) {
					u->color = BLACK;
					p->color = BLACK;
					gp->color = RED;
					s = gp;
					continue;
				}
			}
			if (p->left == s) {
				INTree* t;
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

void RB_insert(INRoot* T, INTree* s) {
	INTree* x = T->node, * y = NIL;
	while (x != NIL) {//find the insert position
		x->max = MAX2(x->max, s->max);//upgrade max field
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
	IN_insert_fix(T, s);//fixup the color
}

void Find_3node(Tree T, int num, INTree** d, int a[]) {//try to find n/4 random nodes to delete
	int list[100], i, j, x;
	srand(time(NULL));
	for (i = 0; i < num; i++) {
		while (1) {
			x = rand() % 30;
			for (j = 0; j < i; j++)
				if (list[j] == x) break;
			if (j == i) {
				list[i] = x;
				break;
			}
		}
	}
	INTree* s;
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

void INTree_delete_fix(INRoot* T, INTree* s, INTree* p) {//fixup the color after delete
	INTree* x;
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

void IN_max_fix(INRoot* T, INTree *s) {//fix the max after delete a node
	while (s != NIL) {
		s->max = MAX2(s->left->max, s->right->max);
		s = s->p;
	}
}

void IN_deleteVer(INRoot* T, INTree* s) {//delete a node
	INTree* c, * p;
	int color;
	if ((s->left != NIL) && (s->right != NIL)) {//s has two children
		INTree* re = s;
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
			INTree_delete_fix(T, c, p);
		IN_max_fix(T, p);
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
		INTree_delete_fix(T, c, p);
	IN_max_fix(T, p);
	free(s);
}

void generatenumber(FILE* fp) {//gnerate nodes' key
	int list1[100], list2[100], i, j, a, b;
	srand(time(NULL));
	for (i = 0; i < 17; i++) {
		while (1) {
			a = rand() % 26;
			b = a + rand() % (26 - a);
			for (j = 0; j < i; j++)
				if (list1[j] == a) break;
			if (j == i) {
				list1[i] = a;
				list2[i] = b;
				break;
			}
		}
	}
	for (i = 17; i < 30; i++) {
		while (1) {
			a = rand() % 20 + 30;
			b = (a + rand() % (20 - a));
			for (j = 0; j < i; j++)
				if (list1[j] == a) break;
			if (j == i) {
				list1[i] = a;
				list2[i] = b;
				break;
			}
		}
	}
	for (i = 0; i < 30; i++)
		fprintf(fp, "%d %d\n", list1[i], list2[i]);
}

void Create_3node(int* l, int * h) {
	l[0] = 5; h[0] = 5;
	l[1] = 26; h[1] = 28;
	l[2] = 34; h[2] = 40;
}

void IN_search(INRoot* T,int *l, int *h, FILE *fp) {
	INTree* x = T->node;
	int i;
	for (i = 0; i < 3; i++) {
		x = T->node;
		while (x != NIL && (l[i] > x->high || h[i] < x->key)) {
			if (x->left != NIL && x->left->max >= l[i])
				x = x->left;
			else
				x = x->right;
		}
		if (x == NIL)
			fprintf(fp, "[%d, %d]is not found!\n", l[i], h[i]);
		else
			fprintf(fp, "[%d, %d] is overlaped by [%d, %d]\n", l[i], h[i], x->key, x->high);
	}
}

int main() {
	FILE* fp1, * fp2, * fp3, * fp4;
	fp1 = fopen("../input/input.txt", "w+");
	fp2 = fopen("../output/inorder.txt", "w+");
	fp3 = fopen("../output/delete_data.txt", "w+");
	fp4 = fopen("../output/search.txt", "w+");
	if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL) {
		printf("Can't open the files!\n");
		exit(0);
	}
	//file pointers

	int i, j;
	int* a, * b;
	int l[5], h[5];
	a = (int*)malloc(MAX_SIZE * sizeof(int));
	b = (int*)malloc(MAX_SIZE * sizeof(int));
	INTree* d[5];
	NIL = (INTree*)malloc(sizeof(INTree));
	NIL->color = BLACK;
	NIL->key = 0;
	NIL->high = 0;
	NIL->max = 0;
	NIL->left = NIL;
	NIL->right = NIL;
	NIL->p = NIL;//set NIL node
	generatenumber(fp1);
	fseek(fp1, 0, SEEK_SET);
	for (j = 0; j < NUM; j++)
		fscanf(fp1, "%d %d", &a[j], &b[j]);//data inputing

	//insert RBTree
	INRoot* T;
	INTree* s;
	T = (INRoot*)malloc(sizeof(INRoot));
	T->node = NIL;
	for (i = 0; i < NUM; i++) {
		s = (INTree*)malloc(sizeof(INTree));
		s->color = RED;
		s->key = a[i];
		s->high = b[i];
		s->max = s->high;
		s->left = NIL;
		s->p = NIL;
		s->right = NIL;
		RB_insert(T, s);
	}//build the RBTree

	fprintf(fp2, "inorder sequence with color info:\n");
	IN_print(T->node, fp2);
	fprintf(fp2, "\n");

	//delete 3 intervals
	Find_3node(T->node, 3, d, a);//find 3 intervals to delete
	fprintf(fp3, "delete intervals:");
	for (i = 0; i < 3; i++)
		fprintf(fp3, "[%d, %d]", d[i]->key, d[i]->high);//output the delete sequence 
	fprintf(fp3, "\n");
	for (i = 0; i < 3; i++)
		IN_deleteVer(T, d[i]);//start to delete
	fprintf(fp3, "inorder sequence with color info after delete %d nodes:\n", 3);
	IN_print(T->node, fp3);

	//search for 3 intervals
	Create_3node(l, h);//create 3 intervals to search
	fprintf(fp4, "search intervals:");
	for (i = 0; i < 3; i++)
		fprintf(fp4, "[%d, %d] ", l[i], h[i]);//output the search sequence 
	fprintf(fp4, "\n");
	IN_search(T, l, h, fp4);//start to search

	fprintf(fp4, "\n");
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	return 0;
}
