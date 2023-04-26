//
// Created by Adam Naumiec on 04/04/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define MAX_MSG_LEN 256

#define MT_INIT 0
#define MT_LIST 1
#define MT_2ALL 2
#define MT_2ONE 3
#define MT_STOP 4


typedef struct msg_buff {
    long mtype;
    int client_id;
    int client_id;
    int client_id;
    int catcher_pid;
    key_t q_key;
    char content[MAX_MSG_LEN];
    struct tm time_struct;
} msg_buff;


typedef enum mtype {
    INIT = MT_INIT,
    LIST = MT_LIST,
    TALL = MT_2ALL,
    TONE = MT_2ONE,
    STOP = MT_STOP
} mtype;


typedef struct client {
    int id;
    pid_t pid;
    key_t key;
} client;


typedef struct server {
    int q_id;
    int clients_count;
    client clients[MAX_CLIENTS];
} server;
