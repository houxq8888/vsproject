#ifndef SERIALTRANSPORT_H
#define SERIALTRANSPORT_H

#include <vector>
#include <functional>
#include <mutex>
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"


class SerialTransport : public itas109::CSerialPortListener{
public:
    using DataReceivedCallback = std::function<void(const std::vector<uint8_t>&)>;
    
    SerialTransport();
    ~SerialTransport();
    
    void onReadEvent(const char* portName, unsigned int readBufferLen) override {
        if (readBufferLen > 0) {
            std::vector<uint8_t> buffer(readBufferLen);
            int recLen = serial_port_.readData(reinterpret_cast<char*>(buffer.data()), readBufferLen);
            
            if (recLen > 0) {
                buffer.resize(recLen);
                std::lock_guard<std::mutex> lock(mutex_);
                if (callback_) {
                    callback_(buffer);
                }
            }
        }
    };
    bool open(const std::string& port, uint32_t baudrate,
              itas109::Parity parity = itas109::ParityNone,
              itas109::DataBits dataBits = itas109::DataBits8,
              itas109::StopBits stopBits = itas109::StopBits::StopOne,
              itas109::FlowControl flowControl = itas109::FlowControl::FlowNone,
              uint32_t readBufferSize = 4096);
              
    void close();
    bool isOpen();
    
    bool writeData(const std::vector<uint8_t>& data);
    void setDataReceivedCallback(DataReceivedCallback callback);
    
private:
    void onDataReceived(void* context, uint8_t* data, uint32_t size);
    
private:
    itas109::CSerialPort serial_port_;
    DataReceivedCallback callback_;
    std::mutex mutex_;
    std::vector<uint8_t> rx_buffer_;
};

#endif