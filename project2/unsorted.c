/* Yicheng Kang
 * COEN 12L T 2:15pm
 * Project 2
 * 4/6/21
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "set.h"

struct set {
	int address;	//Array's address
	int length;	//Array length
	char* *data;	//Number of elements
};

typedef struct set SET;

static int search(SET *sp, char *elt);

//Returns pointer to a new set with max capacity maxElts
SET *createSet(int maxElts) {
	SET *sp;
	sp=malloc(sizeof(SET));
	assert(sp!=NULL);

	sp->address = 0;
	sp->length = maxElts;
	sp->data=malloc(sizeof(char *) *maxElts);
	assert(sp->data!=NULL);
	return sp;
}

//Deallocates memory associated with set pointed by sp
void destroySet(SET *sp) {
	int i;
	assert(sp != NULL);
	
	//frees the data, array of pointers, and the set
	for(i=0; i<sp->address; i++) {
		free(sp->data[i]);
	}
	free(sp->data);
	free(sp);
}

//Returns the number of elements in set pointed by sp
int numElements(SET *sp) {
	assert(sp != NULL);
	return(sp -> address);
}

//Adds elt to the set pointed by sp
void addElement(SET *sp, char *elt) {
	assert((elt!=NULL)&&(sp!=NULL)&&(sp->length>sp->address));
	int pos;
	char * NewElt;
	pos=search(sp, elt);

	//adds to the set if not able to find
	if(pos==-1) {
		NewElt=strdup(elt); 	//copies string and returns a pointer
		assert(NewElt != NULL);
		sp->data[sp->address++] = NewElt;	//increments the counter
	}
}

//Removes elt from the set pointed by sp
void removeElement(SET *sp, char *elt) {
	assert((sp!=NULL)&&(elt!=NULL)); 	//asserts the ability to remove element
	int loc;
	loc=search(sp, elt);	//searches for element
	
	if(loc != -1) {
		free(sp->data[loc]);
		sp->data[loc] = sp->data[--sp->address];	//decrements the counter
	}
}

//Returns matching element if elt is present
char *findElement(SET *sp, char *elt) {
	assert((sp != NULL)&&(elt != NULL));	//asserts the ability to find element
	int loc;
	loc = search(sp, elt);	//searches for element

	if(loc == -1)
		return NULL;
	return sp->data[loc];	//returns point to location
}

//Allocates and returns an array of elements
char **getElements(SET *sp) {
	assert(sp!=NULL);	//asserts the ability to get element
	char ** copydata;	//memory for copy
	copydata = malloc(sizeof(char *)*sp->length);
	assert(copydata != NULL);
	int i;
	for(i=0; i<sp->address; i++)
		copydata[i]=sp->data[i];
	return copydata;
}

//Looks for elt and returns location if found or returns -1 if not
static int search(SET *sp, char *elt) {
	int i;
	for(i=0; i<sp->address; i++) {
		if(strcmp(sp->data[i], elt) == 0)
			return i;
	}
	return -1;
}
