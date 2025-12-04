#include "hgcomwithzmq.h"
#include <cstring>
#include <assert.h>
#include <sstream>
#include "hglog4cplus.h"

namespace HGMACHINE {
HGComWithZmq::HGComWithZmq()
    : m_publisher(NULL),
      m_subscriber(NULL),
      m_publisherCtxHdr(NULL),
      m_subscriberCtxHdr(NULL)
{

}
HGComWithZmq::~HGComWithZmq()
{
    stop();
}
void HGComWithZmq::stop()
{
    zmq_close(m_subscriber);
    zmq_close(m_publisher);
    zmq_ctx_destroy(m_publisherCtxHdr);
    zmq_ctx_destroy(m_subscriberCtxHdr);
}
int HGComWithZmq::basesend()
{
    return 0;
}
int HGComWithZmq::baserecv()
{
    return 0;
}
int HGComWithZmq::sendString(void* socket, char *content)
{
    // init zmq_msg_t,
    zmq_msg_t msg;
    int rc = zmq_msg_init_size(&msg, strlen(content));
    if (rc == -1){
        errCodeInfo();
        return rc;
    }
    memcpy(zmq_msg_data(&msg), content, strlen(content));

    // send
    rc = zmq_msg_send(&msg,socket,0);
    if (rc == -1){
        errCodeInfo();
        return rc;
    }
    zmq_msg_close(&msg);
    return rc;
}
std::string HGComWithZmq::errCodeInfo()
{
    std::ostringstream errInfo;
    int errCode = zmq_errno();
    errInfo << "Zmq error:"<<errCode<<",Err:"<<zmq_strerror(errCode);
    return errInfo.str();
}
char* HGComWithZmq::recvString(void* socket)
{
    zmq_msg_t msg;
    zmq_msg_init(&msg);
    int size = zmq_msg_recv(&msg,socket,0);
    if (size == -1){
        errCodeInfo();
        return NULL;
    }

    char* str = (char*)malloc(size+1);
    memcpy(str, zmq_msg_data(&msg),size);

    zmq_msg_close(&msg);
    str[size] = 0;
    return str;
}
int HGComWithZmq::sendStruct(const ComDataStruct &value)
{
    std::vector<char> buffer=value.serialize();
    int ret=zmq_send(m_publisher,buffer.data(),buffer.size(),0);
    if (ret<0){
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("send failed",LOGERROR);
        #endif
    }
    
    return ret;
}
ComDataStruct HGComWithZmq::recvStruct()
{
    ComDataStruct _comdata;
    zmq_msg_t msg;
    zmq_msg_init(&msg);
    int ret=zmq_msg_recv(&msg,m_subscriber,0);
    if (ret<0){
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("recv struct failed",LOGERROR);
        #endif
    } else {
        const char* data=static_cast<const char*>(zmq_msg_data(&msg));
        size_t data_size=zmq_msg_size(&msg);
        if (!_comdata.deserialize(data,data_size)){
            #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("deserialize failed",LOGERROR);
            #endif
        }
    }
    zmq_msg_close(&msg);
    return _comdata;
}

int HGComWithZmq::createPub(const char* pServerAddr)
{
    sint32 iZmqType = ZMQ_PUB;
    void** ppCtx = &m_publisherCtxHdr;
    void** ppSock = &m_publisher;
    sint32 iSndTimeout = 1000; // millsecond
    void   *pTmpCtx = *ppCtx, *pTmpSock = NULL;
    std::ostringstream ss;
    if (pTmpCtx == NULL)
    {
        // 创建context, zmq的socket 需要在context上进行创建
        if ((pTmpCtx = zmq_ctx_new()) == NULL)
        {
//            printf("ctx error. addr=%s\n", pServerAddr);
            ss.str("");
            ss<<HGCOMSERVICENAME<<"ctx error. addr="<<pServerAddr;
            #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
            #endif
            return -1;
        }
    }

    // 创建zmq socket, socket目前有6种属性，这里使用dealer方式
    if ((pTmpSock = zmq_socket(pTmpCtx, iZmqType)) == NULL)
    {
//        printf("socket error. addr=%s\n", pServerAddr);
        ss.str("");
        ss<<HGCOMSERVICENAME<<"socket error. addr="<<pServerAddr;
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
        #endif
        zmq_ctx_term(pTmpCtx);
        return -1;
    }

    // ZMQ_XPUB_VERBOSE
    // 设置发送超时
    if (zmq_setsockopt(pTmpSock, ZMQ_SNDTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0)
    {
//        printf("setsockopt ZMQ_SNDTIMED error. addr=%s\n", pServerAddr);
        ss.str("");
        ss<<HGCOMSERVICENAME<<"setsockopt ZMQ_SNDTIMED error. addr="<<pServerAddr;
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
        #endif
        zmq_close(pTmpSock);
        zmq_ctx_term(pTmpCtx);
        return -1;
    }

    // 设置发送超时
    iSndTimeout = 1;
    if (zmq_setsockopt(pTmpSock, ZMQ_IMMEDIATE, &iSndTimeout, sizeof(iSndTimeout)) < 0)
    {
//        printf("setsockopt ZMQ_IMMEDIATE error. addr=%s\n", pServerAddr);
        ss.str("");
        ss<<HGCOMSERVICENAME<<"setsockopt ZMQ_IMMEDIATE error. addr="<<pServerAddr;
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
        #endif
        zmq_close(pTmpSock);
        zmq_ctx_term(pTmpCtx);
        return -1;
    }

    // 将发布者绑定到一个tcp节点或一个ipc节点上，绑定地址 ipc:///cache/zmq/0
    int ret = zmq_bind(pTmpSock, pServerAddr);
    if (ret < 0)
    {
//        printf("%s\n",errCodeInfo().c_str());
//        printf("bind error[ret=%d]. addr=%s\n", ret,pServerAddr);
        ss.str("");
        ss<<HGCOMSERVICENAME<<"bind error[ret="<<ret<<". addr="<<pServerAddr<<","<<errCodeInfo();
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
        #endif
        zmq_close(pTmpSock);
        zmq_ctx_term(pTmpCtx);
        return -1;
    }

//    printf("Pub Create Success, bind at: %s\n", pServerAddr);
    ss.str("");
    ss<<HGCOMSERVICENAME<<"Pub Create Success, bind at: "<<pServerAddr;
    #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGINFO);
    #endif
    *ppCtx = pTmpCtx;
    *ppSock = pTmpSock;
    return 0;
}
int HGComWithZmq::createSub(const char* pServerAddr)
{
    sint32 iZmqType = ZMQ_SUB;
    void** ppCtx = &m_subscriberCtxHdr;
    void** ppSock = &m_subscriber;
    sint32 iSndTimeout = 1000; // millsecond
    void   *pTmpCtx = *ppCtx, *pTmpSock = NULL;
    std::ostringstream ss;
    if (pTmpCtx == NULL)
    {
        // 创建context, zmq的socket 需要在context上进行创建
        if ((pTmpCtx = zmq_ctx_new()) == NULL)
        {
//            printf("ctx error. addr=%s\n", pServerAddr);
            ss.str("");
            ss<<HGCOMSERVICENAME<<"ctx error. addr="<<pServerAddr;
            #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
            #endif
            return -1;
        }
    }

    // 创建zmq socket, socket目前有6种属性，这里使用dealer方式
    if ((pTmpSock = zmq_socket(pTmpCtx, iZmqType)) == NULL)
    {
//        printf("socket error. addr=%s\n", pServerAddr);
        ss.str("");
        ss<<HGCOMSERVICENAME<<"socket error. addr="<<pServerAddr;
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
        #endif
        zmq_ctx_term(pTmpCtx);
        return -1;
    }

    if (iZmqType == ZMQ_REQ)
    {
        // 设置发送超时
        if (zmq_setsockopt(pTmpSock, ZMQ_SNDTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0)
        {
//            printf("setsockopt error. addr=%s\n", pServerAddr);
            ss.str("");
            ss<<HGCOMSERVICENAME<<"setsockopt ZMQ_IMMEDIATE error. addr="<<pServerAddr;
            #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
            #endif
            zmq_close(pTmpSock);
            zmq_ctx_term(pTmpCtx);
            return -1;
        }
    }

    if (iZmqType == ZMQ_SUB)
    {
        if (zmq_setsockopt(pTmpSock, ZMQ_SUBSCRIBE, "", 0))
        {
//            printf("setsockopt error. addr=%s\n", pServerAddr);
            ss.str("");
            ss<<HGCOMSERVICENAME<<"setsockopt ZMQ_IMMEDIATE error. addr="<<pServerAddr;
            #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
            #endif
            zmq_close(pTmpSock);
            zmq_ctx_term(pTmpCtx);
            return -1;
        }
    }

    // 绑定地址 ipc:///cache/zmq/0
    int ret = zmq_connect(pTmpSock, pServerAddr);
    if (ret < 0)
    {
//        printf("%s\n",errCodeInfo().c_str());
//        printf("connect error[ret=%d]. addr=%s\n", ret,pServerAddr);
        ss.str("");
        ss<<HGCOMSERVICENAME<<"connect error[ret="<<ret<<". addr="<<pServerAddr<<","<<errCodeInfo();
        #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
        #endif
        zmq_close(pTmpSock);
        zmq_ctx_term(pTmpCtx);
        return -1;
    }

//    printf("Sub Connect Pub Success, at: %s\n", pServerAddr);
    ss.str("");
    ss<<HGCOMSERVICENAME<<"Sub Connect Pub Success, at: "<<pServerAddr;
    #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGINFO);
    #endif
    *ppCtx = pTmpCtx;
    *ppSock = pTmpSock;
    return 0;
}
}
