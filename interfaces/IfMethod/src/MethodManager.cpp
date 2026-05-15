#include "MethodManager.h"
#include "MethodAdapter.h"

using namespace HGMACHINE;

MethodManager::MethodManager() : m_method(new MethodAdapter()) {
}

MethodManager::~MethodManager() {
}

MethodManager& MethodManager::instance() {
    static MethodManager instance;
    return instance;
}

IMethod& MethodManager::get() {
    return *m_method;
}
