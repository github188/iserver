#include "StdAfx.h"
#include "SqlRecordset.h"
#include <sqlite3.h>
#include <mysql.h>

CSQLRecordset::CSQLRecordset(const db_type_t& db_type)
: m_row(0)
, m_column(0)
, m_current_row(0)
, m_db_type(db_type)
, m_sqlite_result(NULL)
, m_mysql_result(NULL)
, m_row_result(NULL)
{
}

CSQLRecordset::~CSQLRecordset(void)
{
   if (m_sqlite_result)
   {
       sqlite3_free_table((char**)m_sqlite_result);
   }
   if (m_mysql_result)
   {
       mysql_free_result((MYSQL_RES*)m_mysql_result);
   }
    
}

__int32 CSQLRecordset::get_int32_item(const char *col_name, __int32 const default_value) const
{
    const char* pvalue = get_item(col_name);
    if (NULL == pvalue)
    {
        return default_value;
    }

    return atoi(pvalue);
}

__int64 CSQLRecordset::get_int64_item(const char *col_name, __int64 const default_value) const
{
    const char* pvalue = get_item(col_name);
    if (NULL == pvalue)
    {
        return default_value;
    }

    return _atoi64(pvalue);
}

const char* CSQLRecordset::get_string_item(const char *col_name, char * const default_value) const
{
    const char* pvalue = get_item(col_name);
    if (NULL == pvalue)
    {
        return default_value;
    }

    return pvalue;
}

const char* CSQLRecordset::get_item(const char *col_name) const
{
    assert(m_current_row > 0);
    assert(m_current_row <= m_row);
    for (std::vector<std::string>::size_type i = 0; i < m_map_index.size(); ++i)
    {
        if (0 == strcmp(col_name, m_map_index[i].c_str()))
        {
            return get_item_(i);
        }
    }

    throw "col_name not found!";
}

const char* CSQLRecordset::get_item(const int col_index) const
{
    assert(m_current_row > 0);
    assert(m_current_row <= m_row);
    if (col_index < 0 || col_index >= m_column)
    {
        throw "col_index out of range!";
    }

    return get_item_(col_index);
}


bool CSQLRecordset::move_next()
{
    if (m_db_type == sqlite_type)
    {
        return move_next_sqlite();
    }
    else if (m_db_type == mysql_type)
    {
        return move_next_mysql();
    }
    else
    {
        return false;
    }
}

void CSQLRecordset::set_data(char **result, int row, int column)
{
    if (m_db_type == sqlite_type)
    {
        return set_data_sqlite(result, row, column);
    }
    else if (m_db_type == mysql_type)
    {
        return set_data_mysql(result, row, column);
    }
    else
    {

    }
}

const char* CSQLRecordset::get_item_(const int col_index) const
{
    if (m_db_type == sqlite_type)
    {
        return get_item_sqlite(col_index);
    }
    else if (m_db_type == mysql_type)
    {
        return get_item_mysql(col_index);
    }
    else
    {
        return NULL;
    }
}

bool CSQLRecordset::move_next_sqlite()
{
    if (is_eof())
    {
        return false;
    }
    ++m_current_row;

    return true;
}

void CSQLRecordset::set_data_sqlite(char **result, int row, int column)
{
    assert(result);
    assert(row >= 0);
    assert(column >= 0);

    if (NULL == result)
    {
        return;
    }

    m_sqlite_result = result;
    m_row = row;
    m_column = column;
    m_current_row = 1;

    m_map_index.clear();
    for (int i = 0; i < column; ++i)
    {
        m_map_index.push_back((const char*)result[i]);
    }
}

const char* CSQLRecordset::get_item_sqlite(const int col_index) const
{
    assert(m_sqlite_result != NULL);
    return m_sqlite_result[m_current_row * m_column + col_index];
}

bool CSQLRecordset::move_next_mysql()
{
    assert(NULL != m_mysql_result);
    if (m_current_row > m_row)
    {
        return false;
    }
    m_row_result = (char*)mysql_fetch_row((MYSQL_RES*)m_mysql_result);
    if (NULL == m_row)
    {
        return false;
    }
    ++m_current_row;

    return true;
}

void CSQLRecordset::set_data_mysql(char **result, int row, int column)
{
    assert(result);
    assert(row >= 0);
    assert(column >= 0);

    if (NULL != m_mysql_result)
    {
        mysql_free_result((MYSQL_RES*)m_mysql_result);
        m_mysql_result = NULL;
    }
    assert(NULL == m_mysql_result);

    m_mysql_result = (char*)result;
    // 在某些情况下m_pResult会为NULL，如内存不足，conn突然断了
    assert(NULL != m_mysql_result);

    m_row = row;
    m_column = column;
    m_current_row = 1;

    m_row_result = (char*)mysql_fetch_row((MYSQL_RES*)m_mysql_result);
    m_map_index.clear();
    MYSQL_FIELD *field = NULL;
    while(NULL != (field = mysql_fetch_field((MYSQL_RES*)m_mysql_result)))
    {
        m_map_index.push_back((const char*)field->name);
    }
    assert(m_map_index.size() == static_cast<size_t>(m_column));
}

const char* CSQLRecordset::get_item_mysql(const int col_index) const
{
    assert(m_row_result);
    if (NULL == m_row_result)
    {
        return NULL;
    }
    return ((MYSQL_ROW)m_row_result)[col_index];
}

const char* CSQLRecordset::get_columu_name(const int column_index)
{
    if (column_index >= (int)m_map_index.size())
    {
        return NULL;
    }
    return m_map_index[column_index].c_str();
}