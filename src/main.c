#include <stdio.h>
#include <string.h>
#include <time.h> 
#include "page_manager.h"
#include "cache.h"
#include "search.h"
#include "tabs.h"
#include "download_manager.h"
#include "exit.h"
#include "discover.h"
#include "webpage.h"
#include "openpage.h"
#include "back_forward.h"
#include "tabs_history.h"

#define COMMAND_MAX_LENGTH 512  

static void copy_clean_url(char *dest, const char *src) {
    strcpy(dest, src);
    dest[strcspn(dest, "\r\n ")] = '\0';
}

static int find_active_page(const char *url, WebPage *page) {
    int i;
    WebPage *pages = pm_get_web_pages();
    int count = pm_get_web_page_count();

    for (i = 0; i < count; i++) {
        if (pages[i].active && strcmp(pages[i].url, url) == 0) {
            *page = pages[i];
            return 1;
        }
    }

    return 0;
}

int main() {
    char command[COMMAND_MAX_LENGTH];
    Cache cache;
    Browser browser;
    Graph graph;
    NavHistory nav;
    HistoryTab tabHistory;

    // 1. Inisialisasi Seluruh Sistem
    pm_init();
    InitCache(&cache);
    InitBrowser(&browser);
    initGraph(&graph);
    navInit(&nav);
    initTab(&tabHistory);
    discover_seed((unsigned int)time(NULL));

    printf("=== Browser Terminal ===\n");
    printf("Format Perintah:\n");
    printf("- Page Manager    : open <url>, add_page <url>, edit_page <url>, delete_page <url>\n");
    printf("- Download Manager: download <url>, tick\n");
    printf("- Tab Browser     : newtab, nexttab, checktab\n");
    printf("- Navigasi        : back, forward, back <step>, forward <step>, history\n");
    printf("- Lainnya         : search <query>, discover, exit\n\n");

    // 3. Loop Utama Interaktif
    while (1) {
        char *input;
        char *argument;

        printf("> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        command[strcspn(command, "\r\n")] = '\0';

        input = command;
        while (*input == ' ' || *input == '\t') {
            input++;
        }

        // --- KELOMPOK PERINTAH EXIT ---
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            exitBrowser(); 
            break; 
        }

        // --- KELOMPOK PERINTAH DISCOVER ---
        if (strcmp(input, "discover") == 0) {
            discover_run(pm_get_web_pages(), pm_get_web_page_count()); 
            continue;
        }

        // --- KELOMPOK PERINTAH TAB BROWSER ---
        if (strcmp(input, "checktab") == 0) {
            printf("\n=== TAB ===\n\n");
            CheckTab(&browser);
            continue;
        }
        if (strcmp(input, "newtab") == 0) {
            NewTab(&browser);
            continue;
        }
        if (strcmp(input, "closetab") == 0) {
            CloseTab(&browser);
            continue;
        }
        if (strncmp(input, "nexttab ", 8) == 0) {
            int steps = 1;
            if (sscanf(input + 8, "%d", &steps) == 1) {
                NextTab(&browser, steps);
            }
            continue;
        }
        if (strncmp(input, "prevtab ", 8) == 0) {
            int steps = 1;
            if (sscanf(input + 8, "%d", &steps) == 1) {
                PrevTab(&browser, steps);
            }
            continue;
        }

        // --- KELOMPOK PERINTAH BACK/FORWARD & TAB HISTORY ---
        if (strcmp(input, "back") == 0) {
            navBack(&nav);
            continue;
        }
        if (strcmp(input, "forward") == 0) {
            navForward(&nav);
            continue;
        }
        if (strncmp(input, "back ", 5) == 0) {
            int steps = 1;
            if (sscanf(input + 5, "%d", &steps) == 1) {
                backPage(&tabHistory, steps);
            }
            continue;
        }
        if (strncmp(input, "forward ", 8) == 0) {
            int steps = 1;
            if (sscanf(input + 8, "%d", &steps) == 1) {
                forwardPage(&tabHistory, steps);
            }
            continue;
        }
        if (strcmp(input, "history") == 0) {
            viewTabHistory(&tabHistory);
            continue;
        }

        // --- KELOMPOK PERINTAH SEARCH ---
        if (strncmp(input, "search ", 7) == 0) {
            argument = input + 7;
            printf("\n=== SEARCH ===\n\n");
            Search(pm_get_web_pages(), pm_get_web_page_count(), argument);
            continue;
        }

        // --- KELOMPOK PERINTAH PAGE MANAGER & CACHE ---
        if (strncmp(input, "open ", 5) == 0) {
            char cleanUrl[URL_MAX_LENGTH];
            WebPage openedPage;

            argument = input + 5;
            op_handle_open(argument, &cache, &graph, &browser);
            copy_clean_url(cleanUrl, argument);
            if (find_active_page(cleanUrl, &openedPage)) {
                navOpenPage(&nav, openedPage);
                visitPage(&tabHistory, cleanUrl);
            }
            continue;
        } 
        if (strncmp(input, "add_page ", 9) == 0) {
            argument = input + 9;
            pm_handle_add_page(argument, &graph);
            continue;
        } 
        if (strncmp(input, "edit_page ", 10) == 0) {
            argument = input + 10;
            pm_handle_edit_page(argument, &graph, &cache);
            continue;
        } 
        if (strncmp(input, "delete_page ", 12) == 0) {
            argument = input + 12;
            pm_handle_delete_page(argument, &graph, &cache);
            continue;
        }

        // --- KELOMPOK PERINTAH DOWNLOAD MANAGER ---
        if (strcmp(input, "tick") == 0) {
            dm_handle_tick();
            continue;
        }
        if (strncmp(input, "download", 8) == 0 &&
            (input[8] == '\0' || input[8] == ' ' || input[8] == '\t')) {

            argument = input + 8;
            while (*argument == ' ' || *argument == '\t') {
                argument++;
            }
            dm_handle_download(argument);
            continue;
        }

        if (strlen(input) == 0) {
            continue;
        }

        printf("Perintah tidak dikenali.\n");
    }

    printf("Program selesai.\n");
    navDestroy(&nav);
    return 0;
}
