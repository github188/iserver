
#pragma once
#include <boost/thread.hpp>

class CBoostLock
{
public:
    CBoostLock(void) {}  
    ~CBoostLock(void) {} 

    virtual void lock(){m_mutex.lock();}
    virtual void unlock(){m_mutex.unlock();}

protected:
    boost::mutex m_mutex;
};
 
 class CBoostGuard : public CBoostLock
 {
 public:
     CBoostGuard(CBoostLock *pBoostLock)
         : m_pMutex(pBoostLock)
     {
        m_pMutex->lock();
     }
     virtual ~CBoostGuard()
     {
         m_pMutex->unlock();
     }
 
     virtual void lock() {return m_pMutex->lock();}  
     virtual void unlock() {return m_pMutex->unlock();} 
 
 private:
     CBoostLock *m_pMutex;
 };

 class CBoostEvent
 {
 public:
     CBoostEvent(){};
     virtual ~CBoostEvent(){};

     void wait()  
     {  
         CBoostGuard scoped_lock(&m_lock);  
         m_condition.wait(scoped_lock);    
     }  
 
     bool timed_wait(int nMillSec)  
     {  
         CBoostGuard scoped_lock(&m_lock);  
         return m_condition.timed_wait(scoped_lock, boost::posix_time::millisec(nMillSec));  
     }
 
     void notify_one() { m_condition.notify_one(); }  

     void notify_all() { m_condition.notify_all(); } 

 private:  
     CBoostLock m_lock;  
     boost::condition_variable_any m_condition;
 };