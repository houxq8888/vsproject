#include "SerialAdapter.h"
#include "CoreSerial.h"

namespace HGMACHINE {

class SerialAdapter::Impl {
public:
    CoreSerial coreSerial;
    std::string portName;
    unsigned int baudRate;
    int dataBits;
    int stopBits;
    char parity;
    std::string lastError;
    SerialDataCallback dataCallback;
    
    Impl() : portName(""), baudRate(9600), dataBits(8), stopBits(1), parity('N') {}
};

SerialAdapter::SerialAdapter() : m_impl(new Impl()) {
}

SerialAdapter::~SerialAdapter() {
    delete m_impl;
}

bool SerialAdapter::initialize() {
    return m_impl->coreSerial.initialize();
}

void SerialAdapter::shutdown() {
    m_impl->coreSerial.shutdown();
}

int SerialAdapter::open() {
    m_impl->coreSerial.setPortName(m_impl->portName);
    m_impl->coreSerial.setBaudRate(m_impl->baudRate);
    m_impl->coreSerial.setDataBits(m_impl->dataBits);
    m_impl->coreSerial.setStopBits(m_impl->stopBits);
    m_impl->coreSerial.setParity(m_impl->parity);
    return m_impl->coreSerial.open();
}

int SerialAdapter::close() {
    return m_impl->coreSerial.close();
}

bool SerialAdapter::isOpened() const {
    return m_impl->coreSerial.isOpened();
}

int SerialAdapter::write(const std::vector<uint8_t>& data) {
    return m_impl->coreSerial.write(data);
}

std::vector<uint8_t> SerialAdapter::read() {
    return m_impl->coreSerial.read();
}

void SerialAdapter::setPortName(const std::string& portName) {
    m_impl->portName = portName;
}

std::string SerialAdapter::getPortName() const {
    return m_impl->portName;
}

void SerialAdapter::setBaudRate(unsigned int baudRate) {
    m_impl->baudRate = baudRate;
}

unsigned int SerialAdapter::getBaudRate() const {
    return m_impl->baudRate;
}

void SerialAdapter::setDataBits(int dataBits) {
    m_impl->dataBits = dataBits;
}

int SerialAdapter::getDataBits() const {
    return m_impl->dataBits;
}

void SerialAdapter::setStopBits(int stopBits) {
    m_impl->stopBits = stopBits;
}

int SerialAdapter::getStopBits() const {
    return m_impl->stopBits;
}

void SerialAdapter::setParity(char parity) {
    m_impl->parity = parity;
}

char SerialAdapter::getParity() const {
    return m_impl->parity;
}

void SerialAdapter::setHead(const std::vector<uint8_t>& head) {
    m_impl->coreSerial.setHead(head);
}

std::vector<uint8_t> SerialAdapter::getHead() const {
    return m_impl->coreSerial.getHead();
}

void SerialAdapter::setTail(const std::vector<uint8_t>& tail) {
    m_impl->coreSerial.setTail(tail);
}

std::vector<uint8_t> SerialAdapter::getTail() const {
    return m_impl->coreSerial.getTail();
}

void SerialAdapter::setFunction(const std::string& key, const std::vector<uint8_t>& value) {
    m_impl->coreSerial.setFunction(key, value);
}

std::vector<uint8_t> SerialAdapter::getFunction(const std::string& key) const {
    return m_impl->coreSerial.getFunction(key);
}

std::map<std::string, std::vector<uint8_t>> SerialAdapter::getAllFunctions() const {
    return m_impl->coreSerial.getAllFunctions();
}

void SerialAdapter::setSeparator(const std::vector<uint8_t>& separator) {
    m_impl->coreSerial.setSeparator(separator);
}

std::vector<uint8_t> SerialAdapter::getSeparator() const {
    return m_impl->coreSerial.getSeparator();
}

void SerialAdapter::setData(const std::vector<uint8_t>& data) {
    m_impl->coreSerial.setData(data);
}

std::vector<uint8_t> SerialAdapter::getData() const {
    return m_impl->coreSerial.getData();
}

void SerialAdapter::setDataCallback(SerialDataCallback callback) {
    m_impl->dataCallback = callback;
    m_impl->coreSerial.setDataCallback(callback);
}

std::vector<size_t> SerialAdapter::findPattern(const std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern) {
    return m_impl->coreSerial.findPattern(data, pattern);
}

std::string SerialAdapter::getLastError() const {
    return m_impl->lastError;
}

void SerialAdapter::clearError() {
    m_impl->lastError.clear();
}

std::vector<std::string> SerialAdapter::listAvailableSerialPorts() {
    return m_impl->coreSerial.listAvailableSerialPorts();
}

} // namespace HGMACHINE
