#include"INetMediator.h"
#include"UdpMediator.h"
#include"../Net/Udp.h"
UdpMediator::UdpMediator() {}
UdpMediator::~UdpMediator() {}

//打开网络
bool UdpMediator::openNet() {
	//构造函数创建对象，实现调用
	m_pNet = new Udp(this);
	return m_pNet->initNet();
}

//关闭网络
void UdpMediator::closeNet() {
	//回收资源
	if (m_pNet) {
		m_pNet->unInitNet();
		delete m_pNet;
		m_pNet = nullptr;
	}
}

//发送数据
bool UdpMediator::sendData(char* data, int len, unsigned long to) {
	return m_pNet->sendData(data,len,to);
}

//转发数据（把net层的数据传给核心处理类）
//net层的数据由Udp使用transmit函数传给Mediator
void UdpMediator::transmitData(char* data, int len, unsigned long from) {
	//TODO:传给核心处理类
	
	//测试代码：打印接收到的数据
	cout <<__func__<< ":" << data<<endl;

	
}