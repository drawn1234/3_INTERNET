#include<iostream>
//头文件不在当前解决方案<> 在当前解决方案" "
#include<winsock2.h >
//导入依赖库
#pragma comment(lib,"Ws2_32.lib")//立即导入 lib-library
using namespace std;

int main()
{
	//1.加载库（使用库函数）/静态库 添加头文件，导入依赖库（lib+库名）
	
	// 加载库相当于把exe拷入到
	//动态库 添加头文件，导入依赖库 动态库dll和exe放在一起然后才能用
	//W-windows S-socket A-api Windows里面关于Socket的一个接口
	WORD version = MAKEWORD(2, 2);//输入参数需要赋值
	WSADATA data;//输出参数不需要赋值
	int err = WSAStartup(version,&data);//未定义标识符 没有导入库
	//实现在库里面实现 除了需要引用头文件，还需要导入依赖库
	
	//函数参数填完了需要判断返回值
	//判断函数调用是否成功 成功就继续，失败就停止 return
	if(0!=err){
		cout << "WSAStartup error" << endl;
		return 1;
	}

	//判断库的版本号对不对
	//函数调用成功了但是库的版本号可能不对，因此需要判断版本号是否正确
	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion)) {
		cout << "WSAStartup version error " << endl;
		//卸载库
		WSACleanup();
		return 1;
	}
	else {
		cout << "WSAStartup success" << endl;
	}


	//2.创建套接字（通信的基石，得先有套接字后面才能用）

	SOCKET sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sock) {
		cout << "socket error" << "  " << WSAGetLastError() << endl;
		//卸载库
		WSACleanup();
		return 1;
	}
	else {
		cout << "socket succuss" << endl;
	}

	//3.绑定网卡和端口号，（告诉系统可以接收发给哪个网卡和端口号的数据）
	//告诉操作系统，当前进程使用的是什么类型的ip地址，哪个端口号，哪个ip
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(67890);//数字大的端口号无人使用//端口 涉及大小端问题 规定网络字节序 转换成大端
	addr.sin_addr.S_un.S_addr = INADDR_ANY;//地址 绑定任意（所有）网卡 
	//ip地址分两种类型：字符串类型 十进制四等分类型"192.168.3.222"（方便用户看） ulong类型（更长）
	//两种类型相互转换 字符串类型转 ulong: inet_addr()
	//ulong转字符串： inet_ntoa() 实际上是in_addr转字符串
	err=bind(sock, (sockaddr*)&addr, sizeof(addr));//指针和指针才能强转
	if (SOCKET_ERROR == err) {
		cout << "bind error" << WSAGetLastError() << endl;//只有网络操作的时候WSAGetLastError可用
		closesocket(sock);//一个进程可以有多个套接字，需要指定关闭哪个套接字
		WSACleanup();
		return 1;
	}
	else {
		cout << "bind success" << endl;
	}
	//传输数据:发送和接收数据,是一个循环
	//先接收数据的是服务端

	int nRecvNum = 0;//局部变量定义在循环的外面 提高效率 避免反复的申请空间
	//减少申请空间的时间
	int nSendNum = 0;
	char recvBuf[9999] = "";
	char sendBuf[9999] = "";
	sockaddr_in addrClient;
	int addrClientSize = sizeof(addrClient);
	while (true) {
		//4.接收数据（阻塞函数 有一步再进行下一步）
		//接收使用的套，接收到的数据，数据的大小，接收的方式，接收方的地址，地址的长度
		nRecvNum=recvfrom(sock,recvBuf,sizeof(recvBuf),0,(sockaddr*)&addrClient,&addrClientSize);
		if (nRecvNum > 0) {
			//打印接收到的数据 接收的ip地址ulong转char
			cout << "ip:"<< inet_ntoa(addrClient.sin_addr)<<" say:" << recvBuf << endl;
		}
		else {
			cout << "recv error" << WSAGetLastError() << endl;
			break;
		}
		//5.发送数据
		gets_s(sendBuf);
		nSendNum=sendto(sock,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrClient,addrClientSize);
		if (SOCKET_ERROR==nSendNum) {
			cout << "send error" << WSAGetLastError() << endl;
			break;

		}
		else{
			cout << "send success" << endl;
		}
	}
	//6.关闭套接字
	closesocket(sock);

	//8.卸载库
	WSACleanup();


	return 0;
	//inet_nota 函数太老系统不适配，关闭SDL 安全检查
}