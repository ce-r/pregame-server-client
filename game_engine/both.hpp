
#ifndef BOTH_HPP
#define BOTH_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>  // for inet_pton()
#include <netinet/in.h>
#include <sys/socket.h>

#define NITS_SOCKET_ERROR   (-1)
#define NITS_SOCKET_OK      (0)
#define MAXLINE 4096
#define MAX_ATTEMPTS 6
#define MY_PORT 8406

static int global_servfd = -1; 

#endif
