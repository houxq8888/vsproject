#ifndef PROTOCOLDECODER_H
#define PROTOCOLDECODER_H


#include "protocolSchema.hpp"
#include <vector>
#include <cstdint>

class ProtocolDecoder {
public:
    explicit ProtocolDecoder(std::shared_ptr<ProtocolSchema> schema);

    std::string check_data_state() const {
        return schema_->check_data_state();
    }
    

    // 解码协议字段
    json decode_payload(uint32_t cmd_id, const std::vector<uint8_t>& payload);
    
private:
    void skip_field(uint8_t wire_type,
                               const std::vector<uint8_t>& payload,
                               size_t& pos);
    json decode_value(uint8_t wire_type,
                                 const std::vector<uint8_t>& payload,
                                 size_t& pos,
                                 const json& field_definition);
    uint64_t decode_varint(const std::vector<uint8_t>& data, size_t& pos);
    bool verify_checksum(const std::vector<uint8_t>& data);
    
    std::shared_ptr<ProtocolSchema> schema_;
};

#endif