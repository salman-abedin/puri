#ifndef _URLS_H_
#define _URLS_H_

#define URL_PATTERN "http[s]?://[[:alnum:]./_?=%:+&#-]*"

typedef struct {
   char** links;
   int count;
} urls_t;

urls_t get_urls(char* path);

#endif
