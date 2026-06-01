#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "webpage.h"
#include "cache.h"
#include "webgraph.h"

void pm_init(void);
void pm_handle_add_page(const char *url, Graph *g);
void pm_handle_edit_page(const char *url, Graph *g, Cache *cache);
void pm_handle_delete_page(const char *url, Graph *g, Cache *cache);

WebPage* pm_get_web_pages(void);
int pm_get_web_page_count(void);
int pm_get_page_id(const char *url);

#endif
