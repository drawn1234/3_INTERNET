//#include <iostream>
//#include <queue>
//#include <vector>
//using namespace std;
//
//// ��������
//const int n = 5;
//// ��Դ����
//const int m = 3;
//
//// ��������Դ����
//vector<int> available(m, 0);
//// ����������
//vector<vector<int>> Max(n, vector<int>(m, 0));
//// �������
//vector<vector<int>> allocation(n, vector<int>(m, 0));
//// �������
//vector<vector<int>> need(n, vector<int>(m, 0));
//// Work����
//vector<int> work(m, 0);
//// Finish����
//vector<bool> finish(n, false);
//
//// ������Դ
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
//// �ж��Ƿ�ȫ
//bool isSafe() {
//    // ��ʼ��work��finish����
//    for (int i = 0; i < m; i++) {
//        work[i] = available[i];
//    }
//    for (int i = 0; i < n; i++) {
//        finish[i] = false;
//    }
//    // Ѱ�����������Ľ���
//    int count = 0;
//    while (count < n) {
//        bool find = false;
//        for (int i = 0; i < n; i++) {
//            if (!finish[i] && need[i] <= work) {
//                // ������Դ
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
//// ������Դ
//void request(int pid, vector<int> request) {
//    if (request[0] > need[pid][0] || request[1] > need[pid][1] || request[2] > need[pid][2]) {
//        cout << "Error: Request is larger than Need." << endl;
//        return;
//    }
//    if (request[0] > available[0] || request[1] > available[1] || request[2] > available[2]) {
//        cout << "Error: Request is larger than Available." << endl;
//        return;
//    }
//    // ������Դ
//    available[0] -= request[0];
//    available[1] -= request[1];
//    available[2] -= request[2];
//    allocation[pid][0] += request[0];
//    allocation[pid][1] += request[1];
//    allocation[pid][2] += request[2];
//    need[pid][0] -= request[0];
//    need[pid][1] -= request[1];
//    need[pid][2] -= request[2];
//    // �ж��Ƿ�ȫ
//    if (isSafe()) {
//        cout << "Request is granted and system is safe." << endl;
//    }
//    else {
//        cout << "Request is granted but system is unsafe." << endl;
//    }
//    // �ع�����
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
//    // ��ʼ����������Դ����
//    available = { 3, 3, 2 };
//    // ��ʼ������������
//    Max = {
//        {7, 5, 3},
//        {3, 2, 2},
//        {9, 0, 2},
//        {2, 2, 2},
//        {4, 2, 1}
//    };
//    // ��ʼ���������
//    allocation = {
//        {0, 1, 0},
//        {2, 0, 0},
//        {3, 0, 2},
//        {0, 0, 2},
//        {0, 1, 0}
//    };
//    // �����������
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < m; j++) {
//            need[i][j] = Max[i][j] - allocation[i][j];
//        }
//    }
//    // ������Դ
//    rq.push({ 0, 5, 0 });
//    while (!rq.empty()) {
//        vector<int> request = rq.front();
//        rq.pop();
//        rq.push(request);
//    }
//    return 0;
//}
