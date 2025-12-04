#ifndef YMODEM
#define YMODEM

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <libserial/SerialPort.h>

// Ymodem 协议常量
#define SOH  0x01  // 128字节包
#define STX  0x02  // 1024字节包
#define EOT  0x04  // 结束传输
#define ACK  0x06  // 确认
#define NAK  0x15  // 否定确认
#define CAN  0x18  // 取消传输
#define CRC16 0x43 // CRC 校验符号

#define PACKET_1024_SIZE 1024
#define PACKET_128_SIZE 128

class Ymodem {
public:
    Ymodem(const std::string& port, LibSerial::BaudRate baud_rate=LibSerial::BaudRate::BAUD_115200);
    ~Ymodem();

    bool sendFile(const std::string& filename);
    bool receiveFile(const std::string& fileName);

private:
    LibSerial::SerialPort serial_port;

    bool waitForChar(unsigned char expected, int timeout_ms = 5000);
    uint16_t calculateCRC(const std::vector<uint8_t>& data);
    bool readPacket(LibSerial::DataBuffer& packet, size_t timeout_ms);
};

#endif //YMODEM
