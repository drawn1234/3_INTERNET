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
//��������ָ�������ʼ����������
void CKernel::setProtocol() {
	cout << __func__ << endl;
	//��ʼ����0
	memset(m_protocol, 0, sizeof(m_protocol));
	//��������
	m_protocol[_def_register_rq - _DEF_PROTOCOL_BASE] = &CKernel::dealRegisterRq;
	m_protocol[_def_login_rq - _DEF_PROTOCOL_BASE] = &CKernel::dealLoginRq;

}

//�򿪷������������磬�������ݿ⣩
bool CKernel::startServer() {
	//1.������
	if (!m_pMediator->openNet()) {
		cout << "������ʧ��" << endl;
		return false;
	}
	//2.�������ݿ�
	char ip[] = "127.0.0.1";
	char user[] = "root";
	char pass[] = "026207";
	char db[] = "colin0318im";
	if (!m_sql.ConnectMySql(ip, user, pass, db)) {
		cout << "�������ݿ�ʧ��" << endl;
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
//�������н��յ�������
void CKernel::dealData(char* data, int len, unsigned long from) {
	//cout << __func__ << ":" << data << "   len:" << len << endl;
	//char s[] = "sgjhgddvgfygaja1db";
	//m_pMediator-> sendData(s, sizeof(s), from);
	cout << __func__ << endl;
	//ȡ��Э��ͷ
	packtype type = *(packtype*)data;

	//���������±�
	int index = type - _DEF_PROTOCOL_BASE - 1;
	//�ж��±��Ƿ�����Ч�ķ�Χ��
	if (index >= 0 && index < _DEF_PROTOCOL_COUNT) {
		//���������±꣬ȡ��������ַ
		PFUN pf = m_protocol[index];
		if (pf) {
			//���þ���Ĵ�����
			(this->*pf)(data, len, from);
		}
		else {
			//��ӡ
			cout << "type2:" << type << endl;
		}
	}
	else {
		cout << "type1:" << type << endl;
	}
	//���տռ�
	delete[] data;
}


//����ע������
void CKernel::dealRegisterRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	_STRU_REGISTER_RQ* rq = (_STRU_REGISTER_RQ*)data;
	cout << "name:" << rq->name << ",pass:" << rq->passw << ",tel:" << rq->tel << endl;

	_STRU_REGISTER_RS* rs = (_STRU_REGISTER_RS*)data;
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);
}

//�����¼����
void CKernel::dealLoginRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	_STRU_LOGIN_RQ* rq = (_STRU_LOGIN_RQ*)data;
	cout <<  ",pass:" << rq->passw << ",tel:" << rq->tel << endl;

	_STRU_LOGIN_RS* rs = (_STRU_LOGIN_RS*)data;
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);

}
