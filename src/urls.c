#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "urls.h"

char* _get_file_string(char* path) {
   char letter;
   int writei;
   long size;
   char* filestring;
   FILE* file;

   writei = 0;

   file = fopen(path, "r");
   fseek(file, 0L, SEEK_END);
   size = ftell(file);
   filestring = malloc(size);

   fseek(file, 0L, SEEK_SET);
   while ((letter = fgetc(file)) != EOF) {
      if (letter == '\n')
         filestring[writei++] = ' ';
      else if (letter == '|' || !isprint(letter))
         continue;
      else
         filestring[writei++] = letter;
   }
   filestring[writei] = '\0';
   fclose(file);
   return filestring;
}

Urls get_urls(char* path) {
   int match_len, url_count;
   char* cursor;
   Urls urls;
   regex_t regx;
   regmatch_t matches[1];

   url_count = 0;
   char* filestring = _get_file_string(path);

   regcomp(&regx, URL_PATTERN, REG_EXTENDED);
   cursor = filestring;
   while (regexec(&regx, cursor, 1, matches, 0) == 0) {
      ++url_count;
      match_len = matches[0].rm_eo - matches[0].rm_so;
      if (url_count == 1)
         urls.links = calloc(match_len + 1, (sizeof *urls.links));
      else
         urls.links = realloc(urls.links, (sizeof *urls.links) * url_count);

      urls.links[url_count - 1] = calloc(match_len + 1, (sizeof **urls.links));
      strncpy(urls.links[url_count - 1], &cursor[matches[0].rm_so], match_len);

      cursor += matches[0].rm_eo;
   }
   urls.count = url_count;

   regfree(&regx);
   return urls;
}
