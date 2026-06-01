#include "openpage.h"
#include "page_manager.h"

int isValidUrl(const char *url) {
    int len = strlen(url);
    if (len < 5) return 0;

    int dotCount = 0;
    for (int i = 0; i < len; i++) {
        if (url[i] == '.') dotCount++;
    }

    if (dotCount < 1) return 0;

    for (int i = 0; i < len; i++) {
        if (!((url[i] >= 'a' && url[i] <= 'z') ||
              (url[i] >= '0' && url[i] <= '9') ||
              url[i] == '.' || url[i] == '-')) {
            return 0;
        }
    }

    if (url[0] == '.' || url[len-1] == '.') return 0;

    return 1;
}

void op_handle_open(const char *url, Cache *cache, Graph *g, Browser *b) {
    char cleanUrl[MAX_STR];
    strcpy(cleanUrl, url);
    cleanUrl[strcspn(cleanUrl, "\r\n ")] = '\0';
    
    if (!isValidUrl(url)) {
        printf("URL tidak valid! Gunakan format URL formal.\n");
        return;
    }

    WebPage *pages = pm_get_web_pages();
    int count = pm_get_web_page_count();
    int page_index = -1;

    for (int i = 0; i < count; i++) {
        if (pages[i].active && strcmp(pages[i].url, cleanUrl) == 0) {
            page_index = i;
            break;
        }
    }

    if (page_index == -1) {
        printf("404 Not Found! Halaman tidak ditemukan.\n");
        return;
    }

    if (b != NULL && b->count > 0) {
        strncpy(b->tabs[b->current].name, cleanUrl, 19);
        b->tabs[b->current].name[19] = '\0';
    }

    char cache_url[MAX_CONTENT];
    strcpy(cache_url, url);
    int cache_index = SearchCache(cache, cache_url);

    if (cache_index != -1) {
        printf("[Status: Cache-Hit] Mengambil data dari cache...\n");
        printf("\n=== %s ===\n%s\n\n", pages[page_index].url, cache->items[cache_index].value);
    } else {
        printf("[Status: Cache-Miss] Mengambil data dari database...\n");
        printf("\n=== %s ===\n%s\n\n", pages[page_index].url, pages[page_index].content);
        AddToCache(cache, cache_url, pages[page_index].content);
    }

    displayLinked(*g, pages[page_index].id);
}
