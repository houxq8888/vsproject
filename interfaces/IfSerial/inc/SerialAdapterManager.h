#ifndef SERIALADAPTERMANAGER_H
#define SERIALADAPTERMANAGER_H

#include "ISerial.h"
#include <memory>

namespace HGMACHINE {

class SerialAdapterManager {
public:
    static SerialAdapterManager& instance();
    
    ISerial& create();
    void destroy(ISerial* serial);
    
private:
    SerialAdapterManager();
    ~SerialAdapterManager();
    SerialAdapterManager(const SerialAdapterManager&) = delete;
    SerialAdapterManager& operator=(const SerialAdapterManager&) = delete;
};

}

#endif
