
/* @file SqlBase.h
*  @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved
*  @brief ������д���ļ���ժҪ
* 
*  @author zyj
*  @date   2014/3/18
*
*  @note �����note��warningΪ��ѡ��Ŀ
*  @note ������д���ļ�����ϸ����������ע��
*  @note ��ʷ��¼��
*  @note v0.1 xxxxxxx
*
*  @warning ������д���ļ���صľ�����Ϣ
*/


#pragma once
#include "sql_udd.h"
#include "SqlRecordset.h"

#include <sqlite3.h>
#include <mysql.h>

class CSQLBase;

bool db_lib_init();
bool db_lib_fini();
CSQLBase* db_create_instanse(db_type_t db_type);
void db_destory_instance(CSQLBase* db);

class CSQLBase
{
public:
    CSQLBase(void);
    virtual ~CSQLBase(void);

    // ����
    virtual bool Connect(const db_conn_info_t& db_info) = 0;
    // �Ͽ�����
    virtual bool Disconnect() = 0;
    // ִ�У��޽����
    virtual bool Excute(const char* sql) = 0;
    // ִ�У��н����
    virtual bool Excute(const char* sql, CSQLRecordset& record_set) = 0;
    
    //������ؽӿ�
    virtual bool BeginTransaction() = 0;
    virtual bool CommitTransaction() = 0;
    virtual bool RollBackTransaction() = 0;
};

