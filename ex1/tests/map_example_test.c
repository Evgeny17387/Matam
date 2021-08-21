#include "../test_utilities.h"
#include <stdlib.h>
#include "../mtm_map/map.h"
#include <string.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define NUMBER_TESTS 22

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

/** Function to be used for copying an int as a key to the map */
static MapKeyElement copyKeyString(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    char* copy = (char*)malloc(strlen(n) + 1);
   if (!copy) {
       return NULL;
    }
    copy = strcpy(copy,n);
    return (MapKeyElement) copy;
}

/** Function to be used for copying a char as a data to the map */
static MapDataElement copyDataString(MapDataElement n) {
    if (!n) {
        return NULL;
    }
    char* copy = (char*)malloc(strlen(n) + 1);
    if (!copy) {
        return NULL;
    }
    copy = strcpy(copy,n);
    return (MapDataElement) copy;
}

/** Function to be used by the map for freeing elements */
static void freeKeyString(MapKeyElement n) {
    free(n);
}

/** Function to be used by the map for freeing elements */
static void freeDataString(MapDataElement n) {
    free(n);
}

/** Function to be used by the map for comparing elements */
static int compareStrings(MapKeyElement n1, MapKeyElement n2) {

    return (strcmp(n1,n2));
}

bool testMapCreateDestroy()
{
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	ASSERT_TEST(map != NULL);
	ASSERT_TEST(mapGetSize(map) == 0);
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

bool testIterator()
{
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	for (int key = 1; key < 400; ++key)
	{
		char val = (char)key;
		++val;
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	for (int key = 800; key >= 400; --key)
	{
		char val = (char)key;
		++val;
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	for (int key = 801; key < 1000; ++key)
	{
		char val = (char)key;
		++val;
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	int i = 1;
	MAP_FOREACH(int*, iter, map)
	{
		ASSERT_TEST(*iter == i);
		i++;
		freeInt(iter);
	}

	i = 1;
	MAP_FOREACH(int*, iter, map)
	{
		ASSERT_TEST(*iter == i);
		i++;
		freeInt(iter);
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

static MapKeyElement badCopyKeyInt(MapKeyElement n)
{
	(void)n;
	return NULL;
}

static MapDataElement badCopyDataChar(MapDataElement n)
{
	(void)n;
	return NULL;
}

bool testNoMemory()
{
	Map map = mapCreate(copyDataChar, badCopyKeyInt, freeChar, freeInt, compareInts);

	for (int key = 1; key < 100; ++key)
	{
		char val = (char)key;
		++val;

		ASSERT_TEST(mapPut(map, &key, &val) == MAP_OUT_OF_MEMORY);
		ASSERT_TEST(mapGetSize(map) == 0);
	}

	mapDestroy(map);

	map = mapCreate(badCopyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	for (int key = 1; key < 100; ++key)
	{
		char val = (char)key;
		++val;

		ASSERT_TEST(mapPut(map, &key, &val) == MAP_OUT_OF_MEMORY);
		ASSERT_TEST(mapGetSize(map) == 0);
	}

	mapDestroy(map);

	return true;
}

bool testDuplicates()
{
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	for (int key = 0; key < 400; key++)
	{
		char val = (char)key;
		++val;
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	for (int key = 399; key >= 0; key--)
	{
		char val = (char)key;
		++val;
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	ASSERT_TEST(mapGetSize(map) == 400);

	mapDestroy(map);

	return true;
}

bool testElemVals()
{
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	for (int key = 0; key < 400; key++)
	{
		char val = (char)(key + 1);
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	ASSERT_TEST(mapGetSize(map) == 400);

	int i = 0;
	MAP_FOREACH(int*, iter, map)
	{
		ASSERT_TEST(*(char*)mapGet(map, iter) == (char)(i + 1));
		i++;
		freeInt(iter);
	}

	for (int key = 0; key < 400; key++)
	{
		char val = (char)key;
		val += 10;
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	ASSERT_TEST(mapGetSize(map) == 400);

	i = 0;
	MAP_FOREACH(int*, iter, map)
	{
		ASSERT_TEST(*(char*)mapGet(map, iter) == (char)(i + 10));
		i++;
		freeInt(iter);
	}

	mapDestroy(map);

	return true;
}

bool testRemove()
{
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	for (int key = 0; key < 400; key++)
	{
		char val = (char)(key + 1);
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	ASSERT_TEST(mapGetSize(map) == 400);

	for (int key = 0; key < 200; key++)
	{
		ASSERT_TEST(mapRemove(map, &key) == MAP_SUCCESS);
	}

	for (int key = 0; key < 200; key++)
	{
		ASSERT_TEST(mapRemove(map, &key) == MAP_ITEM_DOES_NOT_EXIST);
	}

	ASSERT_TEST(mapGetSize(map) == 200);

	int i = 200;
	MAP_FOREACH(int*, iter, map)
	{
		ASSERT_TEST(*(char*)mapGet(map, iter) == (char)(i + 1));
		i++;
		freeInt(iter);
	}

	mapDestroy(map);

	return true;
}

bool testClear()
{
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	ASSERT_TEST(mapGetSize(map) == 0);

	for (int key = 0; key < 400; key++)
	{
		char val = (char)(key + 1);
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	ASSERT_TEST(mapGetSize(map) == 400);

	MAP_FOREACH(int*, iter, map)
	{
		freeInt(iter);
	}

	mapClear(map);

	ASSERT_TEST(mapGetSize(map) == 0);

	for (int key = 0; key < 300; key++)
	{
		char val = (char)(key + 1);
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	ASSERT_TEST(mapGetSize(map) == 300);

	int i = 0;
	MAP_FOREACH(int*, iter, map)
	{
		ASSERT_TEST(*(char*)mapGet(map, iter) == (char)(i + 1));
		i++;
		freeInt(iter);
	}

	mapClear(map);

	ASSERT_TEST(mapGetSize(map) == 0);

	mapDestroy(map);

	return true;
}

bool testCopy()
{
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	for (int key = 0; key < 400; key++)
	{
		char val = (char)(key + 1);
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	ASSERT_TEST(mapGetSize(map) == 400);

	Map copy = mapCopy(map);

	ASSERT_TEST(copy != NULL);

	ASSERT_TEST(mapGetSize(copy) == 400);

	mapDestroy(map);

	int i = 0;
	MAP_FOREACH(int*, iter, copy)
	{
		ASSERT_TEST(*(char*)mapGet(copy, iter) == (char)(i + 1));
		i++;
		freeInt(iter);
	}

	Map copy2 = mapCopy(copy);

	ASSERT_TEST(copy2 != NULL);

	ASSERT_TEST(mapGetSize(copy2) == 400);

	mapDestroy(copy2);

	i = 0;
	MAP_FOREACH(int*, iter, copy)
	{
		ASSERT_TEST(*(char*)mapGet(copy, iter) == (char)(i + 1));
		i++;
		freeInt(iter);
	}

	mapDestroy(copy);

	return true;
}

bool testContains()
{
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	for (int key = 0; key < 400; key++)
	{
		char val = (char)(key + 1);
		ASSERT_TEST(mapPut(map, &key, &val) == MAP_SUCCESS);
	}

	ASSERT_TEST(mapGetSize(map) == 400);

	for (int i = 0; i < 400; i++)
	{
		ASSERT_TEST(mapContains(map, &i) == true);
	}

	for (int i = 500; i < 600; i++)
	{
		ASSERT_TEST(mapContains(map, &i) == false);
	}

	mapDestroy(map);

	return true;
}

bool nullTests()
{
	Map map = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);

	int val = 1;

	ASSERT_TEST(mapCopy(NULL) == NULL);

	ASSERT_TEST(mapGetSize(NULL) == -1);

	ASSERT_TEST(mapGetSize(map) == 0);

	ASSERT_TEST(mapContains(NULL, &val) == false);

	ASSERT_TEST(mapContains(map, NULL) == false);

	ASSERT_TEST(mapPut(NULL, &val, &val) == MAP_NULL_ARGUMENT);

	ASSERT_TEST(mapPut(map, NULL, &val) == MAP_NULL_ARGUMENT);

	ASSERT_TEST(mapPut(map, &val, NULL) == MAP_NULL_ARGUMENT);

	ASSERT_TEST(mapGet(NULL, &val) == NULL);

	ASSERT_TEST(mapGet(map, NULL) == NULL);

	ASSERT_TEST(mapRemove(NULL, &val) == MAP_NULL_ARGUMENT);

	ASSERT_TEST(mapRemove(map, NULL) == MAP_NULL_ARGUMENT);

	ASSERT_TEST(mapGetFirst(NULL) == NULL);

	ASSERT_TEST(mapGetFirst(map) == NULL);

	ASSERT_TEST(mapGetNext(NULL) == NULL);

	ASSERT_TEST(mapGetNext(map) == NULL);

	ASSERT_TEST(mapClear(NULL) == MAP_NULL_ARGUMENT);

	mapDestroy(map);

	return true;
}

bool testMapCreate() {
    Map map1 = mapCreate(copyDataString, copyKeyString, freeKeyString,freeDataString ,  compareStrings);;
    Map map2 = mapCreate(copyDataString, copyKeyString, freeKeyString,freeDataString ,  compareStrings);;
    Map map3 = mapCreate(copyDataString, copyKeyString, freeKeyString,freeDataString ,  compareStrings);;
    mapDestroy(map3);
    ASSERT_TEST(map1 != NULL);
    ASSERT_TEST(map2 != NULL);
    ASSERT_TEST(mapGetSize(map1) == 0);
    ASSERT_TEST(mapGetFirst(map1) == NULL);
    mapPut(map1, "mary", "poppins");
    mapDestroy(map1);
    map1 = NULL;
    mapDestroy(map1);
    ASSERT_TEST(mapGet(map1, "mary") == NULL);
    ASSERT_TEST(mapGetSize(map1) == -1);
    ASSERT_TEST(map2 != NULL);
    mapDestroy(map2);
    return true;
}

bool testMapPutGet() {
    Map map = mapCreate(copyDataString, copyKeyString, freeKeyString,freeDataString ,  compareStrings);;

     char *key1 = "key1";
     char *key2 = "key2";
     char *key3 = "key3";
     char *value1 = "value1";
    char *value2 = "value2";
     char *value3 = "value3";
     char *value4 = "value4";

    int count = 0;

    ASSERT_TEST(mapPut(map, key1, value1) == MAP_SUCCESS);
    count++;
   // char* str1=(char*)mapGet(map, &key2), value2);
   // ASSERT_TEST(strcmp(mapGet(map, key1), value1) == 0);
    ASSERT_TEST(mapPut(map, key2, value2) == MAP_SUCCESS);
    count++;
   // ASSERT_TEST(strcmp((char*)mapGet(map, key2), value2) == 0);
    ASSERT_TEST(mapPut(map, key3, value3) == MAP_SUCCESS);
    count++;

    ASSERT_TEST(mapGetSize(map) == count);
    ASSERT_TEST(mapPut(map, key3, value4) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == count);
    ASSERT_TEST(mapRemove(map, key2) == MAP_SUCCESS);
    count--;
    ASSERT_TEST(mapGetSize(map) == count);
    ASSERT_TEST(mapRemove(map, key1) == MAP_SUCCESS);
    count--;
    ASSERT_TEST(mapRemove(map, key3) == MAP_SUCCESS);
    count--;
    ASSERT_TEST(mapPut(map, key3, value1) == MAP_SUCCESS);
    count++;

    ASSERT_TEST(mapPut(map, key3, NULL) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapPut(map, NULL, value3) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapPut(map, NULL, NULL) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapPut(NULL, key3, value3) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapPut(NULL, NULL, NULL) == MAP_NULL_ARGUMENT);
    ASSERT_TEST(mapGetSize(map) == count);

    ASSERT_TEST(mapPut(map, key1, value2) == MAP_SUCCESS);
    count++;
    ASSERT_TEST(strcmp(mapGet(map, key1), value2) == 0);
    ASSERT_TEST(mapPut(map, key1, value1) == MAP_SUCCESS);
    ASSERT_TEST(strcmp(mapGet(map, key1), value1) == 0);
    ASSERT_TEST(mapRemove(map, key1) == MAP_SUCCESS);
    count--;
    ASSERT_TEST(mapGet(map, key1) == NULL);

    ASSERT_TEST(mapPut(map, key1, value1) == MAP_SUCCESS);
    count++;
    ASSERT_TEST(mapGet(map, key1) != value1);
    ASSERT_TEST(mapGetSize(map) == count);

    ASSERT_TEST(mapGet(map, key1) == mapGet(map, key1));

    ASSERT_TEST(mapPut(map, "key1", value1) == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == count);
    ASSERT_TEST(strcmp(mapGet(map, key1), value1) == 0);

    ASSERT_TEST(mapPut(map, key1, "") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, key2, "") == MAP_SUCCESS);
    count++;
    ASSERT_TEST(mapGetSize(map) == count);
    ASSERT_TEST(mapPut(map, "", value1) == MAP_SUCCESS);
    count++;
    ASSERT_TEST(mapGetSize(map) == count);
    ASSERT_TEST(strcmp(mapGet(map, ""), value1) == 0);
    ASSERT_TEST(strcmp(mapGet(map, key1), "") == 0);

    char *super_long_string =
    "Hi— I’m Ted Mosby. And exactly 45 days from now you and I are "
            "going to meet and we’re going to fall in love and we’re going to "
            "get married and we’re going to have 2 kids and we’re going to "
            "love them and each other so much. All that is 45 days away, but "
            "I’m here now I guess because… I want those extra 45 days with "
            "you. I want each one of them. Look and if I can’t have them I’ll "
            "take the 45 seconds before your boyfriend shows up and punches me "
            "in the face, because… I love you. I’m always gonna love you, til "
            "the end of my days and beyond. You’ll see.Hi— I’m Ted Mosby. And "
            "exactly 45 days from now you and I are "
            "going to meet and we’re going to fall in love and we’re going to "
            "get married and we’re going to have 2 kids and we’re going to "
            "love them and each other so much. All that is 45 days away, but "
            "I’m here now I guess because… I want those extra 45 days with "
            "you. I want each one of them. Look and if I can’t have them I’ll "
            "take the 45 seconds before your boyfriend shows up and punches me "
            "in the face, because… I love you. I’m always gonna love you, til "
            "the end of my days and beyond. You’ll see.Hi— I’m Ted Mosby. And "
            "exactly 45 days from now you and I are "
            "going to meet and we’re going to fall in love and we’re going to "
            "get married and we’re going to have 2 kids and we’re going to "
            "love them and each other so much. All that is 45 days away, but "
            "I’m here now I guess because… I want those extra 45 days with "
            "you. I want each one of them. Look and if I can’t have them I’ll "
            "take the 45 seconds before your boyfriend shows up and punches me "
            "in the face, because… I love you. I’m always gonna love you, til "
            "the end of my days and beyond. You’ll see.Hi— I’m Ted Mosby. And "
            "exactly 45 days from now you and I are "
            "going to meet and we’re going to fall in love and we’re going to "
            "get married and we’re going to have 2 kids and we’re going to "
            "love them and each other so much. All that is 45 days away, but "
            "I’m here now I guess because… I want those extra 45 days with "
            "you. I want each one of them. Look and if I can’t have them I’ll "
            "take the 45 seconds before your boyfriend shows up and punches me "
            "in the face, because… I love you. I’m always gonna love you, til "
            "the end of my days and beyond. You’ll see.Hi— I’m Ted Mosby. And "
            "exactly 45 days from now you and I are "
            "going to meet and we’re going to fall in love and we’re going to "
            "get married and we’re going to have 2 kids and we’re going to "
            "love them and each other so much. All that is 45 days away, but "
            "I’m here now I guess because… I want those extra 45 days with "
            "you. I want each one of them. Look and if I can’t have them I’ll "
            "take the 45 seconds before your boyfriend shows up and punches me "
            "in the face, because… I love you. I’m always gonna love you, til "
            "the end of my days and beyond. You’ll see.";
    ASSERT_TEST(mapPut(map, key1, super_long_string) == MAP_SUCCESS);
    ASSERT_TEST(strcmp(mapGet(map, key1), super_long_string) == 0);
    ASSERT_TEST(mapPut(map, super_long_string, value1) == MAP_SUCCESS);
    ASSERT_TEST(strcmp(mapGet(map, super_long_string), value1) == 0);
    count++;
    ASSERT_TEST(strcmp(mapGet(map, super_long_string), value1) == 0);
    ASSERT_TEST(mapGetSize(map) == count);
    ASSERT_TEST(strcmp(mapGet(map, key2), "") == 0);
    mapDestroy(map);
    return true;
}

bool testMapCopy() {
    ASSERT_TEST(mapCopy(NULL) == NULL);
    Map map1 = mapCreate(copyDataString, copyKeyString, freeKeyString,freeDataString ,  compareStrings);;
    ASSERT_TEST(mapPut(map1, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map1, "key2", "value2") == MAP_SUCCESS);
    Map map2 = mapCopy(map1);
    ASSERT_TEST(strcmp(mapGet(map2, "key1"), mapGet(map1, "key1")) == 0);
    ASSERT_TEST(strcmp(mapGet(map2, "key2"), mapGet(map1, "key2")) == 0);

    ASSERT_TEST(mapGetSize(map2) == mapGetSize(map1));

    ASSERT_TEST(mapGet(map2, "key2") != mapGet(map1, "key2"));
    ASSERT_TEST(mapRemove(map1, "key1") == MAP_SUCCESS);

    // Try re-allocating the same memory so it will change
    ASSERT_TEST(mapPut(map1, "key4", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map1, "key5", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map1, "key6", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map1, "key7", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapGet(map2, "key2") != NULL);

    mapDestroy(map1);
    ASSERT_TEST(strcmp(mapGet(map2, "key1"), "value1") == 0);
    ASSERT_TEST(strcmp(mapGet(map2, "key2"), "value2") == 0);
    mapDestroy(map2);
    return true;
}

bool testMapGetSize() {
    Map map = mapCreate(copyDataString, copyKeyString, freeKeyString,freeDataString ,  compareStrings);;

    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapPut(map, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key1", "value2") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    ASSERT_TEST(mapRemove(map, "key1") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapGetSize(NULL) == -1);
    mapDestroy(map);
    return true;
}

bool testMapContains() {
    Map map1 = mapCreate(copyDataString,copyKeyString,  freeKeyString,freeDataString,  compareStrings);;
    mapPut(map1, "key1", "value1");
    ASSERT_TEST(mapContains(map1, "key1") == true);
    ASSERT_TEST(mapContains(NULL, "key1") == false);
    ASSERT_TEST(mapContains(map1, NULL) == false);
    ASSERT_TEST(mapContains(map1, "dani") == false);
    mapDestroy(map1);
    return true;
}

bool testMapRemove() {
    Map map1 = mapCreate(copyDataString,copyKeyString, freeKeyString,freeDataString,  compareStrings);;
    mapPut(map1, "key1", "value1");
    mapPut(map1, "key2", "value2");
    mapPut(map1, "key3", "value3");
    mapRemove(map1, "key1");
    ASSERT_TEST(mapContains(map1, "key1") == false);
    ASSERT_TEST(mapGetSize(map1) == 2);
    mapPut(map1, "key1", "value1");
    mapRemove(map1, "key3");
    ASSERT_TEST(mapContains(map1, "key3") == false);
    ASSERT_TEST(mapGetSize(map1) == 2);
    mapPut(map1, "key3", "value3");
    mapRemove(map1, "key3");
    ASSERT_TEST(mapContains(map1, "key3") == false);
    ASSERT_TEST(mapGetSize(map1) == 2);
    mapDestroy(map1);
    return true;
}

bool testMapForEach() {
    Map map = mapCreate(copyDataString,copyKeyString,  freeKeyString,freeDataString,  compareStrings);;
    mapPut(map, "key 1", "value 1");
    mapPut(map, "key 2", "value 2");
    mapPut(map, "key 3", "value 3");
    mapPut(map, "key 4", "value 4");
    mapPut(map, "key 5", "value 5");
    int arr[5] = {0};
    int i = 0;
    //checking that the MAP_FOREACH went through all of the keys one at a time
    MAP_FOREACH(int *,key, map) {
        arr[i]++;
        i++;
        freeKeyString(key);
    }
    for (int j = 0; j < 5; j++){
        ASSERT_TEST(arr[j] == 1);
    }
    //trying to iterate over a NULL argument
    bool something_happened = false;
    MAP_FOREACH(int *, key, NULL) {
        ASSERT_TEST(key == NULL);
        freeKeyString(key);
        something_happened = true;
    }
    ASSERT_TEST(something_happened == false);

    //trying to iterate over a empty map
    Map map_2 = mapCreate(copyDataString,copyKeyString, freeKeyString,freeDataString,  compareStrings);;
    MAP_FOREACH(int *, key, map_2) {
        ASSERT_TEST(key == NULL);
        freeKeyString(key);
        something_happened = true;
    }
    ASSERT_TEST(something_happened == false);
    MAP_FOREACH(int *, key,map){
        mapPut(map_2,key,mapGet(map,key));
        freeKeyString(key);
    }
    ASSERT_TEST(mapGetSize(map_2) == mapGetSize(map));
    MAP_FOREACH(int *, key,map_2){
        ASSERT_TEST(mapGet(map,key)!=NULL);
        freeKeyString(key);
    }
    
    mapDestroy(map);
    mapDestroy(map_2);
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
        testPrivateMapRemove,
		testNoMemory,
		testDuplicates,
		testElemVals,
		testRemove,
		testClear,
		testCopy,
		testContains,
		nullTests,
        testMapPutGet,
        testMapCopy,
        testMapGetSize,
        testMapCreate,
        testMapContains,
        testMapRemove,
        testMapForEach
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
        "testMapCreateDestroy",
        "testMapAddAndSize",
        "testMapGet",
        "testIterator",
        "testBadArguments",
        "testPrivateMapPut",
        "testPrivateMapRemove",
		"testNoMemory",
		"testDuplicates",
		"testElemVals",
		"testRemove",
		"testClear",
		"testCopy",
		"testContains",
		"nullTests",
        "testMapPutGet",
        "testMapCopy",
        "testMapGetSize",
        "testMapCreate",
        "testMapContains",
        "testMapRemove",
        "testMapForEach"
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
