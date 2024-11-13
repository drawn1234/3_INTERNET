#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
//绑定依赖库
#pragma comment(lib,"Ws2_32.lib")
using namespace std;
int main()
{
	//1.加载库
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	int err=WSAStartup(version,&data);
	if (err != 0) {
		cout << "WSAStartup erroor" << endl;
		return 1;
	}
	//判断加载库的版本号是否正确

	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion))
	{
		cout << "WSAStartup version is erro" << WSAGetLastError() <<endl;
		WSACleanup();
		return 1;
	}
	else
	{
		cout << "WSAStartup version success" <<endl;
	}
	//2.创建套接字

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET==sock)
	{
		cout << "socket erro" << endl;
		//删除库
		WSACleanup();
		return 1;
	}
	else
	{
		cout << "socket success" << endl;
	}

	//3绑定网卡和端口(告诉操作系统能够接收发给哪个网卡和端口号的数据)
	sockaddr_in sockad;
	sockad.sin_addr.S_un.S_addr = INADDR_ANY;//绑定任意网卡
	//ip的地址分为两种类型： u_long类型ip地址 和十进制四等分字符串类型"198.168.3.222"
	//字符串强转为u_long ：inet_addr()
	//u_long强转为字符串： inet_ntoa()
	sockad.sin_family = AF_INET;//adress family 和前面套接字的应该一样
	sockad.sin_port = htons(67890);//htons 转换成网络字节序
	int erro=bind(sock,(sockaddr*) & sockad,sizeof(sockaddr));
	if (SOCKET_ERROR == erro)
	{
		cout << "bind erro" << WSAGetLastError() << endl;
		//关闭套接字
		closesocket(sock);
		//删除库
		WSACleanup();
		return 1;
			
	}
	else
	{
		cout << "bind success" << endl;
	}
	int  ;
	char revbuf[426]="";
	char senbuf[426]="";

	sockaddr_in adclient;

	int adclilen = sizeof(adclient);
	//int adserlen = sizeof(adserver);

	//因为socket是阻塞的，所以接收和发送都是阻塞的
	//阻塞和非阻塞
	//阻塞：老王烧水，把水壶放在炉子上，一直看着 ->一直占用cpu
	//非阻塞：老王烧水，把水壶放在炉子上，隔一段时间看->cpu可以干别的
	
	//socket默认是阻塞的
	//设置非阻塞，是将socket设置为非阻塞的

	//发送函数的阻塞和非阻塞
	//阻塞 当发送缓冲区空间不足够大的时候，需要等到空间足够才能发送
	//非阻塞 当空间不足够大的时候，有多少空间，就发送多少数据
	//剩余的数据由程序自行处理
	//（需要判断发送数据大小是否小于实际数据的大小）

	////cmd:命令 arg：参数
	//u_long iMode = 1;
	//int erro1=ioctlsocket(sock, FIONBIO, &iMode);
	//if (SOCKET_ERROR == erro1) {
	//	cout << "set socket non_blocking mode fail" << GetLastError << endl;
	//}
	//else {
	//	cout << "set socket non_blocking mode success" << endl;
	//}
	////错误码10035 无法立即完成一个非阻止性套接字操作 没有接收到数据
	////符合预期

	//缓冲区
	//每创建一个socket，操作系统会分配一个接收缓冲区和一个发送缓冲区
	//接收数据就是从接收缓冲区中拷贝数据到进程，发送数据就是从进程拷贝到发送缓冲区
	//一个进程0-4G 2-4G用户空间
	//一个应用程序的一个数据包最大大小为65536，如果数据包大小大于65536会发生丢包
	int nRcvBufSize = 0;
	int nSenBufSize = 0;
	int size = sizeof(nRcvBufSize);
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&nRcvBufSize, &size);
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&nSenBufSize, &size);
	cout << "nRcvBufSize:" << nRcvBufSize << "nSenBufSize:" << nSenBufSize << endl;
	while (true)
	{
		//4接收数据
		int back=recvfrom(sock,revbuf,sizeof(revbuf),0,(sockaddr*)&adclient,&adclilen);
		if (back>0)
		{
			//打印接收到的数据
			cout << "ip: " << inet_ntoa(adclient.sin_addr) << revbuf << endl;
		}
		else if(10035!= GetLastError()){
			//不让接收函数阻塞，没有接收到数据也依然运行
			cout << "recvfrom erro" << GetLastError() << endl;

			break;
		}
		//5发送数据
		gets_s(senbuf);
		int nSendNum = sendto(sock, senbuf,sizeof(senbuf),0,(sockaddr*)&adclient, adclilen);
		if (SOCKET_ERROR==nSendNum)
		{
			cout << "send erro" << GetLastError() << endl;
			break;
		}
		else
		{
			cout << "sen success" << endl;
		}

	}
	//6.删除套接字
	closesocket(sock);
	//7.删除库
	WSACleanup();
	return 0;
}