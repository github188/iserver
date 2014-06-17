/* @file MySQLHelper.h
*  @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved
*  @brief mysql ���ݿ����
* 
*  @author zyj
*  @date   2014/3/28
*
*  @note �����note��warningΪ��ѡ��Ŀ
*  @note ������д���ļ�����ϸ����������ע��
*  @note ��ʷ��¼��
*  @note v0.1 xxxxxxx
*
*  @warning ������д���ļ���صľ�����Ϣ
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

    // �жϴ�����ַ����Ƿ������쳣
    void IsAnException(const char* const msg) const;

    // ��ʱ���mysql�ĸ������ӣ��������Ӳ��ᳬ���趨�ġ�
    void CheckConnect();

private:
    // ����һ���µ�����
    MYSQL* CloneConnect();
    // ��mysql��������Ϊ����״̬
    void   FreeCloned(const MYSQL* mysql);
    // ������и��Ƶ�mysql����
    void   CloseCloned();

private:
    // mysql ������Ϣ
    db_conn_info_t m_db_info;
    // �Ƿ����ӳɹ�
    bool m_connect;
    // mysql����
    MYSQL *m_mysql;

    // ���Ƶ�������Ϣ
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

    // ���mysql���������߳̾��
    HANDLE m_thread_checkconnect;
    // �˳��߳��ź�
    HANDLE m_event_exit;

    // ������Ծ��mysql����
    static const int max_mysql_connect = 5;
};
