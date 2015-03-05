/*
** Client 1 process -- reads from a message queue and replies.
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int main(void)
{
    struct my_msgbuf buf;
    struct my_msgbuf buf1;
    int msqid;
    key_t key;

    if ((key = ftok("server.c", 'B')) == -1) {  /* same key as server */
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, 0644)) == -1) { /* connect to the queue */
        perror("msgget");
        exit(1);
    }
    
    printf("Client: ready to receive messages, Server.\n");

    buf1.mtype=3;
    for(;;) { /* Spock never quits! */
        if (msgrcv(msqid, &buf, sizeof(buf.mtext), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("Server: \"%s\"\n", buf.mtext);

         (void) strcpy(buf1.mtext, "Thanks Client 1");
        size_t len = strlen(buf1.mtext)+1;

        if (msgsnd(msqid, &buf1, len, 0) == -1) /* +1 for '\0' */
            perror("msgsnd");

    }

    return 0;
}
