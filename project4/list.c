/*Yicheng Kang
 * COEN 12L T 2:15pm
 * Project 4
 */

#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct list {
	int (*compare)();
	int count;
	struct node *head;
};

struct node {
	void *data;
	struct node *prev;
	struct node *next;
};

typedef struct node NODE;

static NODE *search(LIST *lp, void *item);

//O(1)
//allocates memory for list and dummy node
LIST *createList (int(*compare) ()) {
	LIST *lp;
	lp=malloc(sizeof(LIST));
	assert(lp != NULL);
	NODE *head;
	head = malloc(sizeof(NODE));
	assert(head != NULL);
	lp ->head=head;
	lp ->head->prev = lp->head;
	lp ->head->next = lp->head;
	lp ->count = 0;
	lp ->compare=compare;
	return lp;
}

//O(n)
//frees dummy node and data
void destroyList (LIST *lp) {
	assert(lp != NULL);
	NODE *np = lp->head;
	while (np->next != np) {
		np->next = np->next->next;
		free(np -> next -> prev);
		np->next->prev = np;
		lp -> count--;
	}
	free(lp -> head);
	free(lp);
}

//O(1)
//returns the counter
int numItems(LIST *lp) {
	assert(lp != NULL);
	return lp->count;
}

//O(1)
//adds first element,sets data to item in new node, node is placed after the dummy
void addFirst(LIST *lp, void *item) {
	assert(lp != NULL && item != NULL);
	NODE *np;
	np=malloc(sizeof(NODE));
	assert(np != NULL);
	np->data=item;
	np->prev=lp->head;
	np->next=lp->head->next;
	lp->head->next->prev = np;
	lp->head->next = np;
	lp->count++;
}

//O(1)
//adds last element, sets data to item in new node placed after the dummy
void addLast(LIST *lp, void *item) {
	assert(lp != NULL && item != NULL);
	NODE *np;
	np = malloc(sizeof(NODE));
	assert(np != NULL);
	np->data=item;
	np->next=lp->head;
	np->prev=lp->head->prev;
	lp->head->prev->next=np;
	lp->head->prev=np;
	lp->count++;
}

//O(1)
//saves data and removes the first item
void *removeFirst(LIST *lp) {
	assert(lp != NULL);
	NODE *np=lp->head;
	void *ret=np->next->data;
	assert(np->next != np);
	np->next=np->next->next;
	free(np->next->prev);
	np->next->prev=np;
	lp->count--;
	return ret;
}

//O(1)
//saves data and removes the last item
void *removeLast(LIST *lp) {
	assert(lp != NULL);
	NODE *np=lp->head;
	void *ret=np->prev->data;
	assert(np->prev != np);
	np->prev=np->prev->prev;
	free(np->prev->next);
	np->prev->next=np;
	lp->count--;
	return ret;
}

//O(1)
//returns first node's data
void *getFirst(LIST *lp) {
	assert(lp != NULL);
	return lp->head->next->data;
}

//O(1)
//returns last node's data
void *getLast(LIST *lp) {
	assert(lp != NULL);
	return lp->head->prev->data;
}

//O(n)
//removes item in the list if it's found
//frees node and decrements the counter
void removeItem(LIST *lp, void *item) {
	assert(lp != NULL && lp->compare != NULL);
	NODE *elt=search(lp, item);
	if(elt != NULL) {
		elt->prev->next=elt->next;
		elt->next->prev=elt->prev;
		free(elt);
		lp->count--;
	}
}

//O(n)
//if found, returns list data
void *findItem(LIST *lp, void *item) {
	assert(lp != NULL && item != NULL && lp->compare != NULL);
	NODE *np=search(lp, item);
	if(np==NULL)
		return NULL;
	return np->data;
}

//O(n)
//gets a copy of the data and traverses list from first to last
void *getItems(LIST *lp) {
	printf("getitems\n");
	assert(lp != NULL);
	void **items;
	items = malloc(sizeof(void *)*lp->count);
	assert(items != NULL);
	int i=0;
	NODE *np=lp->head->next;
	while(np != lp->head) {
		items[i]=np->data;
		np=np->next;
		i++;
	}
	return items;
}

//O(n)
//sequential search to find node with desired item
static NODE *search(LIST *lp, void *item) {
	NODE *np=lp->head->next;
	while(np != lp->head) {
		if((*lp->compare) (np->data, item)==0) {
			return np;
		}
		np=np->next;
	}
	return NULL;
}
