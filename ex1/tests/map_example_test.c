#include "../test_utilities.h"
#include <stdlib.h>
#include "../mtm_map/map.h"

#define NUMBER_TESTS 7

/** Function to be used for copying an int as a key to the map */
static MapKeyElement copyKeyInt(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) n;
    return copy;
}

/** Function to be used for copying a char as a data to the map */
static MapDataElement copyDataChar(MapDataElement n) {
    if (!n) {
        return NULL;
    }
    char *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(char *) n;
    return (MapDataElement) copy;
}

/** Function to be used by the map for freeing elements */
static void freeInt(MapKeyElement n) {
    free(n);
}

/** Function to be used by the map for freeing elements */
static void freeChar(MapDataElement n) {
    free(n);
}

/** Function to be used by the map for comparing elements */
static int compareInts(MapKeyElement n1, MapKeyElement n2) {
    return (*(int *) n1 - *(int *) n2);
}

bool testMapCreateDestroy() {
    Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetFirst(map) == NULL);
    mapDestroy(map);
    return true;
}

bool testMapAddAndSize() {
    Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
    for (int i = 1; i < 1000; ++i) {
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map, &i, &j) == MAP_SUCCESS);
        ASSERT_TEST(mapGetSize(map) == i);
    }
    mapDestroy(map);
    return true;
}

bool testMapGet() {
    Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
    for (int i = 1; i < 1000; ++i) {
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map, &i, &j) == MAP_SUCCESS);
        ASSERT_TEST(mapGetSize(map) == i);
    }

    for (int i = 1; i < 1000; ++i) {
        char j = (char) i;
        ++j;
        char *getVal = (char *) mapGet(map, &i);
        ASSERT_TEST(*getVal == j);
    }
    int i = 0;
    ASSERT_TEST(mapGet(map, &i) == NULL);
    i = 1000;
    ASSERT_TEST(mapGet(map, &i) == NULL);
    mapDestroy(map);
    return true;
}

bool testIterator() {
    Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
    for (int i = 1; i < 400; ++i) {
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map, &i, &j) == MAP_SUCCESS);
    }

    for (int i = 800; i >= 400; --i) {
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map, &i, &j) == MAP_SUCCESS);
    }

    for (int i = 801; i < 1000; ++i) {
        char j = (char) i;
        ++j;
        ASSERT_TEST(mapPut(map, &i, &j) == MAP_SUCCESS);
    }

    int i = 1;
    MAP_FOREACH(int *, iter, map) {
      ASSERT_TEST(*iter == i);
      freeInt(iter);
      i++;
    }

    mapDestroy(map);
    return true;
}

bool testBadArguments()
{
    // mapCreate

    Map map = mapCreate(NULL, copyKeyInt, freeChar, freeInt, compareInts);
    ASSERT_TEST(map == NULL);

    map = mapCreate(copyDataChar, NULL, freeChar, freeInt, compareInts);
    ASSERT_TEST(map == NULL);

    map = mapCreate(copyDataChar, copyKeyInt, NULL, freeInt, compareInts);
    ASSERT_TEST(map == NULL);

    map = mapCreate(copyDataChar, copyKeyInt, freeChar, NULL, compareInts);
    ASSERT_TEST(map == NULL);

    map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, NULL);
    ASSERT_TEST(map == NULL);

    // mapGetFirst

    ASSERT_TEST(mapGetFirst(NULL) == NULL);

    // mapGet

    map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
    ASSERT_TEST(map != NULL);

    int key;

    ASSERT_TEST(mapGet(NULL, &key) == NULL);
    ASSERT_TEST(mapGet(map, NULL) == NULL);
    ASSERT_TEST(mapGet(map, &key) == NULL);

    // mapPut

    char data;

    ASSERT_TEST(mapPut(NULL, &key, &data) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapPut(map, NULL, &data) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapPut(map, &key, NULL) == MAP_NULL_ARGUMENT);

    // mapRemove

    ASSERT_TEST(mapRemove(NULL, &key) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapRemove(map, NULL) == MAP_NULL_ARGUMENT);

    mapDestroy(map);

    return true;
}

bool testPrivateMapPut()
{

    Map map;

    int key;
    char data;

    int *key_pointer;

    map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

    key = 1;
    data = 1;
    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    key_pointer = (int *)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 1);
    free(key_pointer);

    data = 2;
    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    key_pointer = (int *)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 2);
    free(key_pointer);

    key = 4;
    data = 4;
    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 2);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 2);
    free(key_pointer);
    key_pointer = (int*)mapGetNext(map);
    ASSERT_TEST(*key_pointer == 4);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 4);
    free(key_pointer);

    key = 3;
    data = 3;
    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 3);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 2);
    free(key_pointer);
    key_pointer = (int*)mapGetNext(map);
    ASSERT_TEST(*key_pointer == 3);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 3);
    free(key_pointer);
    key_pointer = (int*)mapGetNext(map);
    ASSERT_TEST(*key_pointer == 4);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 4);
    free(key_pointer);

    key = 5;
    data = 5;
    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 4);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 2);
    free(key_pointer);
    key_pointer = (int*)mapGetNext(map);
    ASSERT_TEST(*key_pointer == 3);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 3);
    free(key_pointer);
    key_pointer = (int*)mapGetNext(map);
    ASSERT_TEST(*key_pointer == 4);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 4);
    free(key_pointer);
    key_pointer = (int*)mapGetNext(map);
    ASSERT_TEST(*key_pointer == 5);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 5);
    free(key_pointer);

    mapDestroy(map);

    map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

    key = 2;
    data = 2;
    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 2);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 2);
    free(key_pointer);

    key = 1;
    data = 1;
    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 2);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 1);
    free(key_pointer);
    key_pointer = (int*)mapGetNext(map);
    ASSERT_TEST(*key_pointer == 2);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 2);
    free(key_pointer);

    mapDestroy(map);

    return true;
}

bool testPrivateMapRemove()
{

    Map map;

    int key;
    char data;

    int *key_pointer;

    map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

    key = 1;
    data = 1;
    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 1);
    free(key_pointer);
    ASSERT_TEST(mapRemove(map, &key) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 0);

    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 1);
    free(key_pointer);
    key = 2;
    data = 2;
    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 2);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 1);
    free(key_pointer);
    key_pointer = (int*)mapGetNext(map);
    ASSERT_TEST(*key_pointer == 2);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 2);
    free(key_pointer);
    key = 2;
    ASSERT_TEST(mapRemove(map, &key) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 1);
    free(key_pointer);

    ASSERT_TEST(mapPut(map, &key, &data) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 2);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 1);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 1);
    free(key_pointer);
    key_pointer = (int*)mapGetNext(map);
    ASSERT_TEST(*key_pointer == 2);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 2);
    free(key_pointer);
    key = 1;
    ASSERT_TEST(mapRemove(map, &key) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    key_pointer = (int*)mapGetFirst(map);
    ASSERT_TEST(*key_pointer == 2);
    ASSERT_TEST(*(char*)mapGet(map, key_pointer) == 2);
    free(key_pointer);

    mapDestroy(map);

    return true;
}

/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
        testMapCreateDestroy,
        testMapAddAndSize,
        testMapGet,
        testIterator,
        testBadArguments,
        testPrivateMapPut,
        testPrivateMapRemove
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
        "testMapCreateDestroy",
        "testMapAddAndSize",
        "testMapGet",
        "testIterator",
        "testBadArguments",
        "testPrivateMapPut",
        "testPrivateMapRemove"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: <your_executable_file_name> <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}
