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

//������
bool TcpServerMediator::openNet() {
	return m_pNet->initNet();
}

//�ر�����
void TcpServerMediator::closeNet() {
	m_pNet->unInitNet();
}

//��������
bool TcpServerMediator::sendData(char* data, int len, unsigned long to) {
	return m_pNet->sendData(data, len, to);
}

//��������
void TcpServerMediator::transmitData(char* data, int len, unsigned long from) {
	//TODO�������ݴ������Ĵ�����
	CKernel::pKernel->dealData(data, len, from);
}