#include <windows.h>
#include <iostream>

CRITICAL_SECTION cs; 
int count = 0;        // 全局共享变量

// 线程1函数
DWORD WINAPI Thread1Proc(LPVOID lpParameter) {
    for (int i = 0; i < 5; ++i) {
        EnterCriticalSection(&cs); // 进入临界区
        count++;
        std::cout << "线程1：count = " << count << std::endl;
        Sleep(500);
        LeaveCriticalSection(&cs); // 退出临界区
    }
    return 0;
}

// 线程2函数
DWORD WINAPI Thread2Proc(LPVOID lpParameter) {
    for (int i = 0; i < 5; ++i) {
        EnterCriticalSection(&cs); // 进入临界区
        count++;
        std::cout << "线程2：count = " << count << std::endl;
        Sleep(500);
        LeaveCriticalSection(&cs); // 退出临界区
    }
    return 0;
}

int main() {

    std::cout << "计算机25-5齐浩凯2404010526" << std::endl;

    InitializeCriticalSection(&cs);

    std::cout << "主线程：创建两个子线程..." << std::endl;
    HANDLE hThread1 = CreateThread(NULL, 0, Thread1Proc, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, Thread2Proc, NULL, 0, NULL);

    if (hThread1 == NULL || hThread2 == NULL) {
        std::cerr << "创建线程失败！错误码：" << GetLastError() << std::endl;
        DeleteCriticalSection(&cs);
        return 1;
    }

    // 等待两个子线程执行完毕
    HANDLE hThreads[] = {hThread1, hThread2};
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    std::cout << "主线程：所有线程执行完毕，最终count = " << count << std::endl;

    // 清理资源
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    DeleteCriticalSection(&cs);
    return 0;
}