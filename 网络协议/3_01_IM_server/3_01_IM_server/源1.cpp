//#include"Udp.h"
//
//Udp::Udp() :m_handle(nullptr), m_isRunning(true)
//{
//}
//
//Udp::~Udp()
//{
//}
//
////�������ݵ��̺߳��������ý������ݵĺ�����
//unsigned __stdcall recvThread(void* IpVoid)
//{
//	Udp* pThis = (Udp*)IpVoid;
//	pThis->recvData();
//	return 1;
//}
//
////��ʼ������:���ؿ⣬�����׽��֣���ip�Ͷ˿ڣ������������ݵ��߳�
//bool Udp::initNet()
//{
//	//1.���ؿ�
//	//ħ�����֣������ֶ���ɺ�
//	WORD version = MAKEWORD(_DEF_VERSION_HIGH, _DEF_VERSION_LOW);
//	WSADATA data = {};
//	int err = WSAStartup(version, &data);
//	if (0 != err)
//	{
//		cout << "WSAStartup fail" << endl;
//		return false;
//	}
//
//	//�жϰ汾���Ƿ���ȷ
//	if (_DEF_VERSION_HIGH == HIBYTE(data.wVersion) && _DEF_VERSION_LOW == LOBYTE(data.wVersion))
//	{
//		cout << "WSAStartup success" << endl;
//	}
//	else {
//		cout << "WSAStartup version error" << endl;
//		return false;
//	}
//
//	//2.�����׽���
//	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	if (INVALID_SOCKET == m_sock)
//	{
//		cout << "socket error:" << WSAGetLastError() << endl;
//		return false;
//	}
//	else {
//		cout << "socket success" << endl;
//	}
//
//	//3.��ip�Ͷ˿�
//	sockaddr_in addr;
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(_DEF_UDP_PORT);
//	addr.sin_addr.S_un.S_addr = INADDR_ANY;
//	err = bind(m_sock, (sockaddr*)&addr, sizeof(addr));
//	if (SOCKET_ERROR == err)
//	{
//		cout << "bind error:" << WSAGetLastError() << endl;
//		return false;
//	}
//	else {
//		cout << "bind success" << endl;
//	}
//	//4.�����������ݵ��߳�
//	//CreateThread��ExitThread��һ�ԣ�������߳��е���C++����ʱ��(����strcpy)����Щ����������ռ䲢���Լ����ͷ�
//	//ExitThread���˳��̵߳�ʱ�򲻻��������Ŀռ䣬�ͻ�����ڴ�й©
//	//_beginthreadex�� endthreadex��һ�ԣ� endthreadex���˳��̵߳�ʱ����Ȼ��տռ䣬�ٵ���ExitThread
//	m_handle = (HANDLE)_beginthreadex(0/*�̰߳�ȫ����0��ʹ��Ĭ�ϵİ�ȫ����*/,
//		0/*�̶߳�ջ��С��0��ʹ��Ĭ�϶�ջ��С1M*/,
//		&recvThread/*�߳�Ҫִ�еĺ�������ʼ��ַ*/,
//		this/*�߳�Ҫִ�еĺ����Ĳ���*/,
//		0/*�̴߳�����״̬��0�Ǵ����Ժ����̿�ʼ���У�CREATESUSPENDED�Ǵ����Ժ����*/,
//		nullptr/*�����߳��Ժ󣬲���ϵͳ��ÿ���̷߳�����߳�id*/);
//
//
//	return true;
//}
//
////�������ݣ�udp:ip ulong���ͣ���������˭��tcp:socket unit,�����˷���˭��
//bool Udp::sendData(char* data, int len, unsigned long to)
//{
//	sockaddr_in addr;
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(_DEF_UDP_PORT);
//	addr.sin_addr.S_un.S_addr = to;
//	int nSendNum = sendto(m_sock, data, len, 0, (sockaddr*)&addr, sizeof(addr));
//	if (SOCKET_ERROR == nSendNum)
//	{
//		cout << "sendto error:" << WSAGetLastError() << endl;
//		return false;
//	}
//	return true;
//}
//
////�������ݣ������߳���,һֱѭ���ȴ��������ݣ�
//void Udp::recvData()
//{
//	cout << __func__ << endl;
//	int nRecvNum = 0;
//	char recvBuf[4096] = "";
//	sockaddr_in addrFrom = {};
//	int size = sizeof(addrFrom);
//	while (m_isRunning)
//	{
//		nRecvNum = recvfrom(m_sock, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&addrFrom, &size);
//		if (nRecvNum > 0)
//		{
//			//����һ�����ݰ��ɹ�
//			//���ݽ������ݵĴ�С����һ���µĿռ�
//			char* pPack = new char[nRecvNum];
//			//�ѽ��յ������ݿ������µĿռ���
//			memcpy(pPack, recvBuf, nRecvNum);
//			//TODO:�ѽ��յ������ݴ����н�����
//			//���Դ��룺��ӡ�����յ�������
//			cout << "Udp::recvData:" << pPack << endl;
//		}
//		else {
//			cout << "recvfrom error:" << WSAGetLastError() << endl;
//			break;
//		}
//	}
//}
//
////�ر����磨�����߳���Դ���ر��׽��֣�ж�ؿ⣩
//void Udp::unInitNet()
//{
//	//1.�����߳���Դ
//	//�����߳�ʱ������ϵͳ��ÿ���̷߳���3����Դ��������߳�id���ں˶������ü�������2
//	//��Ҫ�����߳���Դ������Ҫ�����ü��������0�������̹߳������رվ��
//	m_isRunning = false;
//	if (m_handle)
//	{
//		//WaitForSingleObject�ķ���ֵ�������WAIT_TIMEOUT����˵���ȴ����߳��ڵȴ�ʱ������󣬻��ڼ�������
//		if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 1000))
//		{
//			//����̻߳��������У���ǿ��ɱ���߳�
//			TerminateThread(m_handle, -1);
//
//		}
//		//�رվ��
//		CloseHandle(m_handle);
//		m_handle = nullptr;
//	}
//
//	//2.�ر��׽���
//	if (!m_sock && INVALID_SOCKET != m_sock)
//	{
//		closesocket(m_sock);
//	}
//	//3.ж�ؿ�
//	WSACleanup();
//}
