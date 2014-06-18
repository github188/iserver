#pragma once
#include "UserOperator.h"

class CUserMgr;
CUserMgr& GetUserMgr();

class CUserMgr: public boost::noncopyable
{
public:
    CUserMgr(void);
    ~CUserMgr(void);
    // 从数据库获取资源：用户列表、区域列表、用户区域组织列表
    bool LoadRes();
    // 检查在线用户心跳状态
    void CheckState();

private:
    CBoostEvent m_SyncEvent;
    CBoostEvent m_CheckEvent;
    CUserOperator m_userOper;

    CBoostLock m_regionLock;
    std::list<Region> m_lsAllRegion;
    CBoostLock m_userLock;
    std::list<User> m_lsAllUser;
private:
    class CSyncThread
    {
    public:
        CSyncThread()
            : m_nWaitSecond(5000)
        {

        };
        void Start()
        {
            m_thread.reset(new boost::thread(boost::bind(&CSyncThread::SyncRes, this)));
        }
        void Stop()
        {
            m_thread->interrupt();
        }

    private:
        int m_nWaitSecond;
        boost::scoped_ptr<boost::thread> m_thread;
        void SyncRes()
        {
            while(false == GetUserMgr().m_SyncEvent.timed_wait(m_nWaitSecond))
            {
                GetUserMgr().LoadRes();
            }
        }
    };
    CSyncThread m_thSyncRes;

    class CCheckThread
    {
    public:
        CCheckThread()
            : m_nWaitSecond(1500)
        {

        };
        void Start()
        {
            m_thread.reset(new boost::thread(boost::bind(&CCheckThread::ChkState, this)));
        }
        void Stop()
        {
            m_thread->interrupt();
        }

    private:
        int m_nWaitSecond;
        boost::scoped_ptr<boost::thread> m_thread;
        void ChkState()
        {
            while(false == GetUserMgr().m_CheckEvent.timed_wait(m_nWaitSecond))
            {
                GetUserMgr().CheckState();
            }
        }
    };
    CCheckThread m_thCheckState;

public:
    // 验证用户登录
    bool UserLogin(const LoginInfo& loginInfo, LoginResult* pLoginResult);
    // 用户登出处理
    bool UserLogout(const int nUserId);
    // 处理心跳消息
    bool UserHeartbeat(const int nUserId);
    // 获取用户配置
    bool GetUserConfig(const int nUserId, UserConfig* pUserConfig);
    // 设置用户配置
    bool SetUserConfig(const int nUserId, const UserConfig& userConfig);

    /* proto */
    // 获取区域结构
    bool GetRegion_Proto(const char* client_ip);
    // 根据区域ID获取下属的用户
    bool GetUserByRegion_Proto(const char* client_ip, const int nRegionId);
    // 根据用户ID获取用户信息
    bool GetUser_Proto(const int nUserId, User& userInfo);

    /* xml */
    bool Xml_UserLogin(const std::string& login, std::string& result);
    bool Xml_UserLogout(const std::string& logout, std::string& result);
    bool Xml_UserHeartB(const std::string& heartb, std::string& result);
    bool Xml_GetRegion(std::string& region_list_xml);
    bool Xml_NewRegion(const std::string& new_info, std::string& result);
    bool Xml_GetRegionInfo(const std::string& _region_id, std::string& region_info);
    bool Xml_ModRegionInofo(const std::string& reg_info, std::string& result);

    bool Xml_GetUser(const std::string& _region_id, std::string& user_list_xml);
    bool Xml_GetUserInfo(const std::string& _user_id, std::string& user_info);
};




