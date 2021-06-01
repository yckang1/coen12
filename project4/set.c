/*Yicheng Kang
 * COEN12L T 2:15pm
 * Project 4
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "set.h"
#define EXP 20

struct set{
	LIST ** list;
	int counter;
	int length;
	int (*compare)();
	unsigned (*hash)();
};

//O(n)
//Creates hash table with ADT list at a hashed location
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)() ) {
	SET *sp;
	sp=malloc(sizeof(SET));
	assert(sp != NULL);
	sp->counter=0;
	sp->length=(maxElts/EXP);
	sp->compare=compare;
	sp->hash=hash;
	sp->list=malloc(sizeof(LIST*) *sp->length);		//allocates the data array
	assert(sp->list != NULL);
	int i;
	for(i=0; i<sp->length; i++) {
		sp->list[i]=createList(compare);			//creates a list at hashed location
	}
	return sp;
}

//O(n)
//Frees data in hash table and lists
void destroySet(SET *sp) {
	assert(sp != NULL);
	int i;
	for(i=0; i<sp->length; i++) {
		destroyList(sp->list[i]);
	}
	free(sp->list);
	free(sp);
}

//O(1)
//Gets number of elements in set
int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->counter;
}

//O(1) average
//O(n) worst case
//Adds an element to the list and uses an index for hashing
void addElement(SET *sp, void *elt) {
	assert((sp != NULL) && (elt != NULL));
	int ind=(*sp->hash)(elt) % sp->length;
	void *data=findItem(sp->list[ind], elt);
	//Increments counter if found
	if(data==NULL) {
		addFirst(sp->list[ind], elt);
		sp->counter++;
	}
}

//O(1) average
//O(n) worst case
//Removes an element from the set and uses an index for hashing
void removeElement(SET *sp, void *elt) {
	assert((sp != NULL) && (elt != NULL));
	int ind=(*sp->hash)(elt) % sp->length;
	void *data=findItem(sp->list[ind], elt);
	//if not found, decrement counter
	if(data != NULL) {
		removeItem(sp->list[ind], elt);
		sp->counter--;
	}
}

//O(1) average
//O(n) worst case
//Looks for an element in the set
void *findElement(SET *sp, void *elt) {
	assert((sp != NULL) && (elt != NULL));
	int ind=(*sp->hash)(elt) % sp->length;
	return findItem(sp->list[ind], elt);
}

//O(n)
//Creates a copy of the elements in the set
void *getElements(SET *sp) {
	printf("getelements\n");
	assert(sp != NULL);
	void **copydata;
	copydata=malloc(sizeof(void *)*sp->length*EXP);
	void **dataInsert;
	int numInsert;
	int i;
	int size;
	int ind=0;
	for(i=0; i<sp->length; i++) {
		dataInsert=getItems(sp->list[i]);
		printf("getelementsloop\n");
		numInsert=numItems(sp->list[i]);
		size=sizeof(getFirst(sp->list[i]));
		memcpy(copydata+ind, dataInsert, numInsert*sizeof(void *));
		ind += numInsert;
	}
	return copydata;
}
