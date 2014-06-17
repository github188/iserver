#pragma once

class CNetProc;
CNetProc& GetNetProc();

// tcp 
class CClient
{
public:
    CClient(const char* ip, const int port, const int& handle)
        : m_hTcpClient(handle)
        , m_key(format_key(ip, port))
    {
        strcpy_s(m_szIp, 16, ip);
    }

    inline const std::string GetKey() const{return m_key;};
    inline const int& GetHandle() const{return m_hTcpClient;};
private:
    int m_hTcpClient;
    char m_szIp[16];
    int  m_nPort;
    std::string m_key;
};

class CNetProc : public boost::noncopyable
{
public:
    CNetProc(void);
    ~CNetProc(void);
    bool Start(const int nPort);
    bool Stop(void);

    // ���ӷ�����
    bool Connect(const char* ip, const int port);
    // ��������Ͽ�����
    bool Close(const char* ip, const int port);

private:
    int m_hTcpServer;
    int m_nListenPort;
    boost::threadpool::pool m_threadpool;

    CBoostLock m_clientLock;
    std::list<CClient> m_lsClient;
    const int GetClientHandle(const std::string& key);
    void RemoveClient(const std::string& key);

public:
    // ������Ϣ
    bool ProcMsg(const char *msg, int msg_len, bool bNeedRsp);
    // ֱ�ӷ�����Ϣ
    int ReplyMsg(const char* ip, const char* msg, const int msg_len);
    // ������Ϣ
    int SendMsg(const char* ip, const int port, const char* msg, const int msg_len,
        char* reply_data, int& reply_len);

};

