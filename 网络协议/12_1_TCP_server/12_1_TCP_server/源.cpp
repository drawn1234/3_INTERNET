#include<winsock2.h>
#include<iostream>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

int main()
{
	//1.���ؿ�
	WORD version = MAKEWORD(2, 2);
	WSAData data;
	int erro= WSAStartup(version, &data);
	if (0 == erro)
	{
		cout << "WSAStart success" << endl;
	}
	else { cout << "WSASart erro" << endl; }
	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion))
	{
		cout << "version erro" << endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "WSAstar version is success" << endl;
	}
	//2.�����׽���
	SOCKET sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock)
	{
		cout << "socket erro" << GetLastError() << endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << " socket success"<< endl;
	}

	//3.��ip��ַ
	sockaddr_in addrserver;
	addrserver.sin_family = AF_INET;
	addrserver.sin_port = htons(67890);
	addrserver.sin_addr.S_un.S_addr =INADDR_ANY;
	//addrserver.sin_addr.S_un.S_addr = inet_addr("192.168.3.132");
	erro=bind(sock,(sockaddr*)&addrserver,sizeof(addrserver));
	if (SOCKET_ERROR == erro)
	{
		cout << "bind erro" <<GetLastError()<< endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "bind success" << endl;
	}

	//4.����
	int err=listen(sock, SOMAXCONN);
	if (SOCKET_ERROR==err) {
		cout << " listen error" << GetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	else { cout << "listen success" << endl; }
	
	char recvbuf[4086] = "";
	char senbuf[4086] = "";

	sockaddr_in addrclient;
	int adclientLen=sizeof(addrclient);
	while(true)
	{
		//5.��������
		SOCKET socktalk = accept(sock,(sockaddr*)&addrclient,&adclientLen);
		if (INVALID_SOCKET == socktalk) {
			cout << "accept erro" << GetLastError() << endl;
			break;
		}
		else {
			//�ͻ��˽��յ���ַ����ɹ�
			cout << "addrclient ip:" << inet_ntoa(addrclient.sin_addr) << endl;
		}
		while (true)
		{
			//6.������Ϣ
			int nrecvNum=recv(socktalk,recvbuf,sizeof(recvbuf),0);
			if (nrecvNum>0)
			{
				cout << "client say:" << recvbuf<<endl;

			}
			else
			{
				cout << "recv erro" << GetLastError() << endl;
				break;
			}
			//7.������Ϣ
			gets_s(senbuf);
			int nsenNum=send(socktalk,senbuf,sizeof(senbuf),0);
			if (SOCKET_ERROR == nsenNum)
			{
				cout << "send error" << GetLastError() << endl;
				break;
			}
		}
		closesocket(socktalk);
	}


	//7.�ر��׽���
	closesocket(sock);

	//8.ж�ؿ�
	WSACleanup();


	return 0;
}