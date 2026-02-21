/* cache22.c  */

#include "cache22.h"

bool scontinue;
bool ccontinue;

void zero(int8 *buf, int16 size) {
  int8 *p;
  int16 n;
  for (n = 0, p = buf; n < size; n++, p++) {
    *p = 0;
  }
  return;
}

int initserver(int16 port) {
  struct sockaddr_in sock;
  int s;

  sock.sin_family = AF_INET;
  sock.sin_port = htons(port);
  sock.sin_addr.s_addr = inet_addr(HOST);

  s = socket(AF_INET, SOCK_STREAM, 0);

  assert(s > 0);
  errno = 0;
  if (bind(s, (struct sockaddr_in *)&sock, sizeof(sock))) {
    assert_perror(errno);
  }
  errno = 0;
  if (listen(s, 20)) {
    assert_perror(errno);
  }
  log("Server listening on %s : %d\n", HOST, port);

  return s;
}

/// create /User/Login hello from world?

void child_loop(Client *client) {
  char buf[256];
  char cmd[256];
  char folder[256];
  char args[256];

  char *p, *start;

  /* Clear everything safely */
  zero((int8 *)buf, 255);
  zero((int8 *)cmd, 255);
  zero((int8 *)folder, 255);
  zero((int8 *)args, 255);

  /* Read input */
  int r = read(client->s, buf, sizeof(buf) - 1);
  if (r <= 0)
    return;

  buf[r] = '\0'; /* IMPORTANT: null-terminate */

  p = buf;

  // NOTE: CMD

  start = p;
  while (*p && *p != ' ' && *p != '\n' && *p != '\r')
    p++;

  strncpy(cmd, start, p - start);
  cmd[p - start] = '\0';

  if (*p == '\0' || *p == '\n' || *p == '\r')
    goto done;

  /* Skip space */
  while (*p == ' ')
    p++;

  // NOTE: FOLDER

  start = p;
  while (*p && *p != ' ' && *p != '\n' && *p != '\r')
    p++;

  strncpy(folder, start, p - start);
  folder[p - start] = '\0';

  if (*p == '\0' || *p == '\n' || *p == '\r')
    goto done;

  /* Skip space */
  while (*p == ' ')
    p++;

  // NOTE: ARGS
  start = p;
  while (*p && *p != '\n' && *p != '\r')
    p++;

  strncpy(args, start, p - start);
  args[p - start] = '\0';

done:

  dprintf(client->s, "\ncmd:\t%s\n", cmd);
  dprintf(client->s, "folder:\t%s\n", folder);
  dprintf(client->s, "args:\t%s\n", args);
}

void mainloop(int s) {
  int32 len;
  int16 port;
  char *ip;
  int s2;
  struct sockaddr_in cli;
  Client *client;
  pid_t pid;

  len = sizeof(cli);

  s2 = accept(s, (struct sockaddr *)&cli, (unsigned int *)&len);
  if (s2 < 0) {
    return;
  }
  port = (int16)ntohs((int)cli.sin_port);
  ip = inet_ntoa(cli.sin_addr);

  log("connection from %s:%d\n", ip, port);

  client = (Client *)malloc(sizeof(struct s_client));
  assert(client);

  zero((int8 *)client, sizeof(struct s_client));
  client->s = s2;
  client->port = port;
  strncpy(client->ip, ip, 15);

  pid = fork();
  if (pid) {
    free(client);

    return;
  } else {
    dprintf(s2, "100 connected to server \n");
    ccontinue = true;
    while (ccontinue) {
      child_loop(client);
    }
    close(s2);
    free(client);
    return;
  }

  return;
}

int main(int argc, char *argv[]) {

  int16 port;
  char *sport;
  port = 0;
  int s;

  if (argc < 2) {
    sport = PORT;
  } else {
    sport = argv[1];
  }

  port = (int16)atoi(sport);

  s = initserver(port);
  scontinue = true;

  while (scontinue) {
    mainloop(s);
  }

  printf("Shuting down server..\n");
  close(s);
  return 0;
}

#pragma GCC diagnostic pop
