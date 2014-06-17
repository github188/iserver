#pragma once
#include "sql_udd.h"
#include <vector>

class CSQLRecordset
{
public:
    CSQLRecordset(const db_type_t& db_type);
    ~CSQLRecordset(void);

    inline bool is_empty(void) const
    {
        return 0 == m_row;
    }
    inline bool is_eof(void) const
    {
        return m_current_row > m_row;
    }
    inline int get_column_count(void) const
    {
        return m_column;
    }
    inline int get_row_count(void) const
    {
        return m_row;
    }

    // 具体取值函数
    __int32 get_int32_item(const char *col_name, __int32 const default_value = 0) const;
    __int64 get_int64_item(const char *col_name, __int64 const default_value = 0) const;
    const char *get_string_item(const char *col_name, char * const default_value = "") const;

    // 不一样的
    bool move_next();
    void set_data(char **result, int row, int column);

    const char* get_columu_name(const int column_index);
private:
    // 禁止外部调用 = 赋值
    CSQLRecordset& operator=(const CSQLRecordset &);
    CSQLRecordset(const CSQLRecordset &);

    const char* get_item(const char *col_name) const;
    const char* get_item(const int col_index) const;
    // 不一样的
    const char* get_item_(const int col_index) const;


    int m_row;           /* Number of result rows written here */
    int m_column;        /* Number of result columns written here */
    int m_current_row;
    std::vector<std::string> m_map_index;
    db_type_t m_db_type;

    // sqlite
    char const * const *m_sqlite_result;
    // mysql
    char* m_mysql_result;
    char* m_row_result;

private:
    bool move_next_sqlite();
    bool move_next_mysql();

    void set_data_sqlite(char **result, int row, int column);
    void set_data_mysql(char **result, int row, int column);

    const char* get_item_sqlite(const int col_index) const;
    const char* get_item_mysql(const int col_index) const;
};
