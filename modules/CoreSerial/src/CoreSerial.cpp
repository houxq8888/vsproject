#include "CoreSerial.h"
#include "hgserial.h"

namespace HGMACHINE {

class CoreSerial::Impl : public itas109::CSerialPortListener {
public:
    HGSerial hgSerial;
    std::string portName;
    unsigned int baudRate;
    int dataBits;
    int stopBits;
    char parity;
    std::string lastError;
    SerialDataCallback dataCallback;
    
    Impl() : portName(""), baudRate(9600), dataBits(8), stopBits(1), parity('N') {}
    
    static HGSERIAL_STANDARD_INFO toHGSerialInfo(const SerialStandardInfo& info) {
        HGSERIAL_STANDARD_INFO hgInfo;
        hgInfo.head = info.head;
        hgInfo.tail = info.tail;
        hgInfo.function = info.function;
        hgInfo.separator = info.separator;
        hgInfo.data = info.data;
        return hgInfo;
    }
    
    static SerialStandardInfo fromHGSerialInfo(const HGSERIAL_STANDARD_INFO& hgInfo) {
        SerialStandardInfo info;
        info.head = hgInfo.head;
        info.tail = hgInfo.tail;
        info.function = hgInfo.function;
        info.separator = hgInfo.separator;
        info.data = hgInfo.data;
        return info;
    }
    
protected:
    void onReadEvent(const char* portName, unsigned int readBufferLen) override {
        if (dataCallback) {
            std::vector<uint8_t> data = hgSerial.read();
            if (!data.empty()) {
                dataCallback(data);
            }
        }
    }
    
    friend class CoreSerial;
};

CoreSerial::CoreSerial() : m_impl(new Impl()) {
}

CoreSerial::~CoreSerial() {
    delete m_impl;
}

bool CoreSerial::initialize() {
    return true;
}

void CoreSerial::shutdown() {
    m_impl->hgSerial.close();
}

int CoreSerial::open() {
    m_impl->hgSerial.setParam(m_impl->portName, m_impl->baudRate, m_impl->dataBits, m_impl->parity, m_impl->stopBits);
    return m_impl->hgSerial.open();
}

int CoreSerial::close() {
    return m_impl->hgSerial.close();
}

bool CoreSerial::isOpened() const {
    return m_impl->hgSerial.isOpened();
}

int CoreSerial::write(const std::vector<uint8_t>& data) {
    return m_impl->hgSerial.write(data);
}

std::vector<uint8_t> CoreSerial::read() {
    return m_impl->hgSerial.read();
}

void CoreSerial::setPortName(const std::string& portName) {
    m_impl->portName = portName;
}

std::string CoreSerial::getPortName() const {
    return m_impl->portName;
}

void CoreSerial::setBaudRate(unsigned int baudRate) {
    m_impl->baudRate = baudRate;
}

unsigned int CoreSerial::getBaudRate() const {
    return m_impl->baudRate;
}

void CoreSerial::setDataBits(int dataBits) {
    m_impl->dataBits = dataBits;
}

int CoreSerial::getDataBits() const {
    return m_impl->dataBits;
}

void CoreSerial::setStopBits(int stopBits) {
    m_impl->stopBits = stopBits;
}

int CoreSerial::getStopBits() const {
    return m_impl->stopBits;
}

void CoreSerial::setParity(char parity) {
    m_impl->parity = parity;
}

char CoreSerial::getParity() const {
    return m_impl->parity;
}

void CoreSerial::setStandardInfo(const SerialStandardInfo& info) {
    HGSERIAL_STANDARD_INFO hgInfo = Impl::toHGSerialInfo(info);
    m_impl->hgSerial.setStandardInfo(hgInfo);
}

SerialStandardInfo CoreSerial::getStandardInfo() const {
    HGSERIAL_STANDARD_INFO hgInfo = m_impl->hgSerial.getStandardInfo();
    return Impl::fromHGSerialInfo(hgInfo);
}

void CoreSerial::setHead(const std::vector<uint8_t>& head) {
    SerialStandardInfo info = getStandardInfo();
    info.head = head;
    setStandardInfo(info);
}

std::vector<uint8_t> CoreSerial::getHead() const {
    return getStandardInfo().head;
}

void CoreSerial::setTail(const std::vector<uint8_t>& tail) {
    SerialStandardInfo info = getStandardInfo();
    info.tail = tail;
    setStandardInfo(info);
}

std::vector<uint8_t> CoreSerial::getTail() const {
    return getStandardInfo().tail;
}

void CoreSerial::setFunction(const std::string& key, const std::vector<uint8_t>& value) {
    SerialStandardInfo info = getStandardInfo();
    info.function[key] = value;
    setStandardInfo(info);
}

std::vector<uint8_t> CoreSerial::getFunction(const std::string& key) const {
    const SerialStandardInfo& info = getStandardInfo();
    auto it = info.function.find(key);
    if (it != info.function.end()) {
        return it->second;
    }
    return std::vector<uint8_t>();
}

std::map<std::string, std::vector<uint8_t>> CoreSerial::getAllFunctions() const {
    return getStandardInfo().function;
}

void CoreSerial::setSeparator(const std::vector<uint8_t>& separator) {
    SerialStandardInfo info = getStandardInfo();
    info.separator = separator;
    setStandardInfo(info);
}

std::vector<uint8_t> CoreSerial::getSeparator() const {
    return getStandardInfo().separator;
}

void CoreSerial::setData(const std::vector<uint8_t>& data) {
    SerialStandardInfo info = getStandardInfo();
    info.data = data;
    setStandardInfo(info);
}

std::vector<uint8_t> CoreSerial::getData() const {
    return getStandardInfo().data;
}

void CoreSerial::setDataCallback(SerialDataCallback callback) {
    m_impl->dataCallback = callback;
}

std::vector<size_t> CoreSerial::findPattern(const std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern) {
    return m_impl->hgSerial.findPattern(data, pattern);
}

std::string CoreSerial::getLastError() const {
    return m_impl->lastError;
}

void CoreSerial::clearError() {
    m_impl->lastError.clear();
}

std::vector<std::string> CoreSerial::listAvailableSerialPorts() {
    return HGMACHINE::listAvailableSerialPorts();
}

} // namespace HGMACHINE
