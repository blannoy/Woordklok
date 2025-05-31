#include "headers.h"

void filesystemSetup(){
    filesystem.begin();
    debug_printf("Filesystem status %d\n", filesystem.exists("/"));
}

