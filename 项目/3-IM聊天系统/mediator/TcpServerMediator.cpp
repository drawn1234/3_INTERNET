#include"INetMediator.h"
#include"TcpServerMediator.h"
#include"../net/Tcpserver.h"
#include"../CKernel.h"
TcpServerMediator::TcpServerMediator() {
	m_pNet = new TcpServer(this);
}
TcpServerMediator::~TcpServerMediator() {
	if (m_pNet) {
		m_pNet->unInitNet();
		delete m_pNet;
		m_pNet = nullptr;
	}
}

//打开网络
bool TcpServerMediator::openNet() {
	return m_pNet->initNet();
}

//关闭网络
void TcpServerMediator::closeNet() {
	m_pNet->unInitNet();
}

//发送数据
bool TcpServerMediator::sendData(char* data, int len, unsigned long to) {
	return m_pNet->sendData(data, len, to);
}

//接收数据
void TcpServerMediator::transmitData(char* data, int len, unsigned long from) {
	//TODO：把数据传给核心处理类
	CKernel::pKernel->dealData(data, len, from);
}