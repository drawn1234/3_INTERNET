//#include<iostream>
//#include <vector>
//
//using namespace std;
//
//struct Node {
//
//    string processName;//������
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
////������д sort�����е��������� ����ʾ��һ���ṹ�壨��������Ҫ�������ṹ�嵱�е����ݽ��бȽϣ�
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
//    int cnt = 0;//���ڼǽ�����ɵĸ���
//
//    vector<int> v1;//���ܵ���Դ����
//
//    vector<int>  v2;//��need��Ҫ����Դ
//
//    vector<int>  v3;//��¼�����Ҫ�����Available
//
//    //cin >> N >> M;
//    N = 4;//������
//    M = 3;//��Դ��
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
//        //����Max
//
//        for (int j = 0; j < M; j++) {
//
//            cin >> node[i].a[j];
//
//        }
//
//        //����allocation
//
//        for (int j = 0; j < M; j++) {
//
//            cin >> node[i].b[j];
//
//            v1[j] -= node[i].b[j];//������ÿ�μ�ȥ�������Դ ��ôʣ�µ������� available
//
//        }
//
//        //��ȡneed
//
//        for (int j = 0; j < M; j++) {
//
//            node[i].c[j] = node[i].a[j] - node[i].b[j];
//
//        }
//
//        node[i].finish = false;//��ÿ�����̳�ʼ״̬��Ϊ false
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
//    //�㷨���Ĳ���
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
//                break;//ֻҪ��һ�������ʾ� break ��ȥ
//
//            }
//
//        }
//
//        if (node[i].finish == false && count == M) {//count == M˵��ʣ��ĸ�����Դ�������ڸý��̵�����Ҫ��
//
//            for (int j = 0; j < M; j++) {
//
//                v1[j] += node[i].b[j];//��ô��ʱʣ�����Դ����Ϊԭ���ļ��� �ý����ͷ���ռ�е���Դ
//
//            }
//
//            node[i].finish = true;
//
//            cnt++;//��¼��ɽ��̵ĸ���
//
//            // cout << node[i].processName <<' ';
//
//            //�˴�ţ��֮������ ֻҪ�����ҵ����������ľʹ�-1��ʼ����Ѱ������������
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
//        cout << "�ҵ���ȫ���У����ڰ�ȫ״̬��";
//
//    }
//    else {
//
//        cout << "�Ҳ�����ȫ���У����ڲ���ȫ״̬��";
//
//    }
//
//    // for(int i = 0; i < M; i++){
//
//    // cout << v1[i] <<' ';
//
//    // }
//
//    // ��֤����
//
//    // for(int i = 0; i < N; i++){
//
//    //
//
//    // cout << node[i].processName <<' ';
