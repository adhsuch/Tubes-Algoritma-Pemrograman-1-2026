#include "search.h"
#include <ctype.h>
#include <string.h>

void ToLowerString(char str[]) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

void Search(WebPage web[], int n, char query[]) {
    int i;
    int found = 0;

    printf("Search result(s) for \"%s\":\n", query);

    for (i = 0; i < n; i++) {
        // TAMBAHKAN CEK web[i].active AGAR SINKRON SAAT DI-DELETE
        if (web[i].active && strncmp(web[i].url, query, strlen(query)) == 0) {
            printf("- %s\n", web[i].url);
            found = 1;
        }
    }

    if (!found) {
        printf("Tidak Ditemukan\n");
    }
}

void AdvancedSearch(WebPage web[], int n, char query[]) {
    int i;
    int found = 0;

    char lowerQuery[URL_MAX_LENGTH];
    char lowerUrl[URL_MAX_LENGTH];
    char lowerContent[URL_MAX_LENGTH];

    strcpy(lowerQuery, query);
    ToLowerString(lowerQuery);

    printf("Search result(s) for \"%s\":\n", query);

    for (i = 0; i < n; i++) {
        // TAMBAHKAN CEK web[i].active AGAR SINKRON SAAT DI-DELETE
        if (!web[i].active) continue;

        strcpy(lowerUrl, web[i].url);
        strcpy(lowerContent, web[i].content);

        ToLowerString(lowerUrl);
        ToLowerString(lowerContent);

        if (strstr(lowerUrl, lowerQuery) != NULL ||
            strstr(lowerContent, lowerQuery) != NULL) {

            printf("- %s\n", web[i].url);
            found = 1;
        }
    }

    if (!found) {
        printf("Tidak Ditemukan\n");
    }
}