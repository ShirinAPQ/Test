#include<stdio.h>
int main()
{
    int yh[10][10]={0};
    int i,j;
    for(i=0;i<10;i++)
    {
        yh[i][0]=1;
        yh[i][i]=1;
    }
    for(i=2;i<10;i++)
        for(j=1;j<i;j++)
            yh[i][j]=yh[i-1][j-1]+yh[i-1][j];
    for(i=0;i<10;i++)
    {
        for(j=0;j<=i;j++)
            printf("%d ",yh[i][j]);
        printf("\n");
    }
    return 0;
}
