#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#ifndef XID
#define XID 0
#endif

int main(int argc, char** argv) {
    int uid = getuid();

    printf("%d - %d\n", XID, uid);

    exit(1);
}