#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

void dm_init(void);
void dm_handle_download(const char *url);
void dm_handle_tick(void);

#endif