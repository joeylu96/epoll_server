#ifndef _REQUEST_IO_
#define _REQUEST_IO_

#include "header.h"
#include "session.h"

class CRequestIO
{
    public:
        CRequestIO(){};
        ~CRequestIO(){};

        int Init(string ip, int port, CTaskQueue *task_queue);
        static void* Start(void* pParam);

        void Routine();
        void AddEvent(int epollfd,int fd,int state);
        void HandleEvent(int epollfd,struct epoll_event *events,int num,char *buf);
        void HandleAccept(int epollfd);
        void DoRead(int epollfd, int fd, char* buffer);
        void DeleteEvent(int epollfd, int fd, int state);
        int CheckRecData(char* buffer);

        string m_ip;
        int m_port;
        static CTaskQueue* m_task_queue;
        static int m_listen_sock;
};

#endif