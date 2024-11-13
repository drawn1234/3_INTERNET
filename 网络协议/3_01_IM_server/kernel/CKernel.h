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
	//��������ָ�������ʼ����������
	void setProtocol();
	//�򿪷������������磬�������ݿ⣩
	bool startServer();
	//�رշ�������������Դ���ر����磬�Ͽ������ݿ�����ӣ�
	void endServer();
	//�������н��յ�������
	void dealData(char* data,int len,unsigned long from);
	static CKernel* pKernel;

	//����ע������
	void dealRegisterRq(char* data, int len, unsigned long from);

	//�����¼����
	void dealLoginRq(char* data, int len, unsigned long from);
private:
	INetMediator* m_pMediator;
	CMySql m_sql;
	//��������ָ������
    PFUN m_protocol[_DEF_PROTOCOL_COUNT];
};