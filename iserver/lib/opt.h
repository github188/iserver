
#pragma once

enum __protocol
{
    PROTOCOL_TCP,
    PROTOCOL_UDP,
};

enum __lt_ret
{
    LT_OK    = 0,
    LT_ERROR = -1,
};

typedef struct tagclient 
{
    SOCKET s;
    std::string ip;
    int remote_port;
    time_t last_beat;
}CLIENT_INFO;

typedef struct __head 
{
    int data_len;
    int data_type;

    __head()
    {
        data_len = 0;
        data_type = -1;
    }
}HEAD_DATA;


typedef void (__stdcall *LIB_DATA_CB)(const int handle, char *buff, int buff_len, char* reply_buff, int& reply_len, void *puser);

typedef void (__stdcall *LIB_MSG_CB)(int type, int msg_id, void *puser);

// max data size of a packet
const int send_size = 50 * 1024;
const int recv_size = 50 * 1024;

// time out
const int send_time_out = 5000;// ms
const int recv_time_out = 5000;// ms
const int default_select_time   = 3000;
const int connect_check_time    = 1000;
const int reconnect_time        = 5000;