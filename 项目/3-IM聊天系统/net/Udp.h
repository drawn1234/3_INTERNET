#pragma once
#include "INet.h"
class Udp: public INet {
public:
	Udp(INetMediator* p);
	~Udp();
	//��ʼ������
	 bool initNet();

	//��������
	 void recvData();

	//��������
	 bool sendData(char* data, int len, unsigned long to) ;

	//�ر�����
	 void unInitNet();

	 //�������ݵ��̺߳���
	// static unsigned __stdcall recvThread(void* lpvoid);
private:
	HANDLE m_handle;//��ʼ������void*����ָ���κε�ַ
};