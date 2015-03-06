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

//Creating a unique key
    if ((key = ftok("server.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
//Initialize the message queue
    if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget Initialization failed");
        exit(1);
    }
    
    printf("Server Started Running:,Message Queue ID=%d \n [Cltrl+D to quit]:\n", msqid);

    while(count>0) {

        printf("To ID:\n");
        scanf("%ld", &bufsnd.mtype);
        //printf("Message:\n");
        getchar();
        printf("Message:\n");
        fgets(bufsnd.mtext, sizeof bufsnd.mtext, stdin);
        //scanf("%s", bufsnd.mtext);
        //(void) strcpy(bufsnd.mtext, "Am the Boss--->>");
        //message length
        int len = strlen(bufsnd.mtext);

        if(bufsnd.mtext[len-1]=='\n')
        {
            bufsnd.mtext[len-1]='\0';
        }


        if (msgsnd(msqid, &bufsnd, len+1, 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }
        printf("Message sent\n");
        printf("********************************************\n");

        if (msgrcv(msqid, &bufrecv, sizeof(bufrecv.mtext), 9, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        //Message display
        printf("Reply: %s \n", bufrecv.mtext);


    }

//delete the message queue
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }

    return 0;
}
