#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/msg.h>
#define SERVER_KEY_PATHNAME "cpo1server"
#define PROJECT_ID 'L'
/* define the message structure */
struct message_text
{
    int qid;
    char buf[200];
};
struct message
{
    long message_type; // message_type of long type variable is required
    struct message_text message_text;
};



int main()
{


    key_t server_queue_key;
    int server_qid, client_qid;
    struct message my_message, return_message;
    /* create client queue for receiving messages from server */
    if ((client_qid = msgget(IPC_PRIVATE, 0660)) == -1)
    {
        perror("msgget: client_qid");
        exit(1);
    }
    /* create token for the message queue */
    server_queue_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID);
    /* get the message queue ID of the server*/
    if ((server_qid = msgget(server_queue_key, 0)) == -1)
    {
        perror("msgget: server_qid");
        exit(1);
    }
    /* set the message parameters */
    my_message.message_type = 1;
    my_message.message_text.qid = client_qid;
    /* ask input from user */
    printf("Please type a message: ");
    while (fgets(my_message.message_text.buf, 198, stdin))
    {
        /* remove newline from string */
        int length = strlen(my_message.message_text.buf);
        if (my_message.message_text.buf[length - 1] == '\n')
            my_message.message_text.buf[length - 1] = '\0';
        if (strlen(my_message.message_text.buf) == 0)
            break;
        /* send message to server */
        if (msgsnd(server_qid, &my_message,
                   sizeof(struct message_text), 0) == -1)
        {
            perror("client: msgsnd");
            exit(1);
        }
        /* read response from server */
        if (msgrcv(client_qid, &return_message,
                   sizeof(struct message_text), 0, 0) == -1)
        {
            perror("client: msgrcv");
            exit(1);
        }
        /* process return message from server */
        printf("Message received from server: %s\n\n",
               return_message.message_text.buf);
        printf("Please type a message: ");
    }
    /* remove message queue */
    if (msgctl(client_qid, IPC_RMID, NULL) == -1)
    {
        perror("client: msgctl");
        exit(1);
    }
    
    if (msgctl(client_qid, IPC_RMID, NULL) == -1) {
		fprintf(stderr, "Message queue could not be deleted.\n");
		exit(EXIT_FAILURE);
	}
    printf("Client: bye\n");
    exit(0);
}
