#include "utilities.h"

#include <stdlib.h>

MapKeyElement copyInt(MapKeyElement n)
{
    if (!n)
    {
        return NULL;
    }

    int *copy = malloc(sizeof(*copy));
    if (!copy)
    {
        return NULL;
    }

    *copy = *(int *) n;

    return copy;
}

void freeInt(MapKeyElement n)
{
    free(n);
}

int compareInts(MapKeyElement n1, MapKeyElement n2) {
    return (*(int *) n1 - *(int *) n2);
}
