#include <iostream>
#include<fstream>
#include"CPU.h"
using namespace std;
int main() {
    CPU RISC_V_CPU;
    string order_data;
    int counter=0;
    while(cin>>order_data){
        RISC_V_CPU.CPU_MEM.push(order_data);
        ++counter;
    }
    RISC_V_CPU.run();
    return 0;
}