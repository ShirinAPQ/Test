#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_NUM 20   // 最大磁道请求数
#define MAX_CYL 200  // 磁盘最大磁道号(0~199)

int request[MAX_NUM];  // 存储磁盘请求磁道序列
int req_num;           // 实际请求数量
int head_pos;          // 磁头初始位置

// 随机生成磁盘请求序列
void createRandomRequest()
{
    srand((unsigned int)time(NULL));
    printf("请输入磁盘请求数量(1-%d)：", MAX_NUM);
    scanf("%d", &req_num);
    for (int i = 0; i < req_num; i++)
    {
        request[i] = rand() % MAX_CYL;
    }
    printf("随机生成的磁道请求序列：");
    for (int i = 0; i < req_num; i++)
    {
        printf("%d ", request[i]);
    }
    printf("\n");
}

// FCFS 先来先服务算法
void FCFS()
{
    printf("========== FCFS 先来先服务算法 ==========\n");
    int cur = head_pos;
    int total_move = 0;
    printf("磁头访问顺序：%d", cur);
    for (int i = 0; i < req_num; i++)
    {
        total_move += abs(cur - request[i]);
        cur = request[i];
        printf(" -> %d", cur);
    }
    printf("\n总寻道距离：%d\n", total_move);
    printf("平均寻道长度：%.2f\n\n", (double)total_move / req_num);
}

// SSTF 最短寻道时间优先算法
void SSTF()
{
    printf("========== SSTF 最短寻道时间优先算法 ==========\n");
    int temp_req[MAX_NUM];
    // 拷贝请求序列，避免修改原数据
    for (int i = 0; i < req_num; i++)
        temp_req[i] = request[i];

    int cur = head_pos;
    int total_move = 0;
    int count = 0;
    int min_dis, min_idx;
    printf("磁头访问顺序：%d", cur);

    while (count < req_num)
    {
        min_dis = MAX_CYL;
        min_idx = -1;
        // 查找距离当前磁头最近的未访问磁道
        for (int i = 0; i < req_num; i++)
        {
            if (temp_req[i] != -1)
            {
                int dis = abs(cur - temp_req[i]);
                if (dis < min_dis)
                {
                    min_dis = dis;
                    min_idx = i;
                }
            }
        }
        // 访问该磁道
        total_move += min_dis;
        cur = temp_req[min_idx];
        printf(" -> %d", cur);
        temp_req[min_idx] = -1; // 标记已访问
        count++;
    }
    printf("\n总寻道距离：%d\n", total_move);
    printf("平均寻道长度：%.2f\n\n", (double)total_move / req_num);
}

// SCAN 电梯算法(默认初始方向：磁道号增大)
void SCAN()
{
    printf("========== SCAN 电梯算法(向磁道号增大方向) ==========\n");
    int temp_req[MAX_NUM];
    for (int i = 0; i < req_num; i++)
        temp_req[i] = request[i];

    int cur = head_pos;
    int total_move = 0;
    int count = 0;
    printf("磁头访问顺序：%d", cur);

    // 第一步：向磁道号增大方向访问
    while (count < req_num)
    {
        int find = 0;
        // 寻找当前位置右侧(更大磁道号)的请求
        for (int i = 0; i < req_num; i++)
        {
            if (temp_req[i] != -1 && temp_req[i] >= cur)
            {
                int dis = abs(cur - temp_req[i]);
                total_move += dis;
                cur = temp_req[i];
                printf(" -> %d", cur);
                temp_req[i] = -1;
                count++;
                find = 1;
                break;
            }
        }
        if (!find) break; // 右侧无请求，退出第一阶段
    }

    // 第二步：向磁道号减小方向访问剩余请求
    while (count < req_num)
    {
        int find = 0;
        // 寻找当前位置左侧(更小磁道号)的请求
        for (int i = 0; i < req_num; i++)
        {
            if (temp_req[i] != -1 && temp_req[i] <= cur)
            {
                int dis = abs(cur - temp_req[i]);
                total_move += dis;
                cur = temp_req[i];
                printf(" -> %d", cur);
                temp_req[i] = -1;
                count++;
                find = 1;
                break;
            }
        }
        if (!find) break;
    }
    printf("\n总寻道距离：%d\n", total_move);
    printf("平均寻道长度：%.2f\n\n", (double)total_move / req_num);
}

int main()
{
    // 1. 生成随机磁道请求
    createRandomRequest();
    // 2. 输入磁头初始位置
    printf("请输入磁头初始位置(0-%d)：", MAX_CYL - 1);
    scanf("%d", &head_pos);
    printf("=============================================\n\n");

    // 3. 执行三种调度算法
    FCFS();
    SSTF();
    SCAN();

    system("pause");
    return 0;
}