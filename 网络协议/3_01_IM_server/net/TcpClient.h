#pragma once
#include "INet.h"

class TcpClient : public INet {
public:
	TcpClient(INetMediator* p);
	~TcpClient();
	//��ʼ������
	bool initNet();

	//��������
	void recvData();

	//��������
	bool sendData(char* data, int len, unsigned long to=0);

	//�ر�����
	void unInitNet();

	//���������߳�
	static unsigned __stdcall recvThread(void* lpvoid);
private:
	SOCKET m_sock;
	HANDLE m_handle;
	
};