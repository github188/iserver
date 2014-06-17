#pragma once

class CGuard
{
public:
    CGuard(CRITICAL_SECTION *lpcsLock);
    ~CGuard(void);
private:
    CRITICAL_SECTION *m_lpcsLock;
protected:
    CGuard(void);
};

inline CGuard::CGuard(void)
{
    m_lpcsLock = NULL;
}

inline CGuard::~CGuard(void)
{    
    LeaveCriticalSection(m_lpcsLock);
}

inline CGuard::CGuard(CRITICAL_SECTION *lpcsLock)
: m_lpcsLock(lpcsLock)
{        
    EnterCriticalSection(m_lpcsLock);
}

//////////////////////////////////////////////////////////////////////////

