//
// Created by ALIENWARE on 2021/7/1.
//
#include"memory.h"
#include"order.h"
#ifndef CPU_RISC_V_CPU_H
#define CPU_RISC_V_CPU_H
class CPU{
private:
    unsigned int Register[32];
    order *current_order;
    void IF();
    void ID();
    void EX();
    void MEM();
    void WB();
public:
    memory CPU_MEM;
    CPU(long long cap=1000000): CPU_MEM(cap){
        for(int i=0;i<32;++i){
            Register[i]=0;
        }
    };
    void run();
};


#endif //CPU_RISC_V_CPU_H
