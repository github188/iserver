#include "StdAfx.h"
#include "SQLiteHelper.h"

namespace {

    int const BUSY_SLEEP_PER_TIME = 100; // 每次100ms
    int const BUSY_SLEEP_TOTAL_TIME = 2000; // 一共2000ms
    int const BUSY_SLEEP_TOTAL_TIMES = BUSY_SLEEP_TOTAL_TIME / BUSY_SLEEP_PER_TIME;
    int my_busy_handler(void* param, int nTimes);
} //~ end of anonymous namespace

CSQLiteHelper::CSQLiteHelper(void)
: m_sqlite(NULL)
{
}

CSQLiteHelper::~CSQLiteHelper(void)
{
    try
    {
        Disconnect();
    }
    catch (...)
    {
    }
}

// 连接
bool CSQLiteHelper::Connect(const db_conn_info_t& db_info)
{
    if (NULL != m_sqlite && db_info.host == m_host)
    {
        Disconnect();
    }

    m_host = db_info.host;
    if (!boost::filesystem::exists(m_host.c_str()))
    {
        return false;
    }

    //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
    int rc = sqlite3_open(m_host.c_str(), &m_sqlite); 
    if(SQLITE_OK != rc)
    {
        sqlite3_close(m_sqlite);
        m_sqlite = NULL;
        return false;
    }

    // 设置busy
    sqlite3_busy_handler(m_sqlite, my_busy_handler, this);

    return true;
}
// 断开连接
bool CSQLiteHelper::Disconnect()
{
    if (NULL == m_sqlite)
    {
        return true;
    }
    //关闭数据库
    if(sqlite3_close(m_sqlite) != SQLITE_OK)
    {
        return false;
    }

    m_sqlite = NULL;
    return true;
}

// 执行（无结果）
bool CSQLiteHelper::Excute(const char* sql)
{
    assert(sql);
    assert(m_sqlite);
    
    char* msg = NULL;

    int rc = sqlite3_exec(m_sqlite, sql, 0, 0, &msg);
    if(SQLITE_OK == rc)
    {
        return true;
    }

    if(msg != NULL)
    {
        IsAnException(msg);

        //TPLOG_ERROR("exec %s failed-%s", (const char*)fcT2A(sql),
        //    (const char*)fcU2A(errMsg));
        sqlite3_free(msg);
    }

    return false;
}
// 执行（有结果）
bool CSQLiteHelper::Excute(const char* sql, CSQLRecordset& record_set)
{
    assert(sql);
    assert(m_sqlite);
   
    char* msg = NULL;
    int row = -1;
    int colum = -1;
    char **result = NULL; //二维数组存放结果

    if(SQLITE_OK != sqlite3_get_table(m_sqlite, sql, &result, &row, &colum, &msg))
    {
        if(msg != NULL)
        {
            IsAnException(msg);

            //TPLOG_ERROR("exec %s failed-%s", (const char*)fcT2A(sql),
            //    (const char*)fcU2A(errMsg));
            sqlite3_free(msg);
        }
        else
        {
            //TPLOG_ERROR("exec %s failed", (const char*)fcT2A(sql));
        }
        return false;
    }

    record_set.set_data(result, row, colum);

    return true;
}

//事务相关接口
bool CSQLiteHelper::BeginTransaction()
{
    return Excute("begin transaction");
}
bool CSQLiteHelper::CommitTransaction()
{
    return Excute("commit transaction");
}
bool CSQLiteHelper::RollBackTransaction()
{
    return Excute("rollback transaction");
}

void CSQLiteHelper::IsAnException(char const * const msg)
{
    assert(NULL != msg);

    if (NULL != strstr(msg, "no such table:"))
    {
        throw msg;
    }   

    if (NULL != strstr(msg, "no such column:"))
    {
        throw msg;
    }    

    if (NULL != strstr(msg, "syntax error"))
    {
        throw msg;
    }
}

namespace {
    int my_busy_handler(void* param, int nTimes)
    {
        assert(NULL != param);
        if (nTimes > BUSY_SLEEP_TOTAL_TIMES)
        {
            return 0; // busy等待结束了，返回busy_timeout
        }

        CSQLiteHelper *pHelper = reinterpret_cast<CSQLiteHelper *>(param);
        if (NULL != pHelper && nTimes % 10 == 0)
        {
            //TPLOG_INFO("%s busy handler %d",
             //   (LPCSTR)fcT2A(pHelper->GetFileName()),
            //    nTimes);
        }
        sqlite3_sleep(BUSY_SLEEP_PER_TIME);
        return 1;
    }
}
