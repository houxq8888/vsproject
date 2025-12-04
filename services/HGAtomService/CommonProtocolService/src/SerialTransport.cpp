// SerialTransport.cpp
#include "SerialTransport.h"
#include <iostream>


SerialTransport::SerialTransport() {
    serial_port_.setReadIntervalTimeout(10); // 10ms
    serial_port_.connectReadEvent(this);
}

SerialTransport::~SerialTransport() {
    close();
}

bool SerialTransport::open(const std::string& port, uint32_t baudrate,
                          itas109::Parity parity,
                          itas109::DataBits dataBits,
                          itas109::StopBits stopBits,
                          itas109::FlowControl flowControl,
                          uint32_t readBufferSize) {
    if (isOpen()) {
        close();
    }
    
    // 初始化串口参数
    serial_port_.init(port.c_str(), baudrate, parity, dataBits, stopBits, flowControl, readBufferSize);
    
    // 尝试打开串口
    if (!serial_port_.open()) {
        std::cerr << "Failed to open serial port: " << port << std::endl;
        return false;
    }
    
    return true;
}

void SerialTransport::close() {
    if (isOpen()) {
        serial_port_.disconnectReadEvent();
        serial_port_.close();
    }
}

bool SerialTransport::isOpen() {
    bool flag=serial_port_.isOpen();
    return flag;
}

bool SerialTransport::writeData(const std::vector<uint8_t>& data) {
    if (!isOpen() || data.empty()) return false;
    
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t bytesWritten = serial_port_.writeData(data.data(), static_cast<uint32_t>(data.size()));
    return bytesWritten == data.size();
}

void SerialTransport::setDataReceivedCallback(DataReceivedCallback callback) {
    callback_ = std::move(callback);
}

void SerialTransport::onDataReceived(void* context, uint8_t* data, uint32_t size) {
    std::cout << "驱动层收到数据: " << size << "字节\n"; // 调试输出
    auto* self = static_cast<SerialTransport*>(context);
    if (self && self->callback_) {
        std::lock_guard<std::mutex> lock(self->mutex_);
        self->rx_buffer_.insert(self->rx_buffer_.end(), data, data + size);
        self->callback_(self->rx_buffer_);
        self->rx_buffer_.clear();
    }
}

