#include "dfp.h"

int main(){
    DFP monitor;
    int count = 1;

    monitor.recvMQTTMessage();

    while (true){
        for (auto it: monitor.getMSG()){
            std::cout << "Message " << count << ": " << count << std::endl;
            count++;
        }
    }

    return 0;
}