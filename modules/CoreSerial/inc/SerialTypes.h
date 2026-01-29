#ifndef SERIALTYPES_H
#define SERIALTYPES_H

#include <vector>
#include <map>
#include <functional>

namespace HGMACHINE {

struct SerialStandardInfo {
    std::vector<uint8_t> head;
    std::vector<uint8_t> tail;
    std::map<std::string, std::vector<uint8_t>> function;
    std::vector<uint8_t> separator;
    std::vector<uint8_t> data;
    
    SerialStandardInfo() {
        head.clear();
        tail.clear();
        data.clear();
        function.clear();
        separator.clear();
    }
    
    void clear() {
        head.clear();
        tail.clear();
        data.clear();
        function.clear();
        separator.clear();
    }
    
    SerialStandardInfo& operator=(const SerialStandardInfo& obj) {
        head = obj.head;
        tail = obj.tail;
        data = obj.data;
        function = obj.function;
        separator = obj.separator;
        return *this;
    }
    
    bool operator==(const SerialStandardInfo& obj) const {
        return (head == obj.head &&
                tail == obj.tail &&
                data == obj.data &&
                function == obj.function &&
                separator == obj.separator);
    }
    
    bool operator!=(const SerialStandardInfo& obj) const {
        return !(*this == obj);
    }
};

using SerialDataCallback = std::function<void(const std::vector<uint8_t>&)>;

}

#endif // SERIALTYPES_H
