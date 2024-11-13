#include "TcpClient.h"
#include"../mediator/UdpMediator.h"
TcpClient::TcpClient(INetMediator* p) :m_sock(INVALID_SOCKET), m_handle(nullptr)
{
	m_pMediator = p;
}
TcpClient::~TcpClient() {}

unsigned __stdcall TcpClient::recvThread(void* lpvoid) {
	TcpClient* lpclient = (TcpClient*)lpvoid;
	lpclient->recvData();
	return 1;
}

//初始化网络
bool TcpClient::initNet() {
	//加载库
	WORD version = MAKEWORD(DEF_VERSION_HIGH, DEF_VERSION_LOW);
	WSADATA data;
	int err = WSAStartup(version, &data);
	if (0 != err) {
		cout << "WSAStartup success" << endl;
		return false;
	}
	if (DEF_VERSION_HIGH != HIBYTE(data.wVersion) || DEF_VERSION_LOW != LOBYTE(data.wVersion)) {
		cout << "version error" << endl;
		return false;
	}
	//创建套接字
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_sock) {
		cout << "socket error" << endl;
		return false;
	}
	else {
		cout << "socket success" << endl;
	}
	//连接服务器
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEF_TCP_PORT);
	err = connect(m_sock, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == err) {
		cout << "connect error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "connect success" << endl;
	}
	//创建接收数据的线程
	m_handle = (HANDLE)_beginthreadex(nullptr, 0, &recvThread, this, 0, nullptr);

	return true;
}

//接收数据(放在线程里)
void TcpClient::recvData() {
	cout << "TcpClient::" << __func__ << endl;

	//接收数据的长度
	int nRecvNum = 0;
	//保存包的长度
	int nPackLen = 0;
	//记录一个包中累计接收到多少数据
	int noffset = 0;

	//循环接收 
	while (m_isRunning) {
		//接收数据的大小
		nRecvNum = recv(m_sock, (char*)&nPackLen, sizeof(int), 0);
		if (nRecvNum > 0) {
			//接收包内容
			//创建一个接收到的数据大小的空间
			char* PackBuf = new char[nPackLen];
			//循环接收一个包的数据
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
			//TODO：把接收到的数据传给中介者类
			m_pMediator->transmitData(PackBuf, noffset, m_sock);
			////打印接收到的内容
			//cout << "server say: " << PackBuf << endl;

			//offset清零
			noffset = 0;

		}
		else {
			cout << "server recv error1:" << WSAGetLastError() << endl;
		}
	}



}

//发送数据
bool TcpClient::sendData(char* data, int len, unsigned long to) {
	cout << "TcpClient::" << __func__ << endl;

	//粘包问题-用先发数据大小再发包内容
	//1.校验参数合法性
	//-根据参数类型或者意义判断
	if (nullptr == data || len <= 0) {
		cout << " paramater error" << endl;
		return false;
	}

	//2.先发包长度
	int sendNum = send(m_sock, (char*)&len, sizeof(int), 0);
	if (SOCKET_ERROR == sendNum) {
		cout << "send len error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		//cout << "send len success" << endl;
	}
	//3.再发包内容
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

//关闭网络
void TcpClient::unInitNet() {
	//1.回收线程资源
	m_isRunning = false;
	if (m_handle) {
		//WaitForSingleObject的返回值如果等于WAIT_TIMEOUT,就说明等待的线程在等待时间结束后,还在继续运行
		if (WAIT_TIMEOUT == WaitForSingleObject(m_handle/*等待哪个线程,就填哪个线程句柄*/, 1000/*等待的时间,*/))
		{
			//如果线程还继续运行，强制杀死线程
			TerminateThread(/*要杀死的线程*/m_handle,/*退出码*/-1);
		}
		//关闭句柄
		CloseHandle(m_handle);
		m_handle = nullptr;
	}
	//2.关闭套接字
	//如果套接字存在且不是无效的套接字就关闭
	if (!m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	//3.卸载库
	WSACleanup();

}

