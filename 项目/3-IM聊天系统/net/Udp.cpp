#include "Udp.h"
#include"../mediator/UdpMediator.h"

using namespace std;
Udp::Udp(INetMediator* p) :m_handle(nullptr) {
	m_pMediator = p;
}
Udp::~Udp() {

}

//不是类成员变量的写法
unsigned __stdcall recvThread(void* lpvoid)
{
	//当前的对象 类型是void* 使用this调用，因为是UDP的对象，需要强转
	// UDP函数创建的时候就有一个对象
	//如果新建一个对象，该对象不是UDP类的对象，它没有初始化
	//需要在初始化的时候就将对象传入
	Udp* pThis = (Udp*)lpvoid;
	pThis->recvData();
	return 1;
}
//初始化网络
bool Udp::initNet() {
	//加载库
	//魔鬼数字：对理解代码造成困难的数字
	//解决：定义成宏
	//利于理解，多处使用便于修改
	//遇见数字的地方定义成宏 新建一个文件定义
	WORD version = MAKEWORD(DEF_VERSION_HIGH, DEF_VERSION_LOW);
	WSADATA data;
	int err = WSAStartup(version, &data);
	if (0 != err) {
		cout << "WSAStartup error" << endl;
		return false;//bool类型返回false
	}
	else {
		cout << "WSAStartup success" << endl;
	}
	if (HIBYTE(data.wVersion) != DEF_VERSION_HIGH || LOBYTE(data.wVersion) != DEF_VERSION_LOW) {
		cout << "version error" << endl;
		return false;
	}
	else {
		cout << "version right" << endl;
	}

	//创建套接字
	//--接收数据，发送数据都需要使用，需要将该变量定义为类的成员变量
	//在类的任何成员里都可以使用
	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_sock) {
		cout << "socket error" << WSAGetLastError() << endl;
		// WSACleanup(); 有卸载库的函数，不需要卸载库
		return false;
	}
	else {
		cout << "socket success" << endl;
	}
	//绑定网卡和端口号
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEF_UDP_PORT);
	err = bind(m_sock, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == err) {
		cout << "bind error" << WSAGetLastError() << endl;
		/* closesocket(m_sock);
		 WSACleanup();*/
		return false;
	}
	else {
		cout << "bind success" << endl;
	}

	//创建接收数据线程
	//createThread和ExistThread是一对，如果使用createThread,线程结束时操作系统会自动回收结束线程。
	//如果线程中使用了c++运行时库的函数（strcpy），这些函数会申请空间但是不释放
	//ExistTread函数调用时也不释放空间，造成内存泄露
	//_beginthreadex和_endthreadex是一对，_endthreadex会在线程退出的时候先回收空间，在调用ExistThread
	//返回值 HANDLE
	m_handle = (HANDLE)_beginthreadex(nullptr/*安全级别，使用默认安全级别*/,
		0 /*堆栈大小，使用默认堆栈大小1M,决定一个服务端可以创建多少个线程*/,
		&recvThread/*线程函数的起始地址*/,
		this/*线程函数的参数列表*/,
		0 /*初始化标志位,0-创建既运行，还有一个挂起状态*/,
		nullptr/*操作系统给线程分配的id，输出参数,不需要就填空*/);
	return true;
}

//接收数据的线程函数
// 线程需要做的事情：调用接收数据函数
//不是当前类的成员函数，加入！

//unsigned __stdcall Udp::recvThread(void* lpvoid)
//{
   // //当前的对象 类型是void* 使用this调用，因为是UDP的对象，需要强转
   // // UDP函数创建的时候就有一个对象
   // //如果新建一个对象，该对象不是UDP类的对象，它没有初始化
   // //需要在初始化的时候就将对象传入
   // Udp* pThis = (Udp*)lpvoid;
   // pThis->recvData();
   // return 1;
//}



//接收数据（放在线程里，一直循环等待接收）
 //接收数据是阻塞的，如果需要让接收数据的函数运行-创建线程
 //什么时候开始接收数据？ ：当程序运行的时候就开始接收数据
 //绑定完网卡和端口号就需要创建线程接收数据
 //需要一直循环等待接收数据，是一个死循环
 //有两个线程，主函数中调用initNet创建了一个线程，recvTread中创建了了一个线程，在recvData中传入数据
 //数据处理需要一段时间，如果在处理数据的过程中收到新的数据
 //新的数据会将旧的数据覆盖掉
 //解决：将recvBuf作为一个缓冲区，将需要处理的数据拷贝到一个新的空间中，如果旧的数据处理完成，将新创建的空间释放
 // 旧的空间用于接收最新的数据
 //根据接收的数据的大小申请一个新的空间里，然后把接受到的数据传入中介者类
void Udp::recvData() {
	cout << "Udp::" << __func__ << endl;
	char recvBuf[9999] = "";
	int recvBufSize = sizeof(recvBuf);
	sockaddr_in addr;
	int addrSize = sizeof(addr);
	int nRecvNum = 0;
	while (true) {
		nRecvNum = recvfrom(m_sock, recvBuf, recvBufSize, 0, (sockaddr*)&addr, &addrSize);
		if (nRecvNum > 0) {
			//接收一个数据包成功
			//根据接收到的数据大小创建一个新空间
			char* newRecvBuf = new char[nRecvNum];

			//把接收到的数据拷贝到新的空间里
			char* pPack = new char[nRecvNum];
			//不能使用strcpy拷贝，它只拷贝字符串/0结束，但是发送的数据不一定是字符串
			// 可能是一个结构体 如果数据中间有0 ，数据拷贝结束，导致数据拷贝出错
			//使用memcpy 按照指定的数据长度拷贝，应用范围比strcpy更广
			memcpy(pPack, recvBuf, nRecvNum);

			//TODO：把接收到的数据传给中介者类

			m_pMediator->transmitData(pPack, sizeof(pPack), addr.sin_addr.S_un.S_addr);
			//测试代码：打印接收到的数据
			//cout << "ip:" << inet_ntoa(addr.sin_addr)<<pPack << endl;

		}
		else {
			cout << "recv error" << WSAGetLastError() << endl;
			break;
		}
	}
}

//发送数据(udp:ip ulong类型，决定发给谁；TCP：socket uint，决定发给谁)
bool Udp::sendData(char* data, int len, unsigned long to) {
	cout << "Udp::" << __func__ << endl;
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEF_UDP_PORT);
	addr.sin_addr.S_un.S_addr = to;
	int err = sendto(m_sock, data, len, 0, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == err) {
		cout << "send error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "send success" << endl;
	}
	return true;
}

//关闭网络：回收资源
//回收线程资源，让线程结束工作，关闭套接字，卸载库
void Udp::unInitNet() {
	//1.回收线程资源
	//创建线程时，操作系统给每个线程分配三个资源 1.句柄 2.线程ID 3.内核对象（应用程序如果需要和系统交互，需要通过内核对象）
	//每调用一个资源，实际上使用的是计数器，给每个资源计数，为0时全部自动减一
	//需要让引用计数器变为0
	//1.1结束线程工作-调用recvData函数 让函数退出循环
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
	//定义变量
	//但是函数也不能立刻结束，需要等待函数下一次调用的时候才能退出循环
	//等一个单个的线程 等句柄-就是等对应的线程
	//等一定时间，如果超时就说明线程在等待时间结束后还在运行

	//2.关闭套接字
	//如果套接字存在且不是无效的套接字就关闭
	if (!m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	//3.卸载库
	WSACleanup();
	//函数里自带保护机制，如果加载库成功就加1，如果卸载库就减一。
	//不需要判断
}

