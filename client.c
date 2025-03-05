#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main()
{
    //1.创建用于连接服务端的文件套接字
    int cfd;
    if ((cfd = socket(AF_INET,SOCK_STREAM, 0)) == -1)
    {
        perror("Socket Error");
        exit(-1);
    }
    //2.连接服务端
    struct sockaddr_in S_addr;
    S_addr.sin_family = AF_INET;
    S_addr.sin_port = htons(8989);
    if (inet_pton(AF_INET, "192.168.188.99", (void *) &S_addr.sin_addr.s_addr) == -1)
    {
        perror("inet_pton Error");
        exit(-1);
    }
    if (connect(cfd,(struct sockaddr *)&S_addr,sizeof(S_addr)) == -1)
    {
        perror("connect Error");
        exit(-1);
    }
    //3.通信
    int num = 0;
    while (1)
    {
        char buf[2048] = {0};
        scanf("%s",buf);
        int ret = send(cfd,buf,strlen(buf),0);
        char buf2[1024] = {0};
        int res = recv(cfd,buf2,sizeof(buf2),0);
        if (res == 0)
        {
            printf("=========服务器已经断开===========\n");
            break;
        }else if (res > 0)
        {
            printf("recv buf: %s",buf2);
        }else
        {
            perror("Recv Error");
            exit(-1);
        }
        sleep(1);
    }
    //4.断开连接
    close(cfd);
}
