#pragma once
#include "INet.h"

class TcpServer : public INet {
public:
	TcpServer(INetMediator* p);
	~TcpServer();
	//初始化网络
	bool initNet();

	//接收数据
	void recvData();

	//发送数据
	bool sendData(char* data, int len, unsigned long to);

	//关闭网络
	void unInitNet();

	//接收数据线程
	static unsigned __stdcall recvThread(void* lpvoid);

	//接受连接函数
	static unsigned __stdcall acceptThread(void* lpvoid);
private:
	SOCKET m_sock;
	list<HANDLE> m_listHandle;
	map<unsigned int, SOCKET> m_mapThreadToSocket;
};