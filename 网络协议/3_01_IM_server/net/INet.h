#pragma once
#include<WinSock2.h> //ϵͳ·��
#include<Windows.h>
#include<iostream>  
#include<process.h>
#include"def.h"  //��ǰ·��
#include<list>
#include<map>
//#include"../mediator/INetMediator.h"
#pragma comment(lib,"Ws2_32.lib")
using namespace std;
class INetMediator;
class INet {
public:
	INet() :m_isRunning(true) , m_pMediator(nullptr), m_sock(INVALID_SOCKET){}
	//���������� ����{}ʵ�� 
	//INet����û�д���ʵ�֣�ֱ����h�ļ���ʵ�ָ�����
	virtual~INet() {}//�����鹹��������

	//��ʼ������
	//���಻֪������������ʵ�֣�ʹ��TCP����UDP�����ಽʵ�֣�����
	virtual bool initNet() = 0;

	//��������
	//���������������ģ���֪��ʲôʱ����Խ��ܵ����ݣ�û�а취ͨ������ֵ֪��ʲôʱ����ܵ����ݡ�
	//���ú���-��ʼ�������ݡ�
	virtual void recvData() = 0;

	//��������
	//��Ҫʲô������
	//sendto(sock,buf,len,0,addr,size)
	//send(sock,buf,len,0)
	//��Ҫ���͵����ݣ���ַ-��ַ�ж˿ںŲ���仯 ֻ��IP��ַ���ϱ仯
	//ulong ip,SOCKET-uint sock
	//û��ulong��ͷ�ļ� unsigned long�������
	//�������ݣ�data:�������ݵ����ݣ�len�����͵����ݳ��ȣ�to�����͵ĵ�ַ������˭��UDP��ip��TCP��socket��
	virtual bool sendData(char* data,int len,unsigned long to) = 0;

	//�ر�����
	virtual void unInitNet()=0;
protected:
	BOOL m_isRunning;
	SOCKET m_sock;
	INetMediator* m_pMediator;
};
