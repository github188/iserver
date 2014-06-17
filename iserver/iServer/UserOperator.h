#pragma once
#include "sql_helper/SQLBase.h"

const int max_sql_len = 4*1024;

class CUserOperator
{
public:
    CUserOperator(void);
    ~CUserOperator(void);

private:
    CSQLBase* m_pSqlHelper;
    db_type_t m_dbType;
public:
    // ��ȡ���е�������Ϣ
    bool GetRegions(std::list<Region>& lsAllRegion);
    // ��������ID��ȡ�����������û���Ϣ
    bool GetSubUserByRegionId(const int nRegionId, std::list<User>& lsUser);
    // ��������ID��ȡ������������Ϣ
    bool GetSubRegionByRegionId(const int nRegionId, std::list<Region>& lsRegion);
    // �����û�ID��ȡ�û���Ϣ
    bool GetUserInfoByUserId(const int nUserId, User& userInfo);
    // ��������ID��ȡ������Ϣ
    bool GetRegionInfoByRegionId(const int nRegionId, Region& regionInfo);
    // ��ȡ���е��û���Ϣ
    bool GetUsers(std::list<User>& lsAllUser);
    // �����ݿ�
    bool OpenDb(void);
    bool CloseDb(void);
    // ���ݿ�����û���Ϣ(�����û�����)
    bool UpdateUserInfo(const User& userInfo);
    // �����û�ID��ȡ�û�����
    bool GetUserConfigByUserId(const int nUserId, UserConfig* userConfig);
    // ���ݿ����������Ϣ
    bool UpdateRegionInfo(const Region& regionInfo);
    // ����һ������
    bool AddRegionInfo(const Region& regionInfo);
    // ɾ��һ������
    bool DeleteRegionInfo(const Region& regionInfo);
    // ����һ���û�
    bool AddUserInfo(const User& userInfo);
    // ɾ��һ���û�
    bool DeleteUserInfo(const User& userInfo);
    // �����û�����
    bool UpdateUserConfig(const int nUserId, const UserConfig& userConfig);
};
