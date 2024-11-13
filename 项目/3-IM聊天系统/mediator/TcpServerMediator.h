#pragma once
#include"INetMediator.h"

class TcpServerMediator :public INetMediator {
public:
	TcpServerMediator();
	~TcpServerMediator();

	//打开网络
	bool openNet();

	//关闭网络
	void closeNet();

	//发送数据
	bool sendData(char* data, int len, unsigned long to);

	//接收数据
	void transmitData(char* data, int len, unsigned long from);

};