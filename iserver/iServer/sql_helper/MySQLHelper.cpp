#include "StdAfx.h"
#include "MySQLHelper.h"
#include "Guard.h"
#include <process.h>

#pragma warning(push)
#pragma warning(disable:4127)
unsigned _stdcall CheckConnectThread(void* para)
{
    CMySQLHelper* p = (CMySQLHelper* )para;
    if (NULL == p)
    {
        printfd("thread param null\n");
        return 0;
    }

    p->CheckConnect();

    return 0;
}

CMySQLHelper::CMySQLHelper(void)
: m_connect(false)
, m_mysql(NULL)
, m_thread_checkconnect(NULL)
, m_event_exit(NULL)
{
    InitializeCriticalSection(&m_res_lock);

    m_event_exit = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_thread_checkconnect = (HANDLE)_beginthreadex(NULL, 0, CheckConnectThread, this, 0, NULL);
    if (NULL == m_thread_checkconnect)
    {
        printfd("create check thread failed.\n");
    }
}

//lint -sem(CMySQLHelper::Disconnect,cleanup)
CMySQLHelper::~CMySQLHelper(void)
{
    try
    {
        SetEvent(m_event_exit);
        if (NULL != m_thread_checkconnect)
        {
            WaitForSingleObject(m_thread_checkconnect, INFINITE);
            CloseHandle(m_thread_checkconnect);
            m_thread_checkconnect = NULL;
        }
        CloseHandle(m_event_exit);
        m_event_exit = NULL;

        Disconnect();
        CloseCloned();
        DeleteCriticalSection(&m_res_lock);
    }
    catch (...)
    {
        printfd("catch any exceptin...\n");
    }
}

bool CMySQLHelper::Connect(const db_conn_info_t& db_info)
{
    m_db_info = db_info;
    m_mysql = mysql_init(NULL);
    if (NULL == m_mysql)
    {
        printfd("mysql init fail.\n");
        return false;
    }    
    mysql_options(m_mysql, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(m_mysql, MYSQL_OPT_RECONNECT, "1");

    if (NULL == mysql_real_connect(
        m_mysql,
        m_db_info.host.c_str(), 
        m_db_info.username.c_str(),
        m_db_info.password.c_str(), 
        m_db_info.dbschema.c_str(),
        atoi(m_db_info.port.c_str()), 
        NULL, 
        CLIENT_MULTI_QUERIES))
    {
        const char *msg = mysql_error(m_mysql);
        IsAnException(msg);

        return false;
    }

    m_connect = true;

    return true;
}
// 断开连接
bool CMySQLHelper::Disconnect()
{
    if (!m_connect)
    {
        return true;
    }
    if (NULL != m_mysql)
    {
        mysql_close(m_mysql);
        m_mysql = NULL;
    }
    m_connect = false;

    return true;
}

// 执行（无结果）
bool CMySQLHelper::Excute(const char* sql)
{
    assert(sql != NULL);
    if (NULL == sql)
    {
        return false;
    }
    if (NULL == m_mysql)
    {
        printfd("mysql not connected..\n");
        return false;
    }
    MYSQL* mysql = CloneConnect();
    if (NULL == mysql)
    {
        printfd("cloned mysql connect failed.\n");
        return false;
    }

    if (0 != mysql_real_query(mysql, sql, strlen(sql)))
    {
        const char *msg = mysql_error(mysql);
        IsAnException(msg);

        FreeCloned(mysql);
        return false;
    }

    FreeCloned(mysql);
    return true;
}
// 执行（有结果）
bool CMySQLHelper::Excute(const char* sql, CSQLRecordset& record_set)
{
    assert(sql);
    if (NULL == sql)
    {
        return false;
    }
    if (NULL == m_mysql)
    {
        return false;
    }
    MYSQL* mysql = CloneConnect();
    if (NULL == mysql)
    {
        return false;
    }

    if (0 != mysql_real_query(mysql, sql, strlen(sql)))
    {
        FreeCloned(mysql);

        const char *msg = mysql_error(mysql);
        IsAnException(msg);

        return false;
    }

    MYSQL_RES* mysql_res = mysql_store_result(mysql);
    if (NULL == mysql_res)
    {
        printfd("get mysql result failed.\n");
        FreeCloned(mysql);
        return false;
    }

    int row = (int)mysql_num_rows(mysql_res);
    int column = mysql_num_fields(mysql_res);
    record_set.set_data((char**)mysql_res, row, column);

    FreeCloned(mysql);
    return true;
}

//事务相关接口
bool CMySQLHelper::BeginTransaction()
{
    return Excute("begin transaction");
}
bool CMySQLHelper::CommitTransaction()
{
    return Excute("commit transaction");
}
bool CMySQLHelper::RollBackTransaction()
{
    return Excute("rollback transaction");
}

void CMySQLHelper::IsAnException(const char* const msg) const
{
    assert(NULL != msg);
    if (NULL == msg)
    {
        return;
    }

    if (NULL != strstr(msg, "Table")
        && NULL != strstr(msg, "doesn't exist"))
    {
        throw std::logic_error(msg);
    }

    if (NULL != strstr(msg, "Unknown column"))
    {
        throw std::runtime_error(msg);
    }    

    if (NULL != ::strstr(msg, "syntax error"))
    {
        throw std::logic_error(msg);
    }
}

MYSQL* CMySQLHelper::CloneConnect()
{
    // 查找现有的连接
    do 
    {
        CGuard lock(&m_res_lock);
        for (std::list<mysql_res_t>::iterator itr = m_mysql_res.begin();
            itr != m_mysql_res.end(); ++itr)
        {
            if (false == itr->_using)
            {
                printfd("using existed mysql connection.\n");
                itr->_using = true;
                return itr->mysql;
            }
        }
    } while (0);

    // 创建新的连接
    MYSQL* mysql = mysql_init(NULL);
    if (NULL == mysql)
    {
        printfd("mysql_init failed.\n");
        return NULL;
    }    
    mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(mysql, MYSQL_OPT_RECONNECT, "1");

    if (NULL == mysql_real_connect(
        mysql,
        m_db_info.host.c_str(), 
        m_db_info.username.c_str(),
        m_db_info.password.c_str(), 
        m_db_info.dbschema.c_str(),
        atoi(m_db_info.port.c_str()), 
        NULL, 
        CLIENT_MULTI_QUERIES))
    {
        const char *msg = mysql_error(mysql);
        IsAnException(msg);

        return NULL;
    }
   
    // 加入list
    do 
    {
        CGuard lock(&m_res_lock);

        mysql_res_t my_res;
        my_res.mysql = mysql;
        my_res._using = true;

        m_mysql_res.push_back(my_res);
        printfd("mysql connection list size:%d\n", m_mysql_res.size());
    } while (0);

    return mysql;
}

void CMySQLHelper::FreeCloned(const MYSQL* mysql)
{
    CGuard lock(&m_res_lock);
    for (std::list<mysql_res_t>::iterator itr = m_mysql_res.begin();
        itr != m_mysql_res.end(); ++itr)
    {
        if (itr->mysql == mysql)
        {
            itr->_using = false;
            return;
        }
    }
}

void CMySQLHelper::CloseCloned()
{
    CGuard lock(&m_res_lock);
    for (std::list<mysql_res_t>::iterator itr = m_mysql_res.begin();
        itr != m_mysql_res.end(); ++itr)
    {
        if (itr->mysql != NULL)
        {
            mysql_close(itr->mysql);
        }
    }

    m_mysql_res.clear();
}

void CMySQLHelper::CheckConnect()
{
    while(WAIT_OBJECT_0 != WaitForSingleObject(m_event_exit, 2500))
    {
        do 
        {
            CGuard lock(&m_res_lock);
            if (m_mysql_res.size() <= max_mysql_connect-1)
            {
                continue;
            }

            int close_num = m_mysql_res.size() - max_mysql_connect-1;
            int closed = 0;
            for (std::list<mysql_res_t>::iterator itr = m_mysql_res.begin();
                itr != m_mysql_res.end();)
            {
                if (false == itr->_using)
                {
                    mysql_close(itr->mysql);
                    itr = m_mysql_res.erase(itr);
                    
                    if (++closed >= close_num)
                    {
                        continue;
                    }
                }
                else
                {
                     ++itr;
                }
            }

            printfd("mysql connection list size after check:%d\n", m_mysql_res.size());
        } while (0);
    }
}
#pragma warning(pop)