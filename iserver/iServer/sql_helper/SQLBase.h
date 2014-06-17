
/* @file SqlBase.h
*  @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved
*  @brief 这里填写本文件的摘要
* 
*  @author zyj
*  @date   2014/3/18
*
*  @note 下面的note和warning为可选项目
*  @note 这里填写本文件的详细功能描述和注解
*  @note 历史记录：
*  @note v0.1 xxxxxxx
*
*  @warning 这里填写本文件相关的警告信息
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

    // 连接
    virtual bool Connect(const db_conn_info_t& db_info) = 0;
    // 断开连接
    virtual bool Disconnect() = 0;
    // 执行（无结果）
    virtual bool Excute(const char* sql) = 0;
    // 执行（有结果）
    virtual bool Excute(const char* sql, CSQLRecordset& record_set) = 0;
    
    //事务相关接口
    virtual bool BeginTransaction() = 0;
    virtual bool CommitTransaction() = 0;
    virtual bool RollBackTransaction() = 0;
};

