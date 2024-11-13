//#include <windows.h>
//#include <iostream>
//
//// 全局变量
//int count = 0;
//
//// 信号量
//HANDLE g_hSemaphore;
//
//// 子线程函数
//DWORD WINAPI ThreadProc(LPVOID lpParam) {
//    DWORD dwWaitResult;
//
//    // 等待信号量
//    dwWaitResult = WaitForSingleObject(g_hSemaphore, INFINITE);
//    std::cout << "Semaphore Wait Result: " << dwWaitResult << std::endl;
//
//    // 修改全局变量
//    count++;
//    std::cout << "Thread ID: " << GetCurrentThreadId() << ", count: " << count << std::endl;
//
//    // 释放信号量
//    BOOL bReleaseResult = ReleaseSemaphore(g_hSemaphore, 1, NULL);
//    std::cout << "Semaphore Release Success: " << (bReleaseResult ? "Yes" : "No") << std::endl;
//
//    return 0;
//}
//
//int main() {
//    // 创建信号量
//    g_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
//    if (g_hSemaphore == NULL) {
//        std::cout << "Semaphore Create Failed!" << std::endl;
//        return 1;
//    }
//    std::cout << "Semaphore Create Success!" << std::endl;
//
//    // 创建线程
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
//    // 等待线程结束
//    WaitForSingleObject(hThread1, INFINITE);
//    WaitForSingleObject(hThread2, INFINITE);
//
//    // 清理资源
//    CloseHandle(hThread1);
//    CloseHandle(hThread2);
//    CloseHandle(g_hSemaphore);
//
//    return 0;
//}
