#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <string.h>
#include "webpage.h"

#define MAX_CONTENT 256
#define CACHE_MAX_AMOUNT 3
#define MAX_WEB 10

typedef struct {
    char key[MAX_CONTENT];
    char value[MAX_CONTENT];
} CacheItem;

typedef struct {
    CacheItem items[CACHE_MAX_AMOUNT];
    int size;
} Cache;

void InitCache(Cache *c);

int SearchCache(Cache *c, char url[]);

void AddToCache(Cache *c, char url[], char content[]);

void OpenPage(WebPage web[], int n, Cache *cache, char url[]);

void PrintCache(Cache *c);

#endif
