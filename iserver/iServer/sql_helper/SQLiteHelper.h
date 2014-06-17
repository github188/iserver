#pragma once
#include "sqlbase.h"
#include <sqlite3.h>

class CSQLiteHelper :
    public CSQLBase
{
public:
    CSQLiteHelper(void);
    ~CSQLiteHelper(void);

public:
    // 连接
    bool Connect(const db_conn_info_t& db_info);
    // 断开连接
    bool Disconnect();
    // 执行（无结果）
    bool Excute(const char* sql);
    // 执行（有结果）
    bool Excute(const char* sql, CSQLRecordset& record_set);

    //事务相关接口
    bool BeginTransaction();
    bool CommitTransaction();
    bool RollBackTransaction();

    void IsAnException(char const * const msg);

private:
    sqlite3 *m_sqlite;
    std::string m_host;
};
