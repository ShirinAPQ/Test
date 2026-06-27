#include<stdio.h>
double average(double arr[],int n)
{
    double sum=0;
    for(int i=0;i<n;i++)//-------------------------------循环计算数组元素的和
        sum=sum+arr[i];//--------------------------------累加数组元素
    return sum/n;//--------------------------------------返回数组元素的平均值
}
int main()
{
    double num[]={60.0,78,69.5,40.5,80,95.5,60.2,37,100,53.5};//--------创建一个数组
    int len=sizeof(num)/sizeof(num[0]);//-------------------------------计算数组长度
    printf("数组平均值为：%.2lf\n",average(num,len));//-------------------调用函数计算平均值
    return 0;
}
