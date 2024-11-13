#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

int main()
{
	//1.���ؿ�
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

	//2.�����׽���
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
	//3.����
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
		//4.������Ϣ
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

		//5.������Ϣ
		err=recv(sock, recvbuf, rbufsize,0);
		if (err>0){
			cout << "server say:" << recvbuf << endl;
		}
		else {
			cout << "recv error" << GetLastError() << endl;
			break;
		}
	}

	//6.�ر��׽���
	closesocket(sock);

	//7.ж�ؿ�
	WSACleanup();

	return 0;
}
