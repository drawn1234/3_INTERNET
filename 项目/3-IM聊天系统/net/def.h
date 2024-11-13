#pragma once
#include<string.h>
//������صİ汾��
#define DEF_VERSION_HIGH (2)
#define DEF_VERSION_LOW (2)

//UDP�Ķ˿ں�
#define DEF_UDP_PORT (54321)

//TCP�Ķ˿ں�
#define DEF_TCP_PORT (12345)

//��ַת����������
#define _WINSOCK_DEPRECATED_NO_WARNINGS


//�ǳƣ��ֻ��ţ��������󳤶�
#define _DEF_MAX_LENTH 15

//ע����
#define _DEF_REGISTER_SUCCESS      (0)
#define _DEF_REGISTER_TEL_EXISTS   (1)
#define _DEF_REGISTER_NAME_EXISTS  (2)

//��¼���
#define _DEF_LOGIN_SUCCESS         (0)
#define _DEF_LOGIN_TEL_NOT_EXISTS  (1)
#define _DEF_LOGIN_PASSW_ERROR     (2)


//��Ӻ��ѽ��
#define _DEF_ADD_FRIEND_SUCCESS     (0)
#define _DEF_ADD_FRIEND_OFFLINE     (1)
#define _DEF_ADD_FRIEND_REFUSE      (2)
#define _DEF_ADD_FRIEND_NOT_EXISTS  (3)


//���ͽ��
#define _DEF_SEND_SUCCESS           (0)
#define _DEF_SEND_FAIL              (1)

//�û�״̬
#define _DEF_STATUS_ONLINE          (0)
#define _DEF_STATUS_OFFLINE         (1)


//�������ݳ���
#define _DEF_CONTENT_LENGTH (1024*8)

//�ṹ���ĸ���
#define _DEF_PROTOCOL_COUNT (10)
//�����ṹ�����ͱ�����
typedef int packtype;
//�ṹ�����ͺ�
#define _DEF_PROTOCOL_BASE    (1000)
//ע������
#define _def_register_rq      (_DEF_PROTOCOL_BASE+1)
//ע��ظ�
#define _def_register_rs      (_DEF_PROTOCOL_BASE+2)
//��¼����
#define _def_login_rq         (_DEF_PROTOCOL_BASE+3)
//��¼�ظ�
#define _def_login_rs         (_DEF_PROTOCOL_BASE+4)
//��Ӻ�������
#define _def_add_friend_rq    (_DEF_PROTOCOL_BASE+5)
//��Ӻ��ѻظ�
#define _def_add_friend_rs    (_DEF_PROTOCOL_BASE+6)
//��������
#define _def_chat_rq          (_DEF_PROTOCOL_BASE+7)
//����ظ�
#define _def_chat_rs          (_DEF_PROTOCOL_BASE+8)
//��������
#define _def_offline_rq       (_DEF_PROTOCOL_BASE+9)
//�û���Ϣ
#define _def_friend_info      (_DEF_PROTOCOL_BASE+10)



//����ṹ��
//ע�Ṧ�ܣ�����-�ظ�
//ע������:�ֻ��ţ����룬�ǳ�
typedef struct _STRU_REGISTER_RQ {
	//string ��һ�����󣬶���Ĵ�С�ǲ��̶��ģ���Ҫʹ��char
	//string����char�����׳���
	//���캯����ʼ��
	_STRU_REGISTER_RQ() :type(_def_register_rq) {
		memset(tel, 0, _DEF_MAX_LENTH);//��ʼ��ַ��ֵ����С
		memset(passw, 0, _DEF_MAX_LENTH);
		memset(name, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char tel[_DEF_MAX_LENTH];
	char passw[_DEF_MAX_LENTH];
	char name[_DEF_MAX_LENTH];
}_STRU_REGISTER_RQ;


//ע��ظ���������ɹ����绰����/�ǳƱ�ע�ᣩ
//�ÿͻ���֪�����Լ����ã�����������ɺ�,�������ò�ͬ����������
typedef struct _STRU_REGISTER_RS {
	_STRU_REGISTER_RS() :result(_DEF_REGISTER_NAME_EXISTS), type(_def_register_rs) {
	}
	packtype type;
	int result;
}_STRU_REGISTER_RS;

//��¼�����ֻ��ţ�����
typedef struct _STRU_LOGIN_RQ {
	_STRU_LOGIN_RQ() :type(_def_login_rq) {
		memset(tel, 0, _DEF_MAX_LENTH);//��ʼ��ַ��ֵ����С
		memset(passw, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char tel[_DEF_MAX_LENTH];
	char passw[_DEF_MAX_LENTH];
}_STRU_LOGIN_RQ;

//��¼�ظ�:�������¼�ɹ����绰δע�ᣬ�������
typedef struct _STRU_LOGIN_RS {
	_STRU_LOGIN_RS() :result(_DEF_LOGIN_PASSW_ERROR), type(_def_login_rs),userId(0) {

	}
	packtype type;
	int userId;
	int result;
}_STRU_LOGIN_RS;

//��Ӻ��ѣ�����-�ظ�
//��Ӻ������󣺺����ǳƣ��Լ���id���Լ����ǳ�
typedef struct _STRU_ADD_FRIEND_RQ {
	_STRU_ADD_FRIEND_RQ() :type(_def_add_friend_rq) {
		//�Ĵ�������
		memset(friendName, 0, _DEF_MAX_LENTH);//Ŀ�Ŀռ���ʼ��ַ�����ռ�д���ֵ����С
		memset(myName, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char friendName[_DEF_MAX_LENTH];
	int myId;
	char myName[_DEF_MAX_LENTH];
}_STRU_ADD_FRIEND_RQ;

//��Ӻ��ѻظ����������ӳɹ����û������ڣ������Ѿܾ������Ѳ����ߣ�
//�Լ���id�����ѵ�id���Լ����ǳƣ����ѵ��ǳ�
typedef struct _STRU_ADD_FRIEND_RS {
	_STRU_ADD_FRIEND_RS() :result(_DEF_ADD_FRIEND_NOT_EXISTS), type(_def_add_friend_rs) {
		memset(friendName, 0, _DEF_MAX_LENTH);
		memset(myName, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	int myId;
	char myName[_DEF_MAX_LENTH];
	int friendId;
	char friendName[_DEF_MAX_LENTH];
	int result;
}_STRU_ADD_FRIEND_RS;

//���죺����-�ظ�
//���������������ݣ��Լ���id�����ѵ�id��
typedef struct _STRU_CHAT_RQ {
	_STRU_CHAT_RQ() :type(_def_chat_rq) {
		memset(content, 0, _DEF_CONTENT_LENGTH);
	}
	packtype type;
	char content[_DEF_CONTENT_LENGTH];
	int myId;
	int friendId;
}_STRU_CHAT_RQ;

//����ظ������ͳɹ���ʧ��)
typedef struct _STRU_CHAT_RS {
	_STRU_CHAT_RS() :result(_DEF_SEND_FAIL), type(_def_chat_rs), friendId(0){

	}
	packtype type;
	int friendId;
	int result;
}_STRU_CHAT_RS;

//��������:�Լ���id
typedef struct _STRU_OFFLINE_RQ {
	_STRU_OFFLINE_RQ():id(0) , type(_def_offline_rq) {
	}
	packtype type;
	int id;
}_STRU_OFFLINE_RQ;

//�û���Ϣ��type��id���ǳƣ�ǩ����ͷ��id��״̬
typedef struct _STRU_FRIEND_INFO {
	_STRU_FRIEND_INFO() :type(_def_friend_info),id(0),iconId(0),status(_DEF_STATUS_OFFLINE) {
		memset(name, 0, _DEF_MAX_LENTH);
		memset(feeling, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	int id;
	int iconId;
	int status;
	char name[_DEF_MAX_LENTH];
	char feeling[_DEF_MAX_LENTH];
}_STRU_FRIEND_INFO;
