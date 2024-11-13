#include"INetMediator.h"
#include"TcpServerMediator.h"
#include"../net/Tcpserver.h"
#include"../kernel/CKernel.h"
TcpServerMediator::TcpServerMediator() {}
TcpServerMediator::~TcpServerMediator() {}

//������
bool TcpServerMediator::openNet() {
	m_pNet = new TcpServer(this);
	return m_pNet->initNet();
}

//�ر�����
void TcpServerMediator::closeNet() {
	if (m_pNet) {
		m_pNet->unInitNet();
		delete m_pNet;
		m_pNet = nullptr;
	}
}

//��������
bool TcpServerMediator::sendData(char* data, int len, unsigned long to) {
	return m_pNet->sendData(data,len,to);
}

//��������
void TcpServerMediator::transmitData(char* data, int len, unsigned long from) {
	//TODO�������ݴ������Ĵ�����
	CKernel::pKernel->dealData(data,len,from);
}