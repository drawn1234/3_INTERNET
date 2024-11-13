#include"INetMediator.h"
#include"TcpServerMediator.h"
#include"../net/Tcpserver.h"
#include"../kernel/CKernel.h"
TcpServerMediator::TcpServerMediator() {}
TcpServerMediator::~TcpServerMediator() {}

//打开网络
bool TcpServerMediator::openNet() {
	m_pNet = new TcpServer(this);
	return m_pNet->initNet();
}

//关闭网络
void TcpServerMediator::closeNet() {
	if (m_pNet) {
		m_pNet->unInitNet();
		delete m_pNet;
		m_pNet = nullptr;
	}
}

//发送数据
bool TcpServerMediator::sendData(char* data, int len, unsigned long to) {
	return m_pNet->sendData(data,len,to);
}

//接收数据
void TcpServerMediator::transmitData(char* data, int len, unsigned long from) {
	//TODO：把数据传给核心处理类
	CKernel::pKernel->dealData(data,len,from);
}