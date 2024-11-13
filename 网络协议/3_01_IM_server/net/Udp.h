#pragma once
#include "INet.h"
class Udp: public INet {
public:
	Udp(INetMediator* p);
	~Udp();
	//初始化网络
	 bool initNet();

	//接收数据
	 void recvData();

	//发送数据
	 bool sendData(char* data, int len, unsigned long to) ;

	//关闭网络
	 void unInitNet();

	 //接收数据的线程函数
	// static unsigned __stdcall recvThread(void* lpvoid);
private:
	HANDLE m_handle;//初始化类型void*可以指向任何地址
};