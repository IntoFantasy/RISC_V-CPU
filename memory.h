//
// Created by ALIENWARE on 2021/6/29.
//
#include<cstring>
#include <string>

#ifndef CPU_MEMORY_H
#define CPU_MEMORY_H
class memory{
private:
    long long PC;
    long long count;//输入data时候的指针
    long long capacity;
    void doublespace();
public:
    bool jump_flag;
    unsigned char *mem;
    memory(long long cap=1000000);
    void jump(long long offset=4);
    void push(std::string data);
    void set_PC(long long num);
    long long get_PC();
    long long get_count();
};
#endif //CPU_MEMORY_H
