//
// Created by ALIENWARE on 2021/6/30.
//
#include "memory.h"
#ifndef CPU_RISC_V_ORDER_H
#define CPU_RISC_V_ORDER_H
enum operation{lui,auipc,jal,jalr,B,ld,S,I,R,I3,fence};       //仅仅是大类区分（由最后七位决定）,具体分类到exe再执行
class order{
protected:
    int code[4];     //以四位十进制存储
public:
    order()=default;
    order(unsigned char *data);
    virtual void ID_exe(memory &MEM,unsigned int *reg){return;};
    bool EX_need;
    virtual void Ex_exe(memory &MEM,unsigned int *reg){return;};
    bool MEM_need;
    virtual void MEM_exe(memory &MEM,unsigned int *reg){return;};
    bool WB_need;
    virtual void WB_exe(memory &MEM,unsigned int *reg){return;};
    operation op;
};
class Lui:public order{
private:
    int imm;//立即数
    int rd_id;//目标寄存器的地址
public:
    Lui(unsigned char *data);
    void ID_exe(memory &MEM,unsigned int *reg) override;
    void Ex_exe(memory &MEM,unsigned int *reg) override;
    void WB_exe(memory &MEM,unsigned int *reg) override;
};
class Auipc:public order{
private:
    int imm;
    int rd_id;
public:
    Auipc(unsigned char *data);
    void ID_exe(memory &MEM,unsigned int *reg) override;
    void Ex_exe(memory &MEM,unsigned int *reg) override;
    void WB_exe(memory &MEM,unsigned int *reg) override;
};
class Jal:public order{
private:
    int imm;
    int rd_id;
    unsigned int rd_data;
public:
    Jal(unsigned char *data);
    void ID_exe(memory &MEM,unsigned int *reg) override;
    void Ex_exe(memory &MEM,unsigned int *reg) override;
    void WB_exe(memory &MEM,unsigned int *reg) override;
};
class Jalr:public order{
private:
    int imm;
    int rd_id;
    unsigned int rd_data;
    int rs_id;
    unsigned int rs_data;
public:
    Jalr(unsigned char *data);
    void ID_exe(memory &MEM,unsigned int *reg) override;
    void Ex_exe(memory &MEM,unsigned int *reg) override;
    void WB_exe(memory &MEM,unsigned int *reg) override;
};
class Load:public order{
private:
    int func3;
    int rd_id;
    unsigned int rd_data;
    int rs_id;
    unsigned int rs_data;
    int imm;
    int address;
public:
    Load(unsigned char *data);
    void ID_exe(memory &MEM,unsigned int *reg) override;
    void Ex_exe(memory &MEM,unsigned int *reg) override;
    void MEM_exe(memory &MEM,unsigned int *reg) override;
    void WB_exe(memory &MEM,unsigned int *reg) override;

};
class Control:public order{
private:
    int imm;
    int rs_id1;
    unsigned int rs_data1;
    int rs_id2;
    unsigned int rs_data2;
    int func3;
public:
    Control(unsigned char *data);
    void ID_exe(memory &MEM,unsigned int *reg) override;
    void Ex_exe(memory &MEM,unsigned int *reg) override;
};
class I_class:public order{
private:
    int imm;
    int rd_id;
    unsigned int rd_data;
    int rs_id1;
    unsigned int rs_data1;
    int func3;
    int func7;
    int shamt;
public:
    I_class(unsigned char *data);
    void ID_exe(memory &MEM,unsigned int *reg) override;
    void Ex_exe(memory &MEM,unsigned int *reg) override;
    void WB_exe(memory &MEM,unsigned int *reg) override;
};
class R_class:public order{
private:
    int rs_id1;
    unsigned int rs_data1;
    int rs_id2;
    unsigned int rs_data2;
    int rd_id;
    unsigned int rd_data;
    int func3;
    int func7;
public:
    R_class(unsigned char *data);
    void ID_exe(memory &MEM,unsigned int *reg) override;
    void Ex_exe(memory &MEM,unsigned int *reg) override;
    void WB_exe(memory &MEM,unsigned int *reg) override;
};
class Store:public order{
private:
    int rs_id1;
    unsigned int rs_data1;
    int rs_id2;
    unsigned int rs_data2;
    int func3;
    int imm;
    int address;
public:
    Store(unsigned char *data);
    void ID_exe(memory &MEM,unsigned int *reg) override;
    void Ex_exe(memory &MEM,unsigned int *reg) override;
    void MEM_exe(memory &MEM,unsigned int *reg) override;
};
#endif //CPU_RISC_V_ORDER_H
