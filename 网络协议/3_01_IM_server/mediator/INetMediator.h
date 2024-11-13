#pragma once
//#include "./Net/INet.h"
class INet;

//·������-ͬ��Ŀ���ļ������·��Ѱ�� ./������һ��
class INetMediator {
public:
	INetMediator():m_pNet (nullptr){}
	virtual ~INetMediator(){}

	//������
	//�ͳ�ʼ��һ�����ǵ���initNet����
	virtual bool openNet() = 0;

	//�ر�����
	//����unInitNet����
	virtual void closeNet() = 0;

	//��������
	//data:�������ݵ����ݣ�len�����͵����ݳ��ȣ�to�����͵ĵ�ַ������˭��UDP��ip��TCP��socket
	virtual bool sendData(char* data, int len, unsigned long to) = 0;

	//ת�����ݣ���Net����յ������ݴ������Ĵ����ࣩ
	//1.ʲô����
	//2.���ݴ��Ķ��� UDP-ip TCP-socket
	virtual void transmitData(char* data, int len, unsigned long from) = 0;
protected:
	INet* m_pNet;
};