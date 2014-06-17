#pragma once
#include "UserOperator.h"

class CUserMgr;
CUserMgr& GetUserMgr();

class CUserMgr: public boost::noncopyable
{
public:
    CUserMgr(void);
    ~CUserMgr(void);
    // �����ݿ��ȡ��Դ���û��б������б��û�������֯�б�
    bool LoadRes(void);

private:
    CBoostEvent m_SyncEvent;
    CUserOperator m_userOper;

    CBoostLock m_regionLock;
    std::list<Region> m_lsAllRegion;
    CBoostLock m_userLock;
    std::list<User> m_lsAllUser;
private:
    class CSyncThread
    {
    public:
        void Start()
        {
            m_thread.reset(new boost::thread(boost::bind(&CSyncThread::SyncRes, this)));
        }
        void Stop()
        {
            m_thread->interrupt();
        }

    private:
        boost::scoped_ptr<boost::thread> m_thread;
        void SyncRes()
        {
            while(false == GetUserMgr().m_SyncEvent.timed_wait(5*1000))
            {
                GetUserMgr().LoadRes();
            }
        }
    };

    CSyncThread m_thSyncRes;

public:
    // ��֤�û���¼
    bool UserLogin(const LoginInfo& loginInfo, LoginResult* pLoginResult);
    // �û��ǳ�����
    bool UserLogout(const int nUserId);
    // ����������Ϣ
    bool UserHeartbeat(const int nUserId);
    // ��ȡ�û�����
    bool GetUserConfig(const int nUserId, UserConfig* pUserConfig);
    // �����û�����
    bool SetUserConfig(const int nUserId, const UserConfig& userConfig);

    /* proto */
    // ��ȡ����ṹ
    bool GetRegion_Proto(const char* client_ip);
    // ��������ID��ȡ�������û�
    bool GetUserByRegion_Proto(const char* client_ip, const int nRegionId);
    // �����û�ID��ȡ�û���Ϣ
    bool GetUser_Proto(const int nUserId, User& userInfo);

    /* xml */
    bool UserLogin_Xml(const std::string& login, std::string& result);
    bool UserLogout_Xml(const std::string& logout, std::string& result);
    bool UserHeartB_Xml(const std::string& heartb, std::string& result);
    bool GetRegion_Xml(std::string& region_list_xml);
    bool GetRegionInfo_Xml(const std::string& _region_id, std::string& region_info);

    bool GetUser_Xml(const std::string& _region_id, std::string& user_list_xml);
    bool GetUserInfo_Xml(const std::string& _user_id, std::string& user_info);
};




