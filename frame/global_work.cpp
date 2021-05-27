#include "global_work.h"
#include "session_mgr.h"

CTaskQueue* CGlobalWork::m_task_queue=nullptr;
CRequestIO* CGlobalWork::m_requestIO=nullptr;
CWorkProcess* CGlobalWork::m_work_process_list=nullptr;
pthread_mutex_t CGlobalWork::m_lock;

int CGlobalWork::m_work_num=0;

int CGlobalWork::Init()
{
    //初始化工作队列和session_mgr
    m_task_queue=new CTaskQueue;
	if (nullptr!=m_task_queue && 0 != m_task_queue->Init(100))
	{
        cout<<"CGlobalWokr::Init|task_queue init error"<<endl;
		return -1;
	}
    CSessionMgr::GetInstance()->Init(100);

    //初始化requestio
    m_requestIO=new CRequestIO;

    string ip="0.0.0.0";
    int port=5000;
    m_requestIO->Init(ip, port, m_task_queue);

    //初始化workprocess
    m_work_num=4;
    m_work_process_list=new CWorkProcess[m_work_num];
    for(int idx=0;idx<m_work_num;idx++)
    {
        m_work_process_list[idx].Init(m_task_queue);
    }
    return 0;
}

int CGlobalWork::Start()
{
    //pthread_create
    //一个接受socket线程
    //一个工作线程
    pthread_t tThrId = 0;
    for(int idx=0;idx<m_work_num;idx++)
    {
        if (0 != pthread_create(&tThrId, NULL, CWorkProcess::Start, &(m_work_process_list[idx])))
        {
            cout<<"CGlobalWork::Start|create requestIO error idx="<<idx<<endl;
            return -2;
        }
    }

    if (0 != pthread_create(&tThrId, NULL, CRequestIO::Start, m_requestIO))
	{
        cout<<"CGlobalWork::Start|create requestIO error"<<endl;
		return -1;
	}
    cout<<"CGlobalWork::Start|create requestIO suc"<<endl;
    
    return 0;
}