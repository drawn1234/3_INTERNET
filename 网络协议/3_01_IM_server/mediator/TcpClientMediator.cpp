#include"INetMediator.h"
#include"TcpClientMediator.h"
#include"../net/TcpClient.h"
TcpClientMediator::TcpClientMediator() { }
TcpClientMediator::~TcpClientMediator() {}

//������
bool TcpClientMediator::openNet() {
	m_pNet = new TcpClient(this);
	return m_pNet->initNet();
}

//�ر�����
void TcpClientMediator::closeNet() {
	if (m_pNet) {
		m_pNet->unInitNet();
		delete m_pNet;
		m_pNet=nullptr;
	}
}

//��������
bool TcpClientMediator::sendData(char* data, int len, unsigned long to) {
	return m_pNet->sendData(data,len,to);
}

//ת������
void TcpClientMediator::transmitData(char* data, int len, unsigned long from) {
	cout << "server say:"<<data <<"   len:"<<len << endl;
	
}