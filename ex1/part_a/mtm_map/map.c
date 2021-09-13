#include "map.h"

#include <stdlib.h>

#define BAD_ARGUMENT_NULL_MAP -1

#define EMPTY_MAP 0

#define KEY_COMPARE_EQUAL 0
#define KEY_COMPARE_FIRST_GREATER(result) result > KEY_COMPARE_EQUAL ? true : false

typedef struct node {
    struct node* next;
    MapKeyElement key;
    MapDataElement data;
} *Node;

struct Map_t {
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;

    Node head;

    Node current_key_element;
};

// Interenal function
// Assumes all arguments are checked
// Returns pointer for Node in the map with key uquals to keyElement
// If no element found, return NULL
static Node findKeyElement(Map map, MapKeyElement keyElement)
{
    Node iterator = map->head;

    while (NULL != iterator)
    {
        if (KEY_COMPARE_EQUAL == map->compareKeyElements(iterator->key, keyElement))
        {
            break;
        }

        iterator = iterator->next;
    }
    
    return iterator;
}

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements)
{
    if (NULL == copyDataElement ||
        NULL == copyKeyElement ||
        NULL == freeDataElement ||
        NULL == freeKeyElement ||
        NULL == compareKeyElements)
    {
        return NULL;
    }

    Map map = malloc(sizeof(*map));
    if (NULL == map)
    {
        return NULL;
    }

    map->copyDataElement = copyDataElement;
    map->copyKeyElement = copyKeyElement;
    map->freeDataElement = freeDataElement;
    map->freeKeyElement = freeKeyElement;
    map->compareKeyElements = compareKeyElements;

    map->head = NULL;

    map->current_key_element = NULL;

    return map;    
}

void mapDestroy(Map map)
{
    if (NULL == map) {
        return;
    }

    mapClear(map);

    free(map);
}

int mapGetSize(Map map)
{
    if (NULL == map)
    {
        return BAD_ARGUMENT_NULL_MAP;
    }

    int size = 0;

    Node iterator = map->head;

    // ToDo: add size member to map, keep updated using the rest of the functions
    while (NULL != iterator)
    {
        size++;
        iterator = iterator->next;
    }

    return size;
}

MapKeyElement mapGetFirst(Map map)
{
    if (NULL == map)
    {
        return NULL;
    }

    // ToDo: use member istead, once implemented
    if (EMPTY_MAP == mapGetSize(map))
    {
        return NULL;
    }

    map->current_key_element = map->head;

    MapKeyElement key_copy = map->copyKeyElement(map->current_key_element->key);
    if (NULL == key_copy)
    {
        return NULL;
    }

    return key_copy;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    if ((NULL == map) || (NULL == keyElement) || (NULL == dataElement))
    {
        return MAP_NULL_ARGUMENT;
    }

    Node* iterator = &(map->head);

    bool is_to_stop_search = false;

    bool is_to_add_new = false;

    while (!is_to_stop_search)
    {
        if (NULL == *iterator)
        {
            // ToDo: corner case, only for empty list, solve otherwise
            is_to_stop_search = true;
            is_to_add_new = true;
        }
        else
        {
            int compare_result_current = map->compareKeyElements(keyElement, (*iterator)->key);

            if (KEY_COMPARE_EQUAL == compare_result_current)
            {
                is_to_stop_search = true;
            }
            else
            {
                bool is_last_element = NULL == (*iterator)->next;

                if (is_last_element)
                {
                    is_to_stop_search = true;
                    is_to_add_new = true;

                    if (compare_result_current > 0)
                    {
                       iterator = &((*iterator)->next);
                    }
                    else
                    {
                        // ToDo: corner case, only for list with one element, solve otherwise
                        iterator = &(map->head);
                    }
                }
                else
                {
                    int compare_result_next = map->compareKeyElements(keyElement, (*iterator)->next->key);

                    // ToDo: does go over all logical posibilities, implement otherwise, ihsa !!!
                    if ((compare_result_current > 0) && (compare_result_next < 0))
                    {
                        is_to_stop_search = true;
                        is_to_add_new = true;

                        iterator = &((*iterator)->next);
                    }
                    else
                    {
                        iterator = &((*iterator)->next);
                    }
                }
            }
        }
    }

    MapDataElement data_copy = map->copyDataElement(dataElement);
    if (NULL == data_copy)
    {
        return MAP_OUT_OF_MEMORY;
    }

    if (is_to_add_new)
    {
        Node node_new = malloc(sizeof(*node_new));
        if (NULL == node_new)
        {
            map->freeDataElement(data_copy);
            return MAP_OUT_OF_MEMORY;
        }

        MapKeyElement key_copy = map->copyKeyElement(keyElement);
        if (NULL == key_copy)
        {
            free(node_new);
            map->freeDataElement(data_copy);
            return MAP_OUT_OF_MEMORY;
        }

        node_new->next = *iterator;
        node_new->key = key_copy;
        node_new->data = data_copy;

        *iterator = node_new;
    }
    else
    {
        map->freeDataElement((*iterator)->data);
        (*iterator)->data = data_copy;
    }

    return MAP_SUCCESS;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if ((NULL == map) || (NULL == keyElement))
    {
        return NULL;
    }

    Node node = findKeyElement(map, keyElement);

    if (NULL == node)
    {
        return NULL;
    }

    return node->data;
}

MapKeyElement mapGetNext(Map map)
{
    if (NULL == map)
    {
        return NULL;
    }

    if (NULL == map->current_key_element)
    {
        return NULL;
    }

    map->current_key_element = map->current_key_element->next;
    if (NULL == map->current_key_element)
    {
        return NULL;
    }

    MapKeyElement key_copy = map->copyKeyElement(map->current_key_element->key);
    if (NULL == key_copy)
    {
        // ToDo: should we revert value of the current_key_element ?
        return NULL;
    }

    return key_copy;
}

MapResult mapRemove(Map map, MapKeyElement keyElement)
{
    if ((NULL == map) || (NULL == keyElement))
    {
        return MAP_NULL_ARGUMENT;
    }

    Node* iterator = &(map->head);

    while (NULL != *iterator)
    {
        if (KEY_COMPARE_EQUAL == map->compareKeyElements((*iterator)->key, keyElement))
        {
            Node next_node = (*iterator)->next;

            map->freeKeyElement((*iterator)->key);
            map->freeDataElement((*iterator)->data);
            free(*iterator);

            *iterator = next_node;

            return MAP_SUCCESS;
        }

        iterator = &((*iterator)->next);
    }

    return MAP_ITEM_DOES_NOT_EXIST;
}

MapResult mapClear(Map map)
{
    if (NULL == map)
    {
        return MAP_NULL_ARGUMENT;
    }

    Node iterator = map->head;

    while (NULL != iterator)
    {
        Node iterator_next = iterator->next;

        map->freeKeyElement(iterator->key);
        map->freeDataElement(iterator->data);
        
        free(iterator);

        iterator = iterator_next;
    }

    map->head = NULL;

    return MAP_SUCCESS;
}

bool mapContains(Map map, MapKeyElement element)
{
    if ((NULL == map) || (NULL == element))
    {
        return false;
    }

    Node iterator = map->head;

    while (NULL != iterator)
    {
        Node iterator_next = iterator->next;

        if (KEY_COMPARE_EQUAL == map->compareKeyElements(iterator->key, element))
        {
            return true;
        }

        iterator = iterator_next;
    }

    return false;
}

Map mapCopy(Map map)
{
    if (NULL == map)
    {
        return NULL;
    }

    Map map_new = malloc(sizeof(*map_new));
    if (NULL == map_new)
    {
        return NULL;
    }

    map_new->copyDataElement = map->copyDataElement;
    map_new->copyKeyElement = map->copyKeyElement;
    map_new->freeDataElement = map->freeDataElement;
    map_new->freeKeyElement = map->freeKeyElement;
    map_new->compareKeyElements = map->compareKeyElements;

    map_new->head = NULL;

    map_new->current_key_element = NULL;

    Node iterator = map->head;
    Node* iterator_previous = &(map_new->head);

    while (NULL != iterator)
    {
        Node iterator_new = malloc(sizeof(*iterator_new));
        if (NULL == iterator_new)
        {
            mapDestroy(map_new);
            return NULL;
        }
        
        *iterator_previous = iterator_new;

        iterator_new->next = NULL;

        iterator_new->key = map->copyKeyElement(iterator->key);
        iterator_new->data = map->copyDataElement(iterator->data);

        iterator = iterator->next;
        iterator_previous = &(iterator_new->next);
    }

    return map_new;    
}
