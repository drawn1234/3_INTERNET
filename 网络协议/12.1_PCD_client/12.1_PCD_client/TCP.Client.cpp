#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

int main()
{
	//1.加载库
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	int err=WSAStartup(version,&data);
	if (0==err)
	{
		cout << "WSAStartup success" << endl;
	}
	else {
		cout << "WSAStartup error," << err << endl;
		return 1;
	}
	if (2!=HIBYTE(data.wVersion)||2!=LOBYTE(data.wVersion))
	{
		cout << "verion error" << endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "version success" << endl;
	}

	//2.创建套接字
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET==sock)
	{
		cout << "socket error" << GetLastError() << endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "socket success" << endl;
	}
	//3.连接
	sockaddr_in adclt;
	int adcltlen=sizeof(adclt);
	adclt.sin_addr.S_un.S_addr = inet_addr("192.168.134.1");
	adclt.sin_family = AF_INET;
	adclt.sin_port = htons(67890);
	err=connect(sock,(sockaddr*)&adclt,adcltlen);
	if (SOCKET_ERROR==err) {
		cout << "connect error" <<GetLastError()<< endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "connect success" << endl;
	}

	char sendbuf[4086] = "";
	char recvbuf[4086] = "";
	int sbufsize = sizeof(sendbuf);
	int rbufsize = sizeof(recvbuf);
	while(true)
	{
		//4.发送消息
		gets_s(sendbuf);
		int err=send(sock,sendbuf,sbufsize,0);
		if (SOCKET_ERROR==err)
		{
			cout << "send error" << GetLastError() << endl;
			break;
		}
		else {
			cout << "send success"<< endl;
		}

		//5.接收消息
		err=recv(sock, recvbuf, rbufsize,0);
		if (err>0){
			cout << "server say:" << recvbuf << endl;
		}
		else {
			cout << "recv error" << GetLastError() << endl;
			break;
		}
	}

	//6.关闭套接字
	closesocket(sock);

	//7.卸载库
	WSACleanup();

	return 0;
}
