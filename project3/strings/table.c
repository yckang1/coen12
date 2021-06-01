/*Yicheng Kang
 * COEN12L T 2:15pm
 * Project 3
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

struct set {
	int length;
	int counter;
	char *flag;
	char* *data;
};

static unsigned strhash(char *s);
static int search(SET* sp, char* elt, bool *found);

//O(n)
//Makes a hashtable with an array
SET *createSet(int maxElts) {
	SET *sp;
	int i;
	sp=malloc(sizeof(SET));
	assert(sp != NULL);

	sp->counter=0;			//Sets the counter to 0 initially
	sp->length=maxElts;		//Sets length to maximum elements
	sp->flag=malloc(sizeof(char) *maxElts);
	assert(sp->flag != NULL);
	sp->data=malloc(sizeof(char *) * maxElts);
	assert(sp->data != NULL);

	//assigns an empty flag to each element in array
	for(i=0; i<maxElts; i++) {
		sp->flag[i]='E';
	}
	return sp;
}

//O(n)
//function to free data and array and destroy set
void destroySet(SET *sp) {
	assert(sp != NULL);
	int i;
	for(i=0; i<sp->length; i++) {
		if(sp->flag[i]=='F') {
			free(sp->data[i]);
		}
	}
	//frees the data, flag, and set
	free(sp->data);
	free(sp->flag);
	free(sp);
}

//O(1)
//gets the counter for total number of elements in set
int numElements(SET *sp) { 
	assert(sp != NULL);
	return sp->counter;
}

//O(n) worst case
//O(1) average
//adds the element to set
void addElement(SET *sp, char *elt) {
	assert((sp != NULL) & (elt != NULL));
	bool found;
	char *newElt;
	//searches for element with new variable ind
	int ind=search(sp, elt, &found);

	//adds element if not found
	//increments the counter and marks flag as filled
	if(!found) {
		newElt=strdup(elt);
		assert(newElt != NULL);
		sp->data[ind]=newElt;
		sp->flag[ind]='F';
		sp->counter++;
	}
}

//O(n) worst case
//O(1) average
//removes the element from set
void removeElement(SET *sp, char *elt) {
	assert((sp != NULL) & (elt != NULL));
	bool found;
	int ind=search(sp, elt, &found);
	
	//removes element if found
	//decrements the counter and marks flag as deleted
	if(found) {
		free(sp->data[ind]);
		sp->flag[ind]='D';
		sp->counter--;
	}
}

//O(n) worst case
//O(1) average
//finds the element in set
char *findElement(SET *sp, char *elt) {
	assert((sp != NULL) & (elt != NULL));
	bool found;
	int ind=search(sp, elt, &found);
	
	//returns pointer to element if found
	if(!found) {
		return NULL;
	}
	return sp->data[ind];
}

//Runtime O(n)
//creates copy of elements in the set
char **getElements(SET *sp) {
	assert(sp != NULL);
	char **copydata;

	copydata=malloc(sizeof(char *)*sp->length);
	int i;
	int j=0;

	for(i=0;i<sp->length;i++) {
		if(sp->flag[i]=='F') {
			copydata[j] = sp->data[i];
			j++;
		}
	}
	return copydata;
}

//Runtime O(n) worst case
//Runtime O(1) average
//linear probing to search for element in the set with hash function
static int search(SET *sp, char *elt, bool *found) {
	assert((sp != NULL) && (elt != NULL));
	int ind=strhash(elt)%sp->length;
	int delete=-1;
	int i=0;
	int locn;

	//returns index if found
	while(i<sp->length) {
		locn=(ind+i) % (sp->length);
		if(sp->flag[locn]=='D') {
			if(delete==-1) {
				delete=locn;
			}
		}
		//else if to stop searching
		else if(sp->flag[locn]=='E') {
			*found=false;
			if(delete==-1) {
				return locn;
			}
			return delete;
		}
		//else if for if case is found
		else if(strcmp(sp->data[locn], elt)==0) {
			*found=true;
			return locn;
		}
		i++;
	}
	
	*found=false;
	return delete;
}

//Runtime O(1)
//maps string in an index of array with hash function
static unsigned strhash(char *s) {
	unsigned hash=0;
	assert(s != NULL);
	while(*s != '\0')
		hash=31*hash+*s++;
	return hash;
}
