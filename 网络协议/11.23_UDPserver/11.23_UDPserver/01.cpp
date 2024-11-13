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
	int err=WSAStartup(version,&data);
	if (err != 0) {
		cout << "WSAStartup erroor" << endl;
		return 1;
	}
	//�жϼ��ؿ�İ汾���Ƿ���ȷ

	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion))
	{
		cout << "WSAStartup version is erro" << WSAGetLastError() <<endl;
		WSACleanup();
		return 1;
	}
	else
	{
		cout << "WSAStartup version success" <<endl;
	}
	//2.�����׽���

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET==sock)
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

	//3�������Ͷ˿�(���߲���ϵͳ�ܹ����շ����ĸ������Ͷ˿ںŵ�����)
	sockaddr_in sockad;
	sockad.sin_addr.S_un.S_addr = INADDR_ANY;//����������
	//ip�ĵ�ַ��Ϊ�������ͣ� u_long����ip��ַ ��ʮ�����ĵȷ��ַ�������"198.168.3.222"
	//�ַ���ǿתΪu_long ��inet_addr()
	//u_longǿתΪ�ַ����� inet_ntoa()
	sockad.sin_family = AF_INET;//adress family ��ǰ���׽��ֵ�Ӧ��һ��
	sockad.sin_port = htons(67890);//htons ת���������ֽ���
	int erro=bind(sock,(sockaddr*) & sockad,sizeof(sockaddr));
	if (SOCKET_ERROR == erro)
	{
		cout << "bind erro" << WSAGetLastError() << endl;
		//�ر��׽���
		closesocket(sock);
		//ɾ����
		WSACleanup();
		return 1;
			
	}
	else
	{
		cout << "bind success" << endl;
	}
	int  ;
	char revbuf[426]="";
	char senbuf[426]="";

	sockaddr_in adclient;

	int adclilen = sizeof(adclient);
	//int adserlen = sizeof(adserver);

	//��Ϊsocket�������ģ����Խ��պͷ��Ͷ���������
	//�����ͷ�����
	//������������ˮ����ˮ������¯���ϣ�һֱ���� ->һֱռ��cpu
	//��������������ˮ����ˮ������¯���ϣ���һ��ʱ�俴->cpu���Ըɱ��
	
	//socketĬ����������
	//���÷��������ǽ�socket����Ϊ��������

	//���ͺ����������ͷ�����
	//���� �����ͻ������ռ䲻�㹻���ʱ����Ҫ�ȵ��ռ��㹻���ܷ���
	//������ ���ռ䲻�㹻���ʱ���ж��ٿռ䣬�ͷ��Ͷ�������
	//ʣ��������ɳ������д���
	//����Ҫ�жϷ������ݴ�С�Ƿ�С��ʵ�����ݵĴ�С��

	////cmd:���� arg������
	//u_long iMode = 1;
	//int erro1=ioctlsocket(sock, FIONBIO, &iMode);
	//if (SOCKET_ERROR == erro1) {
	//	cout << "set socket non_blocking mode fail" << GetLastError << endl;
	//}
	//else {
	//	cout << "set socket non_blocking mode success" << endl;
	//}
	////������10035 �޷��������һ������ֹ���׽��ֲ��� û�н��յ�����
	////����Ԥ��

	//������
	//ÿ����һ��socket������ϵͳ�����һ�����ջ�������һ�����ͻ�����
	//�������ݾ��Ǵӽ��ջ������п������ݵ����̣��������ݾ��Ǵӽ��̿��������ͻ�����
	//һ������0-4G 2-4G�û��ռ�
	//һ��Ӧ�ó����һ�����ݰ�����СΪ65536��������ݰ���С����65536�ᷢ������
	int nRcvBufSize = 0;
	int nSenBufSize = 0;
	int size = sizeof(nRcvBufSize);
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&nRcvBufSize, &size);
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&nSenBufSize, &size);
	cout << "nRcvBufSize:" << nRcvBufSize << "nSenBufSize:" << nSenBufSize << endl;
	while (true)
	{
		//4��������
		int back=recvfrom(sock,revbuf,sizeof(revbuf),0,(sockaddr*)&adclient,&adclilen);
		if (back>0)
		{
			//��ӡ���յ�������
			cout << "ip: " << inet_ntoa(adclient.sin_addr) << revbuf << endl;
		}
		else if(10035!= GetLastError()){
			//���ý��պ���������û�н��յ�����Ҳ��Ȼ����
			cout << "recvfrom erro" << GetLastError() << endl;

			break;
		}
		//5��������
		gets_s(senbuf);
		int nSendNum = sendto(sock, senbuf,sizeof(senbuf),0,(sockaddr*)&adclient, adclilen);
		if (SOCKET_ERROR==nSendNum)
		{
			cout << "send erro" << GetLastError() << endl;
			break;
		}
		else
		{
			cout << "sen success" << endl;
		}

	}
	//6.ɾ���׽���
	closesocket(sock);
	//7.ɾ����
	WSACleanup();
	return 0;
}