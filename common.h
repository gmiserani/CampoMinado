#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

//struct que representa uma ação do jogador
struct action{
    int type;
    int coordinates[2];
    int board[4][4];
};

void print_matrix(int matrix[4][4]);

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize);
int addrparse(const char *addrstr, const char *portstr, struct sockaddr_storage *storage);
int server_sockaddr_init(const char *protocol_version, const char *portstr, struct sockaddr_storage *storage);
void logexit(const char *msg);
