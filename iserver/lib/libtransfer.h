/**   @file	    libtransfer.h
 *    @note     HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    传送数据出去
 *
 *    @author   zyj
 *    @date     2014/02/19
 *
 *    @note     Description
 *    @note     历史记录：
 *    @note     V1.0.0
 *
 *    @warning 
 */

#ifdef LT_EXPORTS
#define LT_API EXTERN_C __declspec(dllexport)
#else
#define LT_API EXTERN_C __declspec(dllimport)
#endif
#include "opt.h"


// 初始化网络库
LT_API int lt_startup();

// 清理网络库
LT_API int lt_cleanup();

// 连接远程服务器（指定使用的协议）
LT_API int lt_client_connect(const char* ip, const int port, const __protocol protocol);

// 断开连接远程服务器
LT_API int lt_client_disconnect(const int handle);

LT_API int lt_client_clearconnect();

// 发送数据
// reply_data = null 则不返回
LT_API int lt_client_send(const int handle, const char* data, const int data_len, char* reply_data, int& reply_len);


//////////////////////////////////////////////////////////////////////////
// 监听端口
LT_API int lt_server_listen(const int port, LIB_DATA_CB cbf, const __protocol protocol, void* param);

// 关闭监听
LT_API int lt_server_shutdown(const int handle);

LT_API int lt_server_shutdownall();

// 发送数据(必须是已经连接的客户端)
LT_API int lt_server_reply(const int nHandle, const char* ip, const char* data, const int data_len);

LT_API int lt_setmsgcb(LIB_MSG_CB cbf, void* user_data);
