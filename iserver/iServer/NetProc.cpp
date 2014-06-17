#include "StdAfx.h"
#include "NetProc.h"
#include "ProcMsg.h"

CNetProc& GetNetProc()
{
    static CNetProc np;
    return np;
}

void __stdcall cbf_data(const int handle, char *buff, int buff_len, char* reply_buff, int& reply_len, void *puser)
{
    CNetProc* p = (CNetProc* )puser;
    assert(p);
    if (p)
    {
        bool bNeedRsp = false;
        reply_buff = NULL;  // 内部返回...
        reply_len = 0;
        p->ProcMsg(buff, buff_len, bNeedRsp);
    }
}

CNetProc::CNetProc(void)
: m_hTcpServer(-1)
, m_threadpool(20)
, m_nListenPort(-1)
{
    lt_startup();
}

CNetProc::~CNetProc(void)
{
    lt_cleanup();
}

bool CNetProc::Start(const int nPort)
{
    if (nPort == m_nListenPort && -1 != m_hTcpServer)
    {
        return true;
    }
    else
    {
        Stop();
    }
    m_nListenPort = nPort;
    // start listen
    m_hTcpServer = lt_server_listen(m_nListenPort, cbf_data, PROTOCOL_TCP, this);

    return (-1 == m_hTcpServer)?false:true;
}

bool CNetProc::Stop(void)
{
    if (-1 != m_hTcpServer)
    {
        lt_server_shutdown(m_hTcpServer);
    }
    m_hTcpServer = -1;

    // thread pool
    m_threadpool.wait();
    
    return true;
}

// 连接服务器
bool CNetProc::Connect(const char* ip, const int port)
{
    std::string key = format_key(ip, port);

    CBoostGuard lock(&m_clientLock);
    if (-1 != GetClientHandle(key))
    {
        return true;
    }

    int handle = lt_client_connect(ip, port, PROTOCOL_TCP);
    if (-1 == handle)
    {
        return false;
    }

    CClient c(ip, port, handle);
    m_lsClient.push_back(c);

    return true;
}

// 与服务器断开连接
bool CNetProc::Close(const char* ip, const int port)
{
    std::string key = format_key(ip, port);

    CBoostGuard lock(&m_clientLock);
    if (-1 == GetClientHandle(key))
    {
        return true;
    }

    lt_client_disconnect(GetClientHandle(key));
    RemoveClient(key);

    return true;
}

const int CNetProc::GetClientHandle(const std::string& key)
{
    for (std::list<CClient>::const_iterator itr = m_lsClient.begin();
        itr != m_lsClient.end(); ++itr)
    {
        if (itr->GetKey() == key)
        {
            return itr->GetHandle();
        }
    }
    return -1;
}

void CNetProc::RemoveClient(const std::string& key)
{
    for (std::list<CClient>::const_iterator itr = m_lsClient.begin();
        itr != m_lsClient.end(); ++itr)
    {
        if (itr->GetKey() == key)
        {
            m_lsClient.erase(itr);
            return;
        }
    }
}

// 处理消息
bool CNetProc::ProcMsg(const char *msg, int msg_len, bool bNeedRsp)
{
    NetMsgReq netMsg;
    if (!netMsg.ParseFromArray(msg, msg_len))
    {
        //printfd("parse netmsg fail\n");
        return false;
    }

    boost::scoped_ptr<CProcMsg> obj(new CProcMsg(netMsg, bNeedRsp));
    return m_threadpool.schedule(*obj.get());
}

// 发送消息
int CNetProc::ReplyMsg(const char* ip, const char* msg, const int msg_len)
{
    return lt_server_reply(m_hTcpServer, ip, msg, msg_len);
}

int CNetProc::SendMsg(const char* ip, const int port, const char* msg, const int msg_len,
                      char* reply_data, int& reply_len)
{
    std::string key = format_key(ip, port);

    CBoostGuard lock(&m_clientLock);
    int nHandle = GetClientHandle(key);
    if (-1 == nHandle)
    {
        return -1;
    }

    return lt_client_send(nHandle, msg, msg_len, reply_data, reply_len);
}
