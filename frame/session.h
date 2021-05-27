#ifndef _SESSION_
#define _SESSION_

#include "header.h"
#include "queue_t.h"

class CSession{
    public:
        //socket
        int m_socket;

        //user_info

        //session_info
        int m_seq;
        int m_cl_seq;
        string m_req;

    //init reset
    void Init()
    {
        Reset();
    }
    void Reset()
    {
        //data clear
        m_seq=0;
        m_cl_seq=0;
        m_req="";
    }
};

typedef CQueueT<CSession *> CTaskQueue;

#endif