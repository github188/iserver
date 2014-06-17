#include "StdAfx.h"
#include "SQLiteHelper.h"

namespace {

    int const BUSY_SLEEP_PER_TIME = 100; // ÿ��100ms
    int const BUSY_SLEEP_TOTAL_TIME = 2000; // һ��2000ms
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

// ����
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

    //��ָ�������ݿ��ļ�,��������ڽ�����һ��ͬ�������ݿ��ļ�
    int rc = sqlite3_open(m_host.c_str(), &m_sqlite); 
    if(SQLITE_OK != rc)
    {
        sqlite3_close(m_sqlite);
        m_sqlite = NULL;
        return false;
    }

    // ����busy
    sqlite3_busy_handler(m_sqlite, my_busy_handler, this);

    return true;
}
// �Ͽ�����
bool CSQLiteHelper::Disconnect()
{
    if (NULL == m_sqlite)
    {
        return true;
    }
    //�ر����ݿ�
    if(sqlite3_close(m_sqlite) != SQLITE_OK)
    {
        return false;
    }

    m_sqlite = NULL;
    return true;
}

// ִ�У��޽����
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
// ִ�У��н����
bool CSQLiteHelper::Excute(const char* sql, CSQLRecordset& record_set)
{
    assert(sql);
    assert(m_sqlite);
   
    char* msg = NULL;
    int row = -1;
    int colum = -1;
    char **result = NULL; //��ά�����Ž��

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

//������ؽӿ�
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
            return 0; // busy�ȴ������ˣ�����busy_timeout
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
