#pragma once

#ifdef _DEBUG
#define printfd printf
#else
#define printfd __noop
#endif

std::string format_key(const char* ip, const int port);

std::string current_dir();

std::string utf8_ansi(const std::string& utf8_string, std::string ansi_string); // UTF8转换为ANSI  
std::string ansi_utf8(const std::string& ansi_string, std::string utf8_string); // ANSI转换为UTF8  