//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <condition_variable>
//
//std::mutex mtx;
//std::condition_variable cv;
//bool ready = false;
//
//void childThread() {
//    std::cout << "Now in Thread." << std::endl;
//    {
//        std::lock_guard<std::mutex> lock(mtx);
//        ready = true;
//    }
//    cv.notify_one();
//}
//
//int main() {
//    std::cout << "Semaphore create success!" << std::endl;
//    std::cout << "Semaphore open success!" << std::endl;
//
//    std::thread child(childThread);
//
//    std::cout << "Thread create success!" << std::endl;
//
//    std::unique_lock<std::mutex> lock(mtx);
//    cv.wait(lock, [] { return ready; });
//
//    std::cout << "Release Semaphore err=0" << std::endl;
//    std::cout << "Wait for single object ERR=0" << std::endl;
//    std::cout << "Semaphore Release Success. rc=1" << std::endl;
//    std::cout << "dRes=0" << std::endl;
//
//    child.join();
//
//    return 0;
//}
