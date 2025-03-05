# Epoll TCP Server/Client

![C](https://img.shields.io/badge/C-17%2B-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Platform](https://img.shields.io/badge/Platform-Linux-lightgrey)

基于Linux epoll实现的TCP服务器/客户端示例，演示高效I/O多路复用技术的网络编程实现。

## 📌 项目特点

- 使用epoll水平触发(LT)模式
- 支持多客户端并发连接
- 简洁的错误处理机制
- 包含完整客户端实现
- 支持端口复用(SO_REUSEPORT)

## 🚀 快速开始

### 环境要求
- Linux系统
- GCC编译器
- make工具（可选）

### 编译运行
```bash
# 编译服务器和客户端
gcc server.c -o server
gcc client.c -o client

# 启动服务器（需要sudo权限绑定端口）
./server

# 新终端运行客户端
./client
