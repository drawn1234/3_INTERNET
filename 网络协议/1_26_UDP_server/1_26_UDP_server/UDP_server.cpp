#include<iostream>
//ͷ�ļ����ڵ�ǰ�������<> �ڵ�ǰ�������" "
#include<winsock2.h >
//����������
#pragma comment(lib,"Ws2_32.lib")//�������� lib-library
using namespace std;

int main()
{
	//1.���ؿ⣨ʹ�ÿ⺯����/��̬�� ���ͷ�ļ������������⣨lib+������
	
	// ���ؿ��൱�ڰ�exe���뵽
	//��̬�� ���ͷ�ļ������������� ��̬��dll��exe����һ��Ȼ�������
	//W-windows S-socket A-api Windows�������Socket��һ���ӿ�
	WORD version = MAKEWORD(2, 2);//���������Ҫ��ֵ
	WSADATA data;//�����������Ҫ��ֵ
	int err = WSAStartup(version,&data);//δ�����ʶ�� û�е����
	//ʵ���ڿ�����ʵ�� ������Ҫ����ͷ�ļ�������Ҫ����������
	
	//����������������Ҫ�жϷ���ֵ
	//�жϺ��������Ƿ�ɹ� �ɹ��ͼ�����ʧ�ܾ�ֹͣ return
	if(0!=err){
		cout << "WSAStartup error" << endl;
		return 1;
	}

	//�жϿ�İ汾�ŶԲ���
	//�������óɹ��˵��ǿ�İ汾�ſ��ܲ��ԣ������Ҫ�жϰ汾���Ƿ���ȷ
	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion)) {
		cout << "WSAStartup version error " << endl;
		//ж�ؿ�
		WSACleanup();
		return 1;
	}
	else {
		cout << "WSAStartup success" << endl;
	}


	//2.�����׽��֣�ͨ�ŵĻ�ʯ���������׽��ֺ�������ã�

	SOCKET sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sock) {
		cout << "socket error" << "  " << WSAGetLastError() << endl;
		//ж�ؿ�
		WSACleanup();
		return 1;
	}
	else {
		cout << "socket succuss" << endl;
	}

	//3.�������Ͷ˿ںţ�������ϵͳ���Խ��շ����ĸ������Ͷ˿ںŵ����ݣ�
	//���߲���ϵͳ����ǰ����ʹ�õ���ʲô���͵�ip��ַ���ĸ��˿ںţ��ĸ�ip
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(67890);//���ִ�Ķ˿ں�����ʹ��//�˿� �漰��С������ �涨�����ֽ��� ת���ɴ��
	addr.sin_addr.S_un.S_addr = INADDR_ANY;//��ַ �����⣨���У����� 
	//ip��ַ���������ͣ��ַ������� ʮ�����ĵȷ�����"192.168.3.222"�������û����� ulong���ͣ�������
	//���������໥ת�� �ַ�������ת ulong: inet_addr()
	//ulongת�ַ����� inet_ntoa() ʵ������in_addrת�ַ���
	err=bind(sock, (sockaddr*)&addr, sizeof(addr));//ָ���ָ�����ǿת
	if (SOCKET_ERROR == err) {
		cout << "bind error" << WSAGetLastError() << endl;//ֻ�����������ʱ��WSAGetLastError����
		closesocket(sock);//һ�����̿����ж���׽��֣���Ҫָ���ر��ĸ��׽���
		WSACleanup();
		return 1;
	}
	else {
		cout << "bind success" << endl;
	}
	//��������:���ͺͽ�������,��һ��ѭ��
	//�Ƚ������ݵ��Ƿ����

	int nRecvNum = 0;//�ֲ�����������ѭ�������� ���Ч�� ���ⷴ��������ռ�
	//��������ռ��ʱ��
	int nSendNum = 0;
	char recvBuf[9999] = "";
	char sendBuf[9999] = "";
	sockaddr_in addrClient;
	int addrClientSize = sizeof(addrClient);
	while (true) {
		//4.�������ݣ��������� ��һ���ٽ�����һ����
		//����ʹ�õ��ף����յ������ݣ����ݵĴ�С�����յķ�ʽ�����շ��ĵ�ַ����ַ�ĳ���
		nRecvNum=recvfrom(sock,recvBuf,sizeof(recvBuf),0,(sockaddr*)&addrClient,&addrClientSize);
		if (nRecvNum > 0) {
			//��ӡ���յ������� ���յ�ip��ַulongתchar
			cout << "ip:"<< inet_ntoa(addrClient.sin_addr)<<" say:" << recvBuf << endl;
		}
		else {
			cout << "recv error" << WSAGetLastError() << endl;
			break;
		}
		//5.��������
		gets_s(sendBuf);
		nSendNum=sendto(sock,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrClient,addrClientSize);
		if (SOCKET_ERROR==nSendNum) {
			cout << "send error" << WSAGetLastError() << endl;
			break;

		}
		else{
			cout << "send success" << endl;
		}
	}
	//6.�ر��׽���
	closesocket(sock);

	//8.ж�ؿ�
	WSACleanup();


	return 0;
	//inet_nota ����̫��ϵͳ�����䣬�ر�SDL ��ȫ���
}