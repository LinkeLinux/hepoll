#include <public.h>
#define MAX_LISTEN 1024
#define PORT 8888
#define EVENT 8192
int read_buf(int sd,char *buf,int len)
{
    int offset=0;
    int rlen;
    while(len>0){
        rlen =read(sd,buf+offset,len);
        if (rlen <0){
            if(errno ==EINTR|| errno ==EAGAIN ||errno ==EWOULDBLOCK)
                continue;
            else 
            {
                perror("read");
                break;
            }
        }else {
            len -=rlen;
            offset +=rlen;
        }
    }
    return offset;
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
    struct epoll_event event[EVENT];
    struct epoll_event events[EVENT];
    int sd =socket(AF_INET,SOCK_STREAM,0);
    if (sd <0)
    {
        perror("socket");
        exit(-1);
    }
    addr.sin_family=AF_INET;
    addr.sin_port =htons((unsigned short)PORT);
    addr.sin_addr.s_addr= htonl(INADDR_ANY);
    if  (bind(sd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in)) <0)
    {
        perror("bind");
        exit(-1);
    }

    if(listen(sd,MAX_LISTEN)<0)
    {
        perror("listen");
        exit(-1);
    }
    epfd =epoll_create1(0);
    if (epfd <0)
    {
        perror("epoll_create1"); 
        exit(-1);
    }
    if (epoll_ctl(epfd,EPOLL_CTL_ADD,sd,event)<0)
    {
        perror("epoll_ctl");
        exit(-1);
    }
    struct timeval tm={3,0};
    for (;;){
        printf("waitting....\n");
        index =epoll_wait(epfd,events,EVENT,tm);
        for (i=0;i<index;i++){
            if (sd ==events[i].data.fd){
                infd =accept(sd,(struct sockaddr *)&raddr,&rlen);
                printf("recv connect from [%s]\n",inet_ntop(AF_INET,&(raddr.sin_addr),srcIp,sizeof(srcIp)));
                epoll_ctl(epfd,EPOLL_CTL_ADD,infd,event);
            }else if (event[i].events|EPOLLIN) {
                
                memset(buf,0x00,sizeof(buf));
                rlen =read_buf(events[i].data.fd,buf,sizeof(buf));
                memset(srcIp,0x00,sizeof(srcIp));
                printf("recv %d bytes\n",rlen);

            }else {
                perror("epoll_wait");
                exit(-1);
            }
        }
    }
    return 0;
    
}
