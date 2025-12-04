#include "hgsecurity.h"
#include <string>
#include <string.h>
#include <stdio.h>
#include "openssl/aes.h"
#include <unistd.h>
#include <sstream>
#include <fstream>
#include "hglog4cplus.h"
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#pragma comment(lib, "iphlpapi.lib")
#define SAFE_DEL_ARRAY(ptr) if (ptr){delete[] ptr; ptr=NULL;}
#else
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#endif
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <fstream>
#include <array>
#include <memory>
#include <cstdio>

namespace HGMACHINE {

int HGAesEncrypt(char *in,char* key,char *out){
    if (!in || !key || !out) return 0;
    AES_KEY aes;
    if (AES_set_encrypt_key((unsigned char*)key,128,&aes)<0)
        return 0;
    int len = strlen(in),en_len=0;
    while (en_len <len){
        AES_encrypt((unsigned char*)in,(unsigned char*)out,&aes);
        in+=AES_BLOCK_SIZE;
        out+=AES_BLOCK_SIZE;
        en_len+=AES_BLOCK_SIZE;
    }
    return 1;
}
int HGAesDecrypt(char* in,char* key,char *out){
    if (!in || !key || !out) return 0;
    unsigned char iv[AES_BLOCK_SIZE];
    for (int i =0;i<AES_BLOCK_SIZE;++i)
        iv[i]=0;
    AES_KEY aes;
    if (AES_set_decrypt_key((unsigned char*)key,128,&aes)<0)
        return 0;
    int len = strlen(in);
    AES_cbc_encrypt((unsigned char*)in,(unsigned char*)out,len,&aes,iv,AES_DECRYPT);
    return 1;
}
void bin2str(unsigned char* in, char* out, int size,int base)
{
    unsigned char* pt1 = in;
    char* pt2 = out;
    do {
        pt2 += sprintf(pt2,"%02X", *pt1++);
        size --;
    } while (*pt1 && size);
}
#define MSG_LEN 128
bool HGEnDecrypt(bool isEncrypt, const void *inData,
                 void *outData, std::string key)
{
    bool ret =true;
    char encryption[MSG_LEN];
    memset((char*)encryption, 0, MSG_LEN);

    char decStringTemp[MSG_LEN];
    char strdstString[MSG_LEN];
    char strdstStringDec[MSG_LEN];

    memset((char*)decStringTemp,0,MSG_LEN);
    memset((char*)strdstString,0,MSG_LEN);
    memset((char*)strdstStringDec,0,MSG_LEN);
    char keyC[AES_BLOCK_SIZE];
    memset((char*)keyC, 0, AES_BLOCK_SIZE);
    strcpy((char*)keyC, key.c_str());// 可自由设置密钥

    if (isEncrypt) { // 加密
        if (!HGAesEncrypt((char*)inData, keyC, encryption))
        {
            return false;
        }

        bin2str((unsigned char*)encryption, strdstString, strlen((char*)encryption),16);
        strcpy((char*)outData, strdstString);
        ret = true;
    }
    else { // 解密
        if (!HGAesDecrypt(strdstStringDec,keyC,decStringTemp))
        {
            return false;
        }
        ret = true;
    }
    return ret;
}
bool HGEnDecrypt(bool isEncrypt, const std::string &srcString, std::string &dstString,std::string key)
{
    if (isEncrypt)
    {
        char* outData = new char[MSG_LEN];
        memset(outData, 0, MSG_LEN);
        char sourceStringTemp[MSG_LEN];
        memset((char*)sourceStringTemp, 0, MSG_LEN);
        strcpy((char*)sourceStringTemp, srcString.c_str());
        HGEnDecrypt(true, sourceStringTemp,outData, key);
        dstString = "";
        dstString = outData;
        if (outData) {delete []outData; outData=NULL;};
    }
    else
        return false;
    return true;
}
std::string GetCPUSerialNumber(){
    std::ifstream cpuinfo("/pro/cpuinfo");
    std::string line;
    std::string serial;
    if (cpuinfo.is_open()){
        while (std::getline(cpuinfo,line)){
            if (line.find("Serial") != std::string::npos){
                serial=line.substr(line.find(":")+2);
                break;
            }
        }
        cpuinfo.close();
    }
    return serial.empty() ? "No CPU Serial Number found" : serial;
}
std::string GetHardDiskSerialNumber(const std::string& device){
    std::string result;
    std::array<char,128> buffer;
    std::string command="sudo hdparm -i"+device+" | grep SerialNo";
    std::shared_ptr<FILE> pipe(popen(command.c_str(),"r"),pclose);
    if (!pipe){
        return "popen() failed!";
    }

    while (fgets(buffer.data(),buffer.size(),pipe.get()) !=nullptr){
        result+=buffer.data();
    }
    auto pos=result.find("SerialNo=");
    if (pos!=std::string::npos){
        result=result.substr(pos+9);
    }
    return result.empty()?"No Hard Disk Serial Number found" : result;
}
std::vector<std::string> GetMacs()
{
    std::vector<std::string> deviceMacs;
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    ULONG size = sizeof(IP_ADAPTER_INFO);
    PIP_ADAPTER_INFO adapterInfo = (PIP_ADAPTER_INFO)(new unsigned char[size]);
    if (GetAdaptersInfo(adapterInfo, &size) != ERROR_SUCCESS) {
        SAFE_DEL_ARRAY(adapterInfo);
        adapterInfo = (PIP_ADAPTER_INFO)(new unsigned char[size]);
    }
    if (GetAdaptersInfo(adapterInfo, &size) == NO_ERROR) {
        PIP_ADAPTER_INFO adapter=adapterInfo;
        while(adapter)
        {
            // 获得当前MAC
            std::string macAddress=("");
            for(unsigned int i=0;i<adapter->AddressLength;i++)
            {
                char temp[MAXCHAR];
                sprintf_s(temp,MAXCHAR,("%02X"),adapter->Address[i]);
                macAddress+=temp;
                if(i<adapter->AddressLength-1)	macAddress+=(":");
            }
            std::string encryptedString=("");
            deviceMacs.push_back(macAddress);
            adapter=adapter->Next;
        }
    } else {
        // logtext.str("");
        printf("get MAC failed!\n");
    }
    SAFE_DEL_ARRAY(adapterInfo);
#else
    // 初始化网络，获取当前网络设备的信息
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[2048];
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1)
    {
        // 获取MAC失败
        return deviceMacs;
    }
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_ifcu.ifcu_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1)
        return deviceMacs;
    struct ifreq* it = ifc.ifc_ifcu.ifcu_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));
    char szMac[64];
    int count = 0;
    for (; it != end; ++it) {
        strcpy(ifr.ifr_ifrn.ifrn_name, it->ifr_ifrn.ifrn_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (!(ifr.ifr_ifru.ifru_flags & IFF_LOOPBACK)) {
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    count++;
                    unsigned char *ptr;
                    ptr = (unsigned char*)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0];
                    snprintf(szMac, 64, "%02X:%02X:%02X:%02X:%02X:%02X", *ptr, *(ptr+1), *(ptr+2),*(ptr+3),*(ptr+4),*(ptr+5));

                    deviceMacs.push_back(szMac);
                }
            }
        }
    }
#endif
    return deviceMacs;
}

bool isRightAuthority(const std::string &in){
    return true;
    std::vector<std::string> strin=GetMacs();
    std::vector<std::string> strout;
    std::string cpuserial=GetCPUSerialNumber();
    printf("cpu serial number:%s\n",cpuserial.c_str());

    std::ostringstream logtext;
    logtext<<"cpu serial number:"<<cpuserial;
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#else
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
#endif
    for (int i=0;i<int(strin.size());i++){
        logtext.str("");
        logtext<<"MAC:"<<strin[i];
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
#endif
        printf("MAC:%s\n",strin[i].c_str());
        std::string ss;
        bool flag=HGEnDecrypt(true,strin[i],ss,"HGALT");
        if (!flag) {
            printf("failed to endecrypt\n");

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#else
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("failed to endecrypt",LOGERROR);
#endif
            return false;
        }
        printf("ss:%s\n",ss.c_str());
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss,LOGINFO);
#endif
        if (in == ss){
            return true;
        }
    }
    return false;
}

}

