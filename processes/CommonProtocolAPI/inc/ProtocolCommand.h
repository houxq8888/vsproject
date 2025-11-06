/*
 * ProtocolCommand.h
 *
 *  Created on: Jul 7, 2025
 *      Author: xiaoqin.hou
 */

#ifndef INC_PROTOCOLCOMMAND_H_
#define INC_PROTOCOLCOMMAND_H_

#include "ProtocolParser.h"
#include <stdint.h>
#include <stdbool.h>


// Protocol library version
#define PROTOCOL_VERSION_MAJOR 1
#define PROTOCOL_VERSION_MINOR 0
#define PROTOCOL_VERSION_PATCH 0



// 帧头帧尾配置结构体
typedef struct {
    uint8_t* header;      // 帧头数据指针
    uint32_t header_size;  // 帧头长度
    uint8_t* footer;      // 帧尾数据指针
    uint32_t footer_size;  // 帧尾长度

    // Protobuf字段标签
    uint8_t CMD_ID_TAG;      // (1 << 3) | 0 (varint)
    uint8_t CMD_NAME_TAG;     // (2 << 3) | 2 (length-delimited)
    uint8_t PAYLOAD_TAG;      // (3 << 3) | 2 (length-delimited)
    uint8_t SUCCESS_TAG;      // (2 << 3) | 0 (varint)
    uint8_t RESPONSE_DATA_TAG;// (3 << 3) | 2 (length-delimited)
} FrameConfig;
// Field value union
typedef union {
    uint8_t uint8_val;
    uint16_t uint16_val;
    uint32_t uint32_val;
    uint64_t uint64_val;
    int8_t int8_val;
    int16_t int16_val;
    int32_t int32_val;
    int64_t int64_val;
    float float_val;
    double double_val;
    bool bool_val;
    char* string_val;
    struct {
        uint8_t* data;
        uint32_t length;
    } bytes_val;
} FieldValue;

// Command field structure
typedef struct {
    uint8_t field_num;
    FieldType type;
    FieldValue value;
} CommandField;

// Command structure
typedef struct {
    uint32_t id;
    const char* name;
    CommandField* fields;
    uint16_t field_count;
    bool is_response;
} Command;

// Error codes
typedef enum {
    PROTOCOL_SUCCESS = 0,
    PROTOCOL_ERROR_INVALID_ARGUMENT,
    PROTOCOL_ERROR_MEMORY_ALLOCATION,
    PROTOCOL_ERROR_BUFFER_TOO_SMALL,
    PROTOCOL_ERROR_INVALID_DATA,
    PROTOCOL_ERROR_UNKNOWN_COMMAND,
    PROTOCOL_ERROR_FIELD_NOT_FOUND,
    PROTOCOL_ERROR_TYPE_MISMATCH,
    PROTOCOL_ERROR_JSON_PARSING,
    PROTOCOL_ERROR_FRAME_TOO_SHORT,
    PROTOCOL_ERROR_LENGTH_MISMATCH,
    PROTOCOL_ERROR_CRC_MISMATCH,
    /* 新增帧相关错误代码 */
    PROTOCOL_ERROR_INVALID_HEADER,      // 帧头无效
    PROTOCOL_ERROR_INVALID_FOOTER,      // 帧尾无效
    PROTOCOL_ERROR_CHECKSUM_FAILED      // 校验失败
} ProtocolError;

// Protocol initialization and cleanup
void protocol_cleanup(void);

// 添加帧配置获取和设置函数
void protocol_set_frame_config(const FrameConfig* frame_cfg);
const FrameConfig* protocol_get_frame_config(void);

// 修改协议初始化函数，添加帧配置
bool protocol_init(const FrameConfig* frame_cfg);

// Protocol definition management
ProtocolDefinition* protocol_load_from_string(const char* json_str);
void protocol_free_definition(ProtocolDefinition* protocol);

// Command handling
Command* protocol_create_command(const ProtocolDefinition* protocol, uint32_t cmd_id);
void protocol_free_command(Command* cmd);

// Command serialization
ProtocolError protocol_encode_command(const Command* cmd,
                                    uint8_t* buffer,
                                    uint32_t buffer_size,
                                    uint32_t* encoded_size);
ProtocolError protocol_decode_command(const uint8_t* data,
                                    uint32_t data_size,
                                    const ProtocolDefinition* protocol,
                                    Command** cmd);
ProtocolError protocol_encode_response(const Command* cmd,
                                    uint8_t* buffer,
                                    uint32_t buffer_size,
                                    uint32_t* encoded_size);

// Field access
ProtocolError protocol_set_field(Command* cmd,
                               uint8_t field_num,
                               FieldType type,
                               FieldValue value);
ProtocolError protocol_get_field(const Command* cmd,
                               uint8_t field_num,
                               FieldType* type,
                               FieldValue* value);

// Utility functions
uint16_t calculateChecksum(const uint8_t* data, size_t length) ;
bool encode_payload_from_command(const Command* cmd, uint8_t* payload_buf, uint32_t buf_size, uint32_t* payload_len);


ProtocolError protocol_wrap_frame(const uint8_t* payload, uint32_t payload_size,
                                  uint8_t* out_buffer, uint32_t buffer_size,
                                  uint32_t* out_size);
ProtocolError protocol_unwrap_frame(const uint8_t* frame, uint32_t frame_size,
                                  uint8_t* out_payload, uint32_t payload_size,
                                  uint32_t* out_payload_size);
                                  
// Function prototypes
Command* create_response_command(const CommandDefinition* cmd_def);
Command* create_command(const CommandDefinition* cmd_def);
void free_command(Command* cmd);
bool decode_protocol_frame(const uint8_t* data,
                           uint32_t data_size,
                           uint32_t* offset,
                           uint32_t* cmd_id,
                           uint8_t* payload_buf,
                           uint32_t payload_buf_size,
                           uint32_t* payload_len_out,
                           const ProtocolDefinition* protocol,
                           Command** cmd) ;
bool encode_command(uint32_t cmd_id, const char* cmd_name, const uint8_t* payload, uint32_t payload_len,
                    uint8_t* buffer, uint32_t buffer_size, uint32_t* encoded_size);
bool decode_command(const uint8_t* data, uint32_t data_size,const CommandDefinition* cmd_def, Command** cmd);

bool set_command_field(Command* cmd, uint8_t field_num, FieldType type, FieldValue value);
bool get_command_field(const Command* cmd, uint8_t field_num, FieldType* type, FieldValue* value);

#endif /* INC_PROTOCOLCOMMAND_H_ */
