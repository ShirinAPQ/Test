#include <iostream>
#include <vector>
#include <queue>

void A_sort(int *nl,int q) {
    int temp;
    for(int i=1;i<q;i++){
        for(int j=1;j<q-i;j++){
            if(nl[j]>nl[j+1]){
                temp=nl[j];
                nl[j]=nl[j+1];
                nl[j+1]=temp;
            }
        }
    }
}
struct TreeNode{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x):val(x),left(NULL),right(NULL){}
};

int main() {
    //std::cout << "Hello, World!\n";
    //std::cout << "year\n";
    //int year;
    //std::cin>>year;
    //std::cout << "year is " << year << "\n";
    std::cout << "sortNumble, input n numble\n";
    int q;
    std::cin>>q;

    int sortNumble[q];
    for(int i=0;i<q;i++){
        std::cin>>sortNumble[i];
    }

    A_sort(sortNumble,q);

    for(int i=0;i<q;i++){
        std::cout<<sortNumble[i]<< " ";
    }
    std::cout<<"\n";

    return 0;
} 