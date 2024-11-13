#pragma once
#include "INet.h"

class TcpClient : public INet {
public:
	TcpClient(INetMediator* p);
	~TcpClient();
	//初始化网络
	bool initNet();

	//接收数据
	void recvData();

	//发送数据
	bool sendData(char* data, int len, unsigned long to=0);

	//关闭网络
	void unInitNet();

	//接收数据线程
	static unsigned __stdcall recvThread(void* lpvoid);
private:
	SOCKET m_sock;
	HANDLE m_handle;
	
};