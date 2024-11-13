#include"INetMediator.h"
#include"TcpClientMediator.h"
#include"../net/TcpClient.h"
TcpClientMediator::TcpClientMediator() { }
TcpClientMediator::~TcpClientMediator() {}

//打开网络
bool TcpClientMediator::openNet() {
	m_pNet = new TcpClient(this);
	return m_pNet->initNet();
}

//关闭网络
void TcpClientMediator::closeNet() {
	if (m_pNet) {
		m_pNet->unInitNet();
		delete m_pNet;
		m_pNet=nullptr;
	}
}

//发送数据
bool TcpClientMediator::sendData(char* data, int len, unsigned long to) {
	return m_pNet->sendData(data,len,to);
}

//转发数据
void TcpClientMediator::transmitData(char* data, int len, unsigned long from) {
	cout << "server say:"<<data <<"   len:"<<len << endl;
	
}