#pragma once

class CRestRequest;
CRestRequest& GetRestRequest();


class CRestRequest : public boost::noncopyable
{
public:
    CRestRequest(void);
    ~CRestRequest(void);

    int Start(const std::string& port = "6262");
    void Stop();
};
