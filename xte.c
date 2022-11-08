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

/* time in seconds on how long a persistent session should last
 * 0:   never ask for password
 * <0:  always ask for password
 */
#ifndef SESSION_TIME
#define SESSION_TIME (5*60)
#endif

/* path where the session file should be stored */
#ifndef SESSION_FILE
#define SESSION_FILE "/run/xte.sess"
#endif

/* run program with root privileges */
void run(char **argv) {
    if (!(setuid(0) || setgid(0))) {
        execvp(*argv, argv);
    }
    exit(69);
}

/* test if still in valid session */
int check_session() {
    /* session enabled? */
    if (SESSION_TIME < 0) { // could be macro'd
        return 0;
    }

    /* open file */
    FILE* fs = fopen(SESSION_FILE, "r");
    if (!fs) {
        return 0;
    }

    /* read session data */
    unsigned long session_start;
    fscanf(fs, "%lu", &session_start);

    fclose(fs);

    /* check if still in valid session */
    return time(NULL) - session_start < SESSION_TIME;
}

/* get user input and compare with password */
void check_password(int uid) {
    // TODO
    exit(42);
}

/* write new session file */
void create_session() {
    /* no session file necessary */
    if (SESSION_TIME < 0) { // could be macro'd
        return;
    }

    /* open file */
    FILE* fs = fopen(SESSION_FILE, "w");
    if (!fs) {
        return;
    }

    /* write current unix time */
    fprintf(fs, "%lu", time(NULL));

    fclose(fs);
}

int main(int argc, char** argv) {
    /* no command specified */
    if (argc < 2) {
        printf("usage: %s <command>\n", *argv);
        exit(EXIT_SUCCESS);
    }

    int uid = getuid();

    /* already root? */
    if (uid) {
        /* not privileged */
        if (uid != XID) {
            exit(uid);
        }

        /* validation */
        if (SESSION_TIME && !check_session()) {
            /* get password and create new session */
            check_password(uid);
            create_session();
        }
    }

    run(argv+1);
}
