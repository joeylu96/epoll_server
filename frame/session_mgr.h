#ifndef _SESSION_MGR_
#define _SESSION_MGR_

#include "session.h"

class CSessionMgr
{
    private:
        static CSessionMgr* m_instance;
        CTaskQueue* m_mgr_queue;
        CSessionMgr();

    public:
        ~CSessionMgr(){};

        static CSessionMgr* GetInstance();

        int Init(int num);
        int UInit();
        int ReleaseSession(CSession* session);
        int WaitTillSession(CSession** session);
        int GetEmptySessionSize();

        int m_session_num;
        bool m_ready;
        CSession* m_session_list;
};

#endif