#pragma once
#include"../net/def.h"
#include"../MySQL/CMySql.h"
#include<iostream>
#include"../mediator/INetMediator.h"
#include"../MySQL/CMySql.h"
using namespace std;

class CKernel {
public:
	CKernel();
	~CKernel();
	//给明函数指针数组初始化并存数据
	void setProtocol();
	//打开服务器（打开网络，连接数据库）
	bool startServer();
	//关闭服务器（回收资源，关闭网络，断开跟数据库的连接）
	void endServer();
	//处理所有接收到的数据
	void dealData(char* data,int len,unsigned long from);
	static CKernel* pKernel;

	//处理注册请求
	void dealRegisterRq(char* data, int len, unsigned long from);

	//处理登录请求
	void dealLoginRq(char* data, int len, unsigned long from);
private:
	INetMediator* m_pMediator;
	CMySql m_sql;
	//声明函数指针数组
    PFUN m_protocol[_DEF_PROTOCOL_COUNT];
};