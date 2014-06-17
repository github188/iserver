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
    // 获取所有的区域信息
    bool GetRegions(std::list<Region>& lsAllRegion);
    // 根据区域ID获取下属的所有用户信息
    bool GetSubUserByRegionId(const int nRegionId, std::list<User>& lsUser);
    // 根据区域ID获取下属的区域信息
    bool GetSubRegionByRegionId(const int nRegionId, std::list<Region>& lsRegion);
    // 根据用户ID获取用户信息
    bool GetUserInfoByUserId(const int nUserId, User& userInfo);
    // 根据区域ID获取区域信息
    bool GetRegionInfoByRegionId(const int nRegionId, Region& regionInfo);
    // 获取所有的用户信息
    bool GetUsers(std::list<User>& lsAllUser);
    // 打开数据库
    bool OpenDb(void);
    bool CloseDb(void);
    // 数据库更新用户信息(不含用户配置)
    bool UpdateUserInfo(const User& userInfo);
    // 根据用户ID获取用户配置
    bool GetUserConfigByUserId(const int nUserId, UserConfig* userConfig);
    // 数据库更新区域信息
    bool UpdateRegionInfo(const Region& regionInfo);
    // 增加一个区域
    bool AddRegionInfo(const Region& regionInfo);
    // 删除一个区域
    bool DeleteRegionInfo(const Region& regionInfo);
    // 增加一个用户
    bool AddUserInfo(const User& userInfo);
    // 删除一个用户
    bool DeleteUserInfo(const User& userInfo);
    // 更新用户配置
    bool UpdateUserConfig(const int nUserId, const UserConfig& userConfig);
};
