//#include <iostream>
//#include <windows.h>
//using namespace std;
//DWORD WINAPI ThreadFunc(LPVOID lpParam) {
//    // �������voidָ��ת��Ϊʵ������
//    const char* message = static_cast<const char*>(lpParam);
//    // ��ӡ��Ϣ
//    cout << message << endl;
//    return 0; // ����ֵ
//}
//
//int main() {
//    HANDLE hThreadA = NULL, hThreadB = NULL, hThreadC = NULL;
//    DWORD threadIDA = 0, threadIDB = 0, threadIDC = 0;
//    const char* msgA = "Thread A says: hello world��";
//    const char* msgB = "Thread B says: hello world��";
//    const char* msgC = "Thread C says: hello world��";
//
//    // �����߳� A
//    hThreadA = CreateThread(
//        NULL,              // Ĭ�ϰ�ȫ����
//        0,                 // Ĭ�϶�ջ��С
//        ThreadFunc,        // �̺߳���
//        (LPVOID)msgA,      // ���ݸ��̺߳����Ĳ���
//        0,                 // ���� suspended ״̬
//        &threadIDA         // �̱߳�ʶ��
//    );
//
//    // ȷ���߳� A �����ɹ�
//    if (hThreadA == NULL) {
//        cerr << "CreateThread A failed: " << GetLastError() << endl;
//        return 1;
//    }
//
//    // �����߳� B
//    hThreadB = CreateThread(
//        NULL,              // Ĭ�ϰ�ȫ����
//        0,                 // Ĭ�϶�ջ��С
//        ThreadFunc,        // �̺߳���
//        (LPVOID)msgB,      // ���ݸ��̺߳����Ĳ���
//        0,                 // ���� suspended ״̬
//        &threadIDB         // �̱߳�ʶ��
//    );
//
//    // ȷ���߳� B �����ɹ�
//    if (hThreadB == NULL) {
//        cerr << "CreateThread B failed: " << GetLastError() << endl;
//        return 1;
//    }
//
//    // �����߳� C
//    hThreadC = CreateThread(
//        NULL,              // Ĭ�ϰ�ȫ����
//        0,                 // Ĭ�϶�ջ��С
//        ThreadFunc,        // �̺߳���
//        (LPVOID)msgC,      // ���ݸ��̺߳����Ĳ���
//        0,                 // ���� suspended ״̬
//        &threadIDC         // �̱߳�ʶ��
//    );
//
//    // ȷ���߳� C �����ɹ�
//    if (hThreadC == NULL) {
//        cerr << "CreateThread C failed: " << GetLastError() << endl;
//        return 1;
//    }
//
//    // �����߳� A
//    ResumeThread(hThreadA);
//    // �ȴ��߳� A ���
//    WaitForSingleObject(hThreadA, INFINITE);
//
//    // �����߳� B
//    ResumeThread(hThreadB);
//    // �ȴ��߳� B ���
//    WaitForSingleObject(hThreadB, INFINITE);
//
//    // �����߳� C
//    ResumeThread(hThreadC);
//    // �ȴ��߳� C ���
//    WaitForSingleObject(hThreadC, INFINITE);
//
//    // �ر������߳̾��
//    CloseHandle(hThreadA);
//    CloseHandle(hThreadB);
//    CloseHandle(hThreadC);
//
//    return 0;
//}
