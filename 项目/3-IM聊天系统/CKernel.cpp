#include "CKernel.h"
#include"mediator/TcpServerMediator.h"
#include"net/def.h"
#include"MySQL/CMySql.h"
CKernel* CKernel::pKernel = nullptr;
CKernel::CKernel() {
	m_pMediator = new TcpServerMediator;
	pKernel = this;
	setProtocol();
}
CKernel::~CKernel() {
}

//给函数指针数组初始化并且存值
void CKernel::setProtocol() {
	cout << __func__ << endl;
	//初始化成0
	memset(m_protocol, 0, sizeof(m_protocol));
	//存入数据
	m_protocol[_def_register_rq    - _DEF_PROTOCOL_BASE-1] = &CKernel::dealRegisterRq;
	m_protocol[_def_login_rq       - _DEF_PROTOCOL_BASE-1] = &CKernel::dealLoginRq;
	m_protocol[_def_chat_rq        -_DEF_PROTOCOL_BASE - 1] = &CKernel::dealChatRq;
	m_protocol[_def_offline_rq     -_DEF_PROTOCOL_BASE - 1] = &CKernel::dealOfflineRq;
	m_protocol[_def_add_friend_rq  - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRq;
	m_protocol[_def_add_friend_rs  - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRs;

}

//打开服务器
bool CKernel::startServer() {
	//1.打开网络
	if (!m_pMediator->openNet()) {
		cout << "打开网络失败" << endl;
		return false;
	}
	//2.连接数据库
	char ip[] = "127.0.0.1";
	char user[] = "root";
	char pass[] = "026207";
	char db[] = "colin0318im";
	if (!m_sql.ConnectMySql(ip, user, pass, db)) {
		cout << "连接数据库失败" << endl;
		return false;
	}
	return true;
}
void CKernel::endServer() {
	if (m_pMediator) {
		m_pMediator->closeNet();
		delete m_pMediator;
		m_pMediator = nullptr;
	}
}
//处理所有接收到的数据
void CKernel::dealData(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	//取出协议头
	packtype type=*(packtype*)data;
	
	//根据协议头判断是哪个结构体，调用不同的处理函数
	//计算数组下标
	int index = type - _DEF_PROTOCOL_BASE - 1;
	//判断下标是否在有效的范围内
	if (index >= 0 && index < _DEF_PROTOCOL_COUNT) {
		//根据数组下标，取出函数地址
		PFUN pf = m_protocol[index];
		if (pf) {
			//调用具体的处理函数
			(this->*pf)(data, len, from);
		}
		else {
			//打印2:1.定义结构体的时候，type值错了；2.对端发送的结构体不对
			cout << "type2:" << type << endl;
		}
	}
	else {
		//打印1：1.声明结构体的时候，packtype没有放在结构体的最前面，也就是没有第一个声明；
		//2.接收数据的时候offset没有清零-TcpServer
		cout << "type1:" << type << endl;
	}
	//回收空间
	delete[] data;
}

//处理注册请求
void CKernel::dealRegisterRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	_STRU_REGISTER_RQ* rq = (_STRU_REGISTER_RQ*)data;
	//1. 拆包

    //2. 校验电话号码是否被注册
	//从数据库中，根据电话号码查询电话号码
	list<string> lstStr;
	char sql[1024] = "";
	sprintf_s(sql, "select tel from t_user where tel='%s';", rq->tel);
	if (!m_sql.SelectMySql(sql/*要执行的sql语句*/,
		1/*sql语句中查询的列的个数*/, 
		lstStr/*查询到的结果*/)) {
		//两个原因：1.连接数据库失败；
		//2.sql语句有语法错误，或者列名和表对应不上（把日志中打印的sql语句复制到worbunch中运行）

		cout << "查询数据库失败"<<sql <<endl;
		return;
	}
    //判断查询结果是否为空
	if (0 != lstStr.size()) {
		//查询结果不为空，说明电话号码被注册过，注册失败
		_STRU_REGISTER_RS rs;
		rs.result = _DEF_REGISTER_TEL_EXISTS;
		//给客户端回复注册结果
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
		//结果
		return;
	}
	
	//3. 校验昵称是否被注册
	//根据昵称查询昵称
	sprintf_s(sql, "select name from t_user where name='%s';", rq->name);
	if (!m_sql.SelectMySql(sql/*要执行的sql语句*/,
		1/*sql语句中查询的列的个数*/,
		lstStr/*查询到的结果*/)) {
		//两个原因：1.连接数据库失败；
		//2.sql语句有语法错误，或者列名和表对应不上（把日志中打印的sql语句复制到worbunch中运行）

		cout << "查询数据库失败" << sql << endl;
		return;
	}
	if (0 != lstStr.size()) {
		//查询结果不为控，昵称被注册过，注册失败_def_regigster_name_exists
		_STRU_REGISTER_RS rs;
		rs.result = _DEF_REGISTER_NAME_EXISTS;
		//给客户端回复注册结果
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
		//结束
		return;
	}
	

	//4. 把数据插入数据库
	sprintf_s(sql, "insert into t_user(name,tel,passw,feeling,iconid) values ('%s','%s','%s','哈哈哈哈',8);"
		, rq->name, rq->tel, rq->passw);
	if (!m_sql.UpdateMySql(sql)) {
		//两个原因：1.连接数据库失败；
		//2.sql语句有语法错误，或者列名和表对应不上（把日志中打印的sql语句复制到worbunch中运行）

		cout << "更新数据库失败" << sql << endl;
	}
	//5.注册成功
	_STRU_REGISTER_RS rs;
	rs.result = _DEF_REGISTER_SUCCESS;
	//6.把注册结果告诉客户端
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);
}


//获取当前登录的用户的信息以及好友的信息
void CKernel::getUserInfoAndFriendInfo(int id) {

	cout << __func__ << endl;
	//根据自己的id查询自己的信息
	_STRU_FRIEND_INFO userInfo;
	getInfoById(id, &userInfo);
	//把自己的信息发给客户端
	if (m_mapIdToSocket.count(id)>0) {
		m_pMediator->sendData((char*)&userInfo, sizeof(userInfo),m_mapIdToSocket[id]);
	}
	else {
		cout << "m_mapIdToSocket中没有id：" << id << endl;
		return;
	}
	//根据自己的id查询好友的id列表
	list<string> lstStr;
	char sql[1024] = "";
	sprintf_s(sql, "select idb from t_friend where ida=%d;", id);
	if (!m_sql.SelectMySql(sql, 1, lstStr)) {
		cout << "查询数据库失败:"<<sql << endl;
		return;
	}
	//遍历好友的id列表
	int friendId = 0;
	_STRU_FRIEND_INFO friendInfo;
	while (lstStr.size()>0) {
		//取出每个好友的id
		friendId = stoi(lstStr.front());
		lstStr.pop_front();
		//根据好友的id查询好友的信息
		getInfoById(friendId, &friendInfo);
		//把好友的信息发回给客户端
		if (m_mapIdToSocket.count(id) > 0) {
			m_pMediator->sendData((char*)&friendInfo, sizeof(friendInfo), m_mapIdToSocket[id]);
		}
		else {
			cout << "m_mapIdToSocket中没有:" << id << endl;
			return;
		}

		//从数据库查询好友Id列表
		//遍历好友id列表
		//判断好友是否在线，如果在线就通知好友自己上线了
		if (m_mapIdToSocket.count(friendId) > 0) {
			//如果在线，就通知好友自己上线了
			m_pMediator->sendData((char*)&userInfo, sizeof(userInfo), m_mapIdToSocket[friendId]);
		}
	}
}

//根据id查询用户信息
void CKernel::getInfoById(int id, _STRU_FRIEND_INFO* info/*查到的用户信息作为参数返回给调用函数*/) {
	cout << __func__ << endl;
	info->id = id;
	//查询好友是否在线
	if (m_mapIdToSocket.count(id) > 0) {
		//在线
		info->status = _DEF_STATUS_ONLINE;
	}else{
		info->status = _DEF_STATUS_OFFLINE;
	}

	//从数据库中查询昵称，签名，头像id
	list<string> lstStr;
	char sql[1024] = "";
	sprintf_s(sql,"select name,feeling,iconid from t_user where id= %d", id);
	if (!m_sql.SelectMySql(sql,3,lstStr)) {
		cout << "查询数据库失败：" << sql << endl;
		return;
	}
	
	//如果取出的数据个数正确
	if (3 == lstStr.size()) {
		//取出昵称
		strcpy_s(info->name,sizeof(info->name),lstStr.front().c_str());
		lstStr.pop_front();
		//取出签名
		strcpy_s(info->feeling, sizeof(info->feeling), lstStr.front().c_str());
		lstStr.pop_front();
		//取出头像id
		info->iconId = stoi(lstStr.front());
		lstStr.pop_front();
	}
	else {
		cout << "查询昵称，签名，头像id错误：" << sql << endl;
	}

}

//处理登录请求
void CKernel::dealLoginRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	//1.拆包
	_STRU_LOGIN_RQ* rq = (_STRU_LOGIN_RQ*)data;
	//2.从数据库查询，根据电话号码查询密码
	list<string> lstStr;
	char sql[1024] = "";
	sprintf_s(sql, "select passw,id from t_user where tel='%s';", rq->tel);
	if (!m_sql.SelectMySql(sql/*要执行的sql语句*/,
		2/*sql语句中查询的列的个数*/,
		lstStr/*查询到的结果*/)) {
		cout << "查询数据库失败" << sql << endl;
		return;
	}
	//3.判断查询结果是否为空
	_STRU_LOGIN_RS rs;
	if (0 ==lstStr.size() ) {
		//4.查询结果为空，说明电话号码未注册，登录失败
		rs.result = _DEF_LOGIN_TEL_NOT_EXISTS;
	}
	else {
		//5.比较查询到的密码和输入的密码
		string pass = lstStr.front();
		lstStr.pop_front();

		//保存查询到的用户id用来查询好友信息
		int userId = stoi(lstStr.front());
		lstStr.pop_front();

		if (pass==rq->passw) {
			//相等，就是登录成功
			rs.result = _DEF_LOGIN_SUCCESS;
			rs.userId = userId;

			//从数据库查询好友Id列表
			//遍历好友id列表
			//判断好友是否在线，如果在线就通知好友自己上线了


			//保存当前用户的socket
			m_mapIdToSocket[userId] = from;
			//把结果发送给客户端
			m_pMediator->sendData((char*)&rs, sizeof(rs), from);
			//获取当前用户的信息以及好友信息
			getUserInfoAndFriendInfo(userId);
			return;
		}
		else {
			//不相等，就是登录失败，密码错误
			rs.result = _DEF_LOGIN_PASSW_ERROR;
		}
	}
	//6.把结果发送给客户端
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);

}


//处理聊天请求
void CKernel::dealChatRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	//1.拆包
	_STRU_CHAT_RQ* rq = (_STRU_CHAT_RQ*)data;

	//2.判断好友是否在线
	if (m_mapIdToSocket.count(rq->friendId) > 0) {
		//如果好友在线，把聊天请求转发给好友
		m_pMediator->sendData(data, len, m_mapIdToSocket[rq->friendId]);
	}
	else {
		//如果好友不在线，给客户端回复发送失败
		//正常软件，把不在线好友的信息存入数据库，等好友上线
		//把数据库中内容转发给好友，把数据库中保存的信息删掉
		_STRU_CHAT_RS rs;
		rs.friendId = rq->friendId;
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
	}
}

//处理下线请求
void CKernel::dealOfflineRq(char* data, int len, unsigned long from){
	cout << __func__ << endl;
	//1.拆包
	_STRU_OFFLINE_RQ* rq = (_STRU_OFFLINE_RQ*)data;
	//2.查询好友的id列表
	list<string> lstStr;
	char sql[1025] = "";
	sprintf_s(sql, "select idb from t_friend where ida =%d;", rq->id);
	if (!m_sql.SelectMySql(sql, 1, lstStr)) {
		cout << "查询数据库失败:" << sql << endl;
	}
	//3.遍历好友的id列表
	int friendId = 0;
	while (lstStr.size() > 0) {
		//4.取出好友的id
		friendId = stoi(lstStr.front());
		lstStr.pop_front();
		//5.判断好友是否在线，在线就转发下线请求给好友
		if (m_mapIdToSocket.count(friendId) > 0) {
			m_pMediator->sendData(data, len, m_mapIdToSocket[friendId]);
		}
	}
	//6.关闭下线用户的socket，并且回收map的空间
	auto ite = m_mapIdToSocket.find(rq->id);
	if (ite != m_mapIdToSocket.end()) {
		closesocket(ite->second);
		m_mapIdToSocket.erase(ite);
	}

}

//处理添加好友请求
void CKernel::dealAddFriendRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	//1.拆包
	_STRU_ADD_FRIEND_RQ* rq = (_STRU_ADD_FRIEND_RQ*)data;

	//2.根据好友昵称查询好友id
	list<string> lstStr;
	char sql[2024] = "";
	sprintf_s(sql, "select id from t_user where name='%s'", rq->friendName);
	if (!m_sql.SelectMySql(sql,1, lstStr)) {
		cout << "查询数据库失败：" << sql << endl;
		return;
	}
	_STRU_ADD_FRIEND_RS rs;
	
	//3.检查查询结果是否为空
	if (0 == lstStr.size()) {
		//4.查询结果为空，说明昵称不存在，添加失败
		rs.result = _DEF_ADD_FRIEND_NOT_EXISTS;
		strcpy_s(rs.friendName, sizeof(rs.friendName), rq->friendName);
		//把添加结果发回给客户端A
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);	
	}
	else {
		//取出好友的id
		int friendId = stoi(lstStr.front());
		lstStr.pop_front();
		//5.判断好友是否在线
		if (m_mapIdToSocket.count(friendId)> 0) {
			//6.好友在线，把添加好友请求转发给好友
			m_pMediator->sendData(data, len, m_mapIdToSocket[friendId]);
		}
		else {
			//7.好友不在线，添加失败
			rs.result = _DEF_ADD_FRIEND_OFFLINE;
			strcpy_s(rs.friendName, sizeof(rs.friendName), rq->friendName);
			//把结果发回给客户端
			m_pMediator->sendData((char*)&rs, sizeof(rs), from);
		}
	}
}

//处理添加好友回复(B客户端)
void CKernel::dealAddFriendRs(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	//1.拆包
	_STRU_ADD_FRIEND_RS* rs = (_STRU_ADD_FRIEND_RS*)data;


	//2.判断结果是否同意
	if (_DEF_ADD_FRIEND_SUCCESS == rs->result) {
		//3.如果同意，就把结果写进数据库中
		char sql[1024] = "";
		sprintf_s(sql, "insert into t_friend values (%d,%d);", rs->friendId, rs->myId);
		if (!m_sql.UpdateMySql(sql)) {
			cout << "更新数据库失败：" << sql << endl;
		}

		sprintf_s(sql, "insert into t_friend values (%d,%d);", rs->myId, rs->friendId);
		if (!m_sql.UpdateMySql(sql)) {
			cout << "更新数据库失败：" << sql << endl;
		}

		//4.更新双端好友列表
		getUserInfoAndFriendInfo(rs->friendId);
	
		
	}
	//5.不管结果如何，都要将结果发给A客户端
	if (m_mapIdToSocket.count(rs->myId) > 0) {
		m_pMediator->sendData((char*)rs, len, m_mapIdToSocket[rs->myId]);
	}
}
