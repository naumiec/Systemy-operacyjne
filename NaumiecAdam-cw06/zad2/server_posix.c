//
// Created by Adam Naumiec on 04/04/2023.
//
#include "utilities.h"


mqd_t sQueue;
client clients[MAX_CLIENTS];
char tmp[MAX_MSG_LEN];


void signal_handler(int sig) {
    msg_buff msg;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].id != -1) {
            msg.mtype = STOP;
            mqd_t catcher_desc = mq_open(clients[i].content, O_RDWR);
            mq_send(catcher_desc, (char *) &msg, sizeof(msg_buff), 0);
            mq_receive(sQueue, (char *) &msg, sizeof(msg_buff), NULL);
            mq_close(catcher_desc);
        }
    }

    mq_close(sQueue);
    exit(EXIT_SUCCESS);
}


void rcv_init(msg_buff msg) {
    int client_idx = 0;
    while (client_idx < MAX_CLIENTS && clients[client_idx].id != -1) {
        client_idx++;
    }

    if (client_idx == MAX_CLIENTS) {
        printf("Max clients: %d", MAX_CLIENTS);
        msg.client_id = -1;
    } else {
        clients[client_idx].id = msg.client_id;
        strcpy(clients[client_idx].content, msg.content);
    }

    msg.client_id = client_idx;
    mqd_t cQueueDesc = mq_open(msg.content, O_RDWR);
    mq_send(cQueueDesc, (char *) &msg, sizeof(msg_buff), 0);
    mq_close(cQueueDesc);

    struct tm msg_time = msg.time_struct;
    printf("Time: %02d:%02d:%02d, id: %d",
           msg_time.tm_hour,
           msg_time.tm_min,
           msg_time.tm_sec,
           clients[client_idx].id);
}


void rcv_list(msg_buff msg) {
    int idx = msg.client_id;
    strcpy(msg.content, "");

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].id != -1 && i != idx) {
            sprintf(tmp, "Id: %d", clients[i].id);
            strcat(msg.content, tmp);
        }
    }

    struct tm msg_time = msg.time_struct;
    printf("Id: %d, time: %02d:%02d:%02d",
           clients[idx].id,
           msg_time.tm_hour,
           msg_time.tm_min,
           msg_time.tm_sec);

    mqd_t cQueueDesc = mq_open(clients[idx].content, O_RDWR);
    mq_send(cQueueDesc, (char *) &msg, sizeof(msg_buff), 0);
    mq_close(cQueueDesc);
}


void rcv_2all(msg_buff msg) {
    int idx = msg.client_id;
    msg.client_id = clients[idx].id;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (i != idx && clients[i].id != -1) {
            mqd_t catcher_desc = mq_open(clients[i].content, O_RDWR);
            mq_send(catcher_desc, (char *) &msg, sizeof(msg_buff), 0);
            mq_close(catcher_desc);
        }
    }

    struct tm msg_time = msg.time_struct;
    printf("Id: %d, time: %02d:%02d:%02d",
           clients[idx].id,
           msg_time.tm_hour,
           msg_time.tm_min,
           msg_time.tm_sec);
}


void rcv_2one(msg_buff msg) {
    int catcher_pid = msg.catcher_pid;

    int i = 0;
    while (i < MAX_CLIENTS && clients[i].id != catcher_pid) {
        i++;
    }

    if (i == MAX_CLIENTS) {
        printf("Cannot find pid: %d", catcher_pid);
    } else {
        msg.client_id = clients[msg.client_id].id;
        mqd_t catcher_desc = mq_open(clients[i].content, O_RDWR);
        mq_send(catcher_desc, (char *) &msg, sizeof(msg_buff), 0);
        mq_close(catcher_desc);

        struct tm msg_time = msg.time_struct;
        printf("Id: %d, time: %02d:%02d:%02d, pid: %d",
               msg.client_id,
               msg_time.tm_hour,
               msg_time.tm_min,
               msg_time.tm_sec,
               catcher_pid);
    }
}


void rcv_stop(msg_buff msg) {
    int idx = msg.client_id;
    printf("Client %d disconnected", clients[idx].id);
    clients[idx].id = -1;
}


int main() {
    signal(SIGINT, signal_handler);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client *new_client = malloc(sizeof(client));
        new_client->id = -1;
        new_client->content = (char *) calloc(LENN, sizeof(char));
        clients[i] = *new_client;
    }

    mq_unlink(SQUEUE);
    sQueue = create_queue(SQUEUE);
    msg_buff msg;

    while (1) {
        mq_receive(sQueue, (char *) &msg, sizeof(msg_buff), NULL);

        switch (msg.mtype) {
            case INIT:
                rcv_init(msg);
                break;
            case STOP:
                rcv_stop(msg);
                break;
            case LIST:
                rcv_list(msg);
                break;
            case TALL:
                rcv_2all(msg);
                break;
            case TONE:
                rcv_2one(msg);
                break;
            default:
                printf("Wrong message type %ld", msg.mtype);
        }
    }
}
