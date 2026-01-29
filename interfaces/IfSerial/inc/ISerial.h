#ifndef ISERIAL_H
#define ISERIAL_H

#include "IfSerial_global.h"
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace HGMACHINE {

using SerialDataCallback = std::function<void(const std::vector<uint8_t>&)>;

class IFSERIAL_EXPORT ISerial {
public:
    virtual ~ISerial() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    virtual int open() = 0;
    virtual int close() = 0;
    virtual bool isOpened() const = 0;
    
    virtual int write(const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> read() = 0;
    
    virtual void setPortName(const std::string& portName) = 0;
    virtual std::string getPortName() const = 0;
    
    virtual void setBaudRate(unsigned int baudRate) = 0;
    virtual unsigned int getBaudRate() const = 0;
    
    virtual void setDataBits(int dataBits) = 0;
    virtual int getDataBits() const = 0;
    
    virtual void setStopBits(int stopBits) = 0;
    virtual int getStopBits() const = 0;
    
    virtual void setParity(char parity) = 0;
    virtual char getParity() const = 0;
    
    virtual void setHead(const std::vector<uint8_t>& head) = 0;
    virtual std::vector<uint8_t> getHead() const = 0;
    
    virtual void setTail(const std::vector<uint8_t>& tail) = 0;
    virtual std::vector<uint8_t> getTail() const = 0;
    
    virtual void setFunction(const std::string& key, const std::vector<uint8_t>& value) = 0;
    virtual std::vector<uint8_t> getFunction(const std::string& key) const = 0;
    virtual std::map<std::string, std::vector<uint8_t>> getAllFunctions() const = 0;
    
    virtual void setSeparator(const std::vector<uint8_t>& separator) = 0;
    virtual std::vector<uint8_t> getSeparator() const = 0;
    
    virtual void setData(const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> getData() const = 0;
    
    virtual void setDataCallback(SerialDataCallback callback) = 0;
    
    virtual std::vector<size_t> findPattern(const std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern) = 0;
    
    virtual std::string getLastError() const = 0;
    virtual void clearError() = 0;
    
    virtual std::vector<std::string> listAvailableSerialPorts() = 0;
};

} // namespace HGMACHINE

#endif // ISERIAL_H
