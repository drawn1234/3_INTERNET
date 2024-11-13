//#include <iostream>
//#include <windows.h>
//
//DWORD WINAPI ThreadFunc(LPVOID lpParam) {
//    // 打印消息
//    std::cout << "Hello, This is a Thread." << std::endl;
//    return 0; // 返回值
//}
//
//int main() {
//    HANDLE hThread = NULL;
//    DWORD threadID = 0;
//
//    // 创建子线程
//    hThread = CreateThread(
//        NULL,              // 默认安全属性
//        0,                 // 默认堆栈大小
//        ThreadFunc,        // 线程函数
//        NULL,              // 传递给线程函数的参数
//        0,                 // 创建 suspended 状态
//        &threadID          // 线程标识符
//    );
//
//    if (hThread == NULL) {
//        std::cerr << "CreateThread failed: " << GetLastError() << std::endl;
//        return 1;
//    }
//
//    // 等待线程完成
//    WaitForSingleObject(hThread, INFINITE);
//
//    // 关闭并撤销线程
//    CloseHandle(hThread);
//
//    std::cout << "Thread has been terminated." << std::endl;
//
//    return 0;
//}
