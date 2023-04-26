//
// Created by Adam Naumiec on 04/04/2023.
//
#include "utilities.h"


int server_q;
client clients[MAX_CLIENTS];
char tmp[MAX_MSG_LEN];


void signal_handler(int sig) {
    if (server_q != -1) {
        msg_buff *msg = malloc(sizeof(msg_buff));
        msg->mtype = STOP;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].key != -1) {
                int client_qid = msgget(clients[i].key, 0);
                msgsnd(client_qid, msg, sizeof(msg_buff), 0);
            }
        }
    }

    msgctl(server_q, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}


void rcv_init(msg_buff *msg) {
    int client_idx = 0;
    while (client_idx < MAX_CLIENTS && clients[client_idx].key != -1) {
        client_idx++;
    }

    if (client_idx == MAX_CLIENTS) {
        printf("Max clients: %d", MAX_CLIENTS);
        msg->client_id = -1;
    } else {
        clients[client_idx].id = msg->client_id;
        clients[client_idx].key = msg->q_key;
    }

    msg->client_id = client_idx;
    int client_qid = msgget(clients[client_idx].key, 0);
    msgsnd(client_qid, msg, sizeof(msg_buff), 0);

    struct tm msg_time = msg->time_struct;
    printf("Server time: %02d:%02d:%02d, client id: %d",
           msg_time.tm_hour,
           msg_time.tm_min,
           msg_time.tm_sec,
           clients[client_idx].id);
}


void rcv_list(msg_buff *msg) {
    int idx = msg->client_id;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].key != -1 && i != idx) {
            sprintf(tmp, "Id: %d", clients[i].id);
            strcat(msg->content, tmp);
        }
    }

    struct tm msg_time = msg->time_struct;
    printf("Id: %d, time: %02d:%02d:%02d",
           clients[idx].id,
           msg_time.tm_hour,
           msg_time.tm_min,
           msg_time.tm_sec);

    int client_qid = msgget(clients[idx].key, 0);
    msgsnd(client_qid, msg, sizeof(msg_buff), 0);
}


void rcv_2all(msg_buff *msg) {
    int idx = msg->client_id;
    msg->client_id = clients[idx].id;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (i != idx && clients[i].key != -1) {
            int client_qid = msgget(clients[i].key, 0);
            msgsnd(client_qid, msg, sizeof(msg_buff), 0);
        }
    }

    struct tm msg_time = msg->time_struct;
    printf("Id: %d, time: %02d:%02d:%02d",
           clients[idx].id,
           msg_time.tm_hour,
           msg_time.tm_min,
           msg_time.tm_sec);
}


void rcv_2one(msg_buff *msg) {
    int catcher_pid = msg->catcher_pid;

    int i = 0;
    while (i < MAX_CLIENTS && clients[i].id != catcher_pid) {
        i++;
    }

    if (i == MAX_CLIENTS) {
        printf("Pid: %d", catcher_pid);
    } else {
        int client_qid = msgget(clients[i].key, 0);
        msg->client_id = clients[msg->client_id].id;
        msgsnd(client_qid, msg, sizeof(msg_buff), 0);

        struct tm msg_time = msg->time_struct;
        printf("Id: %d, time: %02d:%02d:%02d, pid: %d",
               msg->client_id,
               msg_time.tm_hour,
               msg_time.tm_min,
               msg_time.tm_sec,
               catcher_pid);
    }
}


void rcv_stop(msg_buff *msg) {
    int idx = msg->client_id;
    int client_qid = msgget(clients[idx].key, 0);

    clients[idx].id = -1;
    clients[idx].key = -1;
    msgsnd(client_qid, msg, sizeof(msg_buff), 0);
}


int main() {
    signal(SIGINT, signal_handler);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        client *new_client = malloc(sizeof(client));
        new_client->key = -1;
        clients[i] = *new_client;
    }

    key_t s_key = ftok(getenv("HOME"), 1);
    if (s_key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    server_q = msgget(s_key, IPC_CREAT | 0666);
    if (server_q == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    msg_buff *msg = malloc(sizeof(msg_buff));

    while (1) {
        msgrcv(server_q, msg, sizeof(msg_buff), -6, 0);

        switch (msg->mtype) {
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
                printf("Message error!");
        }
    }
}
