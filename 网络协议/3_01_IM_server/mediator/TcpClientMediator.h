#pragma once
#include"INetMediator.h"

class TcpClientMediator :public INetMediator {
public:
	TcpClientMediator();
	~TcpClientMediator();

	//������
	 bool openNet() ;

	//�ر�����
	 void closeNet() ;

	//��������
	 bool sendData(char* data, int len, unsigned long to) ;

	//��������
	 void transmitData(char* data, int len, unsigned long from) ;

};