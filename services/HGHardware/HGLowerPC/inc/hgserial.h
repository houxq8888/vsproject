#ifndef HGSERIAL_H
#define HGSERIAL_H

#include <map>
#include <vector>
#include "CSerialPort/SerialPort.h"  // 原始头文件路径
#include "CSerialPort/SerialPortInfo.h"
#include "CSerialPort/SerialPortListener.h"
#include <mutex>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
#include <termios.h>
#endif


#include "HGLowerPC_global.h"


namespace HGMACHINE {

typedef struct tagHGSERAIL_STANDARD_INFO{
    std::vector<uint8_t> head;
    std::vector<uint8_t> tail;
    std::map<std::string, std::vector<uint8_t>> function;
    std::vector<uint8_t> separator;
    std::vector<uint8_t> data;
    tagHGSERAIL_STANDARD_INFO()
    {
        head.clear();
        tail.clear();
        data.clear();
        function.clear();
        separator.clear();
    }
    void Clear(){
        head.clear();
        tail.clear();
        data.clear();
        function.clear();
        separator.clear();
    }
    tagHGSERAIL_STANDARD_INFO& operator=(const tagHGSERAIL_STANDARD_INFO& obj)
    {
        head = obj.head;
        tail = obj.tail;
        data = obj.data;
        function = obj.function;
        separator=obj.separator;
        return *this;
    }
    bool operator==(const tagHGSERAIL_STANDARD_INFO& obj) const
    {
        bool ret = false;
        if (head == obj.head &&
                tail == obj.tail &&
                data == obj.data &&
                function == obj.function &&
                separator == obj.separator)
        {
            ret = true;
        }
        return ret;
    }
    bool operator!=(const tagHGSERAIL_STANDARD_INFO& obj) const
    {
        return (!(*this==obj));
    }
} HGSERIAL_STANDARD_INFO, *PHGSERIAL_STANDARD_INFO;

std::vector<std::string> listAvailableSerialPorts();

    class HGSerial : public itas109::CSerialPortListener {
    public:
        HGSerial();
        ~HGSerial();
    
        int open();
        int close();
        bool isOpened(){
            if (!sp.isOpen()) return false;
            return true;
        }
    
        int getStartPos(const std::vector<uint8_t> &contents);
        int getEndPos(const std::vector<uint8_t> &contents);
        std::vector<size_t> findPattern(const std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern);

        int write(const std::vector<uint8_t>& hex);
        std::vector<uint8_t> read();
        HGSERIAL_STANDARD_INFO getStandardInfo() {return m_standard_info;};
        void setStandardInfo(const HGSERIAL_STANDARD_INFO &info);
        void setParam(const std::string& portName,unsigned int speed,int bits,char event,int stop);
    protected:
        void onReadEvent(const char* portName,unsigned int readBufferLen);
    private:
        std::vector<std::vector<uint8_t>> m_readHex;
        HGSERIAL_STANDARD_INFO m_standard_info;
        itas109::CSerialPort sp;
        bool m_smaller;
        bool m_isOpened;
        bool m_valid;
        std::string m_portName;
       // std::mutex m_mutex;
    };
}

#endif // HGSERIAL_H
