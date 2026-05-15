#ifndef SERIALADAPTER_H
#define SERIALADAPTER_H

#include "ISerial.h"
#include <memory>

namespace HGMACHINE {

class SerialAdapter : public ISerial {
public:
    SerialAdapter();
    ~SerialAdapter() override;
    
    bool initialize() override;
    void shutdown() override;
    
    int open() override;
    int close() override;
    bool isOpened() const override;
    
    int write(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> read() override;
    
    void setPortName(const std::string& portName) override;
    std::string getPortName() const override;
    
    void setBaudRate(unsigned int baudRate) override;
    unsigned int getBaudRate() const override;
    
    void setDataBits(int dataBits) override;
    int getDataBits() const override;
    
    void setStopBits(int stopBits) override;
    int getStopBits() const override;
    
    void setParity(char parity) override;
    char getParity() const override;
    
    void setHead(const std::vector<uint8_t>& head) override;
    std::vector<uint8_t> getHead() const override;
    
    void setTail(const std::vector<uint8_t>& tail) override;
    std::vector<uint8_t> getTail() const override;
    
    void setFunction(const std::string& key, const std::vector<uint8_t>& value) override;
    std::vector<uint8_t> getFunction(const std::string& key) const override;
    std::map<std::string, std::vector<uint8_t>> getAllFunctions() const override;
    
    void setSeparator(const std::vector<uint8_t>& separator) override;
    std::vector<uint8_t> getSeparator() const override;
    
    void setData(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> getData() const override;
    
    void setDataCallback(SerialDataCallback callback) override;
    
    std::vector<size_t> findPattern(const std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern) override;
    
    std::string getLastError() const override;
    void clearError() override;
    
    std::vector<std::string> listAvailableSerialPorts() override;
    
private:
    class Impl;
    Impl* m_impl;
};

} // namespace HGMACHINE

#endif
