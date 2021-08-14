#include "map.h"

#include <stdlib.h>

#define BAD_ARGUMENT_NULL_MAP -1

#define EMPTY_MAP 0

#define KEY_COMPARE_EQUAL 0

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
static Node findKeyElement(Map map, MapKeyElement keyElement);

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

    Node iterator = map->head;

    while (NULL != iterator)
    {
        Node iterator_next = iterator->next;

        map->freeKeyElement(iterator->key);
        map->freeDataElement(iterator->data);

        iterator = iterator_next;
    }

    map->head = NULL;

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

    // Search if element with given key already exists in the map
    while (NULL != *iterator)
    {
        if (KEY_COMPARE_EQUAL == map->compareKeyElements((*iterator)->key, keyElement))
        {
            MapDataElement data_copy = map->copyDataElement(dataElement);
            if (NULL == data_copy)
            {
                return MAP_OUT_OF_MEMORY;
            }

            map->freeDataElement((*iterator)->data);

            (*iterator)->data = data_copy;

            return MAP_SUCCESS;
        }

        iterator = &((*iterator)->next);
    }

    // In case element with given key doesn't exist, add new key-element pair to map
    Node node_new = malloc(sizeof(*node_new));
    if (NULL == node_new)
    {
        return MAP_OUT_OF_MEMORY;
    }

    MapKeyElement key_copy = map->copyKeyElement(keyElement);
    if (NULL == key_copy)
    {
        free(node_new);
        return MAP_OUT_OF_MEMORY;
    }

    MapDataElement data_copy = map->copyDataElement(dataElement);
    if (NULL == data_copy)
    {
        free(node_new);
        map->freeKeyElement(key_copy);
        return MAP_OUT_OF_MEMORY;
    }

    node_new->next = NULL;
    node_new->key = key_copy;
    node_new->data = data_copy;

    (*iterator) = node_new;

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
