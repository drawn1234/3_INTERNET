//#include <windows.h>
//#include <iostream>
//#include <vector>
//#include <queue>
//
//const int BUFFER_SIZE = 10; // ��������С
//const int NUM_PRODUCERS = 2; // ����������
//const int NUM_CONSUMERS = 2; // ����������
//
//std::queue<int> buffer; // ������
//HANDLE g_hMutex; // ������
//HANDLE g_hSemaphore; // �ź���
//
//DWORD WINAPI ProducerThread(LPVOID lpParam) {
//    int id = reinterpret_cast<int>(lpParam);
//    int item = 0;
//
//    while (true) {
//        // ��������
//        item++;
//
//        // �ȴ��������п�λ
//        WaitForSingleObject(g_hSemaphore, INFINITE);
//
//        // ��ȡ������
//        WaitForSingleObject(g_hMutex, INFINITE);
//
//        // �������ݲ����뻺����
//        buffer.push(item);
//        std::cout << "Producer " << id << " produced item " << item << ", buffer size: " << buffer.size() << std::endl;
//
//        // �ͷŻ�����
//        ReleaseMutex(g_hMutex);
//
//        // ֪ͨ������
//        ReleaseSemaphore(g_hSemaphore, 1, NULL);
//
//        // �������һ��ʱ��
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
//        // �ȴ��������ǿ�
//        WaitForSingleObject(g_hSemaphore, INFINITE);
//
//        // ��ȡ������
//        WaitForSingleObject(g_hMutex, INFINITE);
//
//        // �ӻ�������������
//        if (!buffer.empty()) {
//            int item = buffer.front();
//            buffer.pop();
//            std::cout << "Consumer " << id << " consumed item " << item << ", buffer size: " << buffer.size() << std::endl;
//        }
//
//        // �ͷŻ�����
//        ReleaseMutex(g_hMutex);
//
//        // ֪ͨ������
//        ReleaseSemaphore(g_hSemaphore, 1, NULL);
//
//        // �������һ��ʱ��
//        Sleep(rand() % 1000);
//    }
//
//    return 0;
//}
//
//int main() {
//    // ��ʼ��������
//    g_hMutex = CreateMutex(NULL, FALSE, NULL);
//    if (g_hMutex == NULL) {
//        std::cout << "Mutex Create Failed!" << std::endl;
//        return 1;
//    }
//
//    // ��ʼ���ź���
//    g_hSemaphore = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
//    if (g_hSemaphore == NULL) {
//        std::cout << "Semaphore Create Failed!" << std::endl;
//        CloseHandle(g_hMutex);
//        return 1;
//    }
//
//    // ���������ߺ��������߳�
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
//    // �ȴ������߳̽���
//    WaitForMultipleObjects(threads.size(), threads.data(), TRUE, INFINITE);
//
//    // ������Դ
//    for (HANDLE hThread : threads) {
//        CloseHandle(hThread);
//    }
//    CloseHandle(g_hMutex);
//    CloseHandle(g_hSemaphore);
//
//    return 0;
//}
