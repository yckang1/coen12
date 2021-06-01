/*Yicheng Kang
 *COEN 12 T 2:15pm
 *Project 5
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pqueue.h"

static int left(int i);
static int right(int i);
static int parent(int i);

//Definitions for left, right, and parent nodes
static int left(int i) {
	return (i*2)+1;
}
static int right(int i) {
	return (i*2)+2;
}
static int parent(int i) {
	return (i-1)/2;
}

//Initialize and define struct pqueue
struct pqueue {
	int count;
	int length;
	void **data;
	int  (*compare)();
};

//O(1)
//Creates a priority queue with starting array of size 10
PQ *createQueue(int (*compare)()) {
	PQ *pq=malloc(sizeof(PQ));
	assert(pq != NULL);
	pq->compare=compare;
	pq->count=0;
	pq->length=10;
	pq->data=malloc(sizeof(void*)*10);
	assert(pq->data != NULL);
	return pq;
}

//O(n)
//Destroys/frees priority queue
void destroyQueue(PQ *pq) {
	int i;
	assert(pq != NULL);
	for(i=0; i<pq->count; i++) {
		free(pq->data[i]);
	}
	free(pq->data);
	free(pq);
	return;
}

//O(1)
//Returns the number of integers in the priority queue
int numEntries(PQ *pq) {
	assert(pq != NULL);
	return pq->count;
}

//O(logn)
//Adds an entry to the priority queue
void addEntry(PQ *pq, void *entry) {
	assert(pq != NULL);
	if(pq->count == pq->length) {
		pq->length *= 2;
		pq->data=realloc(pq->data, sizeof(void*)*pq->length);
	}
	pq->data[pq->count]=entry;
	int ind=pq->count;
	while(pq->count != 0 && (*pq->compare)(pq->data[parent(ind)], pq->data[ind]) > 0) {
		void* swapData = pq->data[ind];
		pq->data[ind]=pq->data[parent(ind)];
		pq->data[parent(ind)]=swapData;
		ind=parent(ind);
	}
	pq->count++;
	return;
}

//O(logn)
//Removes an entry from the priority queue
void *removeEntry(PQ *pq) {
	assert(pq != NULL);
	void* removeData=pq->data[0];
	pq->data[0]=pq->data[pq->count - 1];
	int ind=0;
	while(left(ind) <= pq->count - 1) {
		int smallerchild=left(ind);
		if(right(ind) <= pq->count - 1) {
			if((*pq->compare)(pq->data[right(ind)], pq->data[left(ind)]) < 0) {
				smallerchild = right(ind);
			}
		}
		if((*pq->compare)(pq->data[ind], pq->data[smallerchild]) < 0) {
			break;
		}
		else {
			void* swapData = pq->data[ind];
			pq->data[ind]=pq->data[smallerchild];
			pq->data[smallerchild]=swapData;
		}
		ind = smallerchild;
	}
	pq->count--;
	return removeData;
}
