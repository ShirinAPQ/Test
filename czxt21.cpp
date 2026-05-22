#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// 进程结构体
struct Process {
    string name;
    int arr;    // 到达时间
    int burst;  // 服务时间
    int start;  // 开始时间
    int end;    // 结束时间
    int turn;   // 周转时间
    double wturn;//带权周转时间
};

// FCFS算法
void FCFS(vector<Process> p) {
    int n = p.size();
    // 按到达时间排序
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.arr < b.arr;
    });

    int time = 0;
    double avg_turn = 0, avg_wturn = 0;

    cout << "\n===== 先来先服务 FCFS =====" << endl;
    cout << left << setw(8) << "进程" 
         << setw(8) << "到达" 
         << setw(8) << "服务" 
         << setw(8) << "开始" 
         << setw(8) << "结束" 
         << setw(10) << "周转" 
         << setw(12) << "带权周转" << endl;

    for (int i = 0; i < n; i++) {
        // 若当前时间 < 到达时间，等待
        if (time < p[i].arr) time = p[i].arr;
        p[i].start = time;
        p[i].end = time + p[i].burst;
        p[i].turn = p[i].end - p[i].arr;
        p[i].wturn = (double)p[i].turn / p[i].burst;
        time = p[i].end;

        avg_turn += p[i].turn;
        avg_wturn += p[i].wturn;

        cout << left << setw(8) << p[i].name
             << setw(8) << p[i].arr
             << setw(8) << p[i].burst
             << setw(8) << p[i].start
             << setw(8) << p[i].end
             << setw(10) << p[i].turn
             << setw(12) << fixed << setprecision(2) << p[i].wturn << endl;
    }

    avg_turn /= n;
    avg_wturn /= n;
    cout << "\n平均周转时间：" << fixed << setprecision(3) << avg_turn << endl;
    cout << "平均带权周转时间：" << fixed << setprecision(3) << avg_wturn << endl;
}

// 短进程优先 SPF（非抢占）
void SPF(vector<Process> p) {
    int n = p.size();
    vector<bool> done(n, false);
    int time = 0, cnt = 0;
    double avg_turn = 0, avg_wturn = 0;

    cout << "\n===== 短进程优先 SPF（非抢占） =====" << endl;
    cout << left << setw(8) << "进程" 
         << setw(8) << "到达" 
         << setw(8) << "服务" 
         << setw(8) << "开始" 
         << setw(8) << "结束" 
         << setw(10) << "周转" 
         << setw(12) << "带权周转" << endl;

    while (cnt < n) {
        // 选：到达时间<=当前时间、未完成、服务时间最短
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arr <= time) {
                if (idx == -1 || p[i].burst < p[idx].burst)
                    idx = i;
            }
        }

        if (idx == -1) { // 无就绪进程，时间+1
            time++;
            continue;
        }

        done[idx] = true;
        p[idx].start = time;
        p[idx].end = time + p[idx].burst;
        p[idx].turn = p[idx].end - p[idx].arr;
        p[idx].wturn = (double)p[idx].turn / p[idx].burst;
        time = p[idx].end;
        cnt++;

        avg_turn += p[idx].turn;
        avg_wturn += p[idx].wturn;

        cout << left << setw(8) << p[idx].name
             << setw(8) << p[idx].arr
             << setw(8) << p[idx].burst
             << setw(8) << p[idx].start
             << setw(8) << p[idx].end
             << setw(10) << p[idx].turn
             << setw(12) << fixed << setprecision(2) << p[idx].wturn << endl;
    }

    avg_turn /= n;
    avg_wturn /= n;
    cout << "\n平均周转时间：" << fixed << setprecision(3) << avg_turn << endl;
    cout << "平均带权周转时间：" << fixed << setprecision(3) << avg_wturn << endl;
}

int main() {
    int n;
    cout << "===== 进程调度模拟 =====" << endl;
    cout << "请输入进程数目：";
    cin >> n;

    vector<Process> pro(n);
    for (int i = 0; i < n; i++) {
        cout << "请输入进程" << i+1 << " 名称 到达时间 服务时间：";
        cin >> pro[i].name >> pro[i].arr >> pro[i].burst;
    }

    FCFS(pro);
    SPF(pro);

    return 0;
}