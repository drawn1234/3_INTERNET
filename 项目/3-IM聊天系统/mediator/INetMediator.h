#pragma once
//#include "./Net/INet.h"
class INet;

//路径问题-同项目的文件用相对路径寻找 ./返回上一级
class INetMediator {
public:
	INetMediator():m_pNet (nullptr){}
	virtual ~INetMediator(){}

	//打开网络
	//和初始化一样，是调用initNet函数
	virtual bool openNet() = 0;

	//关闭网络
	//调用unInitNet函数
	virtual void closeNet() = 0;

	//发送数据
	//data:发送数据的内容，len：发送的数据长度，to：发送的地址，发给谁，UDP是ip，TCP是socket
	virtual bool sendData(char* data, int len, unsigned long to) = 0;

	//转发数据（把Net层接收到的数据传给核心处理类）
	//1.什么数据
	//2.数据从哪儿来 UDP-ip TCP-socket
	virtual void transmitData(char* data, int len, unsigned long from) = 0;
protected:
	INet* m_pNet;
};