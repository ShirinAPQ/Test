#include <windows.h>
#include <iostream>
#include <cstdio>

using namespace std;

const int BUFFER_SIZE = 5;  // 缓冲区大小N
int buffer[BUFFER_SIZE];     
int in = 0;                  // 生产者写入
int out = 0;                 // 消费者读取

// 信号量
HANDLE empty_sem;  
HANDLE full_sem;   
HANDLE mutex;      

// 生产者线程函数
DWORD WINAPI Producer(LPVOID lpParam) {
    int producer_id = *(int*)lpParam;
    delete (int*)lpParam;

    for (int i = 0; i < 4; i++) { // 每个生产者生产3个产品
        
        WaitForSingleObject(empty_sem, INFINITE);
        
        WaitForSingleObject(mutex, INFINITE);

        
        buffer[in] = i + 1 + producer_id * 10; // 产品编号
        printf("生产者 %d 生产了产品 %d，存入缓冲区位置 %d\n", 
               producer_id, buffer[in], in);
        in = (in + 1) % BUFFER_SIZE;

        // 释放互斥锁
        ReleaseMutex(mutex);
        // 通知有产品可用
        ReleaseSemaphore(full_sem, 1, NULL);

        Sleep(1000); 
    }
    return 0;
}

// 消费者线程函数
DWORD WINAPI Consumer(LPVOID lpParam) {
    int consumer_id = *(int*)lpParam;
    delete (int*)lpParam;

    for (int i = 0; i < 4; i++) { // 每个消费者消费3个产品
        
        WaitForSingleObject(full_sem, INFINITE);
        
        WaitForSingleObject(mutex, INFINITE);

        int product = buffer[out];
        printf("消费者 %d 消费了产品 %d，从缓冲区位置 %d 取出\n", 
               consumer_id, product, out);
        out = (out + 1) % BUFFER_SIZE;

        // 释放互斥锁
        ReleaseMutex(mutex);
        // 通知有空缓冲区可用
        ReleaseSemaphore(empty_sem, 1, NULL);

        Sleep(1500);
    }
    return 0;
}

int main() {

    std::cout << "计算机25-5齐浩凯2404010526" << std::endl;

    // 初始化信号量和互斥锁
    empty_sem = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    full_sem = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
    mutex = CreateMutex(NULL, FALSE, NULL);

    if (empty_sem == NULL || full_sem == NULL || mutex == NULL) {
        cerr << "创建同步对象失败！错误码：" << GetLastError() << endl;
        return 1;
    }

    // 创建生产者和消费者线程
    const int PRODUCER_NUM = 2;
    const int CONSUMER_NUM = 2;
    HANDLE producers[PRODUCER_NUM];
    HANDLE consumers[CONSUMER_NUM];

    // 生产者线程
    for (int i = 0; i < PRODUCER_NUM; i++) {
        int* id = new int(i + 1);
        producers[i] = CreateThread(NULL, 0, Producer, id, 0, NULL);
        if (producers[i] == NULL) {
            cerr << "创建生产者线程失败！" << endl;
            return 1;
        }
    }

    // 消费者线程
    for (int i = 0; i < CONSUMER_NUM; i++) {
        int* id = new int(i + 1);
        consumers[i] = CreateThread(NULL, 0, Consumer, id, 0, NULL);
        if (consumers[i] == NULL) {
            cerr << "创建消费者线程失败！" << endl;
            return 1;
        }
    }

    // 线程执行完毕
    WaitForMultipleObjects(PRODUCER_NUM, producers, TRUE, INFINITE);
    WaitForMultipleObjects(CONSUMER_NUM, consumers, TRUE, INFINITE);

    printf("\n所有生产者和消费者执行完毕！\n");

    // 清理资源
    for (int i = 0; i < PRODUCER_NUM; i++) {
        CloseHandle(producers[i]);
    }
    for (int i = 0; i < CONSUMER_NUM; i++) {
        CloseHandle(consumers[i]);
    }
    CloseHandle(empty_sem);
    CloseHandle(full_sem);
    CloseHandle(mutex);

    system("pause");
    return 0;
}