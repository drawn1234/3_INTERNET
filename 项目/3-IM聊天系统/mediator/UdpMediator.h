#pragma once
#include"INetMediator.h"
class Udp;
class UdpMediator :public INetMediator {
public:
	UdpMediator();
	~UdpMediator();

	//������
	bool openNet();

	//�ر�����
	void closeNet();

	//��������
	bool sendData(char* data, int len, unsigned long to);

	//��������
	void transmitData(char* data, int len, unsigned long from);

};