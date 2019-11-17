#include <public.h>
#include <thread_pool.h>
#define MAX_LISTEN 1024
#define PORT 8888
#define EVENT 8192
int set_nonblock(int fd);
void do_request(void *arg)
{
    int rlen;
    int infd =*((int *)arg);
    char buf[100]={'\0'};
    printf("recv from connect %d\n",infd);
    rlen =read(infd,buf,sizeof(buf));
    if(rlen ==0){
        //epoll_ctl(epfd,EPOLL_CTL_DEL,infd,NULL);
        close(infd);
    }
    else{
        printf("recv %d bytes,sd:%d\n",rlen,infd);
    }
}
int read_buf(int sd,char *buf,int len)
{
    int offset=0;
    int rlen;
    while(len>0){
        rlen =read(sd,buf+offset,len);
        printf("rlen %d\n",rlen);
        if (rlen <0){
            if(errno ==EINTR || errno ==EAGAIN ||errno ==EWOULDBLOCK)
                continue;
            else 
            {
                perror("read");
                break;
            }
        }else if (rlen==0)
        {   
            printf("rlen is 0\n");
            break;
        }else {
            len -=rlen;
            offset +=rlen;
        }
    }
    return offset;
}
int open_listenfd(int port) 
{
    if (port <= 0) {
        port = 3000;
    }

    int listenfd, optval=1;
    struct sockaddr_in serveraddr;
  
    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
 
    /* Eliminates "Address already in use" error from bind. */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
           (const void *)&optval , sizeof(int)) < 0)
        return -1;

    /* Listenfd will be an endpoint for all requests to port
 *        on any IP address for this host */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons((unsigned short)port); 
    if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, MAX_LISTEN) < 0)
        return -1;
    if (set_nonblock(sd)<0)
    {
        perror("set nonblock failed");
        exit(-1);
    }

    return listenfd;
}
/*
 *The epoll wait call returns a file descriptor with event notification, and for each notified file descriptor, 
 *if readable, the file descriptor must be read until it is empty and errno returns EAGAIN, otherwise the next epoll wait will not return the remaining data 
 *and the event will be thrown away. And if your file descriptor is not unblocked, then the read all the time or write all the time is bound to block the last time
 * */
int set_nonblock(int fd)
{
    int flags, s;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl(fd, F_SETFL, flags);
    if (s == -1) {
        return -1;
    }

    return 0;
}
int AddaeEvent(aeEventLoop *el,int fd, int mask)
{
    struct epoll_event ev;
	
	int op =el->events[fd].mask =AE_NONE ? EPOLL_CTL_ADD:EPOLL_CTL_MOD;
	if (mask & AE_READABLE)
	{
		ev.events =EPOLLIN | EPOLLET;
	}
	if (mask & AE_WRITEABLE)
	{
		ev.events =EPOLLOUT | EPOLLET;
	}
	ev.data.fd =fd;
	
	if (epoll_ctl(el->epfd,op,fd,&ev) <0)
		return -1;
	return 0;
}
int acceptHandle(aeEventLoop *el,int fd, void *clientData,int mask)
{
	
}
int main()
{
    char buf[8192];
    int epfd;
    int infd;
    char srcIp[17];
    struct sockaddr_in addr;
    struct sockaddr_in raddr;
    socklen_t rlen;
    int i,index;
    //request_t r;
    struct epoll_event event;
    struct epoll_event events[EVENT];
    int sd;
    threadpool_t *pool;
    pool=thread_init(POOL_SIZE);
    if (pool==NULL)
    {
        printf("init thread pool failed\n");
        return -1;
    }
    sd =open_listenfd(PORT);
    if(sd <0)
    {
        printf("socket error\n");
        return -1;
    }
    epfd =epoll_create1(0);
    if (epfd <0)
    {
        perror("epoll_create1"); 
        exit(-1);
    }
    //struct timeval tm={3,0};
    for (;;){
        printf("waitting....\n");
        index =epoll_wait(epfd,events,EVENT,-1);
        printf("have %d events\n",index);
        for (i=0;i<index;i++){
            if (sd ==events[i].data.fd){
                infd =accept(sd,(struct sockaddr *)&raddr,&rlen);
                printf("recv connect from [%s],sd:%d\n",inet_ntop(AF_INET,&(raddr.sin_addr),srcIp,sizeof(srcIp)),sd);
                event.data.fd=infd;
                event.events =EPOLLIN;
                set_nonblock(infd);
                epoll_ctl(epfd,EPOLL_CTL_ADD,infd,&event);
            }else {
                #if 1
                 if ((events[i].events & EPOLLERR) ||
                    (events[i].events & EPOLLHUP) ||
                    (!(events[i].events & EPOLLIN))) {
                    close(infd);
                    continue;
                }
                infd =events[i].data.fd;
                printf("infd is %d\n",infd);
                thread_add(pool,do_request,infd);
                #if 0
                memset(buf,0x00,sizeof(buf));
                infd =events[i].data.fd;
                //rlen =read_buf(infd,buf,sizeof(buf));
                rlen =read(infd,buf,sizeof(buf));
                if(rlen ==0){
                    epoll_ctl(epfd,EPOLL_CTL_DEL,infd,NULL);
                    close(infd);
                }
                else{
                    printf("recv %d bytes,sd:%d\n",rlen,sd);
                }
                #endif 
            }
        }
    }
    return 0;
    
}
