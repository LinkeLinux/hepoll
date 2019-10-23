#include <public.h>
#define SERVER_PORT 8888
int main()
{

    //客户端只需要一个套接字文件描述符，用于和服务器通信

    int clientSocket;

    //描述服务器的socket

    struct sockaddr_in serverAddr;

    char sendbuf[200];

    char recvbuf[200];

    int iDataNum;

    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)

    {

        perror("socket");

        return 1;

    }

    serverAddr.sin_family = AF_INET;

    serverAddr.sin_port = htons(SERVER_PORT);

    //指定服务器端的ip，本地测试：127.0.0.1

    //inet_addr()函数，将点分十进制IP转换成网络字节序IP

    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)

    {

        perror("connect");

        return 1;

    }

    printf("连接到主机...\n");

    while(1)

    {

        printf("发送消息:");

        scanf("%s", sendbuf);

        printf("\n");

        send(clientSocket, sendbuf, strlen(sendbuf), 0);



        if(strcmp(sendbuf, "quit") == 0)

            break;
    }

    close(clientSocket);

    return 0;

}

