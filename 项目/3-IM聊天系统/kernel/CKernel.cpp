#include "CKernel.h"
#include"../mediator/TcpServerMediator.h"
CKernel* CKernel:: pKernel = nullptr;
CKernel::CKernel() {
	m_pMediator = new TcpServerMediator;
	pKernel = this;
	setProtocol();
}
CKernel::~CKernel() {
}
//给明函数指针数组初始化并存数据
void CKernel::setProtocol() {
	cout << __func__ << endl;
	//初始化成0
	memset(m_protocol, 0, sizeof(m_protocol));
	//存入数据
	m_protocol[_def_register_rq - _DEF_PROTOCOL_BASE] = &CKernel::dealRegisterRq;
	m_protocol[_def_login_rq - _DEF_PROTOCOL_BASE] = &CKernel::dealLoginRq;

}

//打开服务器（打开网络，连接数据库）
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
	//cout << __func__ << ":" << data << "   len:" << len << endl;
	//char s[] = "sgjhgddvgfygaja1db";
	//m_pMediator-> sendData(s, sizeof(s), from);
	cout << __func__ << endl;
	//取出协议头
	packtype type = *(packtype*)data;

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
			//打印
			cout << "type2:" << type << endl;
		}
	}
	else {
		cout << "type1:" << type << endl;
	}
	//回收空间
	delete[] data;
}


//处理注册请求
void CKernel::dealRegisterRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	_STRU_REGISTER_RQ* rq = (_STRU_REGISTER_RQ*)data;
	cout << "name:" << rq->name << ",pass:" << rq->passw << ",tel:" << rq->tel << endl;

	_STRU_REGISTER_RS* rs = (_STRU_REGISTER_RS*)data;
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);
}

//处理登录请求
void CKernel::dealLoginRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	_STRU_LOGIN_RQ* rq = (_STRU_LOGIN_RQ*)data;
	cout <<  ",pass:" << rq->passw << ",tel:" << rq->tel << endl;

	_STRU_LOGIN_RS* rs = (_STRU_LOGIN_RS*)data;
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);

}
