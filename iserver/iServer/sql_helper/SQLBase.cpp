#include "StdAfx.h"
#include "SqlBase.h"
#include <mysql.h>
#include <sqlite3.h>
#include "SQLiteHelper.h"
#include "MySQLHelper.h"

bool db_lib_init()
{
    if (0 != mysql_library_init(0, NULL, NULL))
    {
        printfd("mysql library init failed\n");
    }
    return true;
}

bool db_lib_fini()
{
    mysql_library_end();
    return true;
}

CSQLBase* db_create_instanse(db_type_t db_type)
{
    if (db_type == mysql_type)
    {
        return new CMySQLHelper;
    }
    else if (db_type == sqlite_type)
    {
        return new CSQLiteHelper;
    }

    printfd("create db instance error: unknown db type\n");
    return NULL;
}

void db_destory_instance(CSQLBase* db)
{
    assert(db);
    if (NULL != db)
    {
        delete db;
    }
}

CSQLBase::CSQLBase(void)
{
}

CSQLBase::~CSQLBase(void)
{
}

