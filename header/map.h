/* map.h */

#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <string.h>
#include "webpage.h"

#define CACHE_MAX_AMOUNT 10
#define MAX_URL_LENGTH URL_MAX_LENGTH
#define MAX_CONTENT_LENGTH CONTENT_MAX_LENGTH

typedef struct {
    char url[MAX_URL_LENGTH];
    char content[MAX_CONTENT_LENGTH];
} CacheItem;

typedef struct {
    CacheItem items[CACHE_MAX_AMOUNT];
    int size;
} Cache;

void MapCreateCache(Cache *c);
int IsCacheFull(Cache c);
int IsCacheEmpty(Cache c);
int MapSearchCache(Cache c, char url[]);
void RemoveOldestCache(Cache *c);
void AddCache(Cache *c, char url[], char content[]);
void MapOpenPage(Cache *c, char url[], char urls[][MAX_URL_LENGTH],
                 char contents[][MAX_CONTENT_LENGTH], int n);
void DisplayCache(Cache c);

#endif
