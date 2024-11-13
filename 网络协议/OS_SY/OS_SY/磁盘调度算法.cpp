#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;
// ��������
void generateRandomTracks(vector<int>& tracks, int numTracks);
void printTracks(const vector<int>& tracks);
void sstf(vector<int> tracks, int currentTrack);
void scan(vector<int> tracks, int currentTrack);
void fifo(vector<int> tracks, int currentTrack);

int main() {
    int numTracks;
    cout << "������Ҫ����Ĵŵ���: ";
    cin >> numTracks;

    vector<int> tracks(numTracks);
    generateRandomTracks(tracks, numTracks);

    cout << "������ɴŵ���: ";
    printTracks(tracks);
    cout << "�������ɳɹ�!\n";

    int choice, currentTrack;
    do {
        cout << "--------------�㷨ѡ��--------------\n";
        cout << "---1.���Ѱ��ʱ���㷨-----\n";
        cout << "---2.ɨ���㷨--------\n";
        cout << "---3.�Ƚ��ȳ��㷨--------\n";
        cout << "---0.�˳�����---------\n";
        cout << "����������ʹ�õķ���: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "�����뵱ǰ�ŵ���: ";
            cin >> currentTrack;
            sstf(tracks, currentTrack);
            break;
        }
        case 2: {
            cout << "�����뵱ǰ�ŵ���: ";
            cin >> currentTrack;
            scan(tracks, currentTrack);
            break;
        }
        case 3: {
            cout << "�����뵱ǰ�ŵ���: ";
            cin >> currentTrack;
            fifo(tracks, currentTrack);
            break;
        }
        case 0:
            cout << "�˳�����\n";
            break;
        default:
            cout << "��Ч�����룬������ѡ��\n";
        }
    } while (choice != 0);

    return 0;
}

void generateRandomTracks(vector<int>& tracks, int numTracks) {
    srand(time(nullptr)); // ��ʼ�����������
    for (int i = 0; i < numTracks; ++i) {
        tracks[i] = rand() % 100 + 1; // ����1��100֮�������ŵ���
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

        cout << "��ǰ���ʵĴŵ�:" << nextTrack << endl;
        //cout << "��ǰ�ûصĴŵ�:" << currentTrack << endl;

        totalMovement += abs(currentTrack - nextTrack);
        currentTrack = nextTrack;
    }

    double averageMovement = static_cast<double>(totalMovement) / totalTracks;
    cout << "�ƶ����ܴŵ���:" << totalMovement << endl;
    cout << "�������ܴŵ���Ϊ:" << totalTracks << endl;
    cout << "�ƶ���ƽ���ŵ���:" << fixed << setprecision(2) << averageMovement << endl;
}

void scan(vector<int> tracks, int currentTrack) {
    sort(tracks.begin(), tracks.end());
    int totalMovement = 0;
    int totalTracks = tracks.size();

    // �ҵ���ǰ�ŵ����ڵ�λ��
    auto it = lower_bound(tracks.begin(), tracks.end(), currentTrack);
    int index = it - tracks.begin();

    // ����ɨ��
    for (int i = index; i < totalTracks; ++i) {
        cout << "��ǰ���ʵĴŵ�:" << tracks[i] << endl;
        totalMovement += abs(currentTrack - tracks[i]);
        currentTrack = tracks[i];
    }

    // ����ɨ��
    for (int i = index - 1; i >= 0; --i) {
        cout << "��ǰ���ʵĴŵ�:" << tracks[i] << endl;
        totalMovement += abs(currentTrack - tracks[i]);
        currentTrack = tracks[i];
    }

    double averageMovement = static_cast<double>(totalMovement) / totalTracks;
    cout << "�ƶ����ܴŵ���:" << totalMovement << endl;
    cout << "�������ܴŵ���Ϊ:" << totalTracks << endl;
    cout << "�ƶ���ƽ���ŵ���:" << fixed << setprecision(2) << averageMovement << endl;
}

void fifo(vector<int> tracks, int currentTrack) {
    int totalMovement = 0;
    int totalTracks = tracks.size();

    for (int i = 0; i < totalTracks; ++i) {
        cout << "��ǰ���ʵĴŵ�:" << tracks[i] << endl;
        totalMovement += abs(currentTrack - tracks[i]);
        currentTrack = tracks[i];
    }

    double averageMovement = static_cast<double>(totalMovement) / totalTracks;
    cout << "�ƶ����ܴŵ���:" << totalMovement << endl;
    cout << "�������ܴŵ���Ϊ:" << totalTracks << endl;
    cout << "�ƶ���ƽ���ŵ���:" << fixed << setprecision(2) << averageMovement << endl;
}
