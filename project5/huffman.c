/*Yicheng Kang
 * COEN 12L T 2:15pm
 * Project 5 Week 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pqueue.h"
#include "pack.h"
#include <ctype.h>

typedef struct node NODE;

static void readFile(char *fileName);
static NODE* makeNode(int data, NODE* left, NODE* right);
static int depth(NODE* np);
static int nodeCompare(NODE* np1, NODE* np2);

int counts[257];
NODE* nodes[257];

//O(n)
//Creates priority queue and prints out bits from characters in text file
int main(int argc, char* argv[]) {
	int i;
	if(argc != 3) {
		printf("%s\n", "error");
		return 0;
	}
	else {
		readFile(argv[1]);
	}
	PQ *pq=createQueue(nodeCompare);	//creates priority queue
	for(i=0; i<256; i++) {
		if(counts[i]>0) {
			nodes[i]=makeNode(counts[i], NULL, NULL);
			addEntry(pq, nodes[i]);
		}
	}
	nodes[256]=makeNode(0, NULL, NULL);
	addEntry(pq, nodes[256]);

	while(numEntries(pq) > 1) {
		NODE* left=removeEntry(pq);
		NODE* right=removeEntry(pq);
		NODE *parent=makeNode(left->count+right->count, left, right);
		addEntry(pq, parent);
	}
	//prints out the bits used for characters in the text file
	for(i=0; i<=256; i++) {
		if(nodes[i] != NULL) {
			if(isprint(i)) {
				printf("'%c'", i);
			}
			else {
				printf("%03o", i);
			}
			printf(": %d x %d bits = %d bits\n", counts[i], depth(nodes[i]), counts[i]*depth(nodes[i]));
		}
	}
	pack(argv[1], argv[2], nodes);
	destroyQueue(pq);
	return 0;
}

//O(n)
//Reads text file and retrieves character at the spot in file
static void readFile(char* fileName) {
	FILE *fp=fopen(fileName, "r");
	if(fp==NULL) {
		printf("%s\n", "File name is missing!");
		return;
	}
	int c=getc(fp);
	while(c != EOF) {
		counts[c]++;
		c=getc(fp);
	}
	fclose(fp);
	return;
}

//O(1)
//Makes node for priority queue
static NODE* makeNode(int data, NODE* left, NODE* right) {
	NODE* root=malloc(sizeof(NODE));
	assert(root != NULL);
	root->count=data;
	root->parent=NULL;
	if(left != NULL) {
		left->parent=root;
	}
	if(right != NULL) {
		right->parent=root;
	}
	return root;
}

//O(1)
//Returns depth of a given node
static int depth(NODE* np) {
	assert(np != NULL);
	if(np->parent==NULL) {
		return 0;
	}
	return depth(np->parent)+1;
}

//O(1)
//Compares both node pointers and returns bigger value
static int nodeCompare(NODE* np1, NODE* np2) {
	assert(np1 != NULL && np2 != NULL);
	return(np1->count < np2->count) ? -1 : (np1->count > np2->count);
}
