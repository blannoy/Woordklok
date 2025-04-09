#include <headers.h>

void filesystemSetup(){
    reportmem(__func__);
    filesystem.begin();
    debug_printf("Filesystem status %d\n", filesystem.exists("/"));
    //verifyConfigFile();
}

