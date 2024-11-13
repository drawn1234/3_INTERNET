//#include <windows.h>
//#include <iostream>
//
//// ȫ�ֱ���
//int count = 0;
//
//// �ź���
//HANDLE g_hSemaphore;
//
//// ���̺߳���
//DWORD WINAPI ThreadProc(LPVOID lpParam) {
//    DWORD dwWaitResult;
//
//    // �ȴ��ź���
//    dwWaitResult = WaitForSingleObject(g_hSemaphore, INFINITE);
//    std::cout << "Semaphore Wait Result: " << dwWaitResult << std::endl;
//
//    // �޸�ȫ�ֱ���
//    count++;
//    std::cout << "Thread ID: " << GetCurrentThreadId() << ", count: " << count << std::endl;
//
//    // �ͷ��ź���
//    BOOL bReleaseResult = ReleaseSemaphore(g_hSemaphore, 1, NULL);
//    std::cout << "Semaphore Release Success: " << (bReleaseResult ? "Yes" : "No") << std::endl;
//
//    return 0;
//}
//
//int main() {
//    // �����ź���
//    g_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
//    if (g_hSemaphore == NULL) {
//        std::cout << "Semaphore Create Failed!" << std::endl;
//        return 1;
//    }
//    std::cout << "Semaphore Create Success!" << std::endl;
//
//    // �����߳�
//    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
//    HANDLE hThread2 = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
//
//    if (hThread1 == NULL || hThread2 == NULL) {
//        std::cout << "Thread Create Failed!" << std::endl;
//        CloseHandle(g_hSemaphore);
//        return 1;
//    }
//    std::cout << "Thread Create Success." << std::endl;
//
//    // �ȴ��߳̽���
//    WaitForSingleObject(hThread1, INFINITE);
//    WaitForSingleObject(hThread2, INFINITE);
//
//    // ������Դ
//    CloseHandle(hThread1);
//    CloseHandle(hThread2);
//    CloseHandle(g_hSemaphore);
//
//    return 0;
//}
