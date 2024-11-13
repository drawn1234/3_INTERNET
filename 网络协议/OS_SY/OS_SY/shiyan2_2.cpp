//#include <windows.h>
//#include <iostream>
//#include <vector>
//#include <queue>
//
//const int BUFFER_SIZE = 10; // 缓冲区大小
//const int NUM_PRODUCERS = 2; // 生产者数量
//const int NUM_CONSUMERS = 2; // 消费者数量
//
//std::queue<int> buffer; // 缓冲区
//HANDLE g_hMutex; // 互斥锁
//HANDLE g_hSemaphore; // 信号量
//
//DWORD WINAPI ProducerThread(LPVOID lpParam) {
//    int id = reinterpret_cast<int>(lpParam);
//    int item = 0;
//
//    while (true) {
//        // 生产数据
//        item++;
//
//        // 等待缓冲区有空位
//        WaitForSingleObject(g_hSemaphore, INFINITE);
//
//        // 获取互斥锁
//        WaitForSingleObject(g_hMutex, INFINITE);
//
//        // 生产数据并放入缓冲区
//        buffer.push(item);
//        std::cout << "Producer " << id << " produced item " << item << ", buffer size: " << buffer.size() << std::endl;
//
//        // 释放互斥锁
//        ReleaseMutex(g_hMutex);
//
//        // 通知消费者
//        ReleaseSemaphore(g_hSemaphore, 1, NULL);
//
//        // 随机休眠一段时间
//        Sleep(rand() % 1000);
//    }
//
//    return 0;
//}
//
//DWORD WINAPI ConsumerThread(LPVOID lpParam) {
//    int id = reinterpret_cast<int>(lpParam);
//
//    while (true) {
//        // 等待缓冲区非空
//        WaitForSingleObject(g_hSemaphore, INFINITE);
//
//        // 获取互斥锁
//        WaitForSingleObject(g_hMutex, INFINITE);
//
//        // 从缓冲区消费数据
//        if (!buffer.empty()) {
//            int item = buffer.front();
//            buffer.pop();
//            std::cout << "Consumer " << id << " consumed item " << item << ", buffer size: " << buffer.size() << std::endl;
//        }
//
//        // 释放互斥锁
//        ReleaseMutex(g_hMutex);
//
//        // 通知生产者
//        ReleaseSemaphore(g_hSemaphore, 1, NULL);
//
//        // 随机休眠一段时间
//        Sleep(rand() % 1000);
//    }
//
//    return 0;
//}
//
//int main() {
//    // 初始化互斥锁
//    g_hMutex = CreateMutex(NULL, FALSE, NULL);
//    if (g_hMutex == NULL) {
//        std::cout << "Mutex Create Failed!" << std::endl;
//        return 1;
//    }
//
//    // 初始化信号量
//    g_hSemaphore = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
//    if (g_hSemaphore == NULL) {
//        std::cout << "Semaphore Create Failed!" << std::endl;
//        CloseHandle(g_hMutex);
//        return 1;
//    }
//
//    // 创建生产者和消费者线程
//    std::vector<HANDLE> threads;
//    for (int i = 0; i < NUM_PRODUCERS; ++i) {
//        HANDLE hThread = CreateThread(NULL, 0, ProducerThread, reinterpret_cast<LPVOID>(i + 1), 0, NULL);
//        if (hThread == NULL) {
//            std::cout << "Producer Thread Create Failed!" << std::endl;
//            break;
//        }
//        threads.push_back(hThread);
//    }
//
//    for (int i = 0; i < NUM_CONSUMERS; ++i) {
//        HANDLE hThread = CreateThread(NULL, 0, ConsumerThread, reinterpret_cast<LPVOID>(i + 1), 0, NULL);
//        if (hThread == NULL) {
//            std::cout << "Consumer Thread Create Failed!" << std::endl;
//            break;
//        }
//        threads.push_back(hThread);
//    }
//
//    // 等待所有线程结束
//    WaitForMultipleObjects(threads.size(), threads.data(), TRUE, INFINITE);
//
//    // 清理资源
//    for (HANDLE hThread : threads) {
//        CloseHandle(hThread);
//    }
//    CloseHandle(g_hMutex);
//    CloseHandle(g_hSemaphore);
//
//    return 0;
//}
