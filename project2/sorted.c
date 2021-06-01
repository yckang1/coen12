/* Yicheng Kang
 * COEN12L T 2:15pm
 * Project 2
 * 4/7/21
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

struct set {
	int length;
	int address;
	char* *data;
};

typedef struct set SET;
static int search(SET *sp, char *elt, bool *found);

SET *createSet(int maxElts) {
	SET *sp;
	sp=malloc(sizeof(SET));
	assert(sp != NULL);
	sp->address = 0;		//initializes to zero
	sp->length = maxElts;	//initialize
	sp->data = malloc(sizeof(char *) * maxElts);
	assert(sp->data != NULL);
	return sp;
}

//Frees the data and the set
void destroySet(SET *sp) {
	int i;
	for(i=0; i<sp->address; i++)
		free(sp->data[i]);
	free(sp->data);
	free(sp);
}

int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->address;
}

//Add strings to the set and search for new element
void addElement(SET *sp, char *elt) {
	assert((elt != NULL)&&(sp != NULL)&&(sp->length>sp->address));
	int i;
	int pos;
	char * NewElt;
	bool found;
	
	pos=search(sp, elt, &found);
	
	//if element not found, add element
	if(!found) {
		NewElt = strdup(elt);
		assert(NewElt != NULL);
		for(i=sp->address; i>pos; i--)
			sp->data[i] = sp->data[i-1];
		sp->data[pos] = NewElt;
		sp->address++;
	}
}

//Remove elements from the set and search to delete
void removeElement(SET *sp, char *elt) {
	assert((sp != NULL)&&(elt != NULL));
	int i;
	int loc;
	bool found;
	
	loc=search(sp, elt, &found);

	//remove element if found
	if(found) {
		free(sp->data[loc]);
		for(i=loc+1; i<sp->address; i++)
			sp->data[i-1] = sp->data[i];
		sp->address--;
	}
}

//Searches for elements
char *findElement(SET *sp, char *elt) {
	assert((sp != NULL)&&(elt != NULL));
	int loc;
	bool found;
	
	loc=search(sp, elt, &found);
	if(!found) {
		return NULL;
	}
	//returns pointer
	return sp->data[loc];
}

//Get elements, memory for data copy
char **getElements(SET *sp) {
	assert(sp != NULL);
	char ** copydata;

	copydata=malloc(sizeof(char *) * sp->length);

	assert(copydata != NULL);

	int i;
	for(i=0; i<sp->address; i++) {
		copydata[i]=sp->data[i];
	}
	return copydata;
}

static int search(SET *sp, char *elt, bool *found) {
	int lo;
	int mid;
	int hi;
	int diff;
	lo=0;
	hi=sp->address-1;

	//binary search, changes bounds of mid
	while(lo <= hi) {
		mid=(lo+hi)/2;
		diff=strcmp(elt, sp->data[mid]);
		if(diff<0) {
			hi=mid-1;
		}
		else if(diff>0) {
			lo=mid+1;
		}
		else {
			*found=true;
			return mid;
		}
	}
	*found = false;
	return lo;
}
