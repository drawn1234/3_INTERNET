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

//登录结果
#define _DEF_LOGIN_SUCCESS         (0)
#define _DEF_LOGIN_TEL_NOT_EXISTS  (1)
#define _DEF_LOGIN_PASSW_ERROR     (2)


//添加好友结果
#define _DEF_ADD_FRIEND_SUCCESS     (0)
#define _DEF_ADD_FRIEND_OFFLINE     (1)
#define _DEF_ADD_FRIEND_REFUSE      (2)
#define _DEF_ADD_FRIEND_NOT_EXISTS  (3)


//发送结果
#define _DEF_SEND_SUCCESS           (0)
#define _DEF_SEND_FAIL              (1)

//用户状态
#define _DEF_STATUS_ONLINE          (0)
#define _DEF_STATUS_OFFLINE         (1)


//聊天内容长度
#define _DEF_CONTENT_LENGTH (1024*8)

//结构体宏的个数
#define _DEF_PROTOCOL_COUNT (10)
//声明结构体类型变量名
typedef int packtype;
//结构体类型宏
#define _DEF_PROTOCOL_BASE    (1000)
//注册请求
#define _def_register_rq      (_DEF_PROTOCOL_BASE+1)
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
//用户信息
#define _def_friend_info      (_DEF_PROTOCOL_BASE+10)



//请求结构体
//注册功能：请求-回复
//注册请求:手机号，密码，昵称
typedef struct _STRU_REGISTER_RQ {
	//string 是一个对象，对象的大小是不固定的，需要使用char
	//string比起char更容易出错
	//构造函数初始化
	_STRU_REGISTER_RQ() :type(_def_register_rq) {
		memset(tel, 0, _DEF_MAX_LENTH);//起始地址，值，大小
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
	_STRU_REGISTER_RS() :result(_DEF_REGISTER_NAME_EXISTS), type(_def_register_rs) {
	}
	packtype type;
	int result;
}_STRU_REGISTER_RS;

//登录请求：手机号，密码
typedef struct _STRU_LOGIN_RQ {
	_STRU_LOGIN_RQ() :type(_def_login_rq) {
		memset(tel, 0, _DEF_MAX_LENTH);//起始地址，值，大小
		memset(passw, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char tel[_DEF_MAX_LENTH];
	char passw[_DEF_MAX_LENTH];
}_STRU_LOGIN_RQ;

//登录回复:结果（登录成功，电话未注册，密码错误）
typedef struct _STRU_LOGIN_RS {
	_STRU_LOGIN_RS() :result(_DEF_LOGIN_PASSW_ERROR), type(_def_login_rs),userId(0) {

	}
	packtype type;
	int userId;
	int result;
}_STRU_LOGIN_RS;

//添加好友：请求-回复
//添加好友请求：好友昵称，自己的id，自己的昵称
typedef struct _STRU_ADD_FRIEND_RQ {
	_STRU_ADD_FRIEND_RQ() :type(_def_add_friend_rq) {
		//寄存器设置
		memset(friendName, 0, _DEF_MAX_LENTH);//目的空间起始地址，往空间写入的值，大小
		memset(myName, 0, _DEF_MAX_LENTH);
	}
	packtype type;
	char friendName[_DEF_MAX_LENTH];
	int myId;
	char myName[_DEF_MAX_LENTH];
}_STRU_ADD_FRIEND_RQ;

//添加好友回复：结果（添加成功，用户不存在，好友已拒绝，好友不在线）
//自己的id，好友的id，自己的昵称，好友的昵称
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

//聊天：请求-回复
//聊天请求：聊天内容，自己的id，好友的id，
typedef struct _STRU_CHAT_RQ {
	_STRU_CHAT_RQ() :type(_def_chat_rq) {
		memset(content, 0, _DEF_CONTENT_LENGTH);
	}
	packtype type;
	char content[_DEF_CONTENT_LENGTH];
	int myId;
	int friendId;
}_STRU_CHAT_RQ;

//聊天回复（发送成功，失败)
typedef struct _STRU_CHAT_RS {
	_STRU_CHAT_RS() :result(_DEF_SEND_FAIL), type(_def_chat_rs), friendId(0){

	}
	packtype type;
	int friendId;
	int result;
}_STRU_CHAT_RS;

//下线请求:自己的id
typedef struct _STRU_OFFLINE_RQ {
	_STRU_OFFLINE_RQ():id(0) , type(_def_offline_rq) {
	}
	packtype type;
	int id;
}_STRU_OFFLINE_RQ;

//用户信息：type，id，昵称，签名，头像id，状态
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
