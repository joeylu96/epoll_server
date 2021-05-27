#ifndef _GLOBAL_WORK_
#define _GLOBAL_WORK_

#include "header.h"
#include "session.h"
#include "request_io.h"
#include "work_process.h"
#include "guard_mutex.h"

class CGlobalWork
{
    private:
        static CTaskQueue* m_task_queue;
        static CRequestIO* m_requestIO;
	    static CWorkProcess* m_work_process_list;
        static int m_work_num;
    public:
        static pthread_mutex_t m_lock;
        static int Init();
        static int Start();
};

#endif