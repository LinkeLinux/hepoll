#include <public.h>
#define SERVER_PORT 8888
struct sockaddr_in serverAddr;
void *sendfunc(void *arg)
{
    
    int clientSocket;
        if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)

        {

            perror("socket");

            return 1;

        }
    printf("clientSocket:%d\n",clientSocket);
    if(connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {

        perror("connect");

        return ;

    }
    printf("连接到主机...\n");
    char sendbuf[100];
    pthread_t pid;
    pid =pthread_self();
    memset(sendbuf,0x00,sizeof(sendbuf));
    sprintf(sendbuf,"hello%d",pid);
    send(clientSocket, sendbuf, strlen(sendbuf), 0);
    
    
}

int main()
{

    //客户端只需要一个套接字文件描述符，用于和服务器通信

    int clientSocket;

    //描述服务器的socket


    char sendbuf[200];

    char recvbuf[200];
    int i;
    int iDataNum;
    pthread_t pid[1024];


    serverAddr.sin_family = AF_INET;

    serverAddr.sin_port = htons(SERVER_PORT);

    //指定服务器端的ip，本地测试：127.0.0.1

    //inet_addr()函数，将点分十进制IP转换成网络字节序IP

    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    for(i=0;i<10;i++){
        pthread_create(&pid[i],NULL,sendfunc,NULL);
    }

   sleep(10);
   close(clientSocket);


    return 0;

}

