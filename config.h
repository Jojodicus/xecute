#ifndef XTE_CONFIG_H
#define XTE_CONFIG_H

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

/* how many tries the user has */
#ifndef MAX_TRIES
#define MAX_TRIES 3
#endif

/* how many seconds the user is timeouted if not successfully authenticated
 * <=0: disabled */
#ifndef TIMEOUT
#define TIMEOUT 60
#endif

/* if signals (like Ctrl+C) should be ignored during penalty waits */
#ifndef SIGNAL_MITIGATION
#define SIGNAL_MITIGATION 1
#endif

/* if hitting an interrupt after a wrong password should trigger timeout */
#ifndef SIGNAL_TIMEOUT
#define SIGNAL_TIMEOUT 1
#endif

/* if each shell should get its own session */
#ifndef SESSION_SHELL
#define SESSION_SHELL 1
#endif

/* size of input buffer */
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 128
#endif

/* some colors for nice looking prompts */
#ifndef DISABLE_COLOR
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#else
#define RED   ""
#define GRN   ""
#define YEL   ""
#define BLU   ""
#define MAG   ""
#define CYN   ""
#define WHT   ""
#define RESET ""
#endif

#endif /* XTE_CONFIG_H */