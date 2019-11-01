#ifndef _EPOLL_H_
#define _EPOLL_H_

typedef struct request {
   int epfd;
   char buf[8192];
}request_t;
#endif 
