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

//������ָ�������ʼ�����Ҵ�ֵ
void CKernel::setProtocol() {
	cout << __func__ << endl;
	//��ʼ����0
	memset(m_protocol, 0, sizeof(m_protocol));
	//��������
	m_protocol[_def_register_rq    - _DEF_PROTOCOL_BASE-1] = &CKernel::dealRegisterRq;
	m_protocol[_def_login_rq       - _DEF_PROTOCOL_BASE-1] = &CKernel::dealLoginRq;
	m_protocol[_def_chat_rq        -_DEF_PROTOCOL_BASE - 1] = &CKernel::dealChatRq;
	m_protocol[_def_offline_rq     -_DEF_PROTOCOL_BASE - 1] = &CKernel::dealOfflineRq;
	m_protocol[_def_add_friend_rq  - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRq;
	m_protocol[_def_add_friend_rs  - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRs;

}

//�򿪷�����
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
	cout << __func__ << endl;
	//ȡ��Э��ͷ
	packtype type=*(packtype*)data;
	
	//����Э��ͷ�ж����ĸ��ṹ�壬���ò�ͬ�Ĵ�����
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
			//��ӡ2:1.����ṹ���ʱ��typeֵ���ˣ�2.�Զ˷��͵Ľṹ�岻��
			cout << "type2:" << type << endl;
		}
	}
	else {
		//��ӡ1��1.�����ṹ���ʱ��packtypeû�з��ڽṹ�����ǰ�棬Ҳ����û�е�һ��������
		//2.�������ݵ�ʱ��offsetû������-TcpServer
		cout << "type1:" << type << endl;
	}
	//���տռ�
	delete[] data;
}

//����ע������
void CKernel::dealRegisterRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	_STRU_REGISTER_RQ* rq = (_STRU_REGISTER_RQ*)data;
	//1. ���

    //2. У��绰�����Ƿ�ע��
	//�����ݿ��У����ݵ绰�����ѯ�绰����
	list<string> lstStr;
	char sql[1024] = "";
	sprintf_s(sql, "select tel from t_user where tel='%s';", rq->tel);
	if (!m_sql.SelectMySql(sql/*Ҫִ�е�sql���*/,
		1/*sql����в�ѯ���еĸ���*/, 
		lstStr/*��ѯ���Ľ��*/)) {
		//����ԭ��1.�������ݿ�ʧ�ܣ�
		//2.sql������﷨���󣬻��������ͱ��Ӧ���ϣ�����־�д�ӡ��sql��临�Ƶ�worbunch�����У�

		cout << "��ѯ���ݿ�ʧ��"<<sql <<endl;
		return;
	}
    //�жϲ�ѯ����Ƿ�Ϊ��
	if (0 != lstStr.size()) {
		//��ѯ�����Ϊ�գ�˵���绰���뱻ע�����ע��ʧ��
		_STRU_REGISTER_RS rs;
		rs.result = _DEF_REGISTER_TEL_EXISTS;
		//���ͻ��˻ظ�ע����
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
		//���
		return;
	}
	
	//3. У���ǳ��Ƿ�ע��
	//�����ǳƲ�ѯ�ǳ�
	sprintf_s(sql, "select name from t_user where name='%s';", rq->name);
	if (!m_sql.SelectMySql(sql/*Ҫִ�е�sql���*/,
		1/*sql����в�ѯ���еĸ���*/,
		lstStr/*��ѯ���Ľ��*/)) {
		//����ԭ��1.�������ݿ�ʧ�ܣ�
		//2.sql������﷨���󣬻��������ͱ��Ӧ���ϣ�����־�д�ӡ��sql��临�Ƶ�worbunch�����У�

		cout << "��ѯ���ݿ�ʧ��" << sql << endl;
		return;
	}
	if (0 != lstStr.size()) {
		//��ѯ�����Ϊ�أ��ǳƱ�ע�����ע��ʧ��_def_regigster_name_exists
		_STRU_REGISTER_RS rs;
		rs.result = _DEF_REGISTER_NAME_EXISTS;
		//���ͻ��˻ظ�ע����
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
		//����
		return;
	}
	

	//4. �����ݲ������ݿ�
	sprintf_s(sql, "insert into t_user(name,tel,passw,feeling,iconid) values ('%s','%s','%s','��������',8);"
		, rq->name, rq->tel, rq->passw);
	if (!m_sql.UpdateMySql(sql)) {
		//����ԭ��1.�������ݿ�ʧ�ܣ�
		//2.sql������﷨���󣬻��������ͱ��Ӧ���ϣ�����־�д�ӡ��sql��临�Ƶ�worbunch�����У�

		cout << "�������ݿ�ʧ��" << sql << endl;
	}
	//5.ע��ɹ�
	_STRU_REGISTER_RS rs;
	rs.result = _DEF_REGISTER_SUCCESS;
	//6.��ע�������߿ͻ���
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);
}


//��ȡ��ǰ��¼���û�����Ϣ�Լ����ѵ���Ϣ
void CKernel::getUserInfoAndFriendInfo(int id) {

	cout << __func__ << endl;
	//�����Լ���id��ѯ�Լ�����Ϣ
	_STRU_FRIEND_INFO userInfo;
	getInfoById(id, &userInfo);
	//���Լ�����Ϣ�����ͻ���
	if (m_mapIdToSocket.count(id)>0) {
		m_pMediator->sendData((char*)&userInfo, sizeof(userInfo),m_mapIdToSocket[id]);
	}
	else {
		cout << "m_mapIdToSocket��û��id��" << id << endl;
		return;
	}
	//�����Լ���id��ѯ���ѵ�id�б�
	list<string> lstStr;
	char sql[1024] = "";
	sprintf_s(sql, "select idb from t_friend where ida=%d;", id);
	if (!m_sql.SelectMySql(sql, 1, lstStr)) {
		cout << "��ѯ���ݿ�ʧ��:"<<sql << endl;
		return;
	}
	//�������ѵ�id�б�
	int friendId = 0;
	_STRU_FRIEND_INFO friendInfo;
	while (lstStr.size()>0) {
		//ȡ��ÿ�����ѵ�id
		friendId = stoi(lstStr.front());
		lstStr.pop_front();
		//���ݺ��ѵ�id��ѯ���ѵ���Ϣ
		getInfoById(friendId, &friendInfo);
		//�Ѻ��ѵ���Ϣ���ظ��ͻ���
		if (m_mapIdToSocket.count(id) > 0) {
			m_pMediator->sendData((char*)&friendInfo, sizeof(friendInfo), m_mapIdToSocket[id]);
		}
		else {
			cout << "m_mapIdToSocket��û��:" << id << endl;
			return;
		}

		//�����ݿ��ѯ����Id�б�
		//��������id�б�
		//�жϺ����Ƿ����ߣ�������߾�֪ͨ�����Լ�������
		if (m_mapIdToSocket.count(friendId) > 0) {
			//������ߣ���֪ͨ�����Լ�������
			m_pMediator->sendData((char*)&userInfo, sizeof(userInfo), m_mapIdToSocket[friendId]);
		}
	}
}

//����id��ѯ�û���Ϣ
void CKernel::getInfoById(int id, _STRU_FRIEND_INFO* info/*�鵽���û���Ϣ��Ϊ�������ظ����ú���*/) {
	cout << __func__ << endl;
	info->id = id;
	//��ѯ�����Ƿ�����
	if (m_mapIdToSocket.count(id) > 0) {
		//����
		info->status = _DEF_STATUS_ONLINE;
	}else{
		info->status = _DEF_STATUS_OFFLINE;
	}

	//�����ݿ��в�ѯ�ǳƣ�ǩ����ͷ��id
	list<string> lstStr;
	char sql[1024] = "";
	sprintf_s(sql,"select name,feeling,iconid from t_user where id= %d", id);
	if (!m_sql.SelectMySql(sql,3,lstStr)) {
		cout << "��ѯ���ݿ�ʧ�ܣ�" << sql << endl;
		return;
	}
	
	//���ȡ�������ݸ�����ȷ
	if (3 == lstStr.size()) {
		//ȡ���ǳ�
		strcpy_s(info->name,sizeof(info->name),lstStr.front().c_str());
		lstStr.pop_front();
		//ȡ��ǩ��
		strcpy_s(info->feeling, sizeof(info->feeling), lstStr.front().c_str());
		lstStr.pop_front();
		//ȡ��ͷ��id
		info->iconId = stoi(lstStr.front());
		lstStr.pop_front();
	}
	else {
		cout << "��ѯ�ǳƣ�ǩ����ͷ��id����" << sql << endl;
	}

}

//�����¼����
void CKernel::dealLoginRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	//1.���
	_STRU_LOGIN_RQ* rq = (_STRU_LOGIN_RQ*)data;
	//2.�����ݿ��ѯ�����ݵ绰�����ѯ����
	list<string> lstStr;
	char sql[1024] = "";
	sprintf_s(sql, "select passw,id from t_user where tel='%s';", rq->tel);
	if (!m_sql.SelectMySql(sql/*Ҫִ�е�sql���*/,
		2/*sql����в�ѯ���еĸ���*/,
		lstStr/*��ѯ���Ľ��*/)) {
		cout << "��ѯ���ݿ�ʧ��" << sql << endl;
		return;
	}
	//3.�жϲ�ѯ����Ƿ�Ϊ��
	_STRU_LOGIN_RS rs;
	if (0 ==lstStr.size() ) {
		//4.��ѯ���Ϊ�գ�˵���绰����δע�ᣬ��¼ʧ��
		rs.result = _DEF_LOGIN_TEL_NOT_EXISTS;
	}
	else {
		//5.�Ƚϲ�ѯ������������������
		string pass = lstStr.front();
		lstStr.pop_front();

		//�����ѯ�����û�id������ѯ������Ϣ
		int userId = stoi(lstStr.front());
		lstStr.pop_front();

		if (pass==rq->passw) {
			//��ȣ����ǵ�¼�ɹ�
			rs.result = _DEF_LOGIN_SUCCESS;
			rs.userId = userId;

			//�����ݿ��ѯ����Id�б�
			//��������id�б�
			//�жϺ����Ƿ����ߣ�������߾�֪ͨ�����Լ�������


			//���浱ǰ�û���socket
			m_mapIdToSocket[userId] = from;
			//�ѽ�����͸��ͻ���
			m_pMediator->sendData((char*)&rs, sizeof(rs), from);
			//��ȡ��ǰ�û�����Ϣ�Լ�������Ϣ
			getUserInfoAndFriendInfo(userId);
			return;
		}
		else {
			//����ȣ����ǵ�¼ʧ�ܣ��������
			rs.result = _DEF_LOGIN_PASSW_ERROR;
		}
	}
	//6.�ѽ�����͸��ͻ���
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);

}


//������������
void CKernel::dealChatRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	//1.���
	_STRU_CHAT_RQ* rq = (_STRU_CHAT_RQ*)data;

	//2.�жϺ����Ƿ�����
	if (m_mapIdToSocket.count(rq->friendId) > 0) {
		//����������ߣ�����������ת��������
		m_pMediator->sendData(data, len, m_mapIdToSocket[rq->friendId]);
	}
	else {
		//������Ѳ����ߣ����ͻ��˻ظ�����ʧ��
		//����������Ѳ����ߺ��ѵ���Ϣ�������ݿ⣬�Ⱥ�������
		//�����ݿ�������ת�������ѣ������ݿ��б������Ϣɾ��
		_STRU_CHAT_RS rs;
		rs.friendId = rq->friendId;
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
	}
}

//������������
void CKernel::dealOfflineRq(char* data, int len, unsigned long from){
	cout << __func__ << endl;
	//1.���
	_STRU_OFFLINE_RQ* rq = (_STRU_OFFLINE_RQ*)data;
	//2.��ѯ���ѵ�id�б�
	list<string> lstStr;
	char sql[1025] = "";
	sprintf_s(sql, "select idb from t_friend where ida =%d;", rq->id);
	if (!m_sql.SelectMySql(sql, 1, lstStr)) {
		cout << "��ѯ���ݿ�ʧ��:" << sql << endl;
	}
	//3.�������ѵ�id�б�
	int friendId = 0;
	while (lstStr.size() > 0) {
		//4.ȡ�����ѵ�id
		friendId = stoi(lstStr.front());
		lstStr.pop_front();
		//5.�жϺ����Ƿ����ߣ����߾�ת���������������
		if (m_mapIdToSocket.count(friendId) > 0) {
			m_pMediator->sendData(data, len, m_mapIdToSocket[friendId]);
		}
	}
	//6.�ر������û���socket�����һ���map�Ŀռ�
	auto ite = m_mapIdToSocket.find(rq->id);
	if (ite != m_mapIdToSocket.end()) {
		closesocket(ite->second);
		m_mapIdToSocket.erase(ite);
	}

}

//������Ӻ�������
void CKernel::dealAddFriendRq(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	//1.���
	_STRU_ADD_FRIEND_RQ* rq = (_STRU_ADD_FRIEND_RQ*)data;

	//2.���ݺ����ǳƲ�ѯ����id
	list<string> lstStr;
	char sql[2024] = "";
	sprintf_s(sql, "select id from t_user where name='%s'", rq->friendName);
	if (!m_sql.SelectMySql(sql,1, lstStr)) {
		cout << "��ѯ���ݿ�ʧ�ܣ�" << sql << endl;
		return;
	}
	_STRU_ADD_FRIEND_RS rs;
	
	//3.����ѯ����Ƿ�Ϊ��
	if (0 == lstStr.size()) {
		//4.��ѯ���Ϊ�գ�˵���ǳƲ����ڣ����ʧ��
		rs.result = _DEF_ADD_FRIEND_NOT_EXISTS;
		strcpy_s(rs.friendName, sizeof(rs.friendName), rq->friendName);
		//����ӽ�����ظ��ͻ���A
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);	
	}
	else {
		//ȡ�����ѵ�id
		int friendId = stoi(lstStr.front());
		lstStr.pop_front();
		//5.�жϺ����Ƿ�����
		if (m_mapIdToSocket.count(friendId)> 0) {
			//6.�������ߣ�����Ӻ�������ת��������
			m_pMediator->sendData(data, len, m_mapIdToSocket[friendId]);
		}
		else {
			//7.���Ѳ����ߣ����ʧ��
			rs.result = _DEF_ADD_FRIEND_OFFLINE;
			strcpy_s(rs.friendName, sizeof(rs.friendName), rq->friendName);
			//�ѽ�����ظ��ͻ���
			m_pMediator->sendData((char*)&rs, sizeof(rs), from);
		}
	}
}

//������Ӻ��ѻظ�(B�ͻ���)
void CKernel::dealAddFriendRs(char* data, int len, unsigned long from) {
	cout << __func__ << endl;
	//1.���
	_STRU_ADD_FRIEND_RS* rs = (_STRU_ADD_FRIEND_RS*)data;


	//2.�жϽ���Ƿ�ͬ��
	if (_DEF_ADD_FRIEND_SUCCESS == rs->result) {
		//3.���ͬ�⣬�Ͱѽ��д�����ݿ���
		char sql[1024] = "";
		sprintf_s(sql, "insert into t_friend values (%d,%d);", rs->friendId, rs->myId);
		if (!m_sql.UpdateMySql(sql)) {
			cout << "�������ݿ�ʧ�ܣ�" << sql << endl;
		}

		sprintf_s(sql, "insert into t_friend values (%d,%d);", rs->myId, rs->friendId);
		if (!m_sql.UpdateMySql(sql)) {
			cout << "�������ݿ�ʧ�ܣ�" << sql << endl;
		}

		//4.����˫�˺����б�
		getUserInfoAndFriendInfo(rs->friendId);
	
		
	}
	//5.���ܽ����Σ���Ҫ���������A�ͻ���
	if (m_mapIdToSocket.count(rs->myId) > 0) {
		m_pMediator->sendData((char*)rs, len, m_mapIdToSocket[rs->myId]);
	}
}
