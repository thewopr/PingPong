#include "lab05.h"

void printsin(struct sockaddr_in *s, char *str1, char *str2) {
     char ip[16];
     printf("%s %s", str1, str2);
     inet_ntop((*s).sin_family, &((*s).sin_addr.s_addr), ip, sizeof(ip));
     printf("ip= %s, port= %d\n", ip, ntohs(s->sin_port));
		 fflush(stdout);
}


ssize_t				
readn(int fd, void *vptr, size_t n)
{
     size_t	nleft;
     ssize_t	nread;
     char	*ptr;
     
     ptr = vptr;
     nleft = n;
     while (nleft > 0) {
	  if ( (nread = read(fd, ptr, nleft)) < 0) {
	       if (errno == EINTR)
		    nread = 0; /* and call read() again */
	       else
		    return(-1);
	  } else if (nread == 0)
	       break; /* EOF */
	  
	  nleft -= nread;
	  ptr   += nread;
     }
     return(n - nleft); /* return >= 0 */
}

ssize_t						
writen(int fd, const void *vptr, size_t n)
{
  size_t		nleft;
  ssize_t		nwritten;
  const char	*ptr;
  
	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
    }
    
    nleft -= nwritten;
    ptr   += nwritten;
  }
  return(n);
}
