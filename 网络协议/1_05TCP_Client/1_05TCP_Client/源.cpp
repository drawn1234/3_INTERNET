#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<winsock2.h>

#pragma comment(lib,"Ws2_32.lib")
using namespace std;
int main() {
	//加载库
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	int err=WSAStartup(version, &data);
	if (0 != err) {
		cout << "WSAStartup error" << endl;
		return 1;
	}
	else {
		cout << "WSAStartup success" << endl;
	}
	if (HIBYTE(version) != 2 || LOBYTE(version) != 2) {
		cout << "version error" << endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "version right" << endl;
	}
	//创建套接字
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock) {
		cout << "socket error" << WSAGetLastError() << endl;
		WSACleanup();
	}
	else {
		cout << "socket success" << endl;
	}
	//连接服务端
	//SOCKET sockTalk=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(54321);
	addr.sin_addr.S_un.S_addr=inet_addr("192.168.134.1");
	int n=connect(sock,(sockaddr*)&addr,sizeof(addr));
	if (SOCKET_ERROR  == n) {
		cout<< "connect error" << WSAGetLastError() <<endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "connect success" << endl;
	}

	int sendNum;
	char sendBuf = {};
	int recvNum;
	char recvBuf = {};
	while (true) {
		//发送数据
		cin >> sendBuf;
		sendNum=send(sock,&sendBuf,sizeof(sendBuf),0);
		if (SOCKET_ERROR != err) {
			cout << "send success" << endl;
		}
		else {
			cout << "send error" << WSAGetLastError() << endl;
			break;
		}
		//接收数据
		recvNum = recv(sock, &recvBuf, sizeof(recvBuf), 0);
		if (recvNum > 0) {
			cout << "server say:" << recvBuf << endl;
		}
		else {
			cout << "recv error" << WSAGetLastError() << endl;
			break;
		}
	}
	//卸载库
	closesocket(sock);
	//closesocket(sockTalk);
	WSACleanup();
	return 0;
}