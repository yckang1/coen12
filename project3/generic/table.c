/*Yicheng Kang
 * COEN 12L 2:15pm
 * Project 3 Generic Implementation
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

struct set {
	int counter;
	int length;
	char **data;
	char *flags;
	int (*compare)();
	unsigned(*hash)();
};

static int search(SET  *sp, void* elt, bool *found);

//O(n)
//Creates a hashtable with identifier array
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
	SET *sp;
	int i;
	sp = malloc(sizeof(SET));
	assert(sp != NULL);

	sp->counter=0;
	sp->length=maxElts;
	sp->compare=compare;
	sp->hash=hash;
	//allocates the array of flags
	sp->flags=malloc(sizeof(char) *maxElts);
	assert(sp->flags != NULL);
	//allocates the array of data
	sp->data=malloc(sizeof(char *) *maxElts);
	assert(sp->data != NULL);
	
	//flags each data point as empty
	for(i=0;i<maxElts;i++) {
		sp->flags[i] = 'E';
	}
	return sp;
}

//O(n)
//frees data point, array, and hash table
void destroySet(SET *sp) {
	assert(sp != NULL);
	free(sp->data);
	free(sp->flags);
	free(sp);
}

//O(1)
//Using assert, returns the counter for total # of elements in set
int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->counter;
}

//O(1) average
//O(n) worst case
//adds element to the set
void addElement(SET *sp, void *elt) {
	assert((sp != NULL) & (elt != NULL));
	bool found;
	int ind=search(sp, elt, &found);
	//adds element if not found, increments counter
	if(!found) {
		sp->data[ind]=elt;
		sp->flags[ind]='F';
		sp->counter++;
	}
}

//O(1) average
//O(n) worst case
//removes element from set
void removeElement(SET *sp, void *elt) {
	assert((sp != NULL) & (elt != NULL));
	bool found;
	int ind=search(sp, elt, &found);
	//deletes element if found and decrements counter
	if(found) {
		sp->flags[ind]='D';
		sp->counter--;
	}
}

//O(1) average
//O(n) worst case
//searches for element in set
void *findElement(SET *sp, void *elt) {
	assert((sp != NULL) & (elt != NULL));
	bool found;
	int ind=search(sp, elt, &found);
	//returns null if element is not found
	if(!found) {
		return NULL;
	}
	return sp->data[ind];
}

//O(n)
//creates copy of elements in the set
void *getElements(SET *sp) {
	assert(sp != NULL);
	void **copydata;
	copydata=malloc(sizeof(void *)*sp->length);
	int i;
	int j=0;
	for(i=0;i<sp->length;i++) {
		if(sp->flags[i] == 'F') {
			copydata[j]=sp->data[i];
			j++;
		}
	}
	return copydata;
}

//O(1) average
//O(n) worst case
//returns the index if found or returns where element would be inserted
static int search(SET *sp, void *elt, bool *found) {
	assert((sp != NULL) && (elt != NULL));
	int ind=(*sp->hash)(elt) % sp->length;
	int deleted = -1;
	int i=0;
	int locn;
	while(i<sp->length) {
		locn=(ind+i) % (sp->length);
		//will note first deleted location
		if(sp->flags[locn]=='D') {
			if(deleted==-1) {
				deleted=locn;
			}
		}
		//if element not found, return deleted index
		else if(sp->flags[locn]=='E') {
			*found=false;
			if(deleted==-1) {
				return locn;
			}
			return deleted;
		}
		//if element is found
		else if((*sp->compare)(sp->data[locn],elt)==0) {
			*found=true;
			return locn;
		}
		i++;
	}
	*found=false;
	return deleted;
}
