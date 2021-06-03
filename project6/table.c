/*Yicheng Kang
 * COEN 12L T 2:15pm
 * Project 6
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

#define empty 0
#define filled 1
#define removed 2

static int search(SET *sp, void *elt, bool *find);
static int partition(void **elts, int low, int high, int (*compare)());
static void quicksort(void **elts, int low, int high, int (*compare)());

//Set structure definition
struct set {
	void **data;
	char* flag;
	int length;
	int counter;
	int (*compare)();
	unsigned (*hash)();
};

//O(n)
//Returns location of elt in set pointed to by sp
static int search(SET *sp, void *elt, bool* find) {
	assert(sp != NULL && elt != NULL);
	int ind = (*sp->hash)(elt) % sp->length;
	int trials=0;
	int deletedslot=-1;
	while(trials < sp->length) {
		if(sp->flag[ind] == empty) {
			*find = false;
			return deletedslot != -1 ? deletedslot : ind;
		}
		//if index is on deleted slot, increase the index by one
		else if(sp->flag[ind] == removed) {
			trials++;
			if(deletedslot == -1) {
				deletedslot=ind;
			}
			ind=(ind+1)%sp->length;
		}
		else {
			//returns the index if element is found. otherwise, increase the index by one
			if((*sp->compare)(sp->data[ind], elt) == 0) {
				*find=true;
				return ind;
			}
			else {
				ind=(ind+1)%sp->length;
				trials++;
			}
		}
	}
	*find=false;
	return deletedslot;
}

//O(n)
//Creates a set
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
	int i;
	SET *sp=(SET*)malloc(sizeof(SET));
	//asserts the set and the pointer exists
	assert(sp != NULL);
	sp->counter=0;
	sp->length=maxElts;
	sp->compare=compare;
	sp->hash=hash;
	sp->data=malloc(sizeof(void*)*maxElts);
	sp->flag=(char*)malloc(sizeof(char)*maxElts);
	assert(sp->data != NULL && sp->flag != NULL);
	
	//every slot is set to empty
	for(i=0; i<sp->length; i++) {
		sp->flag[i] = empty;
	}
	return sp;
}

//O(n)
//Destroys the set
void destroySet(SET *sp) {
	int i;
	assert(sp != NULL);
	
	//frees each character in the set
	for(i=0; i<sp->counter; i++) {
		if(sp->data[i]==NULL) {
			free(sp->data[i]);
		}
	}
	free(sp->data);
	free(sp->flag);
	free(sp);
	return;
}

//O(1)
//Returns number of elements in the set
int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->counter;
}

//O(1)
//Inserts element into the set
void addElement(SET *sp, void *elt) {
	assert(sp != NULL && elt != NULL && sp->counter < sp->length);
	bool find=false;
	int ind=search(sp, elt, &find);
	//allocates memory if word not present in the set
	if(find==false) {
		sp->data[ind]=elt;
		sp->flag[ind]=filled;
		sp->counter++;
	}
	return;
}

//O(1)
//Deletes element from the set
void removeElement(SET *sp, void *elt) {
	assert(sp != NULL && elt != NULL);
	bool find=false;
	int ind=search(sp, elt, &find);
	if(find==false) {
		return;
	}
	//changes marker to removed
	sp->flag[ind]=removed;
	sp->counter--;
	return;
}

//O(1)
//Search for a specific element in the set
void *findElement(SET *sp, void *elt) {
	assert(sp != NULL && elt != NULL);
	bool find =false;
	int ind=search(sp, elt, &find);
	if(find==false) {
		return NULL;
	}
	return sp->data[ind];
}

//O(n)
//Allocates and returns an array of elements in the set
void *getElements(SET *sp) {
	int i,j;
	assert(sp != NULL);
	void** elts=malloc(sizeof(void*)*sp->counter);
	assert(elts != NULL);
	
	for(i=0, j=0; i<sp->length; i++) {
		if(sp->flag[i]==filled) {
			elts[j++]=sp->data[i];
		}
	}
	//sorts the copied array
	quicksort(elts, 0, sp->counter-1, sp->compare);
	return elts;
}

//O(n)
//Partitions subarray around the pivot
static int partition(void** elts, int low, int high, int (*compare)()) {
	int i;
	void* pivot=elts[high];
	int smaller=low-1;
	
	//Swap element if it's less than or equal to the pivot
	for(i=low; i<=high-1; i++) {
		if((*compare)(elts[i], pivot) <= 0) {
			smaller++;
			void* temp=elts[smaller];
			elts[smaller]=elts[i];
			elts[i]=temp;
		}
	}
	//swaps the next element from smaller
	void* swap=elts[smaller+1];
	elts[smaller+1]=elts[high];
	elts[high]=swap;
	return smaller+1;
}

//O(n)
//Sorts the subarrays
static void quicksort(void** elts, int low, int high, int (*compare)()) {
	//Recursively partitions
	if(low<high) {
		int ind=partition(elts, low, high, compare);
		quicksort(elts, low, ind-1, compare);
		quicksort(elts, ind+1, high, compare);
	}
	return;
}
