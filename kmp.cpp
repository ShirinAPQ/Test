#include <iostream>
#include <string>
#include <vector>
using namespace std;

//next1函数计算模式串的next数组
vector<int> next1(const string &p){
    if(p.size() < 2){
        return {0};
    }
    int m = p.size();
    vector<int> next(m);
    next[0] = 0;
    next[1] = 0;
    int a = 0;//前缀相等长度
    int b = 2;//后缀位置
    while(b < m){
        if(p[a] == p[b-1]){//注意为b-1
            next[b] =a + 1;
            a++;//后移
            b++;//后移
        }else if(a > 0){//不相等回退
            a = next[a];
        }else{//仍不相等置0
            next[b] = 0;
            b++;
        }
        }
        return next;
    }
//kmp算法
int kmp(const string &text,const string &pattern){
    vector<int> next = next1(pattern);
    size_t i = 0;//文本串位置,不回溯
    size_t j = 0;//模式串位置
    while(i < text.size() && j < pattern.size()){
        if(text[i] == pattern[j]){
            i++;
            j++;
        }else if(j>0){
            j = next[j];
        }else{
            i++;
        }
    }
    return i-j+1;

}

int main() {
    string text,pattern;
    //cout<<"请输入文本串：";
    cout<<"text:";
    cin>>text;
    //cout<<"请输入模式串：";
    cout<<"pattern:";
    cin>>pattern;
    int result = kmp(text,pattern);

    /*if(result != -1){
        cout<<"模式串在文本串中的位置为："<<result<<endl;
    }else{
        cout<<"未找到模式串。"<<endl;
    }*/
   
    if(result != -1){
        cout<<"pattern'location in text is :"<<result<<endl;
    }else{
        cout<<"pattern not found in text."<<endl;
    }

    return 0;
}