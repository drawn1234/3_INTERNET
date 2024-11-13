#include<iostream>
#include<winsock2.h>
using namespace std;
#pragma comment(lib,"Ws2_32.lib")
int main() {
//1 加载库
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
//2创建套接字
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
	
	/*阻塞：
		 没有接受到数据，就不运行接收函数。
		 像这类必须要发生事件才能继续往下进行的函数称为阻塞函数
		 例：老王烧水，水壶没开，就一直等，直到水壶开才去做别的。
		 
		 非阻塞函数：
		 例：老王烧水，水壶没开，老王去做别的，每隔一段时间回来看水壶开没开
		 
		 
		 实质：
		 socket是阻塞的，和函数没有关系。
		 关系socket使用的方式，是阻塞的还是非阻塞的。
		 socket默认是阻塞的。*/
		 
	//设置socket是非阻塞模式
	/*u_long iMode = 1;
	ioctlsocket(sock,FIONBIO,&iMode);*/

	//获取缓冲区大小
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
		
		//3循环接受数据
		int recvNum = recvfrom(sock,recvBuf,recvBufsize,0,(sockaddr*)&addrTo,&addrToLen);
		//&&10035!= WSAGetLastError()
		if (recvNum<0) {
			cout << "recv erro" << WSAGetLastError() << endl;
			break;
		}
		else {
			cout << "ip:" << inet_ntoa(addrTo.sin_addr) << "  say:" <<recvBuf<< endl;
		}
	////4发送数据
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



//5关闭套接字
	WSACleanup();
//6卸载库
	closesocket(sock);

}