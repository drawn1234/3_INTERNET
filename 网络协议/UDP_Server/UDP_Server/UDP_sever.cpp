#include<iostream>
#include<winsock2.h>
using namespace std;
#pragma comment(lib,"Ws2_32.lib")
int main() {
//1 ���ؿ�
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	int err=WSAStartup(version,&data);
	if (0 != err) {
		cout << "WSAStartup err" << endl;
		return 1;
	}
	else {
		cout << "WSASTartup success" << endl;
	}
	if (2 != HIBYTE(version) || 2 != LOBYTE(version)) {
		cout << "Version err" << endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "Version right" << endl;
	}
//2�����׽���
	SOCKET sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if (INVALID_SOCKET == sock) {
		cout << "socket err " << WSAGetLastError()<<endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "socket success" << endl;
	}
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(56789);
	err = bind(sock,(sockaddr*)&addr,sizeof(addr));
	if (SOCKET_ERROR == err) {
		cout << "bind error" << GetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}else{
		cout << "bind success" << endl;
	
	}
	char sendBuf[9999] = "";
	char recvBuf[9999] = "";
	int sendBuflen = sizeof(sendBuf);
	int recvBufsize = sizeof(recvBuf);
	sockaddr_in addrTo;
	int addrToLen = sizeof(addrTo);
	
	/*������
		 û�н��ܵ����ݣ��Ͳ����н��պ�����
		 ���������Ҫ�����¼����ܼ������½��еĺ�����Ϊ��������
		 ����������ˮ��ˮ��û������һֱ�ȣ�ֱ��ˮ������ȥ����ġ�
		 
		 ������������
		 ����������ˮ��ˮ��û��������ȥ����ģ�ÿ��һ��ʱ�������ˮ����û��
		 
		 
		 ʵ�ʣ�
		 socket�������ģ��ͺ���û�й�ϵ��
		 ��ϵsocketʹ�õķ�ʽ���������Ļ��Ƿ������ġ�
		 socketĬ���������ġ�*/
		 
	//����socket�Ƿ�����ģʽ
	/*u_long iMode = 1;
	ioctlsocket(sock,FIONBIO,&iMode);*/

	//��ȡ��������С
	int bufLen;
	int optval;
	int optvalLen = sizeof(optval);
	err=getsockopt(sock,SOL_SOCKET, SO_RCVBUF,(char*)&optval,&optvalLen);
	if (SOCKET_ERROR == err) {
		cout << "getsockopt  recv error" << GetLastError() << endl;
		return 1;
	}
	else {
		cout << optval << " getsockopt recv  success" << endl;
	}
	int bufLenSend;
	int optvalSend;
	int optvalLenSend = sizeof(optvalSend);
	err = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&optvalSend, &optvalLenSend);
	if (SOCKET_ERROR == err) {
		cout << "getsockopt send  error" << GetLastError() << endl;
		return 1;
	}
	else {
		cout << optvalSend << " getsockopt send success" << endl;
	}
	
	while (true) {
		
		//3ѭ����������
		int recvNum = recvfrom(sock,recvBuf,recvBufsize,0,(sockaddr*)&addrTo,&addrToLen);
		//&&10035!= WSAGetLastError()
		if (recvNum<0) {
			cout << "recv erro" << WSAGetLastError() << endl;
			break;
		}
		else {
			cout << "ip:" << inet_ntoa(addrTo.sin_addr) << "  say:" <<recvBuf<< endl;
		}
	////4��������
	//	gets_s(sendBuf);
	//	int sendNum = sendto(sock, sendBuf, sendBuflen, 0, (sockaddr*)&addrTo, sizeof(addrTo));
	//	if (SOCKET_ERROR == sendNum) {
	//		cout << "send error" << WSAGetLastError() << endl;
	//		break;
	//	}
	//	else {
	//		cout << "send success" << endl;
	//	}
	}



//5�ر��׽���
	WSACleanup();
//6ж�ؿ�
	closesocket(sock);

}