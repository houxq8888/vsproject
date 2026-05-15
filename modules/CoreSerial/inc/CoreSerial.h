#ifndef CORESERIAL_H
#define CORESERIAL_H

#include "CoreSerial_global.h"
#include <vector>
#include <string>
#include <map>
#include <functional>

namespace HGMACHINE {

struct CORESERIAL_EXPORT SerialStandardInfo {
    std::vector<uint8_t> head;
    std::vector<uint8_t> tail;
    std::map<std::string, std::vector<uint8_t>> function;
    std::vector<uint8_t> separator;
    std::vector<uint8_t> data;
};

using SerialDataCallback = std::function<void(const std::vector<uint8_t>&)>;

class CORESERIAL_EXPORT CoreSerial {
public:
    CoreSerial();
    ~CoreSerial();
    
    bool initialize();
    void shutdown();
    
    int open();
    int close();
    bool isOpened() const;
    
    int write(const std::vector<uint8_t>& data);
    std::vector<uint8_t> read();
    
    void setPortName(const std::string& portName);
    std::string getPortName() const;
    
    void setBaudRate(unsigned int baudRate);
    unsigned int getBaudRate() const;
    
    void setDataBits(int dataBits);
    int getDataBits() const;
    
    void setStopBits(int stopBits);
    int getStopBits() const;
    
    void setParity(char parity);
    char getParity() const;
    
    void setStandardInfo(const SerialStandardInfo& info);
    SerialStandardInfo getStandardInfo() const;
    
    void setHead(const std::vector<uint8_t>& head);
    std::vector<uint8_t> getHead() const;
    
    void setTail(const std::vector<uint8_t>& tail);
    std::vector<uint8_t> getTail() const;
    
    void setFunction(const std::string& key, const std::vector<uint8_t>& value);
    std::vector<uint8_t> getFunction(const std::string& key) const;
    std::map<std::string, std::vector<uint8_t>> getAllFunctions() const;
    
    void setSeparator(const std::vector<uint8_t>& separator);
    std::vector<uint8_t> getSeparator() const;
    
    void setData(const std::vector<uint8_t>& data);
    std::vector<uint8_t> getData() const;
    
    void setDataCallback(SerialDataCallback callback);
    
    std::vector<size_t> findPattern(const std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern);
    
    std::string getLastError() const;
    void clearError();
    
    std::vector<std::string> listAvailableSerialPorts();
    
private:
    class Impl;
    Impl* m_impl;
};

} // namespace HGMACHINE

#endif // CORESERIAL_H
