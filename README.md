# epoll_server
基于epoll的简单服务器，具有接受请求，按一定协议解包，处理并反包的功能。
目前尚存在的问题有：
  1.没有使用读取配置文件的方式加载一些服务器静态配置
  2.没有日志系统
  3.epoll接收请求时，没有解决粘包问题。
  当然还有一系列问题尚未解决，以上三个是优先急需解决的问题。