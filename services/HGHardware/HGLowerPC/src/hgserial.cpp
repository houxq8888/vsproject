#include "hgserial.h"
#include <map>
#include <errno.h>
#include <fcntl.h>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#define COM_HEAD 0xEE
#define COM_TAIL1 0xFF
#define COM_TAIL2 0xFC
#define COM_TAIL3 0xFF
#define COM_TAIL4 0xFF

namespace HGMACHINE {

HGSerial::HGSerial()
    : m_smaller(false)
    , m_isOpened(false),
    m_valid(false),
    m_portName("")
{
    // 构造函数体可以为空
}
HGSerial::~HGSerial()
{
    close();
}
int HGSerial::open() { 
    if (!m_valid) return -1;
    printf("Port Name: %s\n", m_portName.c_str());
    sp.setReadIntervalTimeout(100); // read interval timeout 0ms
    sp.open();
    printf("Open %s %s\n", m_portName.c_str(), sp.isOpen() ? "Success" : "Failed");
    printf("Code: %d, Message: %s\n", sp.getLastError(), sp.getLastErrorMsg());
    if (sp.getLastError()!=itas109::ErrorOK) return -1;
    if (!sp.isOpen()) return -1;

    // connect for read
    sp.connectReadEvent(this);
    return 0;
}
int HGSerial::close() {
    if (sp.isOpen())
        sp.close();
    return 0;
}

int HGSerial::getStartPos(const std::vector<uint8_t> &contents){
    int headNum = int(m_standard_info.head.size());
    int posStart=-1;
    for (int i=0;i<int(contents.size())-headNum;i++){
        // contiually size of head is equal to head
        bool flag=true;
        for (int j=0;j<headNum;j++){
            if (contents[i+j]!=m_standard_info.head[j]) {
                flag=false;
                break;
            }
        }
        if (flag) {
            posStart=i;
            break;
        }
    }
    return posStart;
}
std::vector<size_t> HGSerial::findPattern(const std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern) {
    std::vector<size_t> positions;
    if (pattern.empty() || data.size() < pattern.size()){
        return positions; // Avoid underflow and handle edge cases
    }
    for (size_t i = 0; i <= data.size() - pattern.size(); ++i) {
        if (std::equal(pattern.begin(), pattern.end(), data.begin() + i)){
            positions.push_back(i);
        }
    }
    return positions;
}
int HGSerial::getEndPos(const std::vector<uint8_t> &contents){ 
    int posStart=getStartPos(contents);
    int headNum = int(m_standard_info.head.size());
    int tailNum = int(m_standard_info.tail.size());
    int posEnd=-1;
    for (int i=posStart+headNum;i<=int(contents.size())-tailNum;i++){
        bool flag=true;
        for (int j=0;j<tailNum;j++){       
            if (contents[i+j]!=m_standard_info.tail[j]){
                flag=false;
                break;
            }
        }
        if (flag){
            posEnd=i;
            break;
        }
    }
    return posEnd+tailNum-1;
}
std::vector<std::string> listAvailableSerialPorts() {
    std::vector<std::string> ports;
    std::vector<itas109::SerialPortInfo> m_availablePortsList = itas109::CSerialPortInfo::availablePortInfos();
    int availablePortCount = (int)m_availablePortsList.size();

    for (int i = 1; i <= availablePortCount; ++i)
    {
        itas109::SerialPortInfo serialPortInfo = m_availablePortsList[i - 1];
        printf("%d - %s %s %s\n", i, serialPortInfo.portName, serialPortInfo.description, serialPortInfo.hardwareId);
        ports.push_back(serialPortInfo.portName);
    }

    return ports;
}
int HGSerial::write(const std::vector<uint8_t>& hex){
    // std::lock_guard<std::mutex> lock(m_mutex);  // 自动加锁并自动释放
    if (!sp.isOpen()) return -1;
    if (hex.size() <= 0)
    {
        return -1; // 空字符串不进行写入操作
    }
    // write hex data
    sp.writeData(reinterpret_cast<const char*>(hex.data()),hex.size());
    return 0;
}
std::vector<uint8_t> HGSerial::read(){
 //std::lock_guard<std::mutex> lock(m_mutex);  // 自动加锁并自动释放
    // std::vector<uint8_t> hex;
    // read
    if (m_readHex.size() >0){
        std::vector<uint8_t> copyReadData;
        for (int i=0;i<int(m_readHex.size());i++){
            copyReadData.push_back(m_readHex[0][i]);
        }
        m_readHex.erase(m_readHex.begin());
        return copyReadData;
    }
    else return std::vector<uint8_t>();
}
void HGSerial::setStandardInfo(const HGSERIAL_STANDARD_INFO &info)
{
    m_standard_info = info;
}
void HGSerial::setParam(const std::string& portName,unsigned int speed,int bits,char event,int stop){
    std::vector<itas109::SerialPortInfo> m_availablePortsList = itas109::CSerialPortInfo::availablePortInfos();

    printf("AvailableFriendlyPorts:\n");

    int availablePortCount = (int)m_availablePortsList.size();
    for (int i = 1; i <= availablePortCount; ++i)
    {
        itas109::SerialPortInfo serialPortInfo = m_availablePortsList[i - 1];
        if (portName==std::string(serialPortInfo.portName)){
            m_valid=true;
            m_portName=portName;
        }
        // printf("%d - %s %s %s\n", i, serialPortInfo.portName, serialPortInfo.description, serialPortInfo.hardwareId);
    }

    if (m_availablePortsList.size() == 0)
    {
        printf("No valid port\n");
    }
    else
    {
        if (!m_valid) return ;
        itas109::Parity parity ;
        switch (event){
            case 'N':
            parity= itas109::Parity::ParityNone;
            break;
            case 'O':
            parity= itas109::Parity::ParityOdd;
            break;
            case 'E':
            parity= itas109::Parity::ParityEven;
            break;
            case '1':
            parity= itas109::Parity::ParityMark;
            break;
            case '0':
            parity= itas109::Parity::ParitySpace;
            break;
            default:
            parity= itas109::Parity::ParityNone;
        }
        itas109::DataBits dataBits;
        switch (bits){
            case 5:
            dataBits=itas109::DataBits::DataBits5;
            break;
            case 6:
            dataBits=itas109::DataBits::DataBits6;
            break;
            case 7:
            dataBits=itas109::DataBits::DataBits7;
            break;
            case 8:
            dataBits=itas109::DataBits::DataBits8;
            break;
            default:
            dataBits=itas109::DataBits::DataBits8;

        }
        itas109::StopBits stopbits;
        switch (stop){
            case 0:
            stopbits=itas109::StopBits::StopOne;
            break;
            case 1:
            stopbits=itas109::StopBits::StopOneAndHalf;
            break;
            case 2:
            stopbits=itas109::StopBits::StopTwo;
            break;
            default:
            stopbits=itas109::StopBits::StopOne;
        }
        itas109::FlowControl flowControl;
        flowControl=itas109::FlowControl::FlowNone;
        sp.init(portName.c_str(),              // windows:COM1 Linux:/dev/ttyS0
                speed, // baudrate
                parity,   // parity
                dataBits,    // data bit
                stopbits,      // stop bit
                flowControl,     // flow
                4096                   // read buffer size
        );
    }
    HGSERIAL_STANDARD_INFO hgserial_info;
    hgserial_info.head.push_back(COM_HEAD);
    hgserial_info.tail.push_back(COM_TAIL1);
    hgserial_info.tail.push_back(COM_TAIL2);
    hgserial_info.tail.push_back(COM_TAIL3);
    hgserial_info.tail.push_back(COM_TAIL4);
    setStandardInfo(hgserial_info);
}
void HGSerial::onReadEvent(const char* portName,unsigned int readBufferLen){
    if (readBufferLen>0){
        int recLen=0;
        char * str=NULL;
        str = new char[readBufferLen];
        recLen = sp.readData(str,readBufferLen);
        // read
        if (recLen>0){
            str[recLen]='\0';
            m_readHex.push_back(std::vector<uint8_t>(str,str+recLen));
        }
        if (str){
            delete [] str;
            str=NULL;
        }
    }
}
// int HGSerial::read(std::vector<uint8_t> &contents, int timeout)
// {
//     // 使用条件变量或定时器来避免忙等待
//     std::unique_lock<std::mutex> lock(m_mutex);
//     // if (!m_data_available_cv.wait_for(lock, std::chrono::milliseconds(timeout), [this] { return m_serial_port.IsDataAvailable(); })) {
//     //     std::ostringstream log;
//     //     log << HGLOWERPCNAME << "The Read() call timed out waiting for additional data.";
//     //     HGLog4Cplus::getLogInstance(LOG_PATH)->logout(log.str().c_str(), LOGWARN);
//     //     return -1;
//     // }

//     try {
//         #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
//         #else
//         m_serial_port.Read(contents, 0, static_cast<size_t>(timeout));
//         #endif
//     } catch (const ReadTimeout&) {
//         std::ostringstream log;
//         log << HGLOWERPCNAME << "The Read() call timed out waiting for additional data.";
//         // HGLog4Cplus::getLogInstance(LOG_PATH)->logout(log.str().c_str(), LOGWARN);
//         return -1;
//     } catch (const std::exception& e) {
//         std::ostringstream log;
//         log << HGLOWERPCNAME << "An error occurred during Read(): " << e.what();
//         HGLog4Cplus::getLogInstance(LOG_PATH)->logout(log.str().c_str(), LOGERROR);
//         return -1;
//     }

//     return 0;
// }
// #include <stdexcept>

// void HGSerial::write(const std::string &content)
// {
//     if (content.empty())
//     {
//         return; // 空字符串不进行写入操作
//     }

//     std::lock_guard<std::mutex> lock(m_mutex); // 确保线程安全
//     try
//     {
//         #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
//         #else
//         m_serial_port.Write(content);
//         #endif
//     }
//     catch (const std::exception &e)
//     {
//         // 处理异常，例如记录日志或抛出更具体的异常
//         // throw std::runtime_error("Failed to write to serial port: " + std::string(e.what()));
//         std::cerr<<"Failed to write to serial port: "<<e.what()<<std::endl;
//     }
// }


// std::map<std::string,std::map<std::string,std::string>> HGSerial::deSerialize(const std::vector<uint8_t> &contents)
// {
//     std::map<std::string,std::vector<uint8_t>> deCodeContents;

//     m_content.insert(m_content.end(),contents.begin(),contents.end());
//     std::stringstream ss;
//     ss<<std::hex;
//     std::string hex_str;
// //    printf("[Before]len:%d\n",int(m_content.size()));
// //    for (int i=0;i<int(m_content.size());i++){
// //        ss<<std::setw(2)<<std::setfill('0')<<(int)m_content[i]<<"  ";
// //    }
// //    hex_str=ss.str();
// //    printf("\n[EraseBeforehex]:%s\n",hex_str.c_str());


//     // judge head
//     int headNum = int(m_standard_info.head.size());
//     int tailNum = int(m_standard_info.tail.size());
//     int posStart=-1, posEnd=-1;
//     for (int i=0;i<int(m_content.size())-headNum;i++){
//         // contiually size of head is equal to head
//         bool flag=true;
//         for (int j=0;j<headNum;j++){
//             if (m_content[i+j]!=m_standard_info.head[j]) {
//                 flag=false;
//                 break;
//             }
//         }
//         if (flag) {
//             posStart=i;
//             break;
//         }
//     }
//     // judge tail
//     for (int i=posStart+headNum;i<int(m_content.size())-tailNum;i++){
//         bool flag=true;
//         for (int j=0;j<tailNum;j++){
//             if (m_content[i+j]!=m_standard_info.tail[j]){
//                 flag=false;
//                 break;
//             }
//         }
//         if (flag){
//             posEnd=i;
//             break;
//         }
//     }

// //    std::cout<<"["<<posStart<<","<<posEnd<<"]\n";
// //    ss.str("");
// //    std::vector<uint8_t> codeContent;
// //    for (int i=posStart;i<posEnd+tailNum;i++){
// //        ss<<std::setw(2)<<std::setfill('0')<<(int)m_content[i]<<"  ";
// //        codeContent.push_back(m_content[i]);
// //    }
// //    hex_str=ss.str();
// //    printf("\n[single]:%s\n",hex_str.c_str());

//     // function
//     // printf("[Function]len:%d\n",posEnd-posStart-headNum);
//     int functionCodeLen = posEnd-posStart-headNum;
//     std::map<std::string,std::vector<uint8_t>>::iterator iter;
//     std::string functionName="";
//     int posFuncStart=posStart+headNum; int posFuncEnd=-1;
    

//     for (iter=m_standard_info.function.begin();iter!=m_standard_info.function.end();++iter){
//         std::string functionStr=iter->first;
//         std::vector<uint8_t> functionContent=iter->second;
//         if (functionCodeLen < int(functionContent.size())){
//             continue;
//         }

//         bool getFuncFlag=true;
//         for (int k=0;k<int(functionContent.size());k++){
//             if (functionContent[k]!=m_content[posStart+headNum+k]){
//                 getFuncFlag=false;
//                 break;
//             }
//         }
//         if (getFuncFlag){
//             functionName=functionStr;
//             posFuncEnd=posFuncStart+iter->second.size();
//             for (int mm=posFuncEnd;mm<posEnd;mm++){
//                 deCodeContents[functionStr].push_back(m_content[mm]);
//             }
            
//             break;
//         }
//     }

//     // printf("[fun] (%d,%d)\n",posFuncStart,posFuncEnd);
    
//     std::map<std::string,std::map<std::string,std::string>> returnContents;
//     iter=deCodeContents.begin();
//     for (;iter!=deCodeContents.end();iter++){
//         // separator
//         std::vector<uint8_t> data=iter->second;
//         std::vector<int> indices;
//         int m=0;
//         for (int k=0;k<int(m_standard_info.separator.size());k++){
//             int startSearch=0;
//             bool flag=false;
//             for (; m < int(data.size()); m++)
//             {
//                 if (m_standard_info.separator[k] == data[m])
//                 {
//                     startSearch=m+1;
//                     flag=true;
//                     break;
//                 }
//             }
//             if (flag)
//             {
//                 if (k == int(m_standard_info.separator.size() - 1))
//                 {
//                     flag = true;
//                     indices.push_back(startSearch - m_standard_info.separator.size());
//                     if (m < int(data.size() - 1))
//                     {
//                         k = -1;
//                         m++;
//                     }
//                 }
//                 else
//                 {
//                     m++;
//                 }
//                 continue;
//             }
//             else break;
//         }
        
        
//         if (!indices.empty()){
//             int index=0;
//             for (;index<int(indices.size());index++){
//                 // printf("index:%d,",indices[index]);
//                 int start=-1, end=-1;
//                 if (index==0) start=0;
//                 else start=indices[index-1]+1;

//                 for (int mm=start;mm<indices[index];mm++)
//                     returnContents[iter->first][std::to_string(index+1)].push_back(deCodeContents[iter->first][mm]);
//             }
//             for (int mm=indices[indices.size()-1]+1;mm<int(deCodeContents[iter->first].size());mm++)
//                 returnContents[iter->first][std::to_string(index+1)].push_back(char(deCodeContents[iter->first][mm]));
//         } else printf("indices is empty\n");
//     }
//     // printf("end deSerialize\n");
    
//     // decode

//     // delete already decode
//     // printf("posStart:%d,posEnd:%d,tailNum:%d\n",posStart,posEnd,tailNum);
//     // printf("m_content size:%d\n",m_content.size());
//     if (posStart>=0&&posEnd>=0&&(posEnd>posStart)&&((posEnd+tailNum)<=int(m_content.size()-1))){
//         std::vector<uint8_t>::iterator it=m_content.begin();
//         m_content.erase(it+posStart,it+posEnd+tailNum);
//     }

//     return returnContents;
// }

}
