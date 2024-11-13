//#include <iostream>
//#include <windows.h>
//using namespace std;
//DWORD WINAPI ThreadFunc(LPVOID lpParam) {
//    // 将传入的void指针转换为实际类型
//    const char* message = static_cast<const char*>(lpParam);
//    // 打印消息
//    cout << message << endl;
//    return 0; // 返回值
//}
//
//int main() {
//    HANDLE hThreadA = NULL, hThreadB = NULL, hThreadC = NULL;
//    DWORD threadIDA = 0, threadIDB = 0, threadIDC = 0;
//    const char* msgA = "Thread A says: hello world！";
//    const char* msgB = "Thread B says: hello world！";
//    const char* msgC = "Thread C says: hello world！";
//
//    // 创建线程 A
//    hThreadA = CreateThread(
//        NULL,              // 默认安全属性
//        0,                 // 默认堆栈大小
//        ThreadFunc,        // 线程函数
//        (LPVOID)msgA,      // 传递给线程函数的参数
//        0,                 // 创建 suspended 状态
//        &threadIDA         // 线程标识符
//    );
//
//    // 确保线程 A 创建成功
//    if (hThreadA == NULL) {
//        cerr << "CreateThread A failed: " << GetLastError() << endl;
//        return 1;
//    }
//
//    // 创建线程 B
//    hThreadB = CreateThread(
//        NULL,              // 默认安全属性
//        0,                 // 默认堆栈大小
//        ThreadFunc,        // 线程函数
//        (LPVOID)msgB,      // 传递给线程函数的参数
//        0,                 // 创建 suspended 状态
//        &threadIDB         // 线程标识符
//    );
//
//    // 确保线程 B 创建成功
//    if (hThreadB == NULL) {
//        cerr << "CreateThread B failed: " << GetLastError() << endl;
//        return 1;
//    }
//
//    // 创建线程 C
//    hThreadC = CreateThread(
//        NULL,              // 默认安全属性
//        0,                 // 默认堆栈大小
//        ThreadFunc,        // 线程函数
//        (LPVOID)msgC,      // 传递给线程函数的参数
//        0,                 // 创建 suspended 状态
//        &threadIDC         // 线程标识符
//    );
//
//    // 确保线程 C 创建成功
//    if (hThreadC == NULL) {
//        cerr << "CreateThread C failed: " << GetLastError() << endl;
//        return 1;
//    }
//
//    // 启动线程 A
//    ResumeThread(hThreadA);
//    // 等待线程 A 完成
//    WaitForSingleObject(hThreadA, INFINITE);
//
//    // 启动线程 B
//    ResumeThread(hThreadB);
//    // 等待线程 B 完成
//    WaitForSingleObject(hThreadB, INFINITE);
//
//    // 启动线程 C
//    ResumeThread(hThreadC);
//    // 等待线程 C 完成
//    WaitForSingleObject(hThreadC, INFINITE);
//
//    // 关闭所有线程句柄
//    CloseHandle(hThreadA);
//    CloseHandle(hThreadB);
//    CloseHandle(hThreadC);
//
//    return 0;
//}
