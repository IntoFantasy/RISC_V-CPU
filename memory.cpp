//
// Created by ALIENWARE on 2021/6/29.
//

#include "memory.h"
#include<iostream>
memory::memory(long long cap) {
   PC=0;
   count=0;
   capacity=cap;
   mem=new unsigned char[cap];
   jump_flag=false;
   for(int i=0;i<capacity;++i){
       mem[i]=0;
   }
}

void memory::jump(long long offset) {
    PC+=offset;
}

void memory::doublespace() {
   unsigned char *tmp=new unsigned char[2*capacity];
   capacity*=2;
   for(int i=0;i<count;++i){
    tmp[i]=mem[i];
   }
   delete mem;
   mem=tmp;
}

void memory::push(std::string data) {
    if(count==capacity) doublespace();
    if(data[0]=='@'){
         long long ans=0;
         long long ct=1;
         for(int i=data.size()-1;i>=1;--i){
             if(data[i]-'0'>=0&&data[i]-'0'<=9)
                 ans+=ct*(data[i]-'0');
             else
                 ans+=ct*(data[i]-'A'+10);
             ct*=16;
         }
         count=ans;
    }
    else{
        unsigned char tmp;
        if(data[0]-'0'>=0&&data[0]-'0'<=9) tmp=(data[0]-'0')*16;
        else tmp=(data[0]-'A'+10)*16;
        if(data[1]-'0'>=0&&data[1]-'0'<=9) tmp+=(data[1]-'0');
        else tmp+=(data[1]-'A'+10);
        mem[count]=tmp;
        ++count;
    }


}

void memory::set_PC(long long num) {
   PC=num;
}


long long memory::get_PC() {
    return PC;
}

long long memory::get_count() {
    return count;
}





