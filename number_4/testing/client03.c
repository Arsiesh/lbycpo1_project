#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#define SERVER_KEY_PATHNAME "cpo1server"
#define PROJECT_ID 'L'

#define CLIENT1_KEY_PATHNAME "client01"
#define CLIENT2_KEY_PATHNAME "client02"
#define CLIENT3_KEY_PATHNAME "client03"
#define CLIENT4_KEY_PATHNAME "client04"
#define PROJECT_ID1 'A'
#define PROJECT_ID2 'B'
#define PROJECT_ID3 'C'
#define PROJECT_ID4 'D'

/* define the message structure */
struct message_text
{
    int qid, opt,cl;
    char buf[198];
};

struct message
{
    long message_type; // message_type of long type variable is required
    struct message_text message_text;
};

int main()
{
    key_t server_queue_key;
    key_t client01_msg_queue_key;
    key_t client02_msg_queue_key;
    key_t client03_msg_queue_key;
    key_t client04_msg_queue_key;
    int server_qid, client_qid;
    int option;
    struct message my_message, return_message;

    /* create token for client */
    client01_msg_queue_key = ftok(CLIENT1_KEY_PATHNAME, PROJECT_ID1);
    client02_msg_queue_key = ftok(CLIENT2_KEY_PATHNAME, PROJECT_ID2);
    client03_msg_queue_key = ftok(CLIENT3_KEY_PATHNAME, PROJECT_ID3);
    client04_msg_queue_key = ftok(CLIENT4_KEY_PATHNAME, PROJECT_ID4);

    /* create client queue for receiving messages from server */
    if ((client_qid = msgget(client03_msg_queue_key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget: client_qid");
        exit(1);
    }

    /* get the message queue ID of the server */
    server_queue_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID);

    if ((server_qid = msgget(server_queue_key, 0)) == -1)
    {
        perror("msgget: server_qid");
        exit(1);
    }

    /* set the message parameters */
    my_message.message_type = 1;
    my_message.message_text.qid = client_qid;

    /* ask input from user */
    //printf("Please type a message: ");

    fd_set read_fds;
    struct timeval timeout;

    printf("Choose an option\n1: Groupchat\n2: PM\n");
    scanf("%d", &option);
  
    if (option == 2){
    	printf("Which client do you want to PM?\n [1,2,3,4]\n");
    	scanf("%i", &my_message.message_text.cl);
    }
  
    if (option == 1){
    my_message.message_text.opt = 1;
        while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        // Check if there's any input from user
        int select_ret = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
        if (select_ret == -1)
        {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            // User entered some input
            scanf("%s", my_message.message_text.buf);

            /* remove newline from string */
            int length = strlen(my_message.message_text.buf);
            if (my_message.message_text.buf[length - 1] == '\n')
                my_message.message_text.buf[length - 1] = '\0';

            if (strlen(my_message.message_text.buf) == 0)
                break;

            /* send message to server */
            if (msgsnd(server_qid, &my_message, sizeof(struct message_text), 0) == -1)
            {
                perror("client: msgsnd");
                exit(1);
            }
        }

        /* read response from server */
        if (msgrcv(client_qid, &return_message, sizeof(struct message_text), 0, IPC_NOWAIT) != -1)
        {
            /* process return message from server */
            //printf("Please type a message: ");
            printf("%s\n\n",
                   return_message.message_text.buf);
        }
    }
    }else if (option == 2){
    
    my_message.message_text.opt = 2;
        while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        // Check if there's any input from user
        int select_ret = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
        if (select_ret == -1)
        {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            // User entered some input
            scanf("%s", my_message.message_text.buf);

            /* remove newline from string */
            int length = strlen(my_message.message_text.buf);
            if (my_message.message_text.buf[length - 1] == '\n')
                my_message.message_text.buf[length - 1] = '\0';

            if (strlen(my_message.message_text.buf) == 0)
                break;

            /* send message to server */
            if (msgsnd(server_qid, &my_message, sizeof(struct message_text), 0) == -1)
            {
                perror("client: msgsnd");
                exit(1);
            }
        }

        /* read response from server */
        if (msgrcv(client_qid, &return_message, sizeof(struct message_text), 0, IPC_NOWAIT) != -1)
        {
            /* process return message from server */
            //printf("Please type a message: ");
            printf("%s\n\n",
                   return_message.message_text.buf);
        }
    }
    }

    /* remove message queue */
    if (msgctl(client_qid, IPC_RMID, NULL) == -1)
    {
        perror("client: msgctl");
        exit(1);
    }

    printf("Client: bye\n");
    exit(0);
}

