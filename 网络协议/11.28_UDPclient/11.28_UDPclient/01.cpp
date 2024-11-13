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
	int err = WSAStartup(version, &data);
	if (err != 0) {
		cout << "WSAStartup erroor" << endl;
		return 1;
	}
	//判断加载库的版本号是否正确

	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion))
	{
		cout << "WSAStartup version is erro" << endl;
		WSACleanup();
		return 1;
	}
	else
	{
		cout << "WSAStartup version success" << WSAGetLastError() << endl;
	}
	//2.创建套接字

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sock)
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




	char rev[426] = "";
	char sen[426] = "";

	sockaddr_in adclient;
	adclient.sin_addr.S_un.S_addr = inet_addr("192.168.134.1");
	adclient.sin_family = AF_INET;//adress family 和前面套接字的应该一样
	adclient.sin_port = htons(67890);//htons 转换成网络字节序

	int adcllen = sizeof(adclient);
	//int adserlen = sizeof(adclient);
	

		//广播
		// 直接广播
	//adclient.sin_addr.S_un.S_addr = inet_addr("192.168.207.255");
	//adclient.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
	////申请有限广播权限
	//bool val = true;
	//int size = sizeof(val);
	//setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&val, size);

	while (true)
	{		//3发送数据
		gets_s(sen);
		int nSendNum = sendto(sock, sen, sizeof(sen), 0, (sockaddr*)&adclient, adcllen);
		if (SOCKET_ERROR == nSendNum)
		{
			cout << "send erro" << GetLastError() << endl;
			break;
		}
		else
		{
			cout << "send success" << endl;
		}

		//4接收数据
		int back = recvfrom(sock, rev, sizeof(rev), 0, (sockaddr*)&adclient, &adcllen);
		if (back > 0)
		{
			//打印接收到的数据
			cout << "server say:" << rev << endl;
		}
		else {
			cout << "recvfrom erro" << GetLastError() << endl;

			break;
		}

	}
	//6.删除套接字
	closesocket(sock);
	//7.删除库
	WSACleanup();
	return 0;
}