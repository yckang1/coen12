/*Yicheng Kang
 * COEN 12 Term Project
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

//Node structure definition with 2 pointers
typedef struct node {
	void **data;
	int first;
	int length;
	int counter;
	struct node *next;
	struct node *prev;
}NODE;

//List structure definition with both head and tail pointers and doubly linked
typedef struct list {
	int counter;
	NODE* head;
	NODE* tail;
}LIST;

NODE *createNode(int length);

//O(1)
//Returns the list pointer and allocates memory for each list pointer
LIST *createList(void) {
	LIST *lp = malloc(sizeof(LIST));
	lp->head = lp->tail = createNode(8);
	lp->counter = 0;
	return lp;
}

//O(n)
//Traverses list with head pointer and frees each node
void destroyList(LIST *lp) {
	assert(lp != NULL);
	NODE* pDelete;
	while(lp->head != NULL) {
		pDelete = lp->head;
		lp->head  = lp->head->next;
		free(pDelete->data);
		free(pDelete);
	}
	free(lp);
}

//O(1)
//Returns the number of items in the list that's pointed to by lp
int numItems(LIST *lp) {
	assert(lp != NULL);
	return lp->counter;
}

//O(1)
//Checks if the first node is full, insert the item into the first available slot, and increment count using head
void addFirst(LIST *lp, void *item) {
	assert(lp != NULL && item != NULL);

	if(lp->head->counter == lp->head->length) {
		NODE *pFirst = createNode(lp->head->length*2);
		pFirst->next=lp->head;
		lp->head=pFirst;
		pFirst->next->prev=pFirst;
	}
	
	//Initialize first index of array
	int index=((lp->head->first + lp->head->length-1) % lp->head->length);

	//Inserts item into first slot with series of pointers
	lp->head->data[index] = item;
	lp->head->first = index;
	lp->head->counter++;
	lp->counter++;
}

//O(1)
//Checks if the last node is full and inserts item into last slot possible using tail
void addLast(LIST *lp, void *item) {
	assert(lp != NULL &&  item != NULL);

	//Node with double the array's length
	if(lp->tail->counter == lp->tail->length) {
		NODE* pLast = createNode(lp->tail->length*2);
		pLast->prev = lp->tail;
		lp->tail->next=pLast;
		lp->tail = pLast;
	}

	//Initialize last index of array
	int index=((lp->tail->first + lp->tail->counter) % lp->tail->length);

	//Inserts item into last slot with series of pointers
	lp->tail->data[index]=item;
	lp->tail->counter++;
	lp->counter++;
}

//O(1)
//Checks if array is empty. If empty, points to null. If not, frees node and creates index to set the first item in the list to and then returns that element
void *removeFirst(LIST *lp) {
	assert(lp != NULL);

	//If statement to check if array is empty
	if(lp->head->counter==0) {
		lp->head = lp->head->next;
		free(lp->head->prev);
		lp->head->prev=NULL;
	}

	int index = lp->head->first;
	int indextwo = (index+1)%lp->head->length;
	lp->head->first = indextwo;

	//Copy the array data
	void *copy = lp->head->data[index];

	//Decrements counter
	lp->head->counter--;
	lp->counter--;

	return copy;
}

//O(1)
//Checks if array is empty. If empty, points to null. If not, frees node and creates an index to set the last item in the list to and returns that element
void *removeLast(LIST *lp) {
	assert(lp != NULL);

	//If statement to check if the array is empty. Also frees node and its data
	if(lp->tail->counter==0) {
		NODE* pDelete=lp->tail;
		lp->tail = lp->tail->prev;
		lp->tail->next=NULL;
		free(pDelete->data);
		free(pDelete);
	}

	//Index of last item
	int index = ((lp->tail->first + lp->tail->counter)%lp->tail->length);

	//Copy the array data
	void *copy = lp->tail->data[index];

	//Decrements counter
	lp->tail->counter--;
	lp->counter--;

	return copy;
}

//O(logn)
//Assert that the index is valid if its >=0. If it's less than the counter, it'll decrement the counter while traversing the list and return the node in that index's position when it reaches there
void *getItem(LIST *lp, int index) {
	assert(lp != NULL);
	assert(index >= 0 && index < lp->counter);

	//Initializes a node to traverse list
	NODE* pTraverse=lp->head;

	//Decrements the counter while traversing the list
	while(index >= pTraverse->counter) {
		index -= pTraverse->counter;
		pTraverse = pTraverse->next;
	}

	void* copy = pTraverse->data[(pTraverse->first+index)%pTraverse->length];
	return copy;
}

//O(logn)
//Assert that the index is valid if its >=0. Decrements counter while traversing the list and instead of returning the node at index's position, it inserts the item at index.
void setItem(LIST *lp, int index, void *item) {
	assert(lp != NULL && item != NULL);
	assert(index >= 0 && index < lp->counter);

	//Initializes a node to traverse list
	NODE* pTraverse =  lp->head;

	//Decrements the counter while traversing the list
	while(index >= pTraverse->counter) {
		index -= pTraverse->counter;
		pTraverse = pTraverse->next;
	}

	//Inserts item at index
	pTraverse->data[(pTraverse->first+index)%pTraverse->length] = item;
}

//O(1)
//createNode function to allocate memory for the node and array
NODE *createNode(int length) {
	NODE *np = malloc(sizeof(NODE));
	assert(np != NULL);
	np->data=malloc(sizeof(void**) * length);
	np->counter=0;
	np->first=0;
	np->length=length;
	np->next=NULL;
	np->prev=NULL;
	return np;
}
