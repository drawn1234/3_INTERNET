#pragma once
#include "INet.h"

class TcpServer : public INet {
public:
	TcpServer(INetMediator* p);
	~TcpServer();
	//��ʼ������
	bool initNet();

	//��������
	void recvData();

	//��������
	bool sendData(char* data, int len, unsigned long to);

	//�ر�����
	void unInitNet();

	//���������߳�
	static unsigned __stdcall recvThread(void* lpvoid);

	//�������Ӻ���
	static unsigned __stdcall acceptThread(void* lpvoid);
private:
	SOCKET m_sock;
	list<HANDLE> m_listHandle;
	map<unsigned int, SOCKET> m_mapThreadToSocket;
};