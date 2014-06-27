#include "StdAfx.h"
#include "UserMgr.h"
#include "NetProc.h"
#include "xml_def.h"
#include "error_def.h"

CUserMgr& GetUserMgr()
{
    static CUserMgr um;
    return um;
}

CUserMgr::CUserMgr(void)
{
    m_userOper.OpenDb();
    LoadRes();
    m_thSyncRes.Start();
    m_thCheckState.Start();
}

CUserMgr::~CUserMgr(void)
{
    m_SyncEvent.notify_one();
    m_thCheckState.Stop();
    m_thSyncRes.Stop();
    m_userOper.CloseDb();
}


// 从数据库获取资源
bool CUserMgr::LoadRes(void)
{
    CBoostGuard regLock(&m_regionLock);
    m_lsAllRegion.clear();
    if (false == m_userOper.GetRegions(m_lsAllRegion))
    {
        return false;
    }

    CBoostGuard usrLock(&m_userLock);
    m_lsAllUser.clear();
    if (false == m_userOper.GetUsers(m_lsAllUser))
    {
        return false;
    }

    printfd("get user size:%d\n", m_lsAllUser.size());

    return true;
}
// 处理心跳消息
bool CUserMgr::UserHeartbeat(const int nUserId)
{
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == nUserId)
        {
            itr->set_last_beat(time(NULL));
            printfd("user %d heart beat %d\n", nUserId, (int)time(NULL));
            if (time(NULL) % 15 == 0)    // 5s刷新一次
            {
                m_userOper.UpdateUserInfo(*itr);
            }
            break;
        }
    }
    return true;
}

// 验证用户登录
bool CUserMgr::UserLogin(const LoginInfo& loginInfo, LoginResult* pLoginResult)
{
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_name() == loginInfo.user_name())
        {
            if (itr->user_pwd() != loginInfo.user_pwd())
            {
                pLoginResult->set_user_id(-1);
                pLoginResult->set_desc("wrong password!");
                return false;
            }
            else
            {
                pLoginResult->set_user_id(itr->user_id());
                itr->set_longin_time(time(NULL));
                itr->set_user_ip(loginInfo.user_ip());
                m_userOper.UpdateUserInfo(*itr);
                return true;
            }
        }
    }

    pLoginResult->set_user_id(-1);
    pLoginResult->set_desc("no such user!");
    return false;
}

// 用户登出处理
bool CUserMgr::UserLogout(const int nUserId)
{
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == nUserId)
        {
            itr->set_logout_time(time(NULL));
            m_userOper.UpdateUserInfo(*itr);
            return true;
        }
    }

    return true;
}

// 获取用户配置
bool CUserMgr::GetUserConfig(const int nUserId, UserConfig* pUserConfig)
{
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == nUserId)
        {
            pUserConfig->CopyFrom(itr->user_config());
            return true;
        }
    }

    return false;
}

// 设置用户配置
bool CUserMgr::SetUserConfig(const int nUserId, const UserConfig& userConfig)
{
    if (false == m_userOper.UpdateUserConfig(nUserId, userConfig))
    {
        return false;
    }
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == nUserId)
        {
            itr->mutable_user_config()->CopyFrom(userConfig);
            return true;
        }
    }
    return true;
}

// 获取区域结构
bool CUserMgr::GetRegion_Proto(const char* client_ip)
{
    TransMsg transMsg;
    transMsg.set_cmd(CLIENT_GET_REGION_RSP);
    transMsg.set_result(0);
    CBoostGuard regLock(&m_regionLock);
    for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
        itr != m_lsAllRegion.end(); ++itr)
    {
        transMsg.add_region()->CopyFrom(*itr);
    }
    int tmp = 0;
    string strbuf;
    transMsg.SerializeToString(&strbuf);
    int nRet = GetNetProc().SendMsg(client_ip, 7321, strbuf.c_str(), strbuf.length(), NULL, tmp);
    printfd("region msg(%d):\n%s\n", nRet, transMsg.DebugString().c_str());

    return (nRet>0)?true:false;
}

// 根据区域ID获取下属的用户
bool CUserMgr::GetUserByRegion_Proto(const char* client_ip, const int nRegionId)
{
    int nUserCount = 0;
    string strbuf;

    TransMsg transMsg;
    transMsg.set_cmd(CLIENT_GET_USER_RSP);
    transMsg.set_result(0);
    CBoostGuard usrLock(&m_userLock);
    std::list<User>::const_iterator itr = m_lsAllUser.begin();
    while (itr != m_lsAllUser.end())
    {
        if (itr->parent_id() != nRegionId)
        {
            ++itr;
            continue;
        }

        int tmp = 0;
        transMsg.add_user()->CopyFrom(*itr);
        ++nUserCount;
        if (nUserCount == 5 || itr == m_lsAllUser.end())    // 发送一次
        {
            nUserCount = 0;
            transMsg.SerializeToString(&strbuf);
            int nRet = GetNetProc().SendMsg(client_ip, 7321, strbuf.c_str(), strbuf.length(), NULL, tmp);
            printfd("region msg...(%d):\n%s\n", nRet, transMsg.DebugString().c_str());
            transMsg.clear_user();
        }
        ++itr;
    }

    return true;
}

// 根据用户ID获取用户信息
bool CUserMgr::GetUser_Proto(const int nUserId, User& userInfo)
{
    return false;
}


bool CUserMgr::Xml_UserLogin(const std::string& login, std::string& result)
{
    CMarkup xml;
    if (false == xml.SetDoc(login))
    {
        return false;
    }
    if (false == xml.FindElem(XML_ROOT))
    {
        return false;
    }
    xml.IntoElem();
    if (false == xml.FindElem(XML_USER_NAME))
    {
        return false;
    }
    std::string user_name = xml.GetData();
    if (false == xml.FindElem(XML_USER_PASSWORD))
    {
        return false;
    }
    std::string user_password = xml.GetData();
    if (false == xml.FindElem(XML_USER_IP))
    {
        return false;
    }
    std::string user_ip = xml.GetData();

    CMarkup xml_ret;
    xml_ret.SetDoc(XML_HEADER);
    xml_ret.AddElem(XML_ROOT);
    xml_ret.IntoElem();

    _error::_value find_ret = _error::no_such_user;
    int user_id = -1;
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_name() == user_name)
        {
            if (itr->user_pwd() != user_password)
            {
                find_ret = _error::wrong_psw;
            }
            else
            {
                // 已经登录：user_ip
                if (!itr->user_ip().empty() && itr->user_ip() != user_ip)
                {
                    find_ret = _error::already_login;
                    break;
                }
                
                // 正常登录
                find_ret = _error::ok;
                user_id = itr->user_id();

                itr->set_longin_time(time(NULL));
                itr->set_last_beat(time(NULL));
                itr->set_user_ip(user_ip);
                m_userOper.UpdateUserInfo(*itr);
            }
        }
    }

    xml_ret.AddElem(XML_RESULT, find_ret);
    xml_ret.AddElem(XML_RESULT_MSG, _error::to_msg(find_ret));
    xml_ret.AddElem(XML_USER_ID, user_id);
    xml_ret.OutOfElem();
    result = xml_ret.GetDoc();

    return true;
}

bool CUserMgr::Xml_UserLogout(const std::string& logout, std::string& result)
{
    CMarkup xml;
    if (false == xml.SetDoc(logout))
    {
        return false;
    }
    if (false == xml.FindElem(XML_ROOT))
    {
        return false;
    }
    xml.IntoElem();
    if (false == xml.FindElem(XML_USER_ID))
    {
        return false;
    }
    int user_id = atoi(xml.GetData().c_str());
    if (false == xml.FindElem(XML_USER_IP))
    {
        return false;
    }
    std::string user_ip = xml.GetData();

    CMarkup xml_ret;
    xml_ret.SetDoc(XML_HEADER);
    xml_ret.AddElem(XML_ROOT);
    xml_ret.IntoElem();

    _error::_value find_ret = _error::no_such_user;
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == user_id)
        {
            // ip 地址不匹配
            if (itr->user_ip() != user_ip)
            {
                find_ret = _error::no_auth;
                break;
            }
            
            // 注销
            find_ret = _error::ok;

            itr->clear_user_ip();
            itr->clear_last_beat();
            itr->set_logout_time(time(NULL));
            m_userOper.UpdateUserInfo(*itr);
            break;
        }
    }

    xml_ret.AddElem(XML_RESULT, find_ret);
    xml_ret.AddElem(XML_RESULT_MSG, _error::to_msg(find_ret));
    xml_ret.AddElem(XML_USER_ID, user_id);
    xml_ret.OutOfElem();
    result = xml_ret.GetDoc();
    
    return true;
}


bool CUserMgr::Xml_UserHeartB(const std::string& heartb, std::string& result)
{
    CMarkup xml;
    if (false == xml.SetDoc(heartb))
    {
        return false;
    }
    if (false == xml.FindElem(XML_ROOT))
    {
        return false;
    }
    xml.IntoElem();
    if (false == xml.FindElem(XML_USER_ID))
    {
        return false;
    }
    int user_id = atoi(xml.GetData().c_str());
    if (false == xml.FindElem(XML_USER_IP))
    {
        return false;
    }
    std::string user_ip = xml.GetData();

    CMarkup xml_ret;
    xml_ret.SetDoc(XML_HEADER);
    xml_ret.AddElem(XML_ROOT);
    xml_ret.IntoElem();

    _error::_value find_ret = _error::no_such_user;
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == user_id)
        {
            // ip 地址不匹配
            if (itr->user_ip() != user_ip)
            {
                find_ret = _error::no_auth;
                break;
            }

            find_ret = _error::ok;

            itr->set_last_beat(time(NULL));
            printfd("user %d heart beat %d\n", user_id, (int)time(NULL));
            if (time(NULL) % 5 == 0)    // 5s 刷新一次
            {
                m_userOper.UpdateUserInfo(*itr);
            }
            break;
        }
    }

    xml_ret.AddElem(XML_RESULT, find_ret);
    xml_ret.AddElem(XML_RESULT_MSG, _error::to_msg(find_ret));
    xml_ret.AddElem(XML_USER_ID, user_id);
    xml_ret.OutOfElem();
    result = xml_ret.GetDoc();

    return true;
}

bool CUserMgr::Xml_GetRegion(std::string& region_list_xml)
{
    CMarkup xml;
    xml.SetDoc(XML_HEADER);
    xml.AddElem(XML_REGION_LIST);
    xml.IntoElem();

    CBoostGuard regLock(&m_regionLock);
    for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
        itr != m_lsAllRegion.end(); ++itr)
    {
        xml.AddElem(XML_REGION);
        xml.IntoElem();
        xml.AddElem(XML_REGION_ID, itr->id());
        xml.AddElem(XML_REGION_NAME, fcU2A(itr->name().c_str()));
        xml.AddElem(XML_PARENT_ID, itr->parent_id());
        xml.OutOfElem();
    }

    xml.OutOfElem();
    region_list_xml = xml.GetDoc();

    return true;
}

bool CUserMgr::Xml_NewRegion(const std::string& new_info, std::string& result)
{
    CMarkup xml;
    if (false == xml.SetDoc(new_info))
    {
        return false;
    }
    if (false == xml.FindElem(XML_REGION))
    {
        return false;
    }
    xml.IntoElem();
    if (false == xml.FindElem(XML_REGION_NAME))
    {
        return false;
    }
    std::string region_name = xml.GetData();
    if (false == xml.FindElem(XML_PARENT_ID))
    {
        return false;
    }
    int parent_id = atoi(xml.GetData().c_str());

    CMarkup xml_ret;
    xml_ret.SetDoc(XML_HEADER);
    xml_ret.AddElem(XML_ROOT);
    xml_ret.IntoElem();

    // 检查是否存在父级
    _error::_value find_ret = _error::no_such_parent;
    CBoostGuard regLock(&m_regionLock);
    for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
    itr != m_lsAllRegion.end(); ++itr)
    {
        if (itr->id() == parent_id)
        {
            find_ret = _error::ok;
            break;
        }
    }
    // 检查是否有重复名称
    if (_error::ok == find_ret)
    {
        for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
            itr != m_lsAllRegion.end(); ++itr)
        {
            if (itr->parent_id() != parent_id)
            {
                continue;
            }

            if (itr->name() == region_name)
            {
                find_ret = _error::repeated_region;
                break;
            }      
        }

        Region reg;
        reg.set_name(region_name);
        reg.set_parent_id(parent_id);
        if(false == m_userOper.AddRegionInfo(reg))
        {
            find_ret = _error::operator_fail;
        }
    }

    xml_ret.AddElem(XML_RESULT, find_ret);
    xml_ret.AddElem(XML_RESULT_MSG, _error::to_msg(find_ret));
    xml_ret.OutOfElem();
    result = xml_ret.GetDoc();

    return true;
}

bool CUserMgr::Xml_GetRegionInfo(const std::string& _region_id, std::string& region_info)
{
    int region_id = _tstoi(_region_id.c_str());

    CMarkup xml;
    xml.SetDoc(XML_HEADER);
    xml.AddElem(XML_REGION);
    xml.IntoElem();

    CBoostGuard regLock(&m_regionLock);
    for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
        itr != m_lsAllRegion.end(); ++itr)
    {
        if (itr->id() == region_id)
        {
            xml.AddElem(XML_REGION_ID, itr->id());
            xml.AddElem(XML_REGION_NAME, fcU2A(itr->name().c_str()));
            xml.AddElem(XML_PARENT_ID, itr->parent_id());
        }
    }
    xml.OutOfElem();
    region_info = xml.GetDoc();

    return true;
}

bool CUserMgr::Xml_ModRegionInfo(const std::string& reg_info, std::string& result)
{
    CMarkup xml;
    if (false == xml.SetDoc(reg_info))
    {
        return false;
    }
    if (false == xml.FindElem(XML_REGION))
    {
        return false;
    }
    xml.IntoElem();
    if (false == xml.FindElem(XML_REGION_ID))
    {
        return false;
    }
    int region_id = atoi(xml.GetData().c_str());
    if (false == xml.FindElem(XML_REGION_NAME))
    {
        return false;
    }
    std::string region_name = xml.GetData();
    if (false == xml.FindElem(XML_PARENT_ID))
    {
        return false;
    }
    int parent_id = atoi(xml.GetData().c_str());
    
    Region reg; // 修改区域的信息
    reg.set_id(region_id);
    reg.set_name(region_name);
    reg.set_parent_id(parent_id);

    CMarkup xml_ret;
    xml_ret.SetDoc(XML_HEADER);
    xml_ret.AddElem(XML_ROOT);
    xml_ret.IntoElem();

    // 检查该区域是否存在
    _error::_value find_ret = _error::no_such_region;
    CBoostGuard regLock(&m_regionLock);
    for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
        itr != m_lsAllRegion.end(); ++itr)
    {
        if (itr->id() == region_id)
        {
            find_ret = _error::ok;
            break;
        }
    }
    if (_error::ok != find_ret)
    {
        goto _exit;
    }
    
    // 修改名称是否重复
    find_ret = _error::ok;
    for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
        itr != m_lsAllRegion.end(); ++itr)
    {
        if (itr->parent_id() != parent_id)
        {
            continue;
        }
        if (itr->name() == region_name)
        {
            find_ret = _error::repeated_region;
            break;
        }  
    }
    if (_error::ok != find_ret)
    {
        goto _exit;
    }
    
    // 修改父级是否存在
    find_ret = _error::no_such_parent;
    for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
        itr != m_lsAllRegion.end(); ++itr)
    {
        if (itr->id() == parent_id)
        {
            find_ret = _error::ok;
            break;
        }
    }

    if(false == m_userOper.UpdateRegionInfo(reg))
    {
        find_ret = _error::operator_fail;
    }

_exit:
    xml_ret.AddElem(XML_RESULT, find_ret);
    xml_ret.AddElem(XML_RESULT_MSG, _error::to_msg(find_ret));
    xml_ret.OutOfElem();
    result = xml_ret.GetDoc();

    return true;
}

bool CUserMgr::Xml_DelRegionInfo(const std::string& _region_id, std::string& result)
{
    int region_id = atoi(_region_id.c_str());

    CMarkup xml_ret;
    xml_ret.SetDoc(XML_HEADER);
    xml_ret.AddElem(XML_ROOT);
    xml_ret.IntoElem();

    Region reg;
    reg.set_id(region_id);

    // 检查该区域是否存在
    _error::_value find_ret = _error::no_such_region;
    CBoostGuard regLock(&m_regionLock);
    for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
        itr != m_lsAllRegion.end(); ++itr)
    {
        if (itr->id() == region_id)
        {
            find_ret = _error::ok;
            break;
        }
    }
    if (_error::ok != find_ret)
    {
        goto _exit;
    }

    if(false == m_userOper.DeleteRegionInfo(reg))
    {
        find_ret = _error::operator_fail;
    }

_exit:
    xml_ret.AddElem(XML_RESULT, find_ret);
    xml_ret.AddElem(XML_RESULT_MSG, _error::to_msg(find_ret));
    xml_ret.OutOfElem();
    result = xml_ret.GetDoc();

    return true;
}
bool CUserMgr::Xml_GetUser(const std::string& _region_id, std::string& user_list_xml)
{
    int region_id = _tstoi(_region_id.c_str());

    CMarkup xml;
    xml.SetDoc(XML_HEADER);
    xml.AddElem(XML_USER_LIST);
    xml.IntoElem();

    CBoostGuard usrLock(&m_userLock);
    std::list<User>::const_iterator itr = m_lsAllUser.begin();
    while (itr != m_lsAllUser.end())
    {
        if (itr->parent_id() != region_id)
        {
            ++itr;
            continue;
        }

        xml.AddElem(XML_USER);
        xml.IntoElem();
        xml.AddElem(XML_USER_ID, itr->user_id());
        xml.AddElem(XML_PARENT_ID, fcU2A(itr->user_name().c_str()));
        xml.AddElem(XML_USER_NAME, itr->parent_id());
        xml.AddElem(XML_USER_IP, itr->user_ip());
        xml.AddElem(XML_DISPLAY_NAME, fcU2A(itr->display_name().c_str()));

        const UserConfig& uc = itr->user_config();
        xml.AddElem(XML_CONFIG);
        xml.IntoElem();
        xml.AddElem(XML_BUBBLE, uc.use_bubble());
        xml.AddElem(XML_SOUND, uc.use_sound());
        xml.AddElem(XML_FONT_COLOR, uc.font_color());
        xml.AddElem(XML_FONT_FAMILY, fcU2A(uc.font_name().c_str()));
        xml.AddElem(XML_FONT_SIZE, uc.font_size());
        xml.AddElem(XML_AUTO_LOGIN, uc.auto_login());
        xml.OutOfElem();

        xml.OutOfElem();

        ++itr;
    }
    xml.OutOfElem();
    user_list_xml = xml.GetDoc();

    return true;
}

bool CUserMgr::Xml_NewUser(const std::string& user_info, std::string& result)
{
    return true;
}
bool CUserMgr::Xml_GetUserInfo(const std::string& _user_id, std::string& user_info)
{
    int user_id = _tstoi(_user_id.c_str());

    CMarkup xml;
    xml.SetDoc(XML_HEADER);
    xml.AddElem(XML_USER);
    xml.IntoElem();

    bool find_ret = false;
    CBoostGuard usrLock(&m_userLock);
    std::list<User>::const_iterator itr = m_lsAllUser.begin();
    while (itr != m_lsAllUser.end())
    {
        if (itr->user_id() == user_id)
        {
            find_ret = true;

            xml.AddElem(XML_USER_ID, itr->user_id());
            xml.AddElem(XML_PARENT_ID, fcU2A(itr->user_name().c_str()));
            xml.AddElem(XML_USER_NAME, itr->parent_id());
            xml.AddElem(XML_USER_IP, itr->user_ip());
            xml.AddElem(XML_DISPLAY_NAME, fcU2A(itr->display_name().c_str()));

            const UserConfig& uc = itr->user_config();
            xml.AddElem(XML_CONFIG);
            xml.IntoElem();
            xml.AddElem(XML_BUBBLE, uc.use_bubble());
            xml.AddElem(XML_SOUND, uc.use_sound());
            xml.AddElem(XML_FONT_COLOR, uc.font_color());
            xml.AddElem(XML_FONT_FAMILY, fcU2A(uc.font_name().c_str()));
            xml.AddElem(XML_FONT_SIZE, uc.font_size());
            xml.AddElem(XML_AUTO_LOGIN, uc.auto_login());
            xml.OutOfElem();
        }
        ++itr;
    }

    xml.OutOfElem();
    user_info = xml.GetDoc();

    return true;
}

bool CUserMgr::Xml_ModUserInfo(const std::string& user_info, std::string& result)
{
    return true;
}

bool CUserMgr::Xml_DelUserInfo(const std::string& _user_id, std::string& result)
{
    return true;
}
void CUserMgr::CheckState()
{   
    // 执行一次
    time_t tNow = time(NULL);
    CBoostGuard usrLock(&m_userLock);
    std::list<User>::iterator itr = m_lsAllUser.begin();
    while (itr != m_lsAllUser.end())
    {
        if (!itr->user_ip().empty() && _abs64(itr->last_beat() - tNow) >= 15) // 3次心跳
        {
            // 记录登出
            itr->clear_user_ip();
            itr->clear_last_beat();
            itr->set_logout_time(tNow);
            m_userOper.UpdateUserInfo(*itr);

            printfd("user id %d heart beat out of time\n", itr->user_id());
        }
        ++itr;
    }
}