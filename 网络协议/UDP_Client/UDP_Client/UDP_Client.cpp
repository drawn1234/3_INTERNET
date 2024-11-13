#include<iostream>
#include<Winsock2.h>

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

int main() {
	//���ؿ�
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
	//�����׽���
	SOCKET sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if (INVALID_SOCKET == sock) {
		cout << "socket error" << WSAGetLastError()<<endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "socket success" << endl;
	}
	//��д��ַ
	char sendBUf[9999] = "";
	char recvBuf[9999] = "";
	int sendLen = sizeof(sendBUf);
	int recvLen = sizeof(recvBuf);
	sockaddr_in addrTo;
	int addrToLen = sizeof(addrTo);
	addrTo.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons(56789);

	//ֱ�ӹ㲥
	addrTo.sin_addr.S_un.S_addr = inet_addr("192.168.0.255");

	BOOL broadCastVal = TRUE;
	//���㲥
	//���޹㲥 1.��д���޹㲥��ַ
	addrTo.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
	//2.�������޹㲥Ȩ��
	err = setsockopt(sock, SOL_SOCKET, SO_BROADCAST,(char*)&broadCastVal,sizeof(broadCastVal));
	while (true) {
		////��������
		gets_s(sendBUf);
		int senNum = sendto(sock,sendBUf,sendLen,0,(sockaddr*) & addrTo,sizeof(addrTo));
		if (SOCKET_ERROR == senNum) {
			cout << "send error" << GetLastError() << endl;
			break;
		}
		//��������
		//int recvNum = recvfrom(sock,recvBuf,recvLen,0,(sockaddr*)&addrTo,&addrToLen);
		/*int recvNum = recvfrom(sock, recvBuf, recvLen, 0,nullptr,nullptr);
		if (recvNum<0) {
			cout << "recv error" << GetLastError() << endl;
			break;
		}
		cout << "server say:" << recvBuf << endl;*/
	}
	//�ر��׽���
	closesocket(sock);
	//ж�ؿ�
	WSACleanup();
}