#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>


int main()
{
  //1.创建通信套接字
  int sfd = socket(AF_INET,SOCK_STREAM, 0);
  if (sfd < 0)
  {
    perror("SOCKET ERROR");
    exit(-1);
  }

  //设置套接字选项
  int opt = 1;
  //设置端口复用，防止服务器重启时因端口占用而无法重启
  if (setsockopt(sfd,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt))<0)
  {
    perror("SET Sock Option Error");
    exit(-1);
  }
  //2.绑定套接字
  struct sockaddr_in addr;
  addr.sin_port = htons(8989);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(sfd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
  {
    perror("BIND ERROR");
    exit(-1);
  }
  //3.设置监听
  if (listen(sfd, 128) == -1)
  {
    perror("LISTEN ERROR");
    exit(-1);
  }
  //4.创建epoll模型
  int ep = epoll_create(128); //参数没有实际意义大于零就行
  if (ep < 0)
  {
    perror("Epoll Create Error");
    exit(-1);
  }
  //5.初始化模型
  struct epoll_event event;
  event.data.fd = sfd;
  event.events = EPOLLIN;
  if (epoll_ctl(ep,EPOLL_CTL_ADD,sfd,&event) == -1)
  {
    perror("Epoll Control Error");
    exit(-1);
  }

  struct epoll_event events[1024];
  //6.开始检测
  while (1)
  {
    int sum =epoll_wait(ep,events,1024,-1);

    for (int i = 0 ; i < sum;i++)
    {
      if (events[i].events &EPOLLOUT)
      {
        //如果是写事件忽略
        continue;
      }
      int curfd = events[i].data.fd;
      if (curfd == sfd)
      {
        struct epoll_event new_ev;
        struct sockaddr_in in_addr;
        socklen_t addrlen = sizeof(in_addr);
        int cfd = accept(sfd,(struct sockaddr*)&in_addr,&addrlen);
        if (cfd == -1)
        {
          perror("accept Error");
          continue;
        }
        new_ev.events = EPOLLIN;
        new_ev.data.fd = cfd;
        if (epoll_ctl(ep,EPOLL_CTL_ADD,cfd,&new_ev) == -1)
        {
          perror("Epoll Control Error");
          continue;
        }
        //打印新连接的信息
        printf("新链接已建立，ip:%s,port:%d\n",inet_ntoa(in_addr.sin_addr),ntohs(in_addr.sin_port));
      }else//其他客户端通信
      {
        //通信
        char buf[1024];
        int length = recv(curfd,buf,sizeof(buf)-1,0);
        if (length == 0)
        {
          printf("连接已断开\n");
          if (epoll_ctl(ep,EPOLL_CTL_DEL,curfd,&events[i]) == -1)
          {
            perror("EPOLL CONTROL ERROR");
            continue;
          }
          close(curfd);
        }
        else if (length >0)
        {
          buf[length] = '\0';
          printf("%d:收到消息：%s \n",curfd,buf);
          send(curfd, "Server received your message\n", 30, 0);
        }else
        {
          perror("Recv Error");
        }
      }
    }
  }
}
