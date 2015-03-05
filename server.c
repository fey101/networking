/*
** Server Program -- writes to a message queue
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
//Creating a unique key
    if ((key = ftok("server.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
//Initialize the message queue
    if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("Message to the Client Processes, [Cltrl+D to quit]:\n");

    //buf.mtype = 1; //message type

    while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) {

        (void) strcpy(buf.mtext, "Am the Boss");
        int len = strlen(buf.mtext);
        printf("Receiver ID\n");
        scanf("%ld", &buf.mtype);

        /* Remove a newline at end, if it exists */
        if (buf.mtext[len-1] == '\n') 
            {
                buf.mtext[len-1] = '\0';
            }

        if (msgsnd(msqid, &buf, len+1, 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }

        if (msgrcv(msqid, &buf1, sizeof(buf1.mtext), 3, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }
        //reply from client
        printf("Client: \"%s\"\n", buf1.mtext);
    }

//delete the message queue
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }

    return 0;
}
