#include "cache.h"

void InitCache(Cache *c) {
    c->size = 0;
}

int SearchCache(Cache *c, char url[]) {

    int i;

    for (i = 0; i < c->size; i++) {

        if (strcmp(c->items[i].key, url) == 0) {
            return i;
        }
    }

    return -1;
}

void AddToCache(Cache *c, char url[], char content[]) {

    int i;

    if (c->size < CACHE_MAX_AMOUNT) {

        strcpy(c->items[c->size].key, url);
        strcpy(c->items[c->size].value, content);

        c->size++;
    }

    else {

        for (i = 0; i < CACHE_MAX_AMOUNT - 1; i++) {

            c->items[i] = c->items[i + 1];
        }

        strcpy(c->items[CACHE_MAX_AMOUNT - 1].key, url);
        strcpy(c->items[CACHE_MAX_AMOUNT - 1].value, content);
    }
}

void OpenPage(WebPage web[], int n, Cache *cache, char url[]) {

    int i;
    int idx;

    printf("\n>>> open %s\n", url);

    idx = SearchCache(cache, url);

    if (idx != -1) {

        printf("CACHE HIT\n");
        printf("Content: %s\n", cache->items[idx].value);

        return;
    }

    printf("CACHE MISS\n");

    for (i = 0; i < n; i++) {

        if (strcmp(web[i].url, url) == 0) {

            printf("Content: %s\n", web[i].content);

            AddToCache(cache, url, web[i].content);

            return;
        }
    }

    printf("Halaman tidak ditemukan\n");
}

void PrintCache(Cache *c) {

    int i;

    printf("\n=== CACHE ===\n");

    for (i = 0; i < c->size; i++) {

        printf("%d. %s\n", i + 1, c->items[i].key);
    }
}
