#include "StdAfx.h"
#include "utils.h"

std::string format_key(const char* ip, const int port)
{
    boost::format fmt("%1%_%2%");
    fmt %ip%port;

    return fmt.str();
}