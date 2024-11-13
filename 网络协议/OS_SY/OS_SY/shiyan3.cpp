//#include<iostream>
//#include <vector>
//
//using namespace std;
//
//struct Node {
//
//    string processName;//进程名
//
//    int a[100];//Max
//
//    int b[100];//allocation
//
//    int c[100];//need
//
//    bool finish;
//
//}node[1000];
//
////关于重写 sort方法中的两个参数 都表示是一个结构体（即我们需要用两个结构体当中的数据进行比较）
//
//bool sort_c(Node node1, Node node2) {
//
//    return node1.c[0] < node2.c[0];
//
//}
//
//int main() {
//
//    int N, M;
//
//    int cnt = 0;//用于记进程完成的个数
//
//    vector<int> v1;//存总的资源总量
//
//    vector<int>  v2;//存need需要的资源
//
//    vector<int>  v3;//记录最后需要输出的Available
//
//    //cin >> N >> M;
//    N = 4;//进程数
//    M = 3;//资源数
//
//    for (int i = 0; i < M; i++) {
//
//        int resources;
//
//        cin >> resources;
//
//        v1.push_back(resources);
//
//    }
//
//    for (int i = 0; i < N; i++) {
//
//        cin >> node[i].processName;
//
//        //输入Max
//
//        for (int j = 0; j < M; j++) {
//
//            cin >> node[i].a[j];
//
//        }
//
//        //输入allocation
//
//        for (int j = 0; j < M; j++) {
//
//            cin >> node[i].b[j];
//
//            v1[j] -= node[i].b[j];//这里是每次减去分配的资源 那么剩下的最后就是 available
//
//        }
//
//        //求取need
//
//        for (int j = 0; j < M; j++) {
//
//            node[i].c[j] = node[i].a[j] - node[i].b[j];
//
//        }
//
//        node[i].finish = false;//将每个进程初始状态设为 false
//
//    }
//
//    for (int i = 0; i < M; i++) {
//
//        v3.push_back(v1[i]);
//
//    }
//
//    // sort(node,node+N,sort_c);
//
//    //算法核心部分
//
//    for (int i = 0; i < N; i++) {
//
//        int count = 0;
//
//        for (int j = 0; j < M; j++) {
//
//            if (node[i].c[j] <= v1[j]) {
//
//                count++;
//
//            }
//            else {
//
//                break;//只要有一个不合适就 break 出去
//
//            }
//
//        }
//
//        if (node[i].finish == false && count == M) {//count == M说明剩余的各个资源总量大于该进程的所需要的
//
//            for (int j = 0; j < M; j++) {
//
//                v1[j] += node[i].b[j];//那么此时剩余的资源总量为原来的加上 该进程释放其占有的资源
//
//            }
//
//            node[i].finish = true;
//
//            cnt++;//记录完成进程的个数
//
//            // cout << node[i].processName <<' ';
//
//            //此处牛逼之处在于 只要我们找到满足条件的就从-1开始继续寻找满足条件的
//
//            i = -1;
//
//        }
//
//    }
//
//    // cout << endl;
//
//    int flag = 0;
//
//    cout << "name max allocation need available" << endl;
//
//    for (int i = 0; i < N; i++) {
//
//        cout << node[i].processName << ' ';
//
//            for (int j = 0; j < M; j++) {
//
//                cout << node[i].a[j] <<' ';
//
//            }
//
//        cout << "| ";
//
//        for (int j = 0; j < M; j++) {
//
//            cout << node[i].b[j] <<' ';
//
//        }
//
//        cout << "| ";
//
//        for (int j = 0; j < M; j++) {
//
//            cout << node[i].c[j] <<' ';
//
//        }
//
//        cout << ' | ';
//
//        if (flag == 0) {
//
//            for (int j = 0; j < M; j++) {
//
//                if (j == 0)
//
//                    cout << ' ' << v3[j];
//
//                else
//
//                    cout << ' ' << v3[j];
//
//            }
//
//            flag = 1;
//
//        }
//
//        cout << endl;
//
//    }
//
//    if (cnt == N) {
//
//        cout << "找到安全序列，处于安全状态。";
//
//    }
//    else {
//
//        cout << "找不到安全序列，处于不安全状态。";
//
//    }
//
//    // for(int i = 0; i < M; i++){
//
//    // cout << v1[i] <<' ';
//
//    // }
//
//    // 验证数据
//
//    // for(int i = 0; i < N; i++){
//
//    //
//
//    // cout << node[i].processName <<' ';
