#ifndef __WRAPPERS__
#define __WRAPPERS__

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "lab05.h"

extern int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);

extern int Socket(int family, int type, int protocol);

extern ssize_t Recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
extern ssize_t  Sendto(int  s,  const  void  *buf,  size_t  len,  int flags, const struct sockaddr *to,
       socklen_t tolen) ;
extern int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

extern clock_t Clock();

extern ssize_t Readn(int fd, void *ptr, size_t nbytes);
extern void Writen(int fd, void *ptr, size_t nbytes);

extern int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen);

extern int Getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

extern int Close(int fd);

extern int Accept(int sockfd, struct sockaddr *addr, socklen_t* addrlen);
	

#endif /* __LAB03__ */
