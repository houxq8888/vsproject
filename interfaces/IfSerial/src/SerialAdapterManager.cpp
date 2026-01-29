#include "SerialAdapterManager.h"
#include "SerialAdapter.h"

namespace HGMACHINE {

SerialAdapterManager::SerialAdapterManager() {
}

SerialAdapterManager::~SerialAdapterManager() {
}

SerialAdapterManager& SerialAdapterManager::instance() {
    static SerialAdapterManager instance;
    return instance;
}

ISerial& SerialAdapterManager::create() {
    static SerialAdapter adapter;
    return adapter;
}

void SerialAdapterManager::destroy(ISerial* serial) {
    if (serial) {
        delete serial;
    }
}

}
