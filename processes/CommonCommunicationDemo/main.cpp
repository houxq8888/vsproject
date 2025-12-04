#include "demo.h"
#include <iostream>

int main() {
    Demo demo;
    std::string protocol_file="/mnt/extra/projects/Git/HGPro/processes/CommonCommunicationDemo/protocol_commands.json";
    
    if (!demo.initialize("/dev/ttyS1", 115200,protocol_file)) {
        std::cerr << "Initialization failed" << std::endl;
        return 1;
    }
    
    std::cout << "Demo started. Press Ctrl+C to exit." << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    try {
        demo.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}