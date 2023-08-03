#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

struct mesgq{
	long type;
	char text[200]; 
}mq;

void main(){
	int msqid1, len;
	key_t key1 = 1000;

	if ((msqid1 = msgget(key1, 0644)) == -1){
		printf("Server not active\n");
		exit(1);
	}
	printf("Client 1 Ready :\n");
	while (msgrcv(msqid1, &mq, sizeof(mq.text), 1, 0) != -1){
		printf("From Server: \"%s\"\n", mq.text);
		fgets(mq.text, sizeof(mq.text),stdin);
		len = strlen(mq.text);
		if (mq.text[len-1] == '\n')
		mq.text[len-1] = '\0';
		msgsnd(msqid1, &mq, len+1, 0);
	}
	printf("Server Disconnected\n");
}
