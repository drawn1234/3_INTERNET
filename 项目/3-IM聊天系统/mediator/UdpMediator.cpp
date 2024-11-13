#include"INetMediator.h"
#include"UdpMediator.h"
#include"../Net/Udp.h"
UdpMediator::UdpMediator() {}
UdpMediator::~UdpMediator() {}

//������
bool UdpMediator::openNet() {
	//���캯����������ʵ�ֵ���
	m_pNet = new Udp(this);
	return m_pNet->initNet();
}

//�ر�����
void UdpMediator::closeNet() {
	//������Դ
	if (m_pNet) {
		m_pNet->unInitNet();
		delete m_pNet;
		m_pNet = nullptr;
	}
}

//��������
bool UdpMediator::sendData(char* data, int len, unsigned long to) {
	return m_pNet->sendData(data,len,to);
}

//ת�����ݣ���net������ݴ������Ĵ����ࣩ
//net���������Udpʹ��transmit��������Mediator
void UdpMediator::transmitData(char* data, int len, unsigned long from) {
	//TODO:�������Ĵ�����
	
	//���Դ��룺��ӡ���յ�������
	cout <<__func__<< ":" << data<<endl;

	
}