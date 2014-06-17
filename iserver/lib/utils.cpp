#include "StdAfx.h"
#include "utils.h"
#include "fcTchar.h"

std::string format_key(const char* ip, const int port)
{
    boost::format fmt("%1%_%2%");
    fmt %ip%port;

    return fmt.str();
}

std::string current_dir()
{
    return boost::filesystem::initial_path<boost::filesystem::path>().string(); 
}
