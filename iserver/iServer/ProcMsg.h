#pragma once

class CProcMsg
{
public:
    CProcMsg(const NetMsgReq& netMsg, bool bNeedRsp)
        : m_reqMsg(netMsg)
        , m_bNeedRsp(bNeedRsp)
    {};
    virtual ~CProcMsg(){};

    void operator()();
private:
    bool m_bNeedRsp;
    NetMsgReq m_reqMsg;
    NetMsgRsp m_rspMsg;

public:
    // 转发消息
    bool ProcTransMsg(const char* client_ip, const PostMsg& postMsg);
};