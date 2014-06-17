#include "StdAfx.h"
#include "UserOperator.h"

CUserOperator::CUserOperator(void)
: m_pSqlHelper(NULL)
{
    db_lib_init();
}

CUserOperator::~CUserOperator(void)
{
    db_lib_fini();
}

// 获取所有的区域信息
bool CUserOperator::GetRegions(std::list<Region>& lsAllRegion)
{
    CSQLRecordset rs(m_dbType);
    if (false == m_pSqlHelper->Excute("select * from region", rs))
    {
        return false;
    }

    while (false == rs.is_eof())
    {
        //string gb2313 = boost::locale::conv::from_utf(rs.get_string_item("name"), "GB2312");

        Region regionInfo;
        regionInfo.set_id(rs.get_int32_item("_id"));
        regionInfo.set_name(rs.get_string_item("name"));
        regionInfo.set_parent_id(rs.get_int32_item("parent"));
        lsAllRegion.push_back(regionInfo);

        rs.move_next();
    }

    return true;
}

// 根据区域ID获取下属的所有用户信息
bool CUserOperator::GetSubUserByRegionId(const int nRegionId, std::list<User>& lsUser)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "select * from user where parent_region=%d", nRegionId);

    CSQLRecordset rs(m_dbType);
    if (false == m_pSqlHelper->Excute(sql, rs))
    {
        return false;
    }

    while (false == rs.is_eof())
    {
        User userInfo;
        userInfo.set_user_name(rs.get_string_item("user_name"));
        userInfo.set_user_pwd(rs.get_string_item("user_pwd"));
        userInfo.set_user_ip(rs.get_string_item("user_ip"));
        userInfo.set_display_name(rs.get_string_item("client_name"));
        userInfo.set_longin_time(rs.get_int64_item("login_time"));
        userInfo.set_logout_time(rs.get_int64_item("logout_time"));
        userInfo.set_last_beat(rs.get_int64_item("last_beat"));
        userInfo.set_parent_id(rs.get_int32_item("parent_region"));
        userInfo.set_user_id(rs.get_int32_item("_id"));
        GetUserConfigByUserId(userInfo.user_id(), userInfo.mutable_user_config());
        lsUser.push_back(userInfo);

        rs.move_next();
    }

    return true;
}

// 根据区域ID获取下属的区域信息
bool CUserOperator::GetSubRegionByRegionId(const int nRegionId, std::list<Region>& lsRegion)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "select * from region where parent=%d", nRegionId);

    CSQLRecordset rs(m_dbType);
    if (false == m_pSqlHelper->Excute(sql, rs))
    {
        return false;
    }

    while (false == rs.is_eof())
    {
        Region regionInfo;
        regionInfo.set_id(rs.get_int32_item("_id"));
        regionInfo.set_name(rs.get_string_item("name"));
        regionInfo.set_parent_id(rs.get_int32_item("parent"));
        lsRegion.push_back(regionInfo);

        rs.move_next();
    }

    return true;
}

// 根据用户ID获取用户信息
bool CUserOperator::GetUserInfoByUserId(const int nUserId, User& userInfo)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "select * from user where _id=%d", nUserId);

    CSQLRecordset rs(m_dbType);
    if (false == m_pSqlHelper->Excute(sql, rs))
    {
        return false;
    }

    if (false == rs.is_eof())
    {
        userInfo.set_user_name(rs.get_string_item("user_name"));
        userInfo.set_user_pwd(rs.get_string_item("user_pwd"));
        userInfo.set_user_ip(rs.get_string_item("user_ip"));
        userInfo.set_display_name(rs.get_string_item("client_name"));
        userInfo.set_longin_time(rs.get_int64_item("login_time"));
        userInfo.set_logout_time(rs.get_int64_item("logout_time"));
        userInfo.set_last_beat(rs.get_int64_item("last_beat"));
        userInfo.set_parent_id(rs.get_int32_item("parent_region"));
        userInfo.set_user_id(rs.get_int32_item("_id"));
        GetUserConfigByUserId(userInfo.user_id(), userInfo.mutable_user_config());
    }

    return true;
}

// 根据区域ID获取区域信息
bool CUserOperator::GetRegionInfoByRegionId(const int nRegionId, Region& regionInfo)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "select * from region where _id=%d", nRegionId);

    CSQLRecordset rs(m_dbType);
    if (false == m_pSqlHelper->Excute(sql, rs))
    {
        return false;
    }

    if (false == rs.is_eof())
    {
        regionInfo.set_id(rs.get_int32_item("_id"));
        regionInfo.set_name(rs.get_string_item("name"));
        regionInfo.set_parent_id(rs.get_int32_item("parent"));
    }

    return true;
}

// 获取所有的用户信息
bool CUserOperator::GetUsers(std::list<User>& lsAllUser)
{
    CSQLRecordset rs(m_dbType);
    if (false == m_pSqlHelper->Excute("select * from user", rs))
    {
        return false;
    }

    while (false == rs.is_eof())
    {
        User userInfo;
        userInfo.set_user_name(rs.get_string_item("user_name"));
        userInfo.set_user_pwd(rs.get_string_item("user_pwd"));
        userInfo.set_user_ip(rs.get_string_item("user_ip"));
        userInfo.set_display_name(rs.get_string_item("client_name"));
        userInfo.set_longin_time(rs.get_int64_item("login_time"));
        userInfo.set_logout_time(rs.get_int64_item("logout_time"));
        userInfo.set_last_beat(rs.get_int64_item("last_beat"));
        userInfo.set_parent_id(rs.get_int32_item("parent_region"));
        userInfo.set_user_id(rs.get_int32_item("_id"));
        GetUserConfigByUserId(userInfo.user_id(), userInfo.mutable_user_config());
        lsAllUser.push_back(userInfo);

        rs.move_next();
    }

    return true;
}

// 打开数据库
bool CUserOperator::OpenDb(void)
{
    if (NULL != m_pSqlHelper)
    {
        m_pSqlHelper->Disconnect();
        db_destory_instance(m_pSqlHelper);
        m_pSqlHelper = NULL;
    }

    m_pSqlHelper = db_create_instanse(sqlite_type);
    assert(m_pSqlHelper);
    if (NULL == m_pSqlHelper)
    {
        return false;
    }

    m_dbType = sqlite_type;

    db_conn_info_t dbinfo;
    dbinfo.host = current_dir().append("\\localdata.db");
    dbinfo.port = "3306";
    dbinfo.username = "root";
    dbinfo.password = "888";
    dbinfo.dbschema = "ivms8100v3";

    return m_pSqlHelper->Connect(dbinfo);
}

bool CUserOperator::CloseDb(void)
{
    return m_pSqlHelper->Disconnect();
}

// 数据库更新用户信息(不含用户配置)
bool CUserOperator::UpdateUserInfo(const User& userInfo)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "update user set user_name='%s', user_pwd='%s', \
        user_ip='%s', client_name='%s', login_time='%d', \
        logout_time='%d', last_beat='%d', parent_region='%d' \
        where _id=%d", userInfo.user_name().c_str(), userInfo.user_pwd().c_str(),
        userInfo.user_ip().c_str(), userInfo.display_name().c_str(), (int)userInfo.longin_time(),
        (int)userInfo.logout_time(), (int)userInfo.last_beat(), 
        userInfo.parent_id(), userInfo.user_id());

    return m_pSqlHelper->Excute(sql);
}

// 根据用户ID获取用户配置
bool CUserOperator::GetUserConfigByUserId(const int nUserId, UserConfig* userConfig)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "select * from config where user_id=%d", nUserId);

    CSQLRecordset rs(m_dbType);
    if (false == m_pSqlHelper->Excute(sql, rs))
    {
        return false;
    }

    if (false == rs.is_eof())
    {
        //string gb2313 = boost::locale::conv::from_utf(rs.get_string_item("name"), "GB2312");

        userConfig->set_use_sound(rs.get_int32_item("sound"));
        userConfig->set_use_bubble(rs.get_int32_item("bubble"));
        userConfig->set_font_color(rs.get_int32_item("font_color"));
        userConfig->set_font_name(rs.get_string_item("font_name"));
        userConfig->set_font_style(rs.get_int32_item("font_style"));
        userConfig->set_font_size(rs.get_int32_item("font_size"));
        userConfig->set_run_start(rs.get_int32_item("run_start"));
        userConfig->set_auto_login(rs.get_int32_item("auto_login"));
    }

    return true;
}

// 数据库更新区域信息
bool CUserOperator::UpdateRegionInfo(const Region& regionInfo)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "update region set name='%s', parent='%s', where _id=%d",
        regionInfo.name(), regionInfo.id());

    return m_pSqlHelper->Excute(sql);
}

// 增加一个区域
bool CUserOperator::AddRegionInfo(const Region& regionInfo)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "insert into region(name, parent) values('%s', %d)", 
        regionInfo.name().c_str(), regionInfo.parent_id());

    return m_pSqlHelper->Excute(sql);
}

// 删除一个区域
bool CUserOperator::DeleteRegionInfo(const Region& regionInfo)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "delete from user where parent_region=%d", regionInfo.id());

    if (false == m_pSqlHelper->Excute(sql))
    {
        return false;
    }

    sprintf_s(sql, _countof(sql),
        "delete from region where _id = %d", regionInfo.id());

    return m_pSqlHelper->Excute(sql);
}

// 增加一个用户
bool CUserOperator::AddUserInfo(const User& userInfo)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "insert into user(user_name, user_pwd, client_name, parent_region) \
        values('%s', '%s', '%s', %d)", 
        userInfo.user_name().c_str(), userInfo.user_pwd().c_str(),
        userInfo.display_name().c_str(), userInfo.parent_id());

    if (false == m_pSqlHelper->Excute(sql))
    {
        return false;
    }

    sprintf_s(sql, _countof(sql),
        "select * from user where user_name=%s", userInfo.user_name());

    CSQLRecordset rs(m_dbType);
    if (false == m_pSqlHelper->Excute(sql, rs))
    {
        return false;
    }

    int nUserId = -1;
    if (false != rs.is_eof())
    {
        return false;
    }
    nUserId = rs.get_int32_item("_id");

    sprintf_s(sql, _countof(sql),
        "insert into config(user_id) values(%d)", nUserId);

    return m_pSqlHelper->Excute(sql);
}

// 删除一个用户
bool CUserOperator::DeleteUserInfo(const User& userInfo)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "delete from user where _id=%d", userInfo.user_id());

    if (false == m_pSqlHelper->Excute(sql))
    {
        return false;
    }

    sprintf_s(sql, _countof(sql),
        "delete from config where user_id=%d", userInfo.user_id());

    m_pSqlHelper->Excute(sql);

    return true;
}

// 更新用户配置
bool CUserOperator::UpdateUserConfig(const int nUserId, const UserConfig& userConfig)
{
    char sql[max_sql_len] = {0};
    sprintf_s(sql, _countof(sql),
        "update config set sound='%d', bubble='%d', font_color='%d', \
        font_name='%s', font_style='%d',\
        font_size='%d', run_start='%d', auto_login='%d' where _id=%d",
        userConfig.use_sound(), userConfig.use_bubble(), userConfig.font_color(),
        userConfig.font_name().c_str(), userConfig.font_style(), userConfig.font_size(),
        userConfig.run_start(), userConfig.auto_login(), nUserId);

    return m_pSqlHelper->Excute(sql);
}
