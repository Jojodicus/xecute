#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <crypt.h>
#include <shadow.h>
#include <string.h>

/* id of privileged user */
#ifndef XID
#define XID 0
#endif

// /* time in seconds on how long a persistent session should last */
// #ifndef SESSION_TIME
// #define SESSION_TIME (5*60)
// #endif

// #define SESSION_FILE "/run/xte.sess"

/* run program with root privileges */
void run(char **argv) {
    if (!(setuid(0) || setgid(0))) {
        execvp(*argv, argv);
    }
    exit(69);
}

// /* test if still in valid session */
// int check_session() {
//     /* open file */
//     FILE* fs = fopen(SESSION_FILE, "r");
//     if (!fs) {
//         return 0;
//     }

//     /* read session data */
//     unsigned long session_start;
//     fscanf(fs, "%lu", &session_start);

//     /* check if still in valid session */
//     return time(NULL) - session_start < SESSION_TIME;
// }

/* get password and create new session */
int check_password(int uid) {
    return 0;
}

int main(int argc, char** argv) {
    /* no command specified */
    if (argc < 2) {
        printf("usage: %s <command>\n", *argv);
        exit(EXIT_SUCCESS);
    }

    int uid = getuid();

    /* not privileged */
    if (uid && uid != XID) {
        exit(uid);
    }

    // /* validation */
    // if (!(check_session() || check_password(uid))) {
    //     exit(EXIT_FAILURE);
    // }

    run(argv+1);
}
