//#include <iostream>
//#include <queue>
//#include <vector>
//using namespace std;
//
//// 进程数量
//const int n = 5;
//// 资源数量
//const int m = 3;
//
//// 可利用资源总数
//vector<int> available(m, 0);
//// 最大需求矩阵
//vector<vector<int>> Max(n, vector<int>(m, 0));
//// 分配矩阵
//vector<vector<int>> allocation(n, vector<int>(m, 0));
//// 需求矩阵
//vector<vector<int>> need(n, vector<int>(m, 0));
//// Work向量
//vector<int> work(m, 0);
//// Finish向量
//vector<bool> finish(n, false);
//
//// 请求资源
//class request_queue {
//public:
//    queue<vector<int>> q;
//    void push(vector<int> request) {
//        q.push(request);
//    }
//    vector<int> front() {
//        return q.front();
//    }
//    void pop() {
//        q.pop();
//    }
//    bool empty() {
//        return q.empty();
//    }
//};
//
//request_queue rq;
//
//// 判断是否安全
//bool isSafe() {
//    // 初始化work和finish向量
//    for (int i = 0; i < m; i++) {
//        work[i] = available[i];
//    }
//    for (int i = 0; i < n; i++) {
//        finish[i] = false;
//    }
//    // 寻找满足条件的进程
//    int count = 0;
//    while (count < n) {
//        bool find = false;
//        for (int i = 0; i < n; i++) {
//            if (!finish[i] && need[i] <= work) {
//                // 分配资源
//                vector<int> new_work(m, 0);
//                for (int j = 0; j < m; j++) {
//                    new_work[j] = work[j] + allocation[i][j];
//                }
//                work = new_work;
//                finish[i] = true;
//                count++;
//                find = true;
//            }
//        }
//        if (!find) {
//            return false;
//        }
//    }
//    return true;
//}
//
//
//// 请求资源
//void request(int pid, vector<int> request) {
//    if (request[0] > need[pid][0] || request[1] > need[pid][1] || request[2] > need[pid][2]) {
//        cout << "Error: Request is larger than Need." << endl;
//        return;
//    }
//    if (request[0] > available[0] || request[1] > available[1] || request[2] > available[2]) {
//        cout << "Error: Request is larger than Available." << endl;
//        return;
//    }
//    // 分配资源
//    available[0] -= request[0];
//    available[1] -= request[1];
//    available[2] -= request[2];
//    allocation[pid][0] += request[0];
//    allocation[pid][1] += request[1];
//    allocation[pid][2] += request[2];
//    need[pid][0] -= request[0];
//    need[pid][1] -= request[1];
//    need[pid][2] -= request[2];
//    // 判断是否安全
//    if (isSafe()) {
//        cout << "Request is granted and system is safe." << endl;
//    }
//    else {
//        cout << "Request is granted but system is unsafe." << endl;
//    }
//    // 回滚分配
//    available[0] += request[0];
//    available[1] += request[1];
//    available[2] += request[2];
//    allocation[pid][0] -= request[0];
//    allocation[pid][1] -= request[1];
//    allocation[pid][2] -= request[2];
//    need[pid][0] += request[0];
//    need[pid][1] += request[1];
//    need[pid][2] += request[2];
//}
//
//int main() {
//    // 初始化可利用资源总数
//    available = { 3, 3, 2 };
//    // 初始化最大需求矩阵
//    Max = {
//        {7, 5, 3},
//        {3, 2, 2},
//        {9, 0, 2},
//        {2, 2, 2},
//        {4, 2, 1}
//    };
//    // 初始化分配矩阵
//    allocation = {
//        {0, 1, 0},
//        {2, 0, 0},
//        {3, 0, 2},
//        {0, 0, 2},
//        {0, 1, 0}
//    };
//    // 计算需求矩阵
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < m; j++) {
//            need[i][j] = Max[i][j] - allocation[i][j];
//        }
//    }
//    // 请求资源
//    rq.push({ 0, 5, 0 });
//    while (!rq.empty()) {
//        vector<int> request = rq.front();
//        rq.pop();
//        rq.push(request);
//    }
//    return 0;
//}
