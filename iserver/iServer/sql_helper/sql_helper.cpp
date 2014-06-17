// sql_helper.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SQLBase.h"

#include "SQLRecordset.h"


int _tmain(int /*argc*/, _TCHAR* /*argv[]*/)
{
    db_lib_init();
    CSQLBase *p = db_create_instanse(mysql_type);

    db_conn_info_t dbinfo;
    dbinfo.host = "10.64.60.88";
    dbinfo.port = "3306";
    dbinfo.username = "root";
    dbinfo.password = "888";
    dbinfo.dbschema = "ivms8100v3";
    p->Connect(dbinfo);

    CSQLRecordset dd(mysql_type);
    p->Excute("select * from server", dd);
    printf("row count = %d\n", dd.get_row_count());
    db_destory_instance(p);

    db_lib_fini();
    system("pause");
    return 0;
}

