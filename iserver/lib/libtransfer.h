/**   @file	    libtransfer.h
 *    @note     HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    �������ݳ�ȥ
 *
 *    @author   zyj
 *    @date     2014/02/19
 *
 *    @note     Description
 *    @note     ��ʷ��¼��
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


// ��ʼ�������
LT_API int lt_startup();

// ���������
LT_API int lt_cleanup();

// ����Զ�̷�������ָ��ʹ�õ�Э�飩
LT_API int lt_client_connect(const char* ip, const int port, const __protocol protocol);

// �Ͽ�����Զ�̷�����
LT_API int lt_client_disconnect(const int handle);

LT_API int lt_client_clearconnect();

// ��������
// reply_data = null �򲻷���
LT_API int lt_client_send(const int handle, const char* data, const int data_len, char* reply_data, int& reply_len);


//////////////////////////////////////////////////////////////////////////
// �����˿�
LT_API int lt_server_listen(const int port, LIB_DATA_CB cbf, const __protocol protocol, void* param);

// �رռ���
LT_API int lt_server_shutdown(const int handle);

LT_API int lt_server_shutdownall();

// ��������(�������Ѿ����ӵĿͻ���)
LT_API int lt_server_reply(const int nHandle, const char* ip, const char* data, const int data_len);

LT_API int lt_setmsgcb(LIB_MSG_CB cbf, void* user_data);
