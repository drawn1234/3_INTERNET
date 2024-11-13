#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;
// 函数声明
void generateRandomTracks(vector<int>& tracks, int numTracks);
void printTracks(const vector<int>& tracks);
void sstf(vector<int> tracks, int currentTrack);
void scan(vector<int> tracks, int currentTrack);
void fifo(vector<int> tracks, int currentTrack);

int main() {
    int numTracks;
    cout << "请输入要处理的磁道数: ";
    cin >> numTracks;

    vector<int> tracks(numTracks);
    generateRandomTracks(tracks, numTracks);

    cout << "随机生成磁道号: ";
    printTracks(tracks);
    cout << "数据生成成功!\n";

    int choice, currentTrack;
    do {
        cout << "--------------算法选择--------------\n";
        cout << "---1.最短寻道时间算法-----\n";
        cout << "---2.扫描算法--------\n";
        cout << "---3.先进先出算法--------\n";
        cout << "---0.退出程序---------\n";
        cout << "请输入你想使用的方法: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "请输入当前磁道号: ";
            cin >> currentTrack;
            sstf(tracks, currentTrack);
            break;
        }
        case 2: {
            cout << "请输入当前磁道号: ";
            cin >> currentTrack;
            scan(tracks, currentTrack);
            break;
        }
        case 3: {
            cout << "请输入当前磁道号: ";
            cin >> currentTrack;
            fifo(tracks, currentTrack);
            break;
        }
        case 0:
            cout << "退出程序。\n";
            break;
        default:
            cout << "无效的输入，请重新选择。\n";
        }
    } while (choice != 0);

    return 0;
}

void generateRandomTracks(vector<int>& tracks, int numTracks) {
    srand(time(nullptr)); // 初始化随机数种子
    for (int i = 0; i < numTracks; ++i) {
        tracks[i] = rand() % 100 + 1; // 生成1到100之间的随机磁道号
    }
}

void printTracks(const vector<int>& tracks) {
    for (int track : tracks) {
        cout << track << " ";
    }
    cout << endl;
}

void sstf(vector<int> tracks, int currentTrack) {
    int totalMovement = 0;
    int totalTracks = tracks.size();

    while (!tracks.empty()) {
        auto it = min_element(tracks.begin(), tracks.end(), [currentTrack](int a, int b) {
            return abs(a - currentTrack) < abs(b - currentTrack);
            });

        int nextTrack = *it;
        tracks.erase(it);

        cout << "当前访问的磁道:" << nextTrack << endl;
        //cout << "当前访回的磁道:" << currentTrack << endl;

        totalMovement += abs(currentTrack - nextTrack);
        currentTrack = nextTrack;
    }

    double averageMovement = static_cast<double>(totalMovement) / totalTracks;
    cout << "移动的总磁道数:" << totalMovement << endl;
    cout << "经过的总磁道数为:" << totalTracks << endl;
    cout << "移动的平均磁道数:" << fixed << setprecision(2) << averageMovement << endl;
}

void scan(vector<int> tracks, int currentTrack) {
    sort(tracks.begin(), tracks.end());
    int totalMovement = 0;
    int totalTracks = tracks.size();

    // 找到当前磁道所在的位置
    auto it = lower_bound(tracks.begin(), tracks.end(), currentTrack);
    int index = it - tracks.begin();

    // 向右扫描
    for (int i = index; i < totalTracks; ++i) {
        cout << "当前访问的磁道:" << tracks[i] << endl;
        totalMovement += abs(currentTrack - tracks[i]);
        currentTrack = tracks[i];
    }

    // 向左扫描
    for (int i = index - 1; i >= 0; --i) {
        cout << "当前访问的磁道:" << tracks[i] << endl;
        totalMovement += abs(currentTrack - tracks[i]);
        currentTrack = tracks[i];
    }

    double averageMovement = static_cast<double>(totalMovement) / totalTracks;
    cout << "移动的总磁道数:" << totalMovement << endl;
    cout << "经过的总磁道数为:" << totalTracks << endl;
    cout << "移动的平均磁道数:" << fixed << setprecision(2) << averageMovement << endl;
}

void fifo(vector<int> tracks, int currentTrack) {
    int totalMovement = 0;
    int totalTracks = tracks.size();

    for (int i = 0; i < totalTracks; ++i) {
        cout << "当前访问的磁道:" << tracks[i] << endl;
        totalMovement += abs(currentTrack - tracks[i]);
        currentTrack = tracks[i];
    }

    double averageMovement = static_cast<double>(totalMovement) / totalTracks;
    cout << "移动的总磁道数:" << totalMovement << endl;
    cout << "经过的总磁道数为:" << totalTracks << endl;
    cout << "移动的平均磁道数:" << fixed << setprecision(2) << averageMovement << endl;
}
