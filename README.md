```markdown
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
```

## 📂 代码结构
```
├── server.c            - 服务器主程序
├── client.c           - 客户端程序
├── README.md          - 项目文档
└── .gitignore         - Git忽略配置
```

## 🛠️ 功能说明

### 服务器端
- 创建epoll实例管理连接
- 处理新客户端连接
- 接收/响应客户端消息
- 自动清理断开连接

### 客户端
- 建立TCP连接
- 交互式消息发送
- 显示服务器响应
- 断连检测

## 🧪 测试示例
```text
# 服务器输出
新链接已建立，ip:192.168.1.100, port:54321
Client 5: 收到消息：Hello World
连接已断开

# 客户端输出
请输入消息 > Test Message
recv buf: Server received your message
```

## ⚙️ 性能优化建议

1. 使用边缘触发(ET)模式：
   ```c
   event.events = EPOLLIN | EPOLLET;
   ```
2. 设置非阻塞套接字：
   ```c
   fcntl(fd, F_SETFL, O_NONBLOCK);
   ```
3. 增加线程池处理业务逻辑
4. 实现环形缓冲区减少内存拷贝

## 🤝 贡献指南

1. Fork项目仓库
2. 创建特性分支 (`git checkout -b feature/improvement`)
3. 提交修改 (`git commit -m 'Add some feature'`)
4. 推送到分支 (`git push origin feature/improvement`)
5. 创建Pull Request

## 📜 许可证

[MIT License](LICENSE)

## ☎️ 联系方式

如有任何问题或建议，请联系：
- Email: your.email@example.com
- GitHub Issues: [新建Issue](https://github.com/yourname/repo/issues)
```

---

**提示**：使用时请根据实际情况修改以下内容：
1. 替换`your.email@example.com`为真实联系方式
2. 更新GitHub Issues链接地址
3. 如需添加截图，可在`## 🧪 测试示例`部分插入图片链接
4. 如需详细文档，可添加`docs`目录存放说明文件

建议在项目中添加示例配置文件和使用截图，使文档更加完整直观。
