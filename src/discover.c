#include "discover.h"
#include <stdio.h>

/*Konstanta LCG*/
#define LCG_A 1103515245U
#define LCG_C 12345U
#define LCG_M 2147483648U  /* 2^31 */

/* State internal LCG */
static unsigned int lcg_state = 1;

void discover_seed(unsigned int seed) {
    lcg_state = seed;
}

/* Rumus: X(n+1) = (a * X(n) + c) mod m */
unsigned int discover_lcg_next(void) {
    lcg_state = (unsigned int)(
        ((unsigned long long)LCG_A * lcg_state + LCG_C) % LCG_M
    );
    return lcg_state;
}

void discover_run(WebPage web_pages[], int web_page_count) {
    int indices[MAX_WEB_PAGES];
    int count = 0;
    int show_count;
    int i, j, temp;

    // Filter hanya mengambil indeks halaman yang berstatus aktif (active == 1)
    for (i = 0; i < web_page_count; i++) {
        if (web_pages[i].active) {
            indices[count] = i;
            count++;
        }
    }

    if (count == 0) {
        printf("Belum ada halaman web yang tersedia.\n");
        return;
    }

    show_count = (count < DISCOVER_COUNT) ? count : DISCOVER_COUNT;

    for (i = 0; i < show_count; i++) {
        unsigned int range = (unsigned int)(count - i);
        j = i + (int)(discover_lcg_next() % range);

        temp       = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
    
    printf("Berikut adalah beberapa halaman yang mungkin menarik untukmu:\n");
    for (i = 0; i < show_count; i++) {
        printf("  - %s\n", web_pages[indices[i]].url);
    }
}