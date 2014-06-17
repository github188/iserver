#pragma once

class CApiProc
{
public:
    CApiProc();
    virtual ~CApiProc(void){};

    int ApiProc(const _request& req_, _reply& rep_);

};