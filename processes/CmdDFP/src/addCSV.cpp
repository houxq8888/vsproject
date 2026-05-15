#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <vector>
#include <random>
#include <thread>
#include <algorithm>

std::string generateTime(){
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

int main(){
    std::ofstream file("AST_result.csv", std::ios::app);
    if (!file){
        return -1;
    }

    std::vector<std::string> chemicals = {

    };

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> dis(0, 20.0);

    while (true){
        std::string time = generateTime();

        std::shuffle(chemicals.begin(), chemicals.end(), gen);

        for (const auto& chemical : chemicals){
            double result = dis(gen);
            file << time << ",,";
            file << chemical << ",,";
            file << result << ",g/L\n";
        }

        file.flush();

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    file.close();
    return 0;
}