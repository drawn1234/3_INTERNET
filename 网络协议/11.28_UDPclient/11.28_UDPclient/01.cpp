#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
//��������
#pragma comment(lib,"Ws2_32.lib")
using namespace std;
int main()
{
	//1.���ؿ�
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	int err = WSAStartup(version, &data);
	if (err != 0) {
		cout << "WSAStartup erroor" << endl;
		return 1;
	}
	//�жϼ��ؿ�İ汾���Ƿ���ȷ

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
	//2.�����׽���

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sock)
	{
		cout << "socket erro" << endl;
		//ɾ����
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
	adclient.sin_family = AF_INET;//adress family ��ǰ���׽��ֵ�Ӧ��һ��
	adclient.sin_port = htons(67890);//htons ת���������ֽ���

	int adcllen = sizeof(adclient);
	//int adserlen = sizeof(adclient);
	

		//�㲥
		// ֱ�ӹ㲥
	//adclient.sin_addr.S_un.S_addr = inet_addr("192.168.207.255");
	//adclient.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
	////�������޹㲥Ȩ��
	//bool val = true;
	//int size = sizeof(val);
	//setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&val, size);

	while (true)
	{		//3��������
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

		//4��������
		int back = recvfrom(sock, rev, sizeof(rev), 0, (sockaddr*)&adclient, &adcllen);
		if (back > 0)
		{
			//��ӡ���յ�������
			cout << "server say:" << rev << endl;
		}
		else {
			cout << "recvfrom erro" << GetLastError() << endl;

			break;
		}

	}
	//6.ɾ���׽���
	closesocket(sock);
	//7.ɾ����
	WSACleanup();
	return 0;
}