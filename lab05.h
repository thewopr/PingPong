#ifndef __LAB03__
#define __LAB03__

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include "wrappers.h"

/* Change 9900 to the number you were assigned. */
#define MY_PORT 9260

#define SOURCE_TO_GATEWAY MY_PORT+0
#define GATEWAY_TO_SOURCE MY_PORT+1
#define SINK_TO_GATEWAY   MY_PORT+2
#define GATEWAY_TO_SINK   MY_PORT+3

typedef struct { 
  char head; 
  short seq;
  u_long body; 
  char tail; 
} message;

extern void printsin(struct sockaddr_in *s, char *str1, char *str2);
extern ssize_t readn(int fd, void *vptr, size_t n);
extern ssize_t writen(int fd, const void *vptr, size_t n);

#endif /* __LAB03__ */
