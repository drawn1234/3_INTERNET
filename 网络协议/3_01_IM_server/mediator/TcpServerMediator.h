#pragma once
#include"INetMediator.h"

class TcpServerMediator :public INetMediator {
public:
	TcpServerMediator();
	~TcpServerMediator();

	//������
	bool openNet();

	//�ر�����
	void closeNet();

	//��������
	bool sendData(char* data, int len, unsigned long to);

	//��������
	void transmitData(char* data, int len, unsigned long from);

};