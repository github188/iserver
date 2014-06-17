/* @file MySQLHelper.h
*  @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved
*  @brief mysql 数据库操作
* 
*  @author zyj
*  @date   2014/3/28
*
*  @note 下面的note和warning为可选项目
*  @note 这里填写本文件的详细功能描述和注解
*  @note 历史记录：
*  @note v0.1 xxxxxxx
*
*  @warning 这里填写本文件相关的警告信息
*/


#pragma once
#include "sqlbase.h"
#include <mysql.h>
#include <list>

class CMySQLHelper :
    public CSQLBase
{
public:
    CMySQLHelper(void);
    ~CMySQLHelper(void);

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

    // 判断传入的字符串是否属于异常
    void IsAnException(const char* const msg) const;

    // 定时检查mysql的复制连接，保持连接不会超出设定的。
    void CheckConnect();

private:
    // 复制一个新的连接
    MYSQL* CloneConnect();
    // 把mysql连接设置为空闲状态
    void   FreeCloned(const MYSQL* mysql);
    // 清空所有复制的mysql连接
    void   CloseCloned();

private:
    // mysql 连接信息
    db_conn_info_t m_db_info;
    // 是否连接成功
    bool m_connect;
    // mysql连接
    MYSQL *m_mysql;

    // 复制的连接信息
    typedef struct _mysql_res_t
    {
        MYSQL* mysql;
        bool   _using;

        _mysql_res_t()
        {
            mysql = NULL;
            _using = false;
        }
    }mysql_res_t;
    std::list<mysql_res_t> m_mysql_res;
    CRITICAL_SECTION m_res_lock;

    // 检查mysql连接数的线程句柄
    HANDLE m_thread_checkconnect;
    // 退出线程信号
    HANDLE m_event_exit;

    // 保留活跃的mysql连接
    static const int max_mysql_connect = 5;
};
