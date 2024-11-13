#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include <winsock2.h>

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

int main(){
	//���ؿ�
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
	//�����׽���
	SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (INVALID_SOCKET == sock) {
		cout << "socket error��" << WSAGetLastError() << endl;
	}
	else {
		cout << "socket success" << endl;
	}
	//��ip�Ͷ˿ں�
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
	//����
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
	//��������
	// ����ж���ͻ������ӣ���������socket���ͻ��˺�socket��һһ��Ӧ��ϵ
	// �����ݽṹ������map��
	// (���ӳɹ����ص�socket������������ǰ�����ӵ��ͻ��˽���ͨ�ŵ�)
	//ֻ���ȼ�����������
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
		//��������
		recvNum=recv(sockTalk,recvBuf,sizeof(recvBuf),0);
		if (recvNum > 0) {
			cout << "Client say:" << recvBuf << endl;
		}
		else {
			cout << "recv error" << WSAGetLastError() << endl;
			break;
		}
		//��������
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
	//ж�ؿ�
	closesocket(sock);
	closesocket(sockTalk);
	WSACleanup();
	return 0;
}