#ifndef _PURI_H_
#define _PURI_H_

#define HEADER "puri: Puny URL Launcher"
#define FOOTER "h:Quit   j:Down   k:Up   l:launch"
#define URL_PATTERN "http[s]?://[^[:space:]]*"

int count, mark, height, width;
WINDOW* win;
char** urls;

#endif
