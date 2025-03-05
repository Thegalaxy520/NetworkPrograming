#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>

void * func(void* arg)
{
    int cfd = *(int*)arg;
    //先做线程分离
    pthread_detach(pthread_self());
    //然后开始通信
    int times = 0;
    while (1)
    {
        char buf[4096] = {0};
        int len = recv(cfd,buf,sizeof(buf),0);
        if (len ==0)
        {
            printf("客户端已经断开连接\n");
            break;
        }else if (len > 0)
        {
            printf("Received Buffer: %s\n", buf);
            char buf2[100] ={0};
            snprintf(buf2, sizeof(buf2), "Server Received! %d\n", ++times);
            send(cfd, buf2, strlen(buf2), 0);
        }else
        {
            perror("recv Error");
            break;
        }
    }
    close(cfd);
    *(int *)arg = -1;
    return NULL;
}


//创建存储文件描述符的数组
int fds[1024];
int main()
{
    // 忽略 SIGPIPE 信号
    signal(SIGPIPE, SIG_IGN);
    //1.创建套接字                                       
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if (sfd == -1)
    {
        perror("sfd Error");
        exit(-1);
    }
    //2.绑定
    struct sockaddr_in addr;
    addr.sin_port = htons(8989);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sfd,(struct sockaddr*)&addr,sizeof(addr)) == -1)
    {
        perror("Bind Error");
        exit(-1);
    }
    //3.设置监听
    if (listen(sfd, 128) == -1)
    {
        perror("Listen Error");
        exit(-1);
    }
    //4.阻塞等待客户端连接
    memset(fds,-1,sizeof(fds));
    while (1)
    {
        int cfd;
        struct sockaddr_in in_addr;
        socklen_t addrlen = sizeof(in_addr);
        if ((cfd = accept(sfd, (struct sockaddr*)&in_addr, &addrlen)) == -1)
        {
            perror("accept Error");
            continue; // 可以选择继续，而不是退出
        }
        //找一个空闲的文件描述符
        int* ptr = NULL;
        for (int i = 0 ; i < sizeof(fds)/sizeof(int);i++)
        {
            if (fds[i] == -1)
            {
                fds[i] = cfd;
                ptr = &fds[i];
                break;
            }
        }

        pthread_t pthid;
        int ret = pthread_create(&pthid, NULL, func, (void *)ptr);
        if (ret != 0) {
            fprintf(stderr, "pthread_create Error: %s\n", strerror(ret));
            exit(-1);
        }
        char ip_str[200];
        inet_ntop(AF_INET, &(in_addr.sin_addr), ip_str, sizeof(ip_str));
        printf("客户端的ip:%s, 客户端的端口号:%d\n", ip_str, ntohs(in_addr.sin_port));
        // 如果创建成功，则继续循环等待下一个连接
    }


}