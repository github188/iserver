#include "StdAfx.h"
#include "ProcMsg.h"
#include "NetProc.h"
#include "UserMgr.h"

void CProcMsg::operator()()
{
    printfd("req msg:\n%s\n", m_reqMsg.DebugString().c_str());
    bool bRet = false;
    switch (m_reqMsg.cmd())
    {
    case LOGIN_REQUEST:
        {
            m_bNeedRsp = true;
            const LoginInfo& loginInfo = m_reqMsg.login_info();
            bRet = GetUserMgr().UserLogin(loginInfo, m_rspMsg.mutable_login_result());
            if (bRet)
            {
                GetUserMgr().GetUserConfig(m_rspMsg.login_result().user_id(), m_rspMsg.mutable_user_config());
                GetNetProc().Connect(loginInfo.user_ip().c_str(), 7321);
            }

            int result = m_rspMsg.login_result().user_id()==-1?-1:0;
            m_rspMsg.set_cmd(LOGIN_RESPONSE);
            m_rspMsg.set_result(result);
            break;
        }
    case LOGOUT_REQUEST:
        {
            m_bNeedRsp = false;
            bRet = GetUserMgr().UserLogout(m_reqMsg.user_id());
            GetNetProc().Close(m_reqMsg.user_ip().c_str(), 7321);
            break;
        }
    case CLIENT_HEART_BEAT:
        {
            m_bNeedRsp = false;
            bRet = GetUserMgr().UserHeartbeat(m_reqMsg.user_id());
            break;
        }
    case CLIENT_GET_CONFIG:
        {
            m_bNeedRsp = true;
            bRet = GetUserMgr().GetUserConfig(m_reqMsg.user_id(), m_rspMsg.mutable_user_config());
            break;
        }
    case CLIENT_SET_CONFIG:
        {   
            m_bNeedRsp = true;
            bRet = GetUserMgr().SetUserConfig(m_reqMsg.user_id(), m_reqMsg.user_config());
            break;
        }
    case CLIENT_GET_REGION:
        {
            m_bNeedRsp = false;
            bRet = GetUserMgr().GetRegion_Proto(m_reqMsg.user_ip().c_str());
            break;
        }
    case CLIENT_GET_USER:
        {
            m_bNeedRsp = false;
            bRet = GetUserMgr().GetUserByRegion_Proto(m_reqMsg.user_ip().c_str(), m_reqMsg.value());
            break;
        }
    case CLIENT_POST_MSG:
        {
            m_bNeedRsp = true;
            bRet = ProcTransMsg(m_reqMsg.user_ip().c_str(), m_reqMsg.post_msg());
            break;
        }
    default:
        break;
    }

    if (m_bNeedRsp)
    {
        string strbuf;
        m_rspMsg.SerializeToString(&strbuf);
        int nRet = GetNetProc().ReplyMsg(m_reqMsg.user_ip().c_str(), 
            strbuf.c_str(), strbuf.length());

        printfd("rsp msg(len:%d):\n%s\n", nRet, m_rspMsg.DebugString().c_str());
    }

    //delete this;
}


// 转发消息
bool CProcMsg::ProcTransMsg(const char* from_ip, const PostMsg& postMsg)
{
    User userInfo;
    if (false == GetUserMgr().GetUser_Proto(postMsg.to_user_id(), userInfo))
    {
        return false;
    }
    TransMsg transMsg;
    transMsg.set_cmd(CLIENT_POST_MSG);
    transMsg.mutable_post_msg()->CopyFrom(postMsg);
    string strbuf;
    transMsg.SerializeToString(&strbuf);

    boost::scoped_ptr<char> szReply(new char[sizeof(TransMsg)]);
    int  nReply = sizeof(TransMsg);
    int ret = GetNetProc().SendMsg(userInfo.user_ip().c_str(), 7321, strbuf.c_str(), strbuf.length(), szReply.get(), nReply);
    TransMsg rspMsg;
    if (!rspMsg.ParseFromArray(szReply.get(), nReply))
    {
        rspMsg.set_cmd(CLIENT_POST_MSG_RSP);
        rspMsg.set_result(-1);
    }
    rspMsg.SerializeToString(&strbuf);
    ret = GetNetProc().ReplyMsg(from_ip, strbuf.c_str(), strbuf.length());

    return true;
}

