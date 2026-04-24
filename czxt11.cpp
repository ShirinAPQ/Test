#include <windows.h>
#include <iostream>

HANDLE hSemaphore; // 信号量句柄



// 子线程函数
DWORD WINAPI ChildThreadProc(LPVOID lpParameter) {
    std::cout << "子线程：开始执行任务..." << std::endl;
    Sleep(2000); // 模拟耗时操作（2秒）
    std::cout << "子线程：任务执行完毕，准备唤醒父线程" << std::endl;

    // 释放信号量，唤醒等待的父线程
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 0;
}

int main() {

    std::cout << "计算机25-5齐浩凯2404010526" << std::endl;

    // 创建初始值为0、最大值为1的信号量
    hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
    if (hSemaphore == NULL) {
        std::cerr << "创建信号量失败！错误码：" << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "父线程：创建子线程..." << std::endl;
    HANDLE hThread = CreateThread(NULL, 0, ChildThreadProc, NULL, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "创建子线程失败！错误码：" << GetLastError() << std::endl;
        CloseHandle(hSemaphore);
        return 1;
    }

    std::cout << "父线程：等待子线程执行完毕..." << std::endl;
    // 等待信号量被释放，进入阻塞状态
    WaitForSingleObject(hSemaphore, INFINITE);

    std::cout << "父线程：收到子线程通知，继续执行" << std::endl;

    // 清理资源
    CloseHandle(hThread);
    CloseHandle(hSemaphore);
    return 0;
}