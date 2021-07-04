//
// Created by ALIENWARE on 2021/6/30.
//

#include "order.h"
#include<iostream>
order::order(unsigned char *data) {
      for(int i=0;i<4;++i){
          code[3-i]=int(data[i]);
      }
}

Lui::Lui(unsigned char *data) : order(data) {
    op=lui;
    imm=0;
    rd_id=0;
    EX_need=true;
    MEM_need=false;
    WB_need=true;
}

void Lui::ID_exe(memory &MEM,unsigned int *reg) {
    imm=(code[0]<<12)+(code[1]<<4)+(code[2]>>4);
    rd_id=((code[2]%16)<<1)+(code[3]>>7);
}

void Lui::Ex_exe(memory &MEM,unsigned int *reg) {
    imm=imm<<12;
}

void Lui::WB_exe(memory &MEM,unsigned int *reg) {
    reg[rd_id]=imm;
}

Auipc::Auipc(unsigned char *data):order(data) {
    op=auipc;
    imm=0;
    rd_id=0;
    EX_need=true;
    MEM_need=false;
    WB_need=true;
}

void Auipc::ID_exe(memory &MEM,unsigned int *reg) {
    imm=(code[0]<<12)+(code[1]<<4)+(code[2]>>4);
    rd_id=((code[2]%16)<<1)+(code[3]>>7);
}

void Auipc::Ex_exe(memory &MEM,unsigned int *reg) {
    imm=(imm<<12)+MEM.get_PC();
}

void Auipc::WB_exe(memory &MEM,unsigned int *reg) {
    reg[rd_id]=imm;
}


Jal::Jal(unsigned char *data):order(data) {
    op=jal;
    imm=0;
    rd_id=0;
    rd_data=0;
    EX_need=true;
    MEM_need=false;
    WB_need=true;
}

void Jal::ID_exe(memory &MEM,unsigned int *reg) {
    imm=(code[1]>>5)+((code[0]%128)<<3)+(((code[1]>>4)%2)<<10)+(code[2]<<7)+((code[1]%16)<<15);
    imm=imm<<1;
    if(code[0]>=128)  imm-=1048576;
    rd_id=((code[2]%16)<<1)+(code[3]>>7);
    if(rd_id==0) WB_need= false;
}

void Jal::Ex_exe(memory &MEM,unsigned int *reg) {
    rd_data=MEM.get_PC()+4;
    MEM.jump(imm);
    MEM.jump_flag=true;
}

void Jal::WB_exe(memory &MEM,unsigned int *reg) {
    reg[rd_id]=rd_data;
}


Jalr::Jalr(unsigned char *data) : order(data) {
    op=jalr;
    imm=0;
    rd_id=0;
    rd_data=0;
    rs_id=0;
    rs_data=0;
    EX_need=true;
    MEM_need=false;
    WB_need=true;
}

void Jalr::ID_exe(memory &MEM,unsigned int *reg) {
    imm=(code[0]<<4)+(code[1]>>4);
    if(code[0]>=128) imm-=4096;
    rs_id=((code[1]%16)<<1)+(code[2]>>7);
    rs_data=reg[rs_id];
    rd_id=((code[2]%16)<<1)+(code[3]>>7);
    rd_data=((rs_data+imm)>>1)<<1;//最低位置零
    if(rd_id==0)  WB_need=false;
}

void Jalr::Ex_exe(memory &MEM,unsigned int *reg) {
    rd_data=MEM.get_PC()+4;
    MEM.set_PC(rs_data+imm);
    MEM.jump_flag=true;
}

void Jalr::WB_exe(memory &MEM,unsigned int *reg) {
    reg[rd_id]=rd_data;
}

Load::Load(unsigned char *data) : order(data) {
    op=ld;
    imm=0;
    rd_id=0;
    rd_data=0;
    rs_id=0;
    rs_data=0;
    func3=0;
    address=0;
    EX_need=true;
    MEM_need=true;
    WB_need=true;
}

void Load::ID_exe(memory &MEM,unsigned int *reg) {
    imm=(code[0]<<4)+(code[1]>>4);
    if(code[0]>=128) imm-=4096;
    rs_id=((code[1]%16)<<1)+(code[2]>>7);
    rs_data=reg[rs_id];
    rd_id=((code[2]%16)<<1)+(code[3]>>7);
    func3=(code[2]>>4)%8;
}

void Load::Ex_exe(memory &MEM,unsigned int *reg) {
    address=rs_data+imm;
}

void Load::MEM_exe(memory &MEM,unsigned int *reg) {
    switch(func3){
         case 0:case 4:{              //lb,lbu
            rd_data=MEM.mem[address];
            break;
         }
         case 1: case  5:{            //lh,lhu
             rd_data=MEM.mem[address]+(MEM.mem[address+1]<<8);
             break;
         }
         case 2:{
             rd_data=MEM.mem[address]+(MEM.mem[address+1]<<8)+(MEM.mem[address+2]<<16)+(MEM.mem[address+3]<<24);
             break;
         }
    }

}

void Load::WB_exe(memory &MEM,unsigned int *reg) {
    reg[rd_id]=rd_data;
}

Control::Control(unsigned char *data) : order(data) {
    op=ld;
    imm=0;
    rs_id1=0;
    rs_data1=0;
    rs_id2=0;
    rs_data2=0;
    func3=0;
    EX_need=true;
    MEM_need=false;
    WB_need=false;
}

void Control::ID_exe(memory &MEM,unsigned int *reg) {
    op=B;
    imm=code[2]%16+(((code[0]>>1)%64)<<4)+((code[3]>>7)<<10);
    if(code[0]>=128) imm-=2048;
    imm*=2;
    rs_id1=((code[1]%16)<<1)+(code[2]>>7);
    rs_id2=(code[1]>>4)+((code[0]%2)<<4);
    rs_data1=reg[rs_id1];
    rs_data2=reg[rs_id2];
    func3=(code[2]>>4)%8;
}

void Control::Ex_exe(memory &MEM,unsigned int *reg) {
    switch(func3){
        case 0:{
            if(rs_data1==rs_data2) {
                MEM.jump(imm);
                MEM.jump_flag=true;
            }
            break;
        }
        case 1:{
            if(rs_data1!=rs_data2) {
                MEM.jump(imm);
                MEM.jump_flag=true;
            }
            break;
        }
        case 4:{
            int comp1=rs_data1;
            int comp2=rs_data2;
            if(comp1<comp2) {
                MEM.jump(imm);
                MEM.jump_flag=true;
            }
            break;
        }
        case 6:{
            if(rs_data1<rs_data2) {
                MEM.jump(imm);
                MEM.jump_flag=true;
            }
            break;
        }
        case 5:case 7:{
            if(rs_data1>=rs_data2) {
                MEM.jump(imm);
                MEM.jump_flag=true;
            }
            break;
        }
    }
}

I_class ::I_class(unsigned char *data):order(data) {
    op=I;
    imm=0;
    rs_id1=0;
    rs_data1=0;
    rd_id=0;
    rd_data=0;
    shamt=0;
    func3=0;
    func7=0;
    EX_need=true;
    MEM_need=false;
    WB_need=true;
}

void I_class::ID_exe(memory &MEM,unsigned int *reg) {
    imm=(code[0]<<4)+(code[1]>>4);
    if(code[0]>=128) imm-=4096;
    rs_id1=((code[1]%16)<<1)+(code[2]>>7);
    shamt=(code[1]>>4)+((code[0]%2)<<4);
    rd_id=((code[2]%16)<<1)+(code[3]>>7);
    if(rs_id1==0&&imm==255&&func3==0&&rd_id==10){
        std::cout<<reg[10]%256;
        exit(0);
    }
    rs_data1=reg[rs_id1];
    func3=(code[2]>>4)%8;
    func7=code[0]>>2;
}

void I_class::Ex_exe(memory &MEM,unsigned int *reg) {
    switch(func3){
        case 0:{
            rd_data=rs_data1+imm;
            break;
        }
        case 2: case 3:{
            if(rs_data1<imm) rd_data=1;
            else rd_data=0;
            break;
        }
        case 4:{
            rd_data=imm^rs_data1;
            break;
        }
        case 6:{
            rd_data=imm|rs_data1;
            break;
        }
        case 7:{
            rd_data=imm&rs_data1;
            break;
        }
        case 1:{
            if(((code[0]%4)>>1)==0){
                rd_data=rs_data1<<shamt;
            }
            break;
        }
        case 5:{
            if(func7==0){
                if(((code[0]%4)>>1)==0){
                    rd_data=abs(rs_data1>>shamt);
                }
                break;
            }
            else{
                if(((code[0]%4)>>1)==0){
                    rd_data=rs_data1>>shamt;
                }
                break;
            }
        }
    }
}

void I_class::WB_exe(memory &MEM,unsigned int *reg) {
    reg[rd_id]=rd_data;
}

R_class::R_class(unsigned char *data) : order(data) {
    op=R;
    rs_id1=0;
    rs_data1=0;
    rs_id2=0;
    rs_data2=0;
    func3=0;
    func7=0;
    EX_need=true;
    MEM_need=false;
    WB_need=true;
}

void R_class::ID_exe(memory &MEM,unsigned int *reg) {
    func7=code[0]>>1;
    func3=(code[2]>>4)%8;
    rs_id1=((code[1]%16)<<1)+(code[2]>>7);
    rs_id2=(code[1]>>4)+((code[0]%2)<<4);
    rs_data1=reg[rs_id1];
    rs_data2=reg[rs_id2];
    rd_id=((code[2]%16)<<1)+(code[3]>>7);
}

void R_class::Ex_exe(memory &MEM,unsigned int *reg) {
    switch(func3){
        case 0:{
            if(func7==0)
                rd_data=rs_data1+rs_data2;
            else rd_data=rs_data1-rs_data2;
            break;
        }
        case 1:{
            rd_data=rs_data1<<(rs_data2)%32;
            break;
        }
        case 2:case 3:{
            if(rs_data1<rs_data2) rd_data=1;
            else rd_data=0;
            break;
        }
        case 4:{
            rd_data=rs_data1^rs_data2;
            break;
        }
        case 5:{
            if(func7==0) rd_data=abs(rs_data1>>(rs_data2)%12);
            else rd_data=rs_data1>>(rs_data2)%12;
            break;
        }
        case 6:{
            rd_data=rs_data1|rs_data2;
            break;
        }
        case 7:{
            rd_data=rs_data1&rs_data2;
            break;
        }
    }
}

void R_class::WB_exe(memory &MEM,unsigned int *reg) {
    reg[rd_id]=rd_data;
}

Store::Store(unsigned char *data) : order(data) {
    op=S;
    imm=0;
    address=0;
    rs_id1=0;
    rs_data1=0;
    rs_id2=0;
    rs_data2=0;
    func3=0;
    EX_need=true;
    MEM_need=true;
    WB_need=false;
}

void Store::ID_exe(memory &MEM,unsigned int *reg) {
    rs_id1=((code[1]%16)<<1)+(code[2]>>7);
    rs_id2=(code[1]>>4)+((code[0]%2)<<4);
    rs_data1=reg[rs_id1];
    rs_data2=reg[rs_id2];
    func3=(code[2]>>4)%8;
    imm=((code[2]%16)<<1)+(code[3]>>7)+((code[0]>>1)<<5);
    if(code[0]>=128) imm-=4096;
}

void Store::Ex_exe(memory &MEM,unsigned int *reg) {
    address=rs_data1+imm;
}

void Store::MEM_exe(memory &MEM,unsigned int *reg) {
    switch(func3){
        case 0:{
            MEM.mem[address]=rs_data2%256;
            break;
        }
        case 1:{
            MEM.mem[address]=rs_data2%256;
            MEM.mem[address+1]=(rs_data2>>8)%256;
            break;
        }
        case 2:{
            MEM.mem[address]=rs_data2%256;
            MEM.mem[address+1]=(rs_data2>>8)%256;
            MEM.mem[address+2]=(rs_data2>>16)%256;
            MEM.mem[address+3]=(rs_data2>>24)%256;
            break;
        }
    }
}


