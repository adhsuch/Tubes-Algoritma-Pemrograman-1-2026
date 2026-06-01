#include "download_manager.h"
#include "webpage.h"
#include "page_manager.h" // PERBAIKAN: Hubungkan ke page_manager terpusat

#include <stdio.h>
#include <string.h>

#define DOWNLOAD_MAX_AMOUNT 5

typedef struct {
    char url[URL_MAX_LENGTH];
    int remaining_ticks;
} DownloadItem;

static DownloadItem download_queue[DOWNLOAD_MAX_AMOUNT];
static int queue_front = 0;
static int queue_size = 0;

static int dm_calculate_ticks(const char *url) {
    return (int)(strlen(url) / 5) + 2;
}

static int dm_is_letter(char character) {
    return (character >= 'A' && character <= 'Z') ||
           (character >= 'a' && character <= 'z');
}

static int dm_is_alnum(char character) {
    return dm_is_letter(character) ||
           (character >= '0' && character <= '9');
}

static int dm_is_valid_domain_label(const char *url, int start, int end) {
    if (start >= end || end - start > 63 ||
        url[start] == '-' || url[end - 1] == '-') {
        return 0;
    }

    for (int i = start; i < end; i++) {
        if (!dm_is_alnum(url[i]) && url[i] != '-') {
            return 0;
        }
    }

    return 1;
}

static int dm_is_valid_tld(const char *url, int start, int end) {
    if (end - start < 2) {
        return 0;
    }

    for (int i = start; i < end; i++) {
        if (!dm_is_letter(url[i])) {
            return 0;
        }
    }

    return 1;
}

static int dm_is_valid_url(const char *url) {
    int length;
    int label_start = 0;
    int dot_count = 0;

    if (url == NULL) {
        return 0;
    }

    length = (int)strlen(url);
    if (length == 0 || length >= URL_MAX_LENGTH) {
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (url[i] == '.') {
            if (!dm_is_valid_domain_label(url, label_start, i)) {
                return 0;
            }
            dot_count++;
            label_start = i + 1;
        }
    }

    return dot_count > 0 && dm_is_valid_tld(url, label_start, length);
}

static int dm_url_exists_in_web_pages(const char *url) {
    WebPage *pages = pm_get_web_pages();
    int web_page_count = pm_get_web_page_count();

    for (int i = 0; i < web_page_count; i++) {
        if (pages[i].active && strcmp(url, pages[i].url) == 0) {
            return 1; // Ketemu di database aktif browser
        }
    }

    return 0; // Tidak ketemu
}

static int dm_queue_index(int offset) {
    return (queue_front + offset) % DOWNLOAD_MAX_AMOUNT;
}

static int dm_total_pending_ticks(void) {
    int total = 0;

    for (int i = 0; i < queue_size; i++) {
        total += download_queue[dm_queue_index(i)].remaining_ticks;
    }

    return total;
}

void dm_init(void) {
    queue_front = 0;
    queue_size = 0;
}

void dm_handle_download(const char *url) {
    int ticks;
    int insert_index;
    int pending_ticks;

    if (url == NULL || url[0] == '\0') {
        printf("ERROR: URL tidak valid.\n");
        return;
    }

    if (!dm_is_valid_url(url)) {
        printf("ERROR: URL tidak valid.\n");
        return;
    }

    if (!dm_url_exists_in_web_pages(url)) {
        printf("ERROR: URL tidak ditemukan.\n");
        return;
    }

    if (queue_size >= DOWNLOAD_MAX_AMOUNT) {
        printf("Download tidak diterima, antrian sudah penuh.\n");
        return;
    }

    ticks = dm_calculate_ticks(url);
    pending_ticks = dm_total_pending_ticks();
    insert_index = dm_queue_index(queue_size);

    strncpy(download_queue[insert_index].url, url, URL_MAX_LENGTH - 1);
    download_queue[insert_index].url[URL_MAX_LENGTH - 1] = '\0';
    download_queue[insert_index].remaining_ticks = ticks;
    queue_size++;

    printf("Download %s (%d ticks)", download_queue[insert_index].url, ticks);
    if (queue_size > 1) {
        printf(" -> antrian no %d, %d ticks masih tertunda dari antrian sebelumnya",
               queue_size,
               pending_ticks);
    }
    printf("\n");
}

void dm_handle_tick(void) {
    DownloadItem *active_download;

    if (queue_size == 0) {
        printf("Antrian download saat ini kosong.\n");
        return;
    }

    active_download = &download_queue[queue_front];
    active_download->remaining_ticks--;

    if (active_download->remaining_ticks > 0) {
        printf("Downloading %s... (%d ticks tersisa)\n",
               active_download->url,
               active_download->remaining_ticks);
        return;
    }

    printf("%s selesai terdownload!\n", active_download->url);

    queue_front = dm_queue_index(1);
    queue_size--;

    if (queue_size > 0) {
        active_download = &download_queue[queue_front];
        printf("Lanjut downloading %s... (%d ticks tersisa)\n",
               active_download->url,
               active_download->remaining_ticks);
    }
}
