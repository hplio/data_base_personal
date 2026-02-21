/* cache22.h  */

#ifndef CACHE22
#define CACHE22

#define _GNU_SOURCE

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic push

#define HOST "127.0.0.1"
#define PORT "16000"

#define log(x, args...) printf(x, args);

typedef unsigned int int32;
typedef unsigned short int int16;
typedef unsigned char int8;

struct s_client {
  int s;
  char ip[16];
  int16 port;
};

typedef struct s_client Client;

void mainloop(int);
int initserver(int16);
void zero(int8 *, int16);
void child_loop(Client *);
int main(int, char **);

#endif
