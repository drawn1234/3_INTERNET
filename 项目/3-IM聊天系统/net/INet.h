#pragma once
#include<WinSock2.h> //系统路径
#include<Windows.h>
#include<iostream>  
#include<process.h>
#include"def.h"  //当前路径
#include<list>
#include<map>
//#include"../mediator/INetMediator.h"
#pragma comment(lib,"Ws2_32.lib")
using namespace std;
class INetMediator;
class INet {
public:
	INet() :m_isRunning(true), m_pMediator(nullptr), m_sock(INVALID_SOCKET) {}
	//（）；定义 （）{}实现 
	//INet函数没有代码实现，直接在h文件中实现更方便
	virtual~INet() {}//父类虚构是虚析构

	//初始化网络
	//父类不知道网络具体如何实现，使用TCP还是UDP，父类步实现，纯虚
	virtual bool initNet() = 0;

	//接收数据
	//接受数据是阻塞的，不知道什么时候可以接受到数据，没有办法通过返回值知道什么时候接受到数据。
	//调用函数-开始接收数据。
	virtual void recvData() = 0;

	//发送数据
	//需要什么参数？
	//sendto(sock,buf,len,0,addr,size)
	//send(sock,buf,len,0)
	//需要发送的数据，地址-地址中端口号不会变化 只有IP地址不断变化
	//ulong ip,SOCKET-uint sock
	//没有ulong的头文件 unsigned long可以替代
	//发送数据（data:发送数据的内容，len：发送的数据长度，to：发送的地址，发给谁，UDP是ip，TCP是socket）
	virtual bool sendData(char* data, int len, unsigned long to) = 0;

	//关闭网络
	virtual void unInitNet() = 0;
protected:
	BOOL m_isRunning;
	SOCKET m_sock;
	INetMediator* m_pMediator;
};
