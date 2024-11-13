#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include <winsock2.h>

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

int main(){
	//加载库
	WORD version = MAKEWORD(2, 2);
	WSADATA data = {};
	int err1 = WSAStartup(version, &data);
	if (0 != err1) {
		cout << "WSAStartup error" << endl;
		return 1;
	}
	else {
		cout << "WSAStartup success" << endl;
	}
	if (2 != HIBYTE(version) || 2 != LOBYTE(version)) {
		cout << "version error" << endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "version success" << endl;
	}
	//创建套接字
	SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (INVALID_SOCKET == sock) {
		cout << "socket error：" << WSAGetLastError() << endl;
	}
	else {
		cout << "socket success" << endl;
	}
	//绑定ip和端口号
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(54321);
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	err1 = bind(sock, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == err1) {
		cout << "bind error" << WSAGetLastError() << endl;
	}
	else {
		cout << "bind success" << endl;
	}
	//监听
	int err=listen(sock, SOMAXCONN);
	if (SOCKET_ERROR == err){
		cout<<"listen error"<<GetLastError()<<endl;
		closesocket(sock);
		WSACleanup();
		return 1;
		}
	else {
		cout << "listen success" << endl;
	}
	//接收连接
	// 如果有多个客户端连接，会产生多个socket，客户端和socket是一一对应关系
	// 其数据结构保存在map中
	// (连接成功返回的socket就是用来跟当前来连接到客户端进行通信的)
	//只能先监听，后连接
	sockaddr_in addrTo;
	int addrToSize = sizeof(addrTo);
	SOCKET sockTalk=accept(sock,(sockaddr*)&addrTo,&addrToSize);
	if (INVALID_SOCKET==sockTalk) {
		cout << "accept error" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "accept success ip:" << inet_ntoa(addrTo.sin_addr) << endl;
	}
	char recvBuf[9999] = "";
	int recvNum;
	char sendBuf[9999] = "";
	int sendNum;
	while(true) {
		//接收数据
		recvNum=recv(sockTalk,recvBuf,sizeof(recvBuf),0);
		if (recvNum > 0) {
			cout << "Client say:" << recvBuf << endl;
		}
		else {
			cout << "recv error" << WSAGetLastError() << endl;
			break;
		}
		//发送数据
		gets_s(sendBuf);
		//cin >> sendBuf;
		sendNum=send(sockTalk,sendBuf,sizeof(sendBuf),0);
		if (sendNum > 0) {
			cout << "send success" << endl;
		}
		else {
			cout << "send error" << WSAGetLastError() << endl;
			break;
		}
	}
	//卸载库
	closesocket(sock);
	closesocket(sockTalk);
	WSACleanup();
	return 0;
}