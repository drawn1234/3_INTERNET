#include "TcpServer.h"
#include"../mediator/TcpServerMediator.h"
TcpServer::TcpServer(INetMediator* p) :m_sock(INVALID_SOCKET) {
	m_pMediator = p;
}
TcpServer::~TcpServer() {}

unsigned __stdcall TcpServer::recvThread(void* lpvoid) {
	TcpServer* lpthis = (TcpServer*)lpvoid;
	lpthis->recvData();
	return 1;
}

unsigned __stdcall TcpServer::acceptThread(void* lpvoid) {
	TcpServer* lpthis = (TcpServer*)lpvoid;
	sockaddr_in addrto = {};
	int addtoSize = sizeof(addrto);
	SOCKET sockTalk = INVALID_SOCKET;//��ʼ��Ϊ��Ч
	HANDLE handle = nullptr;
	unsigned int threadId = 0;
	//ѭ����������
	while (lpthis->m_isRunning) {
		//��������
		//�������ӳɹ���ʱ�򴴽���socket�����շ����ݣ���Ҫ�ڽ��շ���������ʹ��
		//���ж��socket����socket��ͬ����Ҫ����Ϊ��Ա����
		sockTalk = accept(lpthis->m_sock, (sockaddr*)&addrto, &addtoSize);
		if (INVALID_SOCKET == sockTalk) {
			//�����������ʧ�ܣ���ӡ������־���ȴ��´�����
			cout << "accept errror" << WSAGetLastError() << endl;
			break;
		}
		else {
			//�ɹ���ӡ�ͻ�ip
			cout << "client ip:" << inet_ntoa(addrto.sin_addr) << endl;

			//�������տͻ������ݵ��߳�

			handle = (HANDLE)_beginthreadex(nullptr, 0, &recvThread, lpthis, 0, &threadId);
			if (handle) {
				lpthis->m_listHandle.push_back(handle);
			}
			//���浱ǰ���Ӳ�����socket
			lpthis->m_mapThreadToSocket[threadId] = sockTalk;
		}

	}
	return 1;
}
//��ʼ������
bool TcpServer::initNet() {
	//���ؿ�
	WORD version = MAKEWORD(DEF_VERSION_HIGH, DEF_VERSION_LOW);
	WSADATA data;
	int err = WSAStartup(version, &data);
	if (err != 0) {
		cout << "WSAstartup error" << endl;
		return false;
	}
	else {
		cout << "WSAStarup success " << endl;
	}
	if (DEF_VERSION_HIGH != HIBYTE(data.wVersion) || DEF_VERSION_LOW != LOBYTE(data.wVersion)) {
		cout << "version error" << endl;
		return false;
	}
	else {
		cout << "version right" << endl;
	}
	//�����׽���
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_sock) {
		cout << "socket error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "socket success" << endl;
	}
	//��IP�Ͷ˿ں�
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEF_TCP_PORT);
	err = bind(m_sock, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == err) {
		cout << "bind error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "bind success" << endl;
	}
	//����

	err = listen(m_sock, SOMAXCONN);
	if (SOCKET_ERROR == err) {
		cout << "listen error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "listen success" << endl;
	}
	//�����������ӵ��߳�
	HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, &acceptThread, this, 0, nullptr);
	//�����߳̾��
	if (handle) {
		m_listHandle.push_back(handle);
		cout << "acceptThread:" << GetCurrentThreadId << endl;
	}
	return true;
}

//��������
void TcpServer::recvData() {
	cout << "TcpServer::" << __func__ << endl;
	//����һ������ȴ�acceptThread��socket���浽map���ȥmap��ȡ����
	Sleep(10000);
	//��ȡ��ǰ�̶߳�Ӧ��socket��ʹ�õ�ǰ�̵߳�socketȥ��������
	//�����map��ȡֵ��ʱ���д���Ҫô��Id��ʱ����ˣ�Ҫôkey����
	SOCKET sock = m_mapThreadToSocket[GetCurrentThreadId()];
	if (!sock || INVALID_SOCKET == sock) {
		cout << "TcpServer::recvData error" << WSAGetLastError() << endl;
	}
	//�������ݵĳ���
	int nRecvNum = 0;
	//������ĳ���
	int nPackLen = 0;
	//��¼һ�������ۼƽ��յ���������
	int noffset = 0;

	//ѭ������ 
	while (m_isRunning) {
		//�������ݵĴ�С
		nRecvNum = recv(sock, (char*)&nPackLen, sizeof(int), 0);
		if (nRecvNum > 0) {
			//����һ�����յ������ݴ�С�Ŀռ�
			char* PackBuf = new char[nPackLen];
			//ѭ������һ����������
			while (nPackLen > 0) {
				nRecvNum = recv(sock, PackBuf + noffset, nPackLen, 0);
				if (nRecvNum > 0) {
					noffset += nRecvNum;
					nPackLen -= nRecvNum;
				}
				else {
					cout << "recv error2:" << WSAGetLastError() << endl;
					break;
				}
			}
			//TODO���ѽ��յ������ݴ����н�����
			//����transmit����
			//��Ҫ����ָ�룬ʹ��ָ�봴������ʹ��ָ�����
			//һ���������ݽ�����ɣ���ʱnPackLen=0.noffset��ɰ����ȡ�
			m_pMediator->transmitData(PackBuf, noffset, sock);
			////���Դ���
			////��ӡ���յ�������
			//cout << "client say�� " << PackBuf << endl;

			////���ͻ��˻ظ�һ���ַ���
			//char s[] = "ni hao ";
			//sendData(s, sizeof(s), sock);
			//offset����
			noffset = 0;

		}
		else {
			cout << "server recv error1:" << WSAGetLastError() << endl;
			break;
		}
	}


}

//��������
bool TcpServer::sendData(char* data, int len, unsigned long to) {
	cout << "TcpServer::" << __func__ << endl;
	//1.�жϲ�����Ч��
	if (nullptr == data || len <= 0) {
		cout << "TcpServer::sendData paramater error" << endl;
	}
	//2.�ȷ�����С
	int sendNum = send(to, (char*)&len, sizeof(int), 0);
	if (SOCKET_ERROR == sendNum) {
		cout << "send error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		//cout << "send success" << endl;
	}
	//2.������
	sendNum = send(to, data, len, 0);
	if (SOCKET_ERROR == sendNum) {
		cout << "send error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		// cout << "send success" << endl;
	}
	return true;
}

//�ر�����
void TcpServer::unInitNet() {
	//1.�����߳���Դ
	m_isRunning = false;
	HANDLE handle = nullptr;
	for (auto ite = m_listHandle.begin(); ite != m_listHandle.end();) {
		//ȡ����ǰ�ڵ�ľ��
		handle = *ite;
		if (handle) {
			if (WAIT_TIMEOUT == WaitForSingleObject(handle, 1000)) {
				//û�н���ǿ��ɱ��
				TerminateThread(handle, -1);
			}
			//�رվ��
			CloseHandle(handle);
			handle = nullptr;
		}
		//��list���Ƴ���ǰ��Ч�ڵ㣬����ֵ����һ����Ч�ڵ�
		ite = m_listHandle.erase(ite);
	}
	//2.�ر��׽���
	if (m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	SOCKET sock = INVALID_SOCKET;
	for (auto ite = m_mapThreadToSocket.begin(); ite != m_mapThreadToSocket.end();) {
		//ȡ����ǰ�ڵ��б����socket
		sock = ite->second;
		//�ر�taojiez
		if (sock && INVALID_SOCKET != sock) {
			closesocket(sock);
		}

		//����Ч�ڵ�����Ƴ�������ֵ����һ���ڵ�
		ite = m_mapThreadToSocket.erase(ite);
	}
	//3.ж�ؿ�
	WSACleanup();
}

