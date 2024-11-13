#include "Udp.h"
#include"../mediator/UdpMediator.h"

using namespace std;
Udp::Udp(INetMediator* p) :m_handle(nullptr) {
	m_pMediator = p;
}
Udp::~Udp() {

}

//�������Ա������д��
unsigned __stdcall recvThread(void* lpvoid)
{
	//��ǰ�Ķ��� ������void* ʹ��this���ã���Ϊ��UDP�Ķ�����Ҫǿת
	// UDP����������ʱ�����һ������
	//����½�һ�����󣬸ö�����UDP��Ķ�����û�г�ʼ��
	//��Ҫ�ڳ�ʼ����ʱ��ͽ�������
	Udp* pThis = (Udp*)lpvoid;
	pThis->recvData();
	return 1;
}
//��ʼ������
bool Udp::initNet() {
	//���ؿ�
	//ħ�����֣���������������ѵ�����
	//���������ɺ�
	//������⣬�ദʹ�ñ����޸�
	//�������ֵĵط�����ɺ� �½�һ���ļ�����
	WORD version = MAKEWORD(DEF_VERSION_HIGH, DEF_VERSION_LOW);
	WSADATA data;
	int err = WSAStartup(version, &data);
	if (0 != err) {
		cout << "WSAStartup error" << endl;
		return false;//bool���ͷ���false
	}
	else {
		cout << "WSAStartup success" << endl;
	}
	if (HIBYTE(data.wVersion) != DEF_VERSION_HIGH || LOBYTE(data.wVersion) != DEF_VERSION_LOW) {
		cout << "version error" << endl;
		return false;
	}
	else {
		cout << "version right" << endl;
	}

	//�����׽���
	//--�������ݣ��������ݶ���Ҫʹ�ã���Ҫ���ñ�������Ϊ��ĳ�Ա����
	//������κγ�Ա�ﶼ����ʹ��
	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_sock) {
		cout << "socket error" << WSAGetLastError() << endl;
		// WSACleanup(); ��ж�ؿ�ĺ���������Ҫж�ؿ�
		return false;
	}
	else {
		cout << "socket success" << endl;
	}
	//�������Ͷ˿ں�
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEF_UDP_PORT);
	err = bind(m_sock, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == err) {
		cout << "bind error" << WSAGetLastError() << endl;
		/* closesocket(m_sock);
		 WSACleanup();*/
		return false;
	}
	else {
		cout << "bind success" << endl;
	}

	//�������������߳�
	//createThread��ExistThread��һ�ԣ����ʹ��createThread,�߳̽���ʱ����ϵͳ���Զ����ս����̡߳�
	//����߳���ʹ����c++����ʱ��ĺ�����strcpy������Щ����������ռ䵫�ǲ��ͷ�
	//ExistTread��������ʱҲ���ͷſռ䣬����ڴ�й¶
	//_beginthreadex��_endthreadex��һ�ԣ�_endthreadex�����߳��˳���ʱ���Ȼ��տռ䣬�ڵ���ExistThread
	//����ֵ HANDLE
	m_handle = (HANDLE)_beginthreadex(nullptr/*��ȫ����ʹ��Ĭ�ϰ�ȫ����*/,
		0 /*��ջ��С��ʹ��Ĭ�϶�ջ��С1M,����һ������˿��Դ������ٸ��߳�*/,
		&recvThread/*�̺߳�������ʼ��ַ*/,
		this/*�̺߳����Ĳ����б�*/,
		0 /*��ʼ����־λ,0-���������У�����һ������״̬*/,
		nullptr/*����ϵͳ���̷߳����id���������,����Ҫ�����*/);
	return true;
}

//�������ݵ��̺߳���
// �߳���Ҫ�������飺���ý������ݺ���
//���ǵ�ǰ��ĳ�Ա���������룡

//unsigned __stdcall Udp::recvThread(void* lpvoid)
//{
   // //��ǰ�Ķ��� ������void* ʹ��this���ã���Ϊ��UDP�Ķ�����Ҫǿת
   // // UDP����������ʱ�����һ������
   // //����½�һ�����󣬸ö�����UDP��Ķ�����û�г�ʼ��
   // //��Ҫ�ڳ�ʼ����ʱ��ͽ�������
   // Udp* pThis = (Udp*)lpvoid;
   // pThis->recvData();
   // return 1;
//}



//�������ݣ������߳��һֱѭ���ȴ����գ�
 //���������������ģ������Ҫ�ý������ݵĺ�������-�����߳�
 //ʲôʱ��ʼ�������ݣ� �����������е�ʱ��Ϳ�ʼ��������
 //���������Ͷ˿ںž���Ҫ�����߳̽�������
 //��Ҫһֱѭ���ȴ��������ݣ���һ����ѭ��
 //�������̣߳��������е���initNet������һ���̣߳�recvTread�д�������һ���̣߳���recvData�д�������
 //���ݴ�����Ҫһ��ʱ�䣬����ڴ������ݵĹ������յ��µ�����
 //�µ����ݻὫ�ɵ����ݸ��ǵ�
 //�������recvBuf��Ϊһ��������������Ҫ��������ݿ�����һ���µĿռ��У�����ɵ����ݴ�����ɣ����´����Ŀռ��ͷ�
 // �ɵĿռ����ڽ������µ�����
 //���ݽ��յ����ݵĴ�С����һ���µĿռ��Ȼ��ѽ��ܵ������ݴ����н�����
void Udp::recvData() {
	cout << "Udp::" << __func__ << endl;
	char recvBuf[9999] = "";
	int recvBufSize = sizeof(recvBuf);
	sockaddr_in addr;
	int addrSize = sizeof(addr);
	int nRecvNum = 0;
	while (true) {
		nRecvNum = recvfrom(m_sock, recvBuf, recvBufSize, 0, (sockaddr*)&addr, &addrSize);
		if (nRecvNum > 0) {
			//����һ�����ݰ��ɹ�
			//���ݽ��յ������ݴ�С����һ���¿ռ�
			char* newRecvBuf = new char[nRecvNum];

			//�ѽ��յ������ݿ������µĿռ���
			char* pPack = new char[nRecvNum];
			//����ʹ��strcpy��������ֻ�����ַ���/0���������Ƿ��͵����ݲ�һ�����ַ���
			// ������һ���ṹ�� ��������м���0 �����ݿ����������������ݿ�������
			//ʹ��memcpy ����ָ�������ݳ��ȿ�����Ӧ�÷�Χ��strcpy����
			memcpy(pPack, recvBuf, nRecvNum);

			//TODO���ѽ��յ������ݴ����н�����

			m_pMediator->transmitData(pPack, sizeof(pPack), addr.sin_addr.S_un.S_addr);
			//���Դ��룺��ӡ���յ�������
			//cout << "ip:" << inet_ntoa(addr.sin_addr)<<pPack << endl;

		}
		else {
			cout << "recv error" << WSAGetLastError() << endl;
			break;
		}
	}
}

//��������(udp:ip ulong���ͣ���������˭��TCP��socket uint����������˭)
bool Udp::sendData(char* data, int len, unsigned long to) {
	cout << "Udp::" << __func__ << endl;
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEF_UDP_PORT);
	addr.sin_addr.S_un.S_addr = to;
	int err = sendto(m_sock, data, len, 0, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == err) {
		cout << "send error" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "send success" << endl;
	}
	return true;
}

//�ر����磺������Դ
//�����߳���Դ�����߳̽����������ر��׽��֣�ж�ؿ�
void Udp::unInitNet() {
	//1.�����߳���Դ
	//�����߳�ʱ������ϵͳ��ÿ���̷߳���������Դ 1.��� 2.�߳�ID 3.�ں˶���Ӧ�ó��������Ҫ��ϵͳ��������Ҫͨ���ں˶���
	//ÿ����һ����Դ��ʵ����ʹ�õ��Ǽ���������ÿ����Դ������Ϊ0ʱȫ���Զ���һ
	//��Ҫ�����ü�������Ϊ0
	//1.1�����̹߳���-����recvData���� �ú����˳�ѭ��
	m_isRunning = false;
	if (m_handle) {
		//WaitForSingleObject�ķ���ֵ�������WAIT_TIMEOUT,��˵���ȴ����߳��ڵȴ�ʱ�������,���ڼ�������
		if (WAIT_TIMEOUT == WaitForSingleObject(m_handle/*�ȴ��ĸ��߳�,�����ĸ��߳̾��*/, 1000/*�ȴ���ʱ��,*/))
		{
			//����̻߳��������У�ǿ��ɱ���߳�
			TerminateThread(/*Ҫɱ�����߳�*/m_handle,/*�˳���*/-1);
		}
		//�رվ��
		CloseHandle(m_handle);
		m_handle = nullptr;
	}
	//�������
	//���Ǻ���Ҳ�������̽�������Ҫ�ȴ�������һ�ε��õ�ʱ������˳�ѭ��
	//��һ���������߳� �Ⱦ��-���ǵȶ�Ӧ���߳�
	//��һ��ʱ�䣬�����ʱ��˵���߳��ڵȴ�ʱ�������������

	//2.�ر��׽���
	//����׽��ִ����Ҳ�����Ч���׽��־͹ر�
	if (!m_sock && INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
	}
	//3.ж�ؿ�
	WSACleanup();
	//�������Դ��������ƣ�������ؿ�ɹ��ͼ�1�����ж�ؿ�ͼ�һ��
	//����Ҫ�ж�
}

