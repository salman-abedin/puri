#ifndef URLS_H
#define URLS_H

#define URL_PATTERN "http[s]?://[[:alnum:]./_?=%:+&#-]*"

typedef struct {
   char** links;
   int count;
} urls_t;

urls_t get_urls(char* path);

#endif
