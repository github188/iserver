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
    // ����
    bool Connect(const db_conn_info_t& db_info);
    // �Ͽ�����
    bool Disconnect();
    // ִ�У��޽����
    bool Excute(const char* sql);
    // ִ�У��н����
    bool Excute(const char* sql, CSQLRecordset& record_set);

    //������ؽӿ�
    bool BeginTransaction();
    bool CommitTransaction();
    bool RollBackTransaction();

    void IsAnException(char const * const msg);

private:
    sqlite3 *m_sqlite;
    std::string m_host;
};
