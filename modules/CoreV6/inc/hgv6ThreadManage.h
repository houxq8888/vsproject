#ifndef HGV6THREADMANAGE_H
#define HGV6THREADMANAGE_H

// #include "hgserial.h"
// #include <thread>

// namespace HGMACHINE{
//     typedef struct typsplitDataInfo{
//         size_t start;
//         size_t end;
//     } splitDataInfo;
// enum{
//     LINK_UNKNOWN=-1,
//     LINK_ONLINE=1,
//     LINK_OFFLINE=2
// };
// enum {
//     FLAGSTART=1,
//     FLAGINTERRUPT=2,
//     FLAGEND=3,
//     FLAGRUNTASK=4,
//     FLAGPAUSETASK=5
// };
//     class Hgv6ThreadManage
//     {
//     public:
//         Hgv6ThreadManage();
//         ~Hgv6ThreadManage();
//         void startThread();
//         void stopThread();
//         void runRecv();
//         void runSend();
//     private:
//         void readBuffers();
//         int commOpen();
//         void writeBuffer(const std::string& data);
//         std::vector<splitDataInfo> splitRawDataCAS6(const std::vector<uint8_t> &contents);


//     private:
//         std::thread threadRecvComm, threadSendComm;

//         int m_exitTrd;
//         std::string m_comPort;
//         int m_recvComThreadFlag, m_sendComThreadFlag;
//         HGSerial m_serial;
//     };
// }

#endif // HGV6THREADMANAGE_H