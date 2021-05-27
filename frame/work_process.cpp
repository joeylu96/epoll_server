#include "work_process.h"
#include "socket_tool.h"
#include "session_mgr.h"
#include "global_work.h"

CTaskQueue* CWorkProcess::m_task_queue=nullptr;

int CWorkProcess::Init(CTaskQueue *task_queue)
{
    m_task_queue=task_queue;
}

void* CWorkProcess::Start(void* pParam)
{
    if(pParam!=nullptr)
    {
        CWorkProcess* work_process=(CWorkProcess *)pParam;
        work_process->Routine();
    }
}

void CWorkProcess::Routine()
{
    CSession *session = nullptr;
	while(1)
	{
		if(0 == m_task_queue->WaitTillPop(session))
		{
            cout<<"CWorkProcess::Routine|pop session="<<session<<" task_queue_size="<<m_task_queue->Size()<<endl;
			ProcessRequest(session);
            CSessionMgr::GetInstance()->ReleaseSession(session);
		}
	}
}

int CWorkProcess::ProcessRequest(CSession* session)
{
    int nwrite;
    string rsp=session->m_req;
    
    char res[1024];
    int len=CPackTool::Pack(rsp, res);

    nwrite = LockWrite(session->m_socket,res,len);
    cout<<rsp<<endl;
    if (nwrite == -1)
    {
        cout<<"CWorkProcess::ProcessRequest| send rsp error"<<endl;
        close(session->m_socket);
        return -1;
    }
    return 0;
}

int CWorkProcess::LockWrite(int socket, const char* str, int len)
{
    GuardMutex auto_lock(CGlobalWork::m_lock);
    int nwrite = write(socket,str,len);
    return nwrite;
}
