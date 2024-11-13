#pragma once
#include"net/def.h"
#include<iostream>
#include"mediator/INetMediator.h"
#include"MySQL/CMySql.h"
#include<map>
using namespace std;


//����ָ��������ָ��
class CKernel;
typedef void (CKernel::*PFUN)(char* data, int len, unsigned long from);

class CKernel {
public:
	CKernel();
	~CKernel();
	//������ָ�������ʼ�����Ҵ�ֵ
	void setProtocol();
	//�򿪷������������磬�������ݿ⣩
	bool startServer();
	//�رշ�������������Դ���ر����磬�Ͽ������ݿ�����ӣ�
	void endServer();


	//�������н��յ�������
	void dealData(char* data, int len, unsigned long from);

	//����ע������
	void dealRegisterRq(char* data, int len, unsigned long from);

	//�����¼����
	void dealLoginRq(char* data, int len, unsigned long from);

	//������������
	void dealChatRq(char* data, int len, unsigned long from);

	//������������
	void dealOfflineRq(char* data, int len, unsigned long from);

	//������Ӻ�������(A�ͻ���)
	void dealAddFriendRq(char* data, int len, unsigned long from);


	//������Ӻ��ѻظ�(B�ͻ���)
	void dealAddFriendRs(char* data, int len, unsigned long from);


	//��ȡ��ǰ��¼���û�����Ϣ�Լ����ѵ���Ϣ
	void getUserInfoAndFriendInfo(int id);

	//����id��ѯ�û���Ϣ
	void getInfoById(int id, _STRU_FRIEND_INFO* info);

	


	static CKernel* pKernel;
private:
	INetMediator* m_pMediator;
	CMySql m_sql;
	//��������ָ������
	PFUN m_protocol[_DEF_PROTOCOL_COUNT];
	//�����¼�ɹ����û���socket���û�����ʱҪɾ����Ӧ��socket��
	map<int, SOCKET> m_mapIdToSocket;
};