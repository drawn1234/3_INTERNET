#include<iostream>
#include<Winsock2.h>

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

int main() {
	//加载库
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	int err = WSAStartup(version,&data);
	if (0 != err) {
		cout << "WSAStartup error" << endl;
		return 1;
	}
	if (HIBYTE(version) != 2 || LOBYTE(version) != 2) {
		cout << "verion wrong!" << endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "WSAStartup success"<<endl;
	}
	//创建套接字
	SOCKET sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if (INVALID_SOCKET == sock) {
		cout << "socket error" << WSAGetLastError()<<endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "socket success" << endl;
	}
	//填写地址
	char sendBUf[9999] = "";
	char recvBuf[9999] = "";
	int sendLen = sizeof(sendBUf);
	int recvLen = sizeof(recvBuf);
	sockaddr_in addrTo;
	int addrToLen = sizeof(addrTo);
	addrTo.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons(56789);

	//直接广播
	addrTo.sin_addr.S_un.S_addr = inet_addr("192.168.0.255");

	BOOL broadCastVal = TRUE;
	//发广播
	//有限广播 1.填写有限广播地址
	addrTo.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
	//2.申请有限广播权限
	err = setsockopt(sock, SOL_SOCKET, SO_BROADCAST,(char*)&broadCastVal,sizeof(broadCastVal));
	while (true) {
		////发送数据
		gets_s(sendBUf);
		int senNum = sendto(sock,sendBUf,sendLen,0,(sockaddr*) & addrTo,sizeof(addrTo));
		if (SOCKET_ERROR == senNum) {
			cout << "send error" << GetLastError() << endl;
			break;
		}
		//接收数据
		//int recvNum = recvfrom(sock,recvBuf,recvLen,0,(sockaddr*)&addrTo,&addrToLen);
		/*int recvNum = recvfrom(sock, recvBuf, recvLen, 0,nullptr,nullptr);
		if (recvNum<0) {
			cout << "recv error" << GetLastError() << endl;
			break;
		}
		cout << "server say:" << recvBuf << endl;*/
	}
	//关闭套接字
	closesocket(sock);
	//卸载库
	WSACleanup();
}