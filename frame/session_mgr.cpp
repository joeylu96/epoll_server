#include "session_mgr.h"

CSessionMgr * CSessionMgr::m_instance = nullptr;

CSessionMgr::CSessionMgr()
{
    m_ready=false;
    m_session_num=0;
};

CSessionMgr* CSessionMgr::GetInstance()
{
    if (nullptr == m_instance)
    {
        m_instance = new CSessionMgr;
    }
    return m_instance;
}
        
int CSessionMgr::Init(int num)
{
    m_session_num=num;
    
    m_mgr_queue = new CTaskQueue;
    if (m_mgr_queue==nullptr || (0 != m_mgr_queue->Init(m_session_num + 1)))
    {
        cout<<"CSessionMgr::Init|m_mgr_queue init error"<<endl;
        return -1;
    }

    m_session_list=new CSession[m_session_num];
    for (int idx = 0; idx < m_session_num; idx ++)
    {
        m_session_list[idx].Init();
        m_mgr_queue->WaitTillPush(&m_session_list[idx]);
    }

    m_ready=true;
    return 0;
}

int CSessionMgr::UInit()
{
    m_ready = false;
    m_session_num = 0;
    
    if(NULL!=m_session_list)
        delete []m_session_list;
    
    if(NULL != m_mgr_queue)
        delete m_mgr_queue;

    return 0;
}

int CSessionMgr::ReleaseSession(CSession* session)
{
    if(nullptr == m_mgr_queue || nullptr == session)
    {
        return -1;
    }
    
    session->Reset();
    if (0 != m_mgr_queue->WaitTillPush(session))
    {
        return -2;
    }
    return 0;
}

int CSessionMgr::WaitTillSession(CSession** session)
{
    if (false == m_ready)
    {
        *session = nullptr;
        return -1;
    }
    if (0 != m_mgr_queue->WaitTillPop(*session))
    {
        return -2;
    }
    return 0;
}

int CSessionMgr::GetEmptySessionSize()
{
    return m_mgr_queue->Size();
}