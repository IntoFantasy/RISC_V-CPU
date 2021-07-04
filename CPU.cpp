//
// Created by ALIENWARE on 2021/7/1.
//
#include<map>
#include<iostream>
#include "CPU.h"
std::map<int,operation> orders={{55,lui},{23,auipc},{111,jal},{103,jalr},{99,B},{3,ld},{35,S},{19,I},{51,R},{15,fence},{115,I3}};
void CPU::IF() {
    int order_int=int(CPU_MEM.mem[CPU_MEM.get_PC()]);
    order_int%=128;
    operation op=orders[order_int];
    unsigned char *data=CPU_MEM.mem+CPU_MEM.get_PC();
    switch(op){
        case lui:{current_order=new Lui(data); break;}
        case auipc:{current_order=new Auipc(data);break;}
        case jal:{current_order=new Jal(data);break;}
        case jalr:{current_order=new Jalr(data);break;}
        case B:{current_order=new Control(data);break;}
        case ld:{current_order=new Load(data);break;}
        case S:{current_order=new Store(data);break;}
        case I:{current_order=new I_class(data);break;}
        case R:{current_order=new R_class(data);break;}
    }
}

void CPU::ID() {
    current_order->ID_exe(CPU_MEM,Register);
}

void CPU::EX() {
    current_order->Ex_exe(CPU_MEM,Register);
}

void CPU::MEM() {
    current_order->MEM_exe(CPU_MEM,Register);
}

void CPU::WB() {
    current_order->WB_exe(CPU_MEM,Register);
}

void CPU::run() {
    while(true) {
        IF();
        ID();
        if (current_order->EX_need)
            EX();
        if (current_order->MEM_need)
            MEM();
        if (current_order->WB_need)
            WB();
        if(!CPU_MEM.jump_flag) CPU_MEM.jump(4);
        else CPU_MEM.jump_flag=false;
    }
}

