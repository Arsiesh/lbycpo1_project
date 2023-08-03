#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
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
    int qid, opt, cl;
    char buf[200];
};
struct message
{
    long message_type; // message_type of long type variable is required
    struct message_text message_text;
};
int main()
{
    key_t server_msg_queue_key;
    key_t client01_msg_queue_key;
    key_t client02_msg_queue_key;
    key_t client03_msg_queue_key;
    key_t client04_msg_queue_key;
    int qid, client1_qid, client2_qid, client3_qid, client4_qid;
    int client01_qid, client02_qid, client03_qid, client04_qid;
    struct message message;
    /* create token for the message queue */
    server_msg_queue_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID);
    client01_msg_queue_key = ftok(CLIENT1_KEY_PATHNAME, PROJECT_ID1);
    client02_msg_queue_key = ftok(CLIENT2_KEY_PATHNAME, PROJECT_ID2);
    client03_msg_queue_key = ftok(CLIENT3_KEY_PATHNAME, PROJECT_ID3);
    client04_msg_queue_key = ftok(CLIENT4_KEY_PATHNAME, PROJECT_ID4);

    /* create queue for receiving messages from clients */
    if ((qid = msgget(server_msg_queue_key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    /* create client queue for receiving messages from server */
    if ((client1_qid = msgget(client01_msg_queue_key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget: client_qid");
        exit(1);
    }

    if ((client2_qid = msgget(client02_msg_queue_key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget: client_qid");
        exit(1);
    }

    if ((client3_qid = msgget(client03_msg_queue_key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget: client_qid");
        exit(1);
    }

    if ((client4_qid = msgget(client04_msg_queue_key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget: client_qid");
        exit(1);
    }

    printf("Server: ready to receive requests.\n");

    /* get the message queue ID of the clients*/
    if ((client01_qid = msgget(client01_msg_queue_key, 0)) == -1)
    {
        perror("msgget: client01_qid");
        exit(1);
    }

    if ((client02_qid = msgget(client02_msg_queue_key, 0)) == -1)
    {
        perror("msgget: client02_qid");
        exit(1);
    }

    if ((client03_qid = msgget(client03_msg_queue_key, 0)) == -1)
    {
        perror("msgget: client03_qid");
        exit(1);
    }

    if ((client04_qid = msgget(client04_msg_queue_key, 0)) == -1)
    {
        perror("msgget: client04_qid");
        exit(1);
    }

    while (1)
    {
        /* read an incoming message from client*/
        if (msgrcv(qid, &message, sizeof(struct message_text), 0, 0) == -1)
        {
            perror("msgrcv");
            exit(1);
        }

        int client_num;
        int client_qid = message.message_text.qid;
        if (client_qid == client01_qid)
            client_num = 1;
        else if (client_qid == client02_qid)
            client_num = 2;
        else if (client_qid == client03_qid)
            client_num = 3;
        else if (client_qid == client04_qid)
            client_num = 4;

        printf("Server: message received from %d.\n", client_num);

        /* process message */

        char response_buf[200];
        int remaining_space;

        if (message.message_text.opt == 1)
            remaining_space = sizeof(response_buf) - snprintf(response_buf, sizeof(response_buf), "Client %d: ", client_num);
        else if (message.message_text.opt == 2)
            remaining_space = sizeof(response_buf) - snprintf(response_buf, sizeof(response_buf), "(PM) Client %d: ", client_num);
        strncat(response_buf, message.message_text.buf, remaining_space);

        strncpy(message.message_text.buf, response_buf, sizeof(message.message_text.buf) - 1);

        message.message_text.qid = qid;

        /* send reply message to client */

        if (message.message_text.opt == 1)
        {
            if (msgsnd(client01_qid, &message, sizeof(struct message_text), 0) == -1)
            {
                perror("msgsnd");
                exit(1);
            }

            if (msgsnd(client02_qid, &message, sizeof(struct message_text), 0) == -1)
            {
                perror("msgsnd");
                exit(1);
            }

            if (msgsnd(client03_qid, &message, sizeof(struct message_text), 0) == -1)
            {
                perror("msgsnd");
                exit(1);
            }

            if (msgsnd(client04_qid, &message, sizeof(struct message_text), 0) == -1)
            {
                perror("msgsnd");
                exit(1);
            }
        }
        else if (message.message_text.opt == 2)
        {
            if (message.message_text.cl == 1)
            {
                if (msgsnd(client01_qid, &message, sizeof(struct message_text), 0) == -1)
                {
                    perror("msgsnd");
                    exit(1);
                }
            }
            else if (message.message_text.cl == 2)
            {
                if (msgsnd(client02_qid, &message, sizeof(struct message_text), 0) == -1)
                {
                    perror("msgsnd");
                    exit(1);
                }
            }
            else if (message.message_text.cl == 3)
            {
                if (msgsnd(client03_qid, &message, sizeof(struct message_text), 0) == -1)
                {
                    perror("msgsnd");
                    exit(1);
                }
            }
            else if (message.message_text.cl == 4)
            {
                if (msgsnd(client04_qid, &message, sizeof(struct message_text), 0) == -1)
                {
                    perror("msgsnd");
                    exit(1);
                }
            }

            printf("Server: response sent to clients\n");
        }
    }
}
