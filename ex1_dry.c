#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef struct node_t {
    int x;
    struct node_t *next;
} *Node;

typedef enum {
    SUCCESS=0,
    MEMORY_ERROR,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);

Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code)
{
    if (NULL == error_code)
    {
        return NULL;
    }

    if ((NULL == list1) || (NULL == list2))
    {
        *error_code = NULL_ARGUMENT;
        return NULL;
    }

    if (!isListSorted(list1) || !isListSorted(list2))
    {
        *error_code = UNSORTED_LIST;
        return NULL;
    }

    Node list_merged = NULL;
    Node node_previous = NULL;

    bool is_first_node = true;
   
    while ((NULL != list1) || (NULL != iterator2))
    {
        Node node_new = malloc(sizeof(*node_new));
        if (NULL == node_new)
        {
            Node iterator = list_merged;
            while (NULL != iterator)
            {
                Node temp = iterator->next;
                free(iterator);
                iterator = temp;
            }

            *error_code = MEMORY_ERROR;
            return NULL;
        }

        node_new->next = NULL:

        if (is_first_node)
        {
            list_merged = node_new;
            is_first_node = false;
        }
        else
        {
            node_previous->next = node_new;
        }

        node_previous = node_new;

        if (NULL == list1)
        {
            node_new->x = list2->x;
            list2 = list2->next;
        }
        else if (NULL == list2)
        {
            node_new->x = list1->x;
            list1 = list1->next;
        }
        else if (list1->x < list2->x)
        {
            node_new->x = list1->x;
            list1 = list1->next;
        }
        else
        {
            node_new->x = list2->x;
            list2 = list2->next;
        }
    }

    *error_code = SUCCESS;
    return list_merged;
}
