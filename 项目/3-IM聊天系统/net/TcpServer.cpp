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
	SOCKET sockTalk = INVALID_SOCKET;//初始化为无效
	HANDLE handle = nullptr;
	unsigned int threadId = 0;
	//循环接受连接
	while (lpthis->m_isRunning) {
		//接受连接
		//接收连接成功的时候创建的socket用于收发数据，需要在接收发送数据中使用
		//会有多个socket，且socket不同，需要定义为成员变量
		sockTalk = accept(lpthis->m_sock, (sockaddr*)&addrto, &addtoSize);
		if (INVALID_SOCKET == sockTalk) {
			//如果接收连接失败，打印错误日志，等待下次连接
			cout << "accept errror" << WSAGetLastError() << endl;
			break;
		}
		else {
			//成功打印客户ip
			cout << "client ip:" << inet_ntoa(addrto.sin_addr) << endl;

			//创建接收客户端数据的线程

			handle = (HANDLE)_beginthreadex(nullptr, 0, &recvThread, lpthis, 0, &threadId);
			if (handle) {
				lpthis->m_listHandle.push_back(handle);
			}
			//保存当前连接产生的socket
			lpthis->m_mapThreadToSocket[threadId] = sockTalk;
		}

	}
	return 1;
}
//初始化网络
bool TcpServer::initNet() {
	//加载库
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
	//创建套接字
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_sock) {
		cout << "socket error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "socket success" << endl;
	}
	//绑定IP和端口号
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
	//监听

	err = listen(m_sock, SOMAXCONN);
	if (SOCKET_ERROR == err) {
		cout << "listen error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "listen success" << endl;
	}
	//创建接受连接的线程
	HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, &acceptThread, this, 0, nullptr);
	//保存线程句柄
	if (handle) {
		m_listHandle.push_back(handle);
		cout << "acceptThread:" << GetCurrentThreadId << endl;
	}
	return true;
}

//接收数据
void TcpServer::recvData() {
	cout << "TcpServer::" << __func__ << endl;
	//休眠一会儿：等待acceptThread把socket保存到map里。再去map中取数据
	Sleep(10000);
	//获取当前线程对应的socket，使用当前线程的socket去接收数据
	//如果从map中取值的时候有错误，要么存Id的时候错了，要么key错了
	SOCKET sock = m_mapThreadToSocket[GetCurrentThreadId()];
	if (!sock || INVALID_SOCKET == sock) {
		cout << "TcpServer::recvData error" << WSAGetLastError() << endl;
	}
	//接收数据的长度
	int nRecvNum = 0;
	//保存包的长度
	int nPackLen = 0;
	//记录一个包中累计接收到多少数据
	int noffset = 0;

	//循环接收 
	while (m_isRunning) {
		//接收数据的大小
		nRecvNum = recv(sock, (char*)&nPackLen, sizeof(int), 0);
		if (nRecvNum > 0) {
			//创建一个接收到的数据大小的空间
			char* PackBuf = new char[nPackLen];
			//循环接收一个包的数据
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
			//TODO：把接收到的数据传给中介者类
			//调用transmit函数
			//需要创建指针，使用指针创建对象，使用指针调用
			//一个包的数据接收完成，此时nPackLen=0.noffset变成包长度。
			m_pMediator->transmitData(PackBuf, noffset, sock);
			////测试代码
			////打印接收到的内容
			//cout << "client say： " << PackBuf << endl;

			////给客户端回复一个字符串
			//char s[] = "ni hao ";
			//sendData(s, sizeof(s), sock);
			//offset清零
			noffset = 0;

		}
		else {
			cout << "server recv error1:" << WSAGetLastError() << endl;
			break;
		}
	}


}

//发送数据
bool TcpServer::sendData(char* data, int len, unsigned long to) {
	cout << "TcpServer::" << __func__ << endl;
	//1.判断参数有效性
	if (nullptr == data || len <= 0) {
		cout << "TcpServer::sendData paramater error" << endl;
	}
	//2.先发包大小
	int sendNum = send(to, (char*)&len, sizeof(int), 0);
	if (SOCKET_ERROR == sendNum) {
		cout << "send error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		//cout << "send success" << endl;
	}
	//2.发数据
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

//关闭网络
void TcpServer::unInitNet() {
	//1.回收线程资源
	m_isRunning = false;
	HANDLE handle = nullptr;
	for (auto ite = m_listHandle.begin(); ite != m_listHandle.end();) {
		//取出当前节点的句柄
		handle = *ite;
		if (handle) {
			if (WAIT_TIMEOUT == WaitForSingleObject(handle, 1000)) {
				//没有结束强制杀死
				TerminateThread(handle, -1);
			}
			//关闭句柄
			CloseHandle(handle);
			handle = nullptr;
		}
		//从list中移除当前无效节点，返回值是下一个有效节点
		ite = m_listHandle.erase(ite);
	}
	//2.关闭套接字
	if (m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	SOCKET sock = INVALID_SOCKET;
	for (auto ite = m_mapThreadToSocket.begin(); ite != m_mapThreadToSocket.end();) {
		//取出当前节点中保存的socket
		sock = ite->second;
		//关闭taojiez
		if (sock && INVALID_SOCKET != sock) {
			closesocket(sock);
		}

		//把无效节点从中移除，返回值是下一个节点
		ite = m_mapThreadToSocket.erase(ite);
	}
	//3.卸载库
	WSACleanup();
}

