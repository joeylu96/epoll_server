#ifndef _WORK_PROCESS_
#define _WORK_PROCESS_

#include "header.h"
#include "session.h"
#include "guard_mutex.h"

class CWorkProcess
{
    public:
        CWorkProcess(){};
        ~CWorkProcess(){};

        int Init(CTaskQueue *task_queue);
        static void* Start(void* pParam);
        void Routine();
        
        int ProcessRequest(CSession* session);
        int LockWrite(int socket, const char* str, int len);

        static CTaskQueue* m_task_queue;
};

#endif