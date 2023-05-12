#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
#include <stdlib.h>
/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------
/**
 *  Linked list representation -- R2 Curs2 (Slide 8)
 */
typedef struct ListNode{
	Item elem; // Stored node value
	struct ListNode* next; // link to next node
	struct ListNode* prev; // link to prev node
} ListNode;

/**
 *  Double linked list representation -- desen Curs 3 (Slide 19)
 */
typedef struct List{
	ListNode* first; // link to the first node
	ListNode* last; // link to the last node
}List;
// -----------------------------------------------------------------------------

/**
 * Create a new node
 *  [input]: Item
 *  [output]: List*
 */
ListNode *createNode(Item elem) {
    ListNode *newNode = malloc(sizeof(ListNode));
    newNode->elem = elem;
    newNode->next = NULL;
    newNode->prev = NULL;
	return newNode;
}

/**
 * Create a new (empty)  list  -- Silde-urile 13/14 Curs 2
 *  [input]: None
 *  [output]: List*
 */
List* createList(void){
    List *newList = malloc(sizeof(List));
    newList->first = NULL;
    newList->last = NULL;
	return newList;
}
// -----------------------------------------------------------------------------


/**
 * Determines if the list is empty
 *  [input]: List*
 *  [output]: 1 - empty/ 0 - not empty
 */
int isEmpty(List *list){
	if (list->first == NULL && list->last == NULL) return 1;
	return 0;
}
// -----------------------------------------------------------------------------


/**
 * Determines if a list contains a specified element
 *  [input]: List*, Item
 *  [output]: int (1 - contains/ 0 - not contains)
 */
int contains(List *list, Item elem){
	ListNode *temp = list->first;

    while (temp != NULL) {
        if (temp->elem == elem)
            return 1;
        temp = temp->next;
    }
    return 0;
}
// -----------------------------------------------------------------------------


/**
 * Insert a new element in the list at the specified position.
 * Note: Position numbering starts with the position at index zero
 *  [input]: List*, Item, int
 *  [output]: void
 */
void insertAt(List* list, Item elem, int pos){
    if (pos < 0) return;
	// Guard against young player errors
	if(list == NULL) return;
    if (!list->first || !list->last) return;

    ListNode *newNode = createNode(elem);
    if (pos == 0) {
        newNode->next = list->first;
        newNode->prev = list->first->prev;
        list->first->prev = newNode;
        list->first = newNode;
    }
    else {
        ListNode *temp = list->first;
        while(--pos) {
            temp = temp->next;
        }
        temp->prev->next = newNode;
        newNode->prev = temp->prev;
        newNode->next = temp;
        temp->prev = newNode;
    }
}
// -----------------------------------------------------------------------------


/**
 * Delete the first element instance from a list.
 *  [input]: List*, Item
 *  [output]: void
 */
void deleteOnce(List *list, Item elem){
	// Guard against young player errors
	if(list == NULL) return;

    ListNode *temp = list->first;
    if (temp == NULL) return;

    while (temp->next) {
        if (temp->elem == elem) {
            if (temp == list->first) {
                list->first = list->first->next;
                list->first->prev = NULL;
                temp->next = NULL;
                free(temp);
                return;
            }
            else {
                if (temp->next == NULL) {
                    temp->prev->next = NULL;
                    temp->prev = NULL;
                    free(temp);
                    return;
                }
                else {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                    temp->next = NULL;
                    temp->prev = NULL;
                    free(temp);
                    return;
                }
            }
        }
        temp = temp->next;
    }
}
// -----------------------------------------------------------------------------


/**
 * Compute list length
 *  [input]: List*
 *  [output]: int
 */
int length(List *list){
	// Guard against young player errors
    if(list == NULL) return 0;

    ListNode *temp = list->first;
    if (temp == NULL || list->last == NULL) return 0;
    else {
        int len = 0;
        while (temp != NULL) {
            len++;
            temp = temp->next;
        }
        return len;
    }
}

// -----------------------------------------------------------------------------



/**
 * Destroy a list.
 *  [input]: List*
 *  [output]: void
 */
List* destroyList(List* list){
	// Guard against young player errors
	if(list == NULL) return NULL;

	ListNode *temp = NULL;
    while(list->first != NULL) {
        temp = list->first;
        temp->next = NULL;
        temp->prev = NULL;
        list->first = list->first->next;
        free(temp);
    }
    list = NULL;
	return list;
}
// -----------------------------------------------------------------------------


#endif //_DOUBLE_LINKED_LIST_H_
