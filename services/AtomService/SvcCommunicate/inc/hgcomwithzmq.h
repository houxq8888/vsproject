#ifndef HGCOMWITHZMQ_H
#define HGCOMWITHZMQ_H

#include "hgbasecom.h"
#include "zmq.h"
#include <fstream>
#include <sstream>
#include <string.h>

namespace HGMACHINE {

typedef signed int sint32;

typedef struct {
    std::string id;
    unsigned int pid;
    // 序列化函数
    std::vector<char> serialize() const {
        std::vector<char> buffer;
        size_t id_size=id.size();
        buffer.resize(sizeof(id_size)+id_size+sizeof(pid));
        char* ptr=buffer.data();

        memcpy(ptr,&id_size,sizeof(id_size));
        ptr+=sizeof(id_size);
        memcpy(ptr,id.data(),id_size);
        ptr+=id_size;
        memcpy(ptr,&pid,sizeof(pid));
        return buffer;
    }

    // 反序列化函数
    bool deserialize(const char* data,size_t data_size) {
        const char* ptr=data;
        size_t remaining=data_size;
        if (remaining<sizeof(size_t)) return false;
        size_t id_size;
        mempcpy(&id_size,ptr,sizeof(id_size));
        ptr+=sizeof(id_size);
        remaining-=sizeof(id_size);
        //
        if (remaining<id_size) return false;
        id.assign(ptr,id_size);
        ptr+=id_size;
        remaining-=id_size;

        // 
        if (remaining<sizeof(pid)) return false;
        memcpy(&pid,ptr,sizeof(pid));
        return true;
    }
} ComDataStruct;

class HGCOMMUNICATE_SERVICE_EXPORT HGComWithZmq : public HGBaseCommon
{
public:
    HGComWithZmq();
    ~HGComWithZmq();

    virtual int basesend();
    virtual int baserecv();

    void stop();
    int createPub(const char* pServerAddr);
    int createSub(const char* pServerAddr);

    int sendString(void* socket,char *content);
    char* recvString(void* socket);

    int sendStruct(const ComDataStruct& value);
    ComDataStruct recvStruct();

private:
    std::string errCodeInfo();

private:
    void* m_publisherCtxHdr;
    void* m_subscriberCtxHdr;
    void* m_subscriber;
    void* m_publisher;
};
}

#endif // HGCOMWITHZMQ_H
