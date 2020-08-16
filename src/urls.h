#ifndef URLS_H
#define URLS_H

#define URL_PATTERN "http[s]?://[-[:alnum:]./_?=%:+&#]*"

typedef struct {
   char** links;
   int count;
} Urls;

Urls get_urls(char* path);

#endif
