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

typedef struct msgbuffer {

    long mtype;
    char mtext[300];
}msgbuf;

int main(void)
{
    msgbuf bufrecv;
    msgbuf bufsnd;
    int msqid;
    key_t key;
    int count=1;

    if ((key = ftok("server.c", 'B')) == -1) {  /* same key as server */
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, 0644)) == -1) { /* connect to the queue */
        perror("msgget");
        exit(1);
    }
    
    printf("Client_1 Ready to Receive Messages, Message Queue=%d.\n", msqid);

   
    while(count>0) { 

        if (msgrcv(msqid, &bufrecv, sizeof(bufrecv.mtext), 1, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        printf("******************************\n");
        printf("Received Message\n");
        // printf("From: %ld\n", bufrecv.mtype);
        printf("Message: \"%s\"\n", bufrecv.mtext);
        // scanf("%s", &bufsnd.mtext);
        
        bufsnd.mtype=9;
        (void) strcpy(bufsnd.mtext, "Faith: Thanks Got your Message");
        size_t len = strlen(bufsnd.mtext)+1;

        if (msgsnd(msqid, &bufsnd, len, 0) == -1) {
            perror("msgsnd");
        } 
        
    }

    return 0;
}
