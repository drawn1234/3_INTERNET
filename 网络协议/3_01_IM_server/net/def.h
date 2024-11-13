#pragma once
#include<string.h>
//定义加载的版本号
#define DEF_VERSION_HIGH (2)
#define DEF_VERSION_LOW (2)

//UDP的端口号
#define DEF_UDP_PORT (54321)

//TCP的端口号
#define DEF_TCP_PORT (12345)

//地址转换报错消除
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//昵称，手机号，密码的最大长度
#define _DEF_MAX_LENTH 15

//注册结果
#define _DEF_REGISTER_SUCCESS      (0)
#define _DEF_REGISTER_TEL_EXISTS   (1)
#define _DEF_REGISTER_NAME_EXISTS  (2)
#define _DEF_REGISTER_NAME_FAIL    (3)
//登录结果
#define _DEF_LOGIN_SUCCESS         (0)
#define _DEF_LOGIN_TEL_NOT_EXISTS  (1)
#define _DEF_LOGIN_PASSW_ERROR     (2)
#define _DEF_LOGIN_PASSW_FAIL      (3)

//添加好友结果
#define _DEF_ADD_FRIEND_SUCCESS     (0)
#define _DEF_ADD_FRIEND_OFFLINE     (1)
#define _DEF_ADD_FRIEND_REFUSE      (2)
#define _DEF_ADD_FRIEND_NOT_EXISTS  (3)
#define _DEF_ADD_FRIEND_FAIL        (4)

//发送结果
#define _DEF_SEND_SUCCESS           (0)
#define _DEF_SEND_FAIL              (1)




//聊天内容长度
#define _DEF_CONTENT_LENGTH (1024*8)

//结构体宏的个数
#define _DEF_PROTOCOL_COUNT (10)
//声明结构体类型变量名
typedef int packtype;
//声明结构体类型宏
#define _DEF_PROTOCOL_BASE  (1000)

//注册请求
#define _def_register_rq       (_DEF_PROTOCOL_BASE+1)
//注册回复
#define _def_register_rs      (_DEF_PROTOCOL_BASE+2)
//登录请求
#define _def_login_rq         (_DEF_PROTOCOL_BASE+3)
//登录回复
#define _def_login_rs         (_DEF_PROTOCOL_BASE+4)
//添加好友请求
#define _def_add_friend_rq    (_DEF_PROTOCOL_BASE+5)
//添加好友回复
#define _def_add_friend_rs    (_DEF_PROTOCOL_BASE+6)
//聊天请求
#define _def_chat_rq          (_DEF_PROTOCOL_BASE+7)
//聊天回复
#define _def_chat_rs          (_DEF_PROTOCOL_BASE+8)
//下线请求
#define _def_offline_rq       (_DEF_PROTOCOL_BASE+9)




//请求结构体
//注册功能：请求-回复
//注册请求:手机号，密码，昵称
typedef struct _STRU_REGISTER_RQ {
	//string 是一个对象，对象的大小是不固定的，需要使用char
	//比起char更容易出错
	//构造函数初始化
	_STRU_REGISTER_RQ():type(_def_register_rq) {
		memset(tel,0, _DEF_MAX_LENTH);//起始地址，值，大小
		memset(passw, 0, _DEF_MAX_LENTH);
		memset(name, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char tel[_DEF_MAX_LENTH];
	char passw[_DEF_MAX_LENTH];
	char name[_DEF_MAX_LENTH];
}_STRU_REGISTER_RQ;


//注册回复：结果（成功：电话号码/昵称被注册）
//让客户端知道，自己设置，将结果声明成宏,可以适用不同场景，语言
typedef struct _STRU_REGISTER_RS {
	_STRU_REGISTER_RS():result(_DEF_REGISTER_NAME_FAIL), type(_def_register_rs) {
	}
	packtype type;
	int result;
}_STRU_REGISTER_RS;

//登录请求：手机号，密码
typedef struct _STRU_LOGIN_RQ {
	_STRU_LOGIN_RQ() :type(_def_login_rq) {
		memset(tel,0, _DEF_MAX_LENTH);//起始地址，值，大小
		memset(passw, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char tel[_DEF_MAX_LENTH];
	char passw[_DEF_MAX_LENTH];
}_STRU_LOGIN_RQ;
//登录回复:结果（登录成功，电话未注册，密码错误）
typedef struct _STRU_LOGIN_RS {
	_STRU_LOGIN_RS():result(_DEF_LOGIN_PASSW_FAIL), type(_def_login_rs) {

	}
	packtype type;
	int result;
}_STRU_LOGIN_RS;
//添加好友：请求-回复
//添加好友请求：好友昵称，自己的id，自己的昵称
typedef struct _STRU_ADD_FRIEND_RQ {
	_STRU_ADD_FRIEND_RQ() :type(_def_add_friend_rq) {
		memset(friendName, 0, _DEF_MAX_LENTH);
		memset(myId, 0, _DEF_MAX_LENTH);
		memset(myName, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char friendName[_DEF_MAX_LENTH];
	char myId[_DEF_MAX_LENTH];
	char myName[_DEF_MAX_LENTH];
}_STRU_ADD_FRIEND_RQ;

//添加好友回复：结果（添加成功，用户不存在，好友已拒绝，好友不在线）
//自己的id，好友的id，自己的昵称，好友的昵称
typedef struct _STRU_ADD_FRIEND_RS {
	_STRU_ADD_FRIEND_RS():result(_DEF_ADD_FRIEND_FAIL), type(_def_add_friend_rs) {
		memset(friendId, 0, _DEF_MAX_LENTH);
		memset(friendName, 0, _DEF_MAX_LENTH);
		memset(myId, 0, _DEF_MAX_LENTH);
		memset(myName, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char myId[_DEF_MAX_LENTH];
	char myName[_DEF_MAX_LENTH];
	char friendId[_DEF_MAX_LENTH];
	char friendName[_DEF_MAX_LENTH];
	int result;
}_STRU_ADD_FRIEND_RS;

//聊天：请求-回复
//聊天请求：聊天内容，自己的id，好友的id，
typedef struct _STRU_CHAT_RQ {
	_STRU_CHAT_RQ() :type(_def_chat_rq) {
		memset(content, 0, _DEF_CONTENT_LENGTH);
		memset(myId, 0, _DEF_MAX_LENTH);
		memset(friendId, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char content[_DEF_CONTENT_LENGTH];
	char myId[_DEF_MAX_LENTH];
	char friendId[_DEF_MAX_LENTH];
}_STRU_CHAT_RQ;
//聊天回复（发送成功，失败)
typedef struct _STRU_CHAT_RS {
	_STRU_CHAT_RS() :result(_DEF_SEND_FAIL), type(_def_chat_rs) {
		
	}
	packtype type;
	int result;
}_STRU_CHAT_RS;
//下线请求:自己的id
typedef struct _STRU_OFFLINE_RQ {
	_STRU_OFFLINE_RQ() :type(_def_offline_rq) {
		memset(id, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char id[_DEF_MAX_LENTH];
}_STRU_OFFLINE_RQ;
