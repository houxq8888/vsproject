#include "YModem.h"
#include <iostream>

int main(){
    Ymodem y("/dev/pts/4",LibSerial::BaudRate::BAUD_115200);
    if(y.sendFile("a.txt")){
        std::cout<<"发送成功"<<std::endl;
    }

    return 0;
}
