// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� ISERVER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// ISERVER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef ISERVER_EXPORTS
#define ISERVER_API __declspec(dllexport)
#else
#define ISERVER_API __declspec(dllimport)
#endif

// �����Ǵ� iServer.dll ������
class ISERVER_API CiServer {
public:
	CiServer(void);
	// TODO: �ڴ�������ķ�����
};

extern ISERVER_API int niServer;

ISERVER_API int fniServer(void);
