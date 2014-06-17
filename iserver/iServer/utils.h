#pragma once

#ifdef _DEBUG
#define printfd printf
#else
#define printfd __noop
#endif

std::string format_key(const char* ip, const int port);