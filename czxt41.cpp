#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX 20    // 最大磁道请求数
int track[MAX];   // 存放磁道序列
int num;          // 磁道数量
int curTrack;     // 当前磁头位置

// 随机生成磁道序列
void RandTrack()
{
    srand((unsigned int)time(NULL));
    printf("请输入要处理的磁道数：");
    scanf("%d", &num);
    printf("随机生成磁道号:\n");
    for (int i = 0; i < num; i++)
    {
        track[i] = rand() % 200; // 磁道范围 0~199
        printf("%d ", track[i]);
    }
    printf("数据生成成功!\n\n");
}

// 最短寻道时间优先 SSTF
void SSTF()
{
    int temp[MAX], flag[MAX] = {0};
    int total = 0, now = curTrack;
    // 拷贝磁道
    for (int i = 0; i < num; i++)
        temp[i] = track[i];

    printf("排序后的磁道分布:\n");
    for (int i = 0; i < num; i++)
        printf("%d ", temp[i]);
        printf("\n");

    printf("请输入当前磁道号:\n");
    scanf("%d", &now);
    curTrack = now;
    printf("当前访问的磁道:%d\n", now);

    for (int k = 0; k < num; k++)
    {
        int min = 9999, pos = 0;
        // 找距离当前磁道最近且未访问的磁道
        for (int i = 0; i < num; i++)
        {
            if (flag[i] == 0 && abs(temp[i] - now) < min)
            {
                min = abs(temp[i] - now);
                pos = i;
            }
        }
        flag[pos] = 1;
        total += min;
        now = temp[pos];
        printf("当前访问的磁道:%d\n", now);
    }

    printf("经过的总磁道数为:%d\n", total);
    printf("移动的平均磁道数:%.2f\n\n", (double)total / num);
}

// 电梯算法 SCAN (默认向磁道增大方向)
void SCAN()
{
    int temp[MAX], flag[MAX] = {0};
    int total = 0, now = curTrack;
    for (int i = 0; i < num; i++)
        temp[i] = track[i];

    printf("排序后的磁道分布:\n");
    for (int i = 0; i < num; i++)
        printf("%d ", temp[i]);
        printf("\n");

    printf("请输入当前磁道号:\n");
    scanf("%d", &now);
    curTrack = now;
    printf("当前访问的磁道:%d\n", now);

    // 第一步：向磁道号增大方向移动
    for (int i = 0; i < num; i++)
    {
        int select = -1;
        for (int j = 0; j < num; j++)
        {
            if (flag[j] == 0 && temp[j] >= now)
            {
                if (select == -1 || temp[j] < temp[select])
                    select = j;
            }
        }
        if (select == -1) break;
        flag[select] = 1;
        total += abs(temp[select] - now);
        now = temp[select];
        printf("当前访问的磁道:%d\n", now);
    }

    // 第二步：向磁道号减小方向移动
    for (int i = 0; i < num; i++)
    {
        int select = -1;
        for (int j = 0; j < num; j++)
        {
            if (flag[j] == 0 && temp[j] <= now)
            {
                if (select == -1 || temp[j] > temp[select])
                    select = j;
            }
        }
        if (select == -1) break;
        flag[select] = 1;
        total += abs(temp[select] - now);
        now = temp[select];
        printf("当前访问的磁道:%d\n", now);
    }

    printf("经过的总磁道数为:%d\n", total);
    printf("移动的平均磁道数:%.2f\n\n", (double)total / num);
}

int main()
{
    int op;
    RandTrack(); // 生成随机磁道

    while (1)
    {
        printf("算法选择\n");
        printf("1.最短寻道时间算法\n");
        printf("2.扫描算法\n");
        printf("0.退出程序\n");
        printf("请输入你想使用的方法:");
        scanf("%d", &op);
        if (op == 0)
            break;
        switch (op)
        {
            case 1:
                SSTF();
                break;
            case 2:
                SCAN();
                break;
            default:
                printf("输入错误，请重新选择！\n\n");
                break;
        }
    }
    system("pause");
    return 0;
}