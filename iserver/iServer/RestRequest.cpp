#include "StdAfx.h"
#include "RestRequest.h"
#include "ApiProc.h"

CRestRequest& GetRestRequest()
{
    static CRestRequest rr;
    return rr;
}


void _stdcall do_api(const _request& req, _reply& rep, void* p)
{
    printf("*** %s %s %s\n", req.method.c_str(), req.uri.c_str(), req.extern_string.c_str());

    boost::scoped_ptr<CApiProc> obj(new CApiProc);
    obj->ApiProc(req, rep); // 必须是阻塞的函数
}

CRestRequest::CRestRequest(void)
{
}

CRestRequest::~CRestRequest(void)
{
}

int CRestRequest::Start(const std::string& port)
{
    int ret = http_run(port, 2, current_dir().append("\\www"), do_api, NULL);
    return ret;
}

void CRestRequest::Stop()
{
    http_stop();
}