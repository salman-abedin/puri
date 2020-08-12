/*
 * puri
 * minimal CLI URL launcher
 */

#include "ui.h"
#include "urls.h"

int main(int argc, char* argv[]) {
   urls_t urls = get_urls(argv[1]);
   init(urls.links, urls.count);
   drawui();
   drawitems();
   handleinput();
   cleanup();
   return 0;
}
