
#ifndef GUARD_SQL_UDD_H
#define GUARD_SQL_UDD_H
#include <string>

// using utf8 
typedef struct _db_conn_info_t {
    std::string host;
    std::string port;
    std::string username;
    std::string password;
    std::string dbschema;
}db_conn_info_t;


typedef enum _db_type_t {
    unknown_db,
    sqlite_type,
    mysql_type,
}db_type_t;

#endif // end of GUARD_SQL_UDD_H