#include <sys/types.h>
#include <sys/epoll.h>
#include "request_io.h"
#include "socket_tool.h"
#include "session_mgr.h"

int CRequestIO::m_listen_sock=NULL;
CTaskQueue* CRequestIO::m_task_queue=nullptr;

int CRequestIO::Init(string ip, int port, CTaskQueue *task_queue)
{
    m_ip=ip;
    m_port=port;
    m_task_queue=task_queue;
    m_listen_sock=CSocketTool::getTcpSocket(ip.c_str(), port);
}

void* CRequestIO::Start(void* pParam)
{
    if(pParam!=nullptr)
    {
        CRequestIO* request_io=(CRequestIO *)pParam;
        request_io->Routine();
    }
}

void CRequestIO::Routine()
{
    if(listen(m_listen_sock, 20)!=0)
    {
        cout<<"CRequestIO::Routine|listen error"<<endl;
        return;
    }
    
    int epollfd;
    struct epoll_event events[100];
    char buf[1024];
    memset(buf,0,1024);
    epollfd = epoll_create(1000);
    AddEvent(epollfd, m_listen_sock, EPOLLIN);

    while(1)
    {
        int num=epoll_wait(epollfd, events, 100, -1);
        HandleEvent(epollfd,events,num,buf);
    }

    close(m_listen_sock);
}

void CRequestIO::AddEvent(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}

void CRequestIO::HandleEvent(int epollfd,struct epoll_event *events,int num,char *buf)
{
    int fd;
    for(int idx=0;idx<num;idx++)
    {
        fd = events[idx].data.fd;
        if ((fd == m_listen_sock) &&(events[idx].events & EPOLLIN))
        {
            HandleAccept(epollfd);
        }
        else if (events[idx].events & EPOLLIN)
        {
            DoRead(epollfd, fd, buf);
        }
    }
}

void CRequestIO::HandleAccept(int epollfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t  cliaddrlen;
    clifd = accept(m_listen_sock,(struct sockaddr*)&cliaddr,&cliaddrlen);
    if (clifd == -1)
    {
        cout<<"CRequestIO::HandleAccept|clifd error"<<endl;
        return;
    }
    else
    {
        cout<<"CRequestIO::HandleAccept|accept a new client addr"<<inet_ntoa(cliaddr.sin_addr)<<endl;
        AddEvent(epollfd,clifd,EPOLLIN);
    }
}

void CRequestIO::DoRead(int epollfd, int fd, char* buf)
{
    int nread;
    nread = read(fd,buf,1024);
    if(nread<8)
    {
        //没有按协议来包 不处理
        cout<<"CRequestIO::DoRead|pack len < 8|close socket"<<endl;
        close(fd);
        DeleteEvent(epollfd,fd,EPOLLIN);
        return;
    }
    else 
    {
        cout<<"CRequestIO::DoRead|rec msg="<<buf<<endl;
        int ret=CheckRecData(buf);
        cout<<"CRequestIO::DoRead|nread="<<nread<<" req_len="<<ret<<endl;
        if(ret<0)
        {
            cout<<"CRequestIO::DoRead|buf head&len error"<<endl;
            close(fd);
            DeleteEvent(epollfd,fd,EPOLLIN);
            return;
        }

        //正常情况，加入工作进入工作队列
        CSession *session=nullptr;
        if (CSessionMgr::GetInstance()->WaitTillSession(&session) != 0)
        {
            cout<<"CRequestIO::DoRead|get session fail"<<endl;
            return;
        }

        //校验请求长度
        if(nread!=(ret+8))
        {
            cout<<"CRequestIO::DoRead|len check error"<<endl;
            close(fd);
            DeleteEvent(epollfd,fd,EPOLLIN);
            return;
        }

        string req="";
        for(int idx=0;idx<ret;idx++)
        {
            req+=buf[8+idx];
        }
        session->m_socket=fd;
        session->m_req=req;
        m_task_queue->WaitTillPush(session);
    }
}

int CRequestIO::CheckRecData(char* buf)
{
    string head=buf;
    head=head.substr(0,4);
    if(head!="head")
    {
        return -1;
    }

    int len=0;
    memcpy(&len, &buf[4],4);
    return len;
}

void CRequestIO::DeleteEvent(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}
