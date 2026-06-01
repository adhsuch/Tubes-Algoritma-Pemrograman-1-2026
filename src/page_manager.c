#include "page_manager.h"
#include "openpage.h"
#include <stdio.h>
#include <string.h>

static WebPage web_pages[MAX_WEB_PAGES];
static int web_page_count = 0;

static const WebPage initial_web_pages[] = {
    {1, "gravityfalls.com", "Gravity Falls", 1},
    {2, "google.com", "Mesin pencari", 1},
    {3, "github.com", "Platform source code", 1},
    {4, "gideon.org", "Tenda telepati Gideon", 1},
    {5, "journal-3.nf", "Jurnal misteri nomor 3", 1}
};

static void remove_newline(char *text) { text[strcspn(text, "\r\n")] = '\0'; }

/* --- SEKARANG DIANGKAT KE ATAS AGAR DIKENAL OLEH FUNGSI DI BAWAHNYA --- */
static int get_next_page_id(void) {
    int max_id = 0;
    for (int i = 0; i < web_page_count; i++) {
        if (web_pages[i].id > max_id) max_id = web_pages[i].id;
    }
    return max_id + 1;
}

int pm_get_page_id(const char *url) {
    for (int i = 0; i < web_page_count; i++) {
        if (web_pages[i].active && strcmp(web_pages[i].url, url) == 0) return web_pages[i].id;
    }
    return -1;
}

void pm_init(void) {
    web_page_count = 0;
    int initial_count = (int)(sizeof(initial_web_pages) / sizeof(initial_web_pages[0]));
    for (int i = 0; i < initial_count && web_page_count < MAX_WEB_PAGES; i++) {
        web_pages[web_page_count] = initial_web_pages[i];
        web_page_count++;
    }
}

void pm_handle_add_page(const char *url, Graph *g) {
    if (!isValidUrl(url)) {
        printf("URL tidak valid!\n"); return;
    }
    if (pm_get_page_id(url) != -1) {
        printf("Sudah terdapat halaman dengan url %s. Gunakan url lain yang belum terdaftar!\n", url); return;
    }
    if (web_page_count >= MAX_WEB_PAGES) {
        printf("Database halaman web sudah penuh.\n"); return;
    }

    printf("Masukkan konten (Akhiri dengan titik '.' di baris baru):\n");
    char content[CONTENT_MAX_LENGTH] = "";
    char line[URL_MAX_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        remove_newline(line);
        if (strcmp(line, ".") == 0) break;
        if (content[0] != '\0') strcat(content, "\n");
        strcat(content, line);
    }

    web_pages[web_page_count].id = get_next_page_id();
    strcpy(web_pages[web_page_count].url, url);
    strcpy(web_pages[web_page_count].content, content);
    web_pages[web_page_count].active = 1;
    web_page_count++;

    printf("Masukkan linked pages (Ketik 'DONE' jika sudah selesai):\n");
    wg_read_linked_pages(g, web_pages[web_page_count - 1].id);
    printf("Halaman %s berhasil ditambahkan!\n", url);
}

void pm_handle_edit_page(const char *url, Graph *g, Cache *cache) {
    if (!isValidUrl(url)) return;
    int page_id = pm_get_page_id(url);
    if (page_id == -1) {
        printf("Tidak ada halaman dengan url %s!\n", url); return;
    }
    
    int page_index = -1;
    for (int i = 0; i < web_page_count; i++) {
        if (web_pages[i].id == page_id) { page_index = i; break; }
    }

    printf("Konten saat ini:\n%s\n", web_pages[page_index].content);
    printf("Masukkan konten baru (akhiri dengan '.' atau ketik '.' saja jika tidak mengubah):\n");
    
    char content[CONTENT_MAX_LENGTH] = "";
    char line[URL_MAX_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        remove_newline(line);
        if (strcmp(line, ".") == 0) break;
        if (content[0] != '\0') strcat(content, "\n");
        strcat(content, line);
    }
    if (content[0] != '\0') strcpy(web_pages[page_index].content, content);

    printf("Masukkan linked pages baru (Ketik 'DONE' jika selesai, atau 'SKIP' jika tidak mengubah):\n");
    if (fgets(line, sizeof(line), stdin) != NULL) {
        remove_newline(line);
        if (strcmp(line, "SKIP") != 0) {
            wg_clear_out_edges(g, page_id);
            if (strcmp(line, "DONE") != 0) {
                if (isValidUrl(line)) {
                    int target_id = pm_get_page_id(line);
                    if (target_id != -1) insertEdge(g, page_id, target_id);
                    else printf("URL tidak ditemukan!\n");
                }
                wg_read_linked_pages(g, page_id);
            }
        }
    }
    InitCache(cache); 
    printf("Halaman %s berhasil diperbarui!\n", url);
}

void pm_handle_delete_page(const char *url, Graph *g, Cache *cache) {
    if (!isValidUrl(url)) return;
    int page_id = pm_get_page_id(url);
    if (page_id == -1) {
        printf("Tidak ada halaman dengan url %s!\n", url); return;
    }
    wg_remove_all_edges_related_to(g, page_id);
    for (int i = 0; i < web_page_count; i++) {
        if (web_pages[i].id == page_id) {
            web_pages[i].active = 0; break;
        }
    }
    InitCache(cache);
    printf("Membersihkan relasi linked pages...\n");
    printf("Halaman %s berhasil dihapus!\n", url);
}

WebPage* pm_get_web_pages(void) { return web_pages; }
int pm_get_web_page_count(void) { return web_page_count; }
