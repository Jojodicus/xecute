#if _XOPEN_CRYPT >= 0
#include <crypt.h>
#endif

#include <pwd.h>
#include <shadow.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"

static int tries = 0;

enum session_status {
    AUTHENTICATED,
    TIMEOUTED
};

struct session {
    time_t end_time;
    enum session_status status;
};

/* helper for opening sessions */
static FILE *open_session(const char *mode) {
    #if SESSION_SHELL
        char session_file[BUFFER_SIZE];

	/* avoid session for wrong pid (max pid_t is +18,446,744,073,709,551,615) */
	if (BUFFER_SIZE - 27 < strlen(SESSION_FILE)){
	    exit(161);
	}

        snprintf(session_file, BUFFER_SIZE, "%s.%d", SESSION_FILE, getppid());
    #else
        char *session_file = SESSION_FILE;
    #endif
    /* open file */
    return fopen(session_file, mode);
}

/* write new session file */
static void create_session(enum session_status new_status) {
    FILE* fs = open_session("w");
    if (!fs) {
        return;
    }

    /* create session */
    time_t now = time(NULL);
    struct session new_session = {
        .status = new_status,
        .end_time = now
    };
    if (new_status == AUTHENTICATED) {
        new_session.end_time += (SESSION_TIME);
    }
    if (new_status == TIMEOUTED) {
        new_session.end_time += (TIMEOUT);
    }

    /* write time and status */
    fwrite(&new_session, sizeof(struct session), 1, fs);

    fclose(fs);
}

/* test if still in valid session */
static int check_session(void) {
    FILE* fs = open_session("r");
    if (!fs) {
        return 0;
    }

    /* read session data */
    struct session old_session;
    if (fread(&old_session, sizeof(struct session), 1, fs) != 1) {
        return 0;
    }

    fclose(fs);

    time_t now = time(NULL);

    /* check if still in valid session */
    if (old_session.status == AUTHENTICATED && now < old_session.end_time) {
        return 1;
    }

    /* check if timeouted */
    if (old_session.status == TIMEOUTED && now < old_session.end_time) {
        printf("timeouted, try again in "RED"%lu"RESET" seconds.\n", old_session.end_time - now);
        exit(7);
    }

    return 0;
}

/* helper for setting all signal handlers */
static void set_all_signals(void (*handler)(int)) {
    int signum;
    for (signum = 1; signum < _NSIG; ++signum) {
        signal(signum, handler);
    }
}

static void signal_handler(int signum) {
    #if SIGNAL_TIMEOUT
        /* write timout session before quitting */
        if (tries > 0) {
            create_session(TIMEOUTED);
        }
    #endif

    /* reset invisible terminal */
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    exit(1);
}

/* run program with root privileges */
static void run(char **argv) {
    if (!(setuid(0) || setgid(0))) {
        execvp(*argv, argv);
    }
    exit(69);
}

/* read password from stdin */
static void read_password(char *buffer) {
    static struct termios oldt, newt;
    int i = 0;
    int c;

    /* saving the old settings of STDIN_FILENO and copy settings for resetting */
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    /* setting the approriate bit in the termios struct */
    newt.c_lflag &= ~(ECHO);

    /* setting the new bits */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    /* reading the password from the console */
    while ((c = getchar()) != '\n' && c != EOF && i < BUFFER_SIZE - 1){
        buffer[i++] = c;
    }
    buffer[i] = '\0';

    /* resetting our old STDIN_FILENO */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

/* get user input and compare with password */
static void check_password(const char* pname, int uid) {
    /* prevent signal exploitation */
    set_all_signals(signal_handler);

    while (tries < MAX_TRIES) {
        printf(BLU"["YEL"%s"BLU"]"RESET" - password: ", pname);
        fflush(stdout);

        /* get hashed user pw */
        struct passwd* pw = getpwuid(uid);
        struct spwd* shadowEntry = getspnam(pw->pw_name);

        /* read password input and hash it */
        char input[BUFFER_SIZE];
        read_password(input);
        char* hashed_pw = crypt(input, shadowEntry->sp_pwdp); // TODO: get at compile time

        /* cleanup */
        ++tries; // prevent signal exploitation
        memset(input, 1337, BUFFER_SIZE);
        putchar('\n');
        fflush(stdout);

        /* compare hashes */
        if (!strcmp(shadowEntry->sp_pwdp, hashed_pw)) {
            /* success */
            return;
        }

        /* penalty */
        #if SIGNAL_MITIGATION
            set_all_signals(SIG_IGN);
        #endif
        sleep(3);
        #if SIGNAL_MITIGATION
            set_all_signals(signal_handler);
        #endif
        printf(RED"wrong password\n"RESET);
    }

    /* too many tries */
    #if TIMEOUT > 0
        create_session(TIMEOUTED);
    #endif
    exit(42);
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
        #if SESSION_TIME != 0
            if (!check_session()) {
                /* get password and create new session */
                check_password(*argv, uid);
                #if SESSION_TIME > 0 // no session if always asking for password
                    create_session(AUTHENTICATED);
                #endif
            }
        #endif
    }

    run(argv+1);
}
