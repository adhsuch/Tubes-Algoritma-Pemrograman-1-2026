/* map.c */

#include "map.h"

void MapCreateCache(Cache *c) {
    c->size = 0;
}

int IsCacheFull(Cache c) {
    return c.size == CACHE_MAX_AMOUNT;
}

int IsCacheEmpty(Cache c) {
    return c.size == 0;
}

int MapSearchCache(Cache c, char url[]) {
    int i;

    for (i = 0; i < c.size; i++) {
        if (strcmp(c.items[i].url, url) == 0) {
            return i;
        }
    }

    return -1;
}

void RemoveOldestCache(Cache *c) {
    int i;

    for (i = 0; i < c->size - 1; i++) {
        c->items[i] = c->items[i + 1];
    }

    c->size--;
}

void AddCache(Cache *c, char url[], char content[]) {
    if (IsCacheFull(*c)) {
        RemoveOldestCache(c);
    }

    strcpy(c->items[c->size].url, url);
    strcpy(c->items[c->size].content, content);

    c->size++;
}

void MapOpenPage(Cache *c, char url[], char urls[][MAX_URL_LENGTH],
                 char contents[][MAX_CONTENT_LENGTH], int n) {

    int idx;
    int i;

    idx = MapSearchCache(*c, url);

    if (idx != -1) {
        printf("Cache Hit\n");
        printf("Content: %s\n", c->items[idx].content);
        return;
    }

    printf("Cache Miss\n");

    for (i = 0; i < n; i++) {
        if (strcmp(urls[i], url) == 0) {

            printf("Content: %s\n", contents[i]);

            AddCache(c, url, contents[i]);

            return;
        }
    }

    printf("Halaman tidak ditemukan\n");
}

void DisplayCache(Cache c) {
    int i;

    printf("\n===== CACHE =====\n");

    for (i = 0; i < c.size; i++) {
        printf("%d. %s\n", i + 1, c.items[i].url);
    }
}
