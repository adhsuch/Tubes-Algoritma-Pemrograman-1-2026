#ifndef WEBPAGE_H
#define WEBPAGE_H
 
/* ADT SEDERHANA: WebPage dan LinkedPage */
 
/* Konstanta */
#define MAX_WEB_PAGES      100
#define MAX_LINKED_PAGES   300
#define URL_MAX_LENGTH     256
#define CONTENT_MAX_LENGTH 4096
#define LINE_MAX_LENGTH    8192

typedef struct {
    int  id;
    char url[URL_MAX_LENGTH];
    char content[CONTENT_MAX_LENGTH];
    int  active;
} WebPage;
 
typedef struct {
    int id;
    int source_id;
    int target_id;
    int active;
} LinkedPage;
 
#endif