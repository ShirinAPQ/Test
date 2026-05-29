#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

// 全局统计
int total_instruction = 0;
int diseffect = 0; // 缺页次数

// 辅助：判断页面是否在内存
bool isInMemory(const vector<int>& mem, int page) {
    for (int x : mem) if (x == page) return true;
    return false;
}

// FIFO
void FIFO(const vector<int>& pages, int frameNum) {
    diseffect = 0;
    queue<int> q;
    vector<int> mem;
    cout << "\n===== FIFO 置换过程 =====" << endl;
    for (int page : pages) {
        cout << "访问页面 " << page << "：";
        if (isInMemory(mem, page)) {
            cout << "命中 | 内存：";
        } else {
            diseffect++;
            if (mem.size() < frameNum) {
                mem.push_back(page);
                q.push(page);
            } else {
                int out = q.front(); q.pop();
                for (int& x : mem) if (x == out) { x = page; break; }
                q.push(page);
                cout << "缺页，置换 " << out << " → ";
            }
            cout << "缺页 | 内存：";
        }
        for (int x : mem) cout << x << " ";
        cout << endl;
    }
    double rate = 1.0 - (double)diseffect / pages.size();
    cout << "FIFO 缺页次数：" << diseffect 
         << "，缺页率：" << fixed << setprecision(2) << (double)diseffect/pages.size()*100 << "%"
         << "，命中率：" << rate*100 << "%" << endl;
}

// OPT：找未来最久不使用的页
int findOPT(const vector<int>& pages, int cur, const vector<int>& mem) {
    int idx = -1, farthest = cur;
    for (int i = 0; i < mem.size(); i++) {
        int j;
        for (j = cur; j < pages.size(); j++) {
            if (pages[j] == mem[i]) break;
        }
        if (j > farthest) { farthest = j; idx = i; }
    }
    return idx;
}

void OPT(const vector<int>& pages, int frameNum) {
    diseffect = 0;
    vector<int> mem;
    cout << "\n===== OPT 置换过程 =====" << endl;
    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        cout << "访问页面 " << page << "：";
        if (isInMemory(mem, page)) {
            cout << "命中 | 内存：";
        } else {
            diseffect++;
            if (mem.size() < frameNum) {
                mem.push_back(page);
            } else {
                int outIdx = findOPT(pages, i+1, mem);
                int out = mem[outIdx];
                mem[outIdx] = page;
                cout << "缺页，置换 " << out << " → ";
            }
            cout << "缺页 | 内存：";
        }
        for (int x : mem) cout << x << " ";
        cout << endl;
    }
    double rate = 1.0 - (double)diseffect / pages.size();
    cout << "OPT 缺页次数：" << diseffect 
         << "，缺页率：" << fixed << setprecision(2) << (double)diseffect/pages.size()*100 << "%"
         << "，命中率：" << rate*100 << "%" << endl;
}

// LRU：用时间戳
void LRU(const vector<int>& pages, int frameNum) {
    diseffect = 0;
    vector<int> mem;
    unordered_map<int, int> lastUse; // 页面→最近访问时间
    cout << "\n===== LRU 置换过程 =====" << endl;
    for (int time = 0; time < pages.size(); time++) {
        int page = pages[time];
        cout << "访问页面 " << page << "：";
        if (isInMemory(mem, page)) {
            lastUse[page] = time;
            cout << "命中 | 内存：";
        } else {
            diseffect++;
            if (mem.size() < frameNum) {
                mem.push_back(page);
                lastUse[page] = time;
            } else {
                // 找最久未用
                int outIdx = 0;
                for (int i = 1; i < mem.size(); i++) {
                    if (lastUse[mem[i]] < lastUse[mem[outIdx]])
                        outIdx = i;
                }
                int out = mem[outIdx];
                mem[outIdx] = page;
                lastUse.erase(out);
                lastUse[page] = time;
                cout << "缺页，置换 " << out << " → ";
            }
            cout << "缺页 | 内存：";
        }
        for (int x : mem) cout << x << " ";
        cout << endl;
    }
    double rate = 1.0 - (double)diseffect / pages.size();
    cout << "LRU 缺页次数：" << diseffect 
         << "，缺页率：" << fixed << setprecision(2) << (double)diseffect/pages.size()*100 << "%"
         << "，命中率：" << rate*100 << "%" << endl;
}

// 随机生成页面序列（0~9，长度20）
vector<int> generatePages(int len=20) {
    vector<int> res;
    srand((unsigned)time(0));
    for (int i=0; i<len; i++) res.push_back(rand()%10);
    return res;
}

int main() {
    int frameNum;
    cout << "请输入内存物理块数（3或4）：";
    cin >> frameNum;
    vector<int> pages = generatePages(20);
    cout << "页面访问序列：";
    for (int x : pages) cout << x << " ";
    cout << endl;

    FIFO(pages, frameNum);
    OPT(pages, frameNum);
    LRU(pages, frameNum);

    cout << "\n===== 结论 =====" << endl;
    cout << "1. 内存块数越多，缺页率越低（空间换时间）" << endl;
    cout << "2. 性能：OPT > LRU > FIFO（理论最优→实用最优→简单）" << endl;
    cout << "3. FIFO简单但可能Belady异常；LRU接近OPT但开销大" << endl;
    return 0;
}