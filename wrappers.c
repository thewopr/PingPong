#include <netinet/in.h>
#include <inttypes.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "wrappers.h"

int Bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen) {
	int retval;

	if( (retval = bind(sockfd,my_addr,addrlen)) < 0  ) {
		perror("Bind error");
		exit(-1);
	}
	return(retval);
}
int Socket(int family, int type, int protocol) {
		int retval;
		if( (retval = socket(family, type, protocol)) < 0) {
			perror("Socket error");
			exit(-1);
		}
		return(retval);
}

ssize_t Recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen) {
		int retval;
		if( (retval = recvfrom(s,buf,len,flags,from,fromlen) < 0)) {
				perror("Recvfrom error");
				exit(-1);
			}
			return(retval);
}

ssize_t  Sendto(int  s,  const  void  *buf,  size_t  len,  int flags, const struct sockaddr *to,
       socklen_t tolen) {

	int retval;
	if( (retval = sendto(s,buf,len,flags,to,tolen)) < 0) {
		perror("Sendto error");
		exit(-1);
	}
	return(retval);
}

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {

	int retval;

		if( (retval = select(nfds,readfds,writefds,exceptfds,timeout)) < 0) {
			perror("Select error");
			exit(-1);
		}
	return(retval);
}

clock_t Clock() {
	clock_t retval;

	if( (retval = clock()) == -1) {
		perror("Clock had an error");
		exit(-1);
	}

	return retval;
}


ssize_t Readn(int fd, void *ptr, size_t nbytes)
{
     ssize_t		n;
     
     if ( (n = readn(fd, ptr, nbytes)) < 0) {
	  printf("readn error\n");
	  exit(-1);
     }
     return(n);
}

void Writen(int fd, void *ptr, size_t nbytes)
{
  if (writen(fd, ptr, nbytes) != nbytes) {
    printf("writen error\n");
    exit(-1);
  }
}

int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen) {

    int retval;

    if((retval = connect(sockfd,serv_addr, addrlen)) < 0) {
        perror("Connect error");
        exit(-1);
    }
    return retval;
}
int Getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {

    int retval;

    if((retval = getaddrinfo(node,service, hints, res)) < 0) {
        perror("getaddrinfo error");
        exit(-1);
    }

    return retval;
}

int Close(int fd) {
    int retval;

    if((retval = close(fd)) < 0) {
        perror("close error");
        exit(-1);
    }

	return retval;
}

int Accept(int sockfd, struct sockaddr * addr, socklen_t* addrlen) {
	int retval;

	if(( retval = accept(sockfd, addr, addrlen)) < 0) {
		perror("Accept error");
		exit(-1);
	}
	return retval;
}


