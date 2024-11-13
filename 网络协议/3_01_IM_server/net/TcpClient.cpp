#include "TcpClient.h"
#include"../mediator/UdpMediator.h"
TcpClient::TcpClient(INetMediator* p):m_sock(INVALID_SOCKET), m_handle(nullptr)
{
	m_pMediator = p;
}
TcpClient::~TcpClient() {}

unsigned __stdcall TcpClient::recvThread(void* lpvoid) {
	TcpClient* lpclient = (TcpClient*)lpvoid;
	lpclient->recvData();
	return 1;
}

//��ʼ������
bool TcpClient::initNet() {
	//���ؿ�
	WORD version = MAKEWORD(DEF_VERSION_HIGH, DEF_VERSION_LOW);
	WSADATA data;
	int err = WSAStartup(version,&data);
	if (0 != err) {
		cout << "WSAStartup success" << endl;
		return false;
	}
	if (DEF_VERSION_HIGH != HIBYTE(data.wVersion) || DEF_VERSION_LOW != LOBYTE(data.wVersion)) {
		cout << "version error" << endl;
		return false;
	}
	//�����׽���
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (INVALID_SOCKET == m_sock) {
		cout << "socket error" << endl;
		return false;
	}
	else {
		cout << "socket success" << endl;
	}
	//���ӷ�����
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEF_TCP_PORT);
    err=connect(m_sock, (sockaddr*)&addr,sizeof(addr) );
	if (SOCKET_ERROR == err) {
		cout << "connect error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "connect success" << endl;
	}
	//�����������ݵ��߳�
	m_handle = (HANDLE)_beginthreadex(nullptr, 0,&recvThread,this,0,nullptr);
	
	return true;
}

//��������(�����߳���)
void TcpClient::recvData() {
	cout << "TcpClient::" << __func__ << endl;
	
	//�������ݵĳ���
	int nRecvNum = 0;
	//������ĳ���
	int nPackLen = 0;
	//��¼һ�������ۼƽ��յ���������
	int noffset = 0;

	//ѭ������ 
	while (m_isRunning) {
		//�������ݵĴ�С
		nRecvNum = recv(m_sock, (char*)&nPackLen,sizeof(int), 0);
		if (nRecvNum > 0) {
			//���հ�����
			//����һ�����յ������ݴ�С�Ŀռ�
			char* PackBuf = new char[nPackLen];
			//ѭ������һ����������
			while (nPackLen > 0) {
				nRecvNum = recv(m_sock, PackBuf + noffset, nPackLen, 0);
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
			m_pMediator->transmitData(PackBuf, noffset, m_sock);
			////��ӡ���յ�������
			//cout << "server say: " << PackBuf << endl;

			//offset����
			noffset = 0;

		}
		else {
			cout << "server recv error1:" << WSAGetLastError() << endl;
		}
	}
	


}

//��������
bool TcpClient::sendData(char* data, int len, unsigned long to) {
	cout << "TcpClient::" << __func__  << endl;
	
	//ճ������-���ȷ����ݴ�С�ٷ�������
	//1.У������Ϸ���
	//-���ݲ������ͻ��������ж�
	if (nullptr == data || len <= 0) {
		cout << " paramater error" << endl;
		return false;
	}

	//2.�ȷ�������
	int sendNum = send(m_sock,(char*) &len, sizeof(int), 0);
	if (SOCKET_ERROR == sendNum) {
		cout << "send len error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		//cout << "send len success" << endl;
	}
	//3.�ٷ�������
	sendNum = send(m_sock, data, len, 0);
	if (SOCKET_ERROR == sendNum) {
		cout << "send data error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		//cout << "send data success" << endl;
	}
	return true;
}

//�ر�����
void TcpClient::unInitNet() {
	//1.�����߳���Դ
	m_isRunning = false;
	if (m_handle) {
		//WaitForSingleObject�ķ���ֵ�������WAIT_TIMEOUT,��˵���ȴ����߳��ڵȴ�ʱ�������,���ڼ�������
		if (WAIT_TIMEOUT == WaitForSingleObject(m_handle/*�ȴ��ĸ��߳�,�����ĸ��߳̾��*/, 1000/*�ȴ���ʱ��,*/))
		{
			//����̻߳��������У�ǿ��ɱ���߳�
			TerminateThread(/*Ҫɱ�����߳�*/m_handle,/*�˳���*/-1);
		}
		//�رվ��
		CloseHandle(m_handle);
		m_handle = nullptr;
	}
	//2.�ر��׽���
	//����׽��ִ����Ҳ�����Ч���׽��־͹ر�
	if (!m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	//3.ж�ؿ�
	WSACleanup();

}

