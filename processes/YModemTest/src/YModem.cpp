#include "YModem.h"

Ymodem::Ymodem(const std::string& port, LibSerial::BaudRate baud_rate) {
    serial_port.Open(port);
    serial_port.SetBaudRate(LibSerial::BaudRate(baud_rate));
    serial_port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
    serial_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
    serial_port.SetParity(LibSerial::Parity::PARITY_NONE);
}

Ymodem::~Ymodem() {
    serial_port.Close();
}

// 计算 CRC-16
uint16_t Ymodem::calculateCRC(const std::vector<uint8_t>& data) {
    uint16_t crc = 0xFFFF; // 初始值
    const uint16_t poly = 0x1021; // 多项式

    for (uint8_t byte : data) {
        crc ^= (static_cast<uint16_t>(byte) << 8); // 当前字节左移 8 位与 CRC 异或
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// 等待特定字符
bool Ymodem::waitForChar(unsigned char expected, int timeout_ms) {
    std::string response;
    serial_port.Read(response, 1, timeout_ms);
    return !response.empty() && static_cast<unsigned char>(response[0]) == expected;
}

// 发送文件
bool Ymodem::sendFile(const std::string& filename) {
    // 1. 接收信号 'C'
    if(!waitForChar('C', 5000)){
        std::cerr << "[ERROR] 接收端未准备好" << std::endl;
        return false;
    }

    // 2. 发送起始帧
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "[ERROR] 文件打开失败: " << filename << std::endl;
        return false;
    }

    // 获取文件大小
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // 构造文件头
    LibSerial::DataBuffer header_block;
    header_block.push_back(SOH);   // SOH
    header_block.push_back(0x00);  // 包号
    header_block.push_back(0xFF);  // 包号补码

    // 数据区 文件名+'\0'+文件大小+' '
    header_block.insert(header_block.end(), filename.begin(), filename.end());
    header_block.push_back('\0');
    header_block.push_back(' ');
    header_block.insert(header_block.end(), std::to_string(file_size).begin(), std::to_string(file_size).end());
    header_block.resize(128 + 3, 0x00);  // 填充数据区至128字节

    // crc高位和crc低位
    uint16_t crc = calculateCRC(header_block);
    header_block.push_back(crc >> 8);
    header_block.push_back(crc & 0xFF);

    // 发送起始帧
    std::cout << "[INFO] 发送起始帧，大小: " << header_block.size() << " 字节" << std::endl;
    serial_port.Write(header_block);

    // 3. 等待文件头ACK
    if (!waitForChar(ACK, 5000)) {
        std::cerr << "[ERROR] 文件头 ACK 超时" << std::endl;
        return false;
    }
    std::cout << "[INFO] 文件头 ACK 确认" << std::endl;

    // 4. 发送数据帧（包号从0x01开始）

    // TODO 读临时文件，获取文件名和已传输的字节
    // TODO 判断文件名是否是传输的文件，如果是文件指针偏移file_szie - 传输字节，否则文件指针置于初始位置即可
    std::ifstream ifs_temp("temp.txt",std::ios::binary);
    if(!ifs_temp){
        std::cerr << "[ERROR] 文件打开失败: " << "temp.txt" << std::endl;
        return false;
    }
    std::string line;
    while(std::getline(ifs_temp, line)){
        std::istringstream iss(line);
        std::string temp_name;
        std::string temp_size;
        
        if(!(iss >> temp_name >> temp_size)){
            std::cerr << "[ERROR] 无效数据: " << line <<std::endl;
            continue;
        }

        if(temp_name == filename){
            file.seekg(stoi(temp_size));
            std::cout << "[INFO] 断点续传 文件: " << temp_name << " 指针偏移: " <<temp_size <<std::endl;
            break;
        }
    }
    // 关闭temp.txt
    ifs_temp.close();

    
    // (包号从0x01开始)
    LibSerial::DataBuffer data_buffer(PACKET_1024_SIZE, 0);
    uint8_t packet_no = 0x01;
    int retry_count = 0;
    const int MAX_RETRIES = 5;

    int ofs_temp_size = 0;
    while (!file.eof()) {
        file.read(reinterpret_cast<char*>(data_buffer.data()), PACKET_1024_SIZE);
        size_t bytes_read = file.gcount();
        if (bytes_read == 0) break;

        uint8_t header = (bytes_read > 128) ? STX : SOH;

        // 构造数据帧
        LibSerial::DataBuffer packet;

        // 帧头 header+包号+包号补码
        packet.push_back(header);
        packet.push_back(packet_no);
        packet.push_back(~packet_no);

        // 数据区
        packet.insert(packet.end(), data_buffer.begin(), data_buffer.begin() + bytes_read);
        if (header == SOH) {
            packet.resize(128 + 3, 0x1A);  // 填充数据区剩余部分
        } else {
            packet.resize(1024 + 3, 0x1A);  // 填充数据区剩余部分
        }

        // crc高位和crc低位
        uint16_t crc = calculateCRC(packet);
        packet.push_back(crc >> 8);
        packet.push_back(crc & 0xFF);

        // 发送数据帧（带重试）
        bool ack_received = false;
        retry_count = 0;
        do {
            std::cout << "[INFO] 发送数据帧，包号: " << (int)packet_no << " 数据帧大小: " << packet.size() << " 字节" << std::endl;
            serial_port.Write(packet);

            unsigned char response;
            serial_port.ReadByte(response, 5000);

            if (response == ACK) {
                ack_received = true;
                break;
            } else if (response == NAK) {
                retry_count++;
            } else {
                std::cerr << "[ERROR] 非预期响应: " << (int)response << std::endl;
                return false;
            }
        } while (retry_count < MAX_RETRIES);

        if (!ack_received) {

            // TODO 写临时文件 记录文件名+' '+已传输字节大小
            std::ofstream ofs_temp("temp.txt", std::ios::binary);
            if(!ofs_temp){
                std::cerr << "[ERROR] 文件打开失败: " << "temp.txt" << std::endl;
                return false;
            }
            ofs_temp << filename << " " << ofs_temp_size << std::endl;
            
            // 关闭temp.txt
            ofs_temp.close();

            std::cerr << "[ERROR] 包号 " << (int)packet_no << " 发送失败，已重试 " << retry_count << " 次" << std::endl;
            return false;
        }

        if(header == SOH){
            ofs_temp_size += ((int)packet_no * 128);
        }else{
            ofs_temp_size += ((int)packet_no * 1024);
        }

        packet_no++;
    }

    // 5. 发送EOT
    LibSerial::DataBuffer eot_packet(1, EOT);
    serial_port.Write(eot_packet);
    std::cout << "[INFO] 发送 EOT 结束传输" << std::endl;

    // 等待最终ACK
    return waitForChar(ACK, 5000);
}

// 接收文件
bool Ymodem::receiveFile(const std::string& fileName) {
    std::cout << "[INFO] 发送 'C' 请求传输..." << std::endl;
    serial_port.Write(std::vector<uint8_t>{'C'});  // 发送初始化信号 'C'

    // 2. 接收起始帧
    LibSerial::DataBuffer header_packet;
    if (!readPacket(header_packet, 5000)) {
        std::cerr << "[ERROR] Header packet timeout" << std::endl;
        return false;
    }

    // 打印接收到的起始帧数据
    std::cout << "[DEBUG] 收到起始帧，大小: " << header_packet.size() << " 字节 " <<std::endl;

    // 解析文件头
    if (header_packet.size() < 3 || header_packet[0] != SOH ||
        header_packet[1] != 0x00 || header_packet[2] != 0xFF) {
        std::cerr << "[ERROR] Invalid header packet" << std::endl;
        return false;
    }

    // 提取文件名和大小
    std::string file_metadata(reinterpret_cast<const char*>(&header_packet[3]), 128);
    size_t null_pos = file_metadata.find('\0');
    if (null_pos == std::string::npos) {
        std::cerr << "[ERROR] Invalid header packet format" << std::endl;
        return false;
    }
    std::string filename = file_metadata.substr(0, null_pos);
    std::string filesize_str = file_metadata.substr(null_pos + 1);
    size_t file_size = std::stoi(filesize_str);

    std::cout << "[INFO] 接收到文件名: " << filename << " 文件大小: " << file_size << " 字节" << std::endl;

    // 创建文件
    std::ofstream file(fileName, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "[ERROR] 无法创建文件: " << fileName << std::endl;
        return false;
    }

    // 3. 发送ACK确认文件头
    serial_port.Write(std::vector<uint8_t>{ACK});
    std::cout << "[INFO] 发送 ACK 确认文件头" << std::endl;

    // 4. 接收数据帧
    // TODO 读临时文件
    std::ifstream ifs_temp("temp.txt", std::ios::binary);
    if(!ifs_temp){
        std::cerr << "[ERROR] 文件打开失败: " << "temp.txt" << std::endl;
        return false;
    }
    std::string line;
    while(std::getline(ifs_temp, line)){
        std::istringstream iss(line);
        std::string temp_name;
        std::string temp_size;

        if(!(iss >> temp_name >> temp_size)){
            std::cerr << "[ERROR] 无效数据: " << line << std::endl;
            continue;
        }

        if(temp_name == filename){
            file.seekp(stoi(temp_size));
            // 更改接收文件大小
            file_size -= stoi(temp_size);
            std::cout << "[INFO] 断点续传 文件: " << temp_name << "指针偏移: " << temp_size << std::endl;
            break;
        }
    }
    // 关闭temp.txt
    ifs_temp.close();


    uint8_t expected_packet_no = 0x01;
    size_t received_bytes = 0;

    while (received_bytes < file_size) {
        LibSerial::DataBuffer data_packet;
        if (!readPacket(data_packet, 5000)) {
            std::cerr << "[ERROR] Data packet timeout" << std::endl;
            return false;
        }

        std::cout << "[DEBUG] 收到数据帧，大小: " << data_packet.size() << " 预期包号: " << (int)expected_packet_no << std::endl;

        // 检查EOT（传输结束信号）
        if (data_packet[0] == EOT) {
            serial_port.Write(std::vector<uint8_t>{ACK});
            std::cout << "[INFO] 收到 EOT 结束传输，发送 ACK 确认" << std::endl;
            break;
        }

        // 校验包号和补码
        uint8_t packet_no = data_packet[1];
        uint8_t complement = data_packet[2];
        if (packet_no != expected_packet_no || (packet_no + complement) != 0xFF) {
            std::cerr << "[ERROR] 包号错误: " << (int)packet_no << " (应为 " << (int)expected_packet_no << ")" << std::endl;
            serial_port.Write(std::vector<uint8_t>{NAK});
            continue;
        }

        // 计算数据帧长度（128 或 1024）
        size_t data_check = (data_packet[0] == STX) ? 1024 : 128;
        size_t data_length = std::min(data_check, file_size - received_bytes);

        // 检查数据包大小是否正确
        if (data_packet.size() != (data_check + 3 + 2)) {
            std::cerr << "[ERROR] 数据包大小不匹配: 期望 " << (data_check + 3 + 2)
                      << " 字节, 实际收到 " << data_packet.size() << " 字节" << std::endl;
            serial_port.Write(std::vector<uint8_t>{NAK});
            continue;
        }

        // 校验CRC
        uint16_t received_crc = (data_packet[data_packet.size() - 2] << 8) |
                                data_packet[data_packet.size() - 1];
        LibSerial::DataBuffer crc_data(data_packet.begin(), data_packet.end() - 2);
        uint16_t calculated_crc = calculateCRC(crc_data);

        if (received_crc != calculated_crc) {
            std::cerr << "[ERROR] CRC 校验失败! 计算: " << calculated_crc
                      << " 但收到: " << received_crc << std::endl;
            serial_port.Write(std::vector<uint8_t>{NAK});
            continue;
        }

        // 写入数据
        file.write(reinterpret_cast<const char*>(crc_data.data() + 3), data_length);
        received_bytes += data_length;

        std::cout << "[INFO] 已接收 " << received_bytes << " / " << file_size << " 字节" << std::endl;

        // 发送ACK确认数据包
        serial_port.Write(std::vector<uint8_t>{ACK});
        std::cout << "[INFO] 发送 ACK 确认包号 " << (int)packet_no << std::endl;

        expected_packet_no++;
    }

    serial_port.Write(std::vector<uint8_t>{ACK});
    std::cout << "[INFO] 发送 ACK 结束确认包号 " << std::endl;

    file.close();

    // TODO 清空temp.txt
    std::ofstream ofs_temp("temp.txt", std::ios::trunc);

    std::cout << "[SUCCESS] 文件接收完成!" << std::endl;
    return true;
}


bool Ymodem::readPacket(LibSerial::DataBuffer& packet, size_t timeout_ms) {
    packet.clear();

    // Step 1: 读取包头（3字节）
    LibSerial::DataBuffer header;
    serial_port.Read(header, 3, timeout_ms);

    // 检查EOT
    if (header[0] == EOT) {
        LibSerial::DataBuffer ack_packet(1, ACK);
        serial_port.Write(ack_packet);
        return false;
    }

    // 确定数据包长度
    size_t data_size = (header[0] == STX) ? 1024 : 128;
    size_t total_size = data_size + 3 + 2;  // 数据 + 包头 + CRC

    // Step 2: 读取剩余数据
    LibSerial::DataBuffer remaining;
    serial_port.Read(remaining, total_size - 3, timeout_ms);

    // 合并包头和数据
    packet.insert(packet.end(), header.begin(), header.end());
    packet.insert(packet.end(), remaining.begin(), remaining.end());
    return true;
}
