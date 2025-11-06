/*
 * ProtocolCommand.c
 *
 *  Created on: Jul 7, 2025
 *      Author: xiaoqin.hou
 */

#include "ProtocolCommand.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static FrameConfig current_frame_cfg = {0};

// Library initialization flag
static bool protocol_initialized = false;

static uint32_t skip_unknown_field(const uint8_t* data, uint32_t pos, uint32_t data_size, uint8_t wire_type) {
    uint32_t len = 0;
    int shift;
    switch (wire_type) {
        case 0: // varint
            while (pos < data_size) {
                uint8_t b = data[pos++];
                if (!(b & 0x80)) break;
            }
            break;
        case 1: // 64-bit
            if (pos + 8 > data_size) return 0;
            pos += 8;
            break;
        case 2: // length-delimited
            len = 0;
            shift = 0;
            while (true) {
                if (pos >= data_size) return 0;
                uint8_t byte = data[pos++];
                len |= (byte & 0x7F) << shift;
                shift += 7;
                if (!(byte & 0x80)) break;
            }
            if (pos + len > data_size) return 0;
            pos += len;
            break;
        case 5: // 32-bit
            if (pos + 4 > data_size) return 0;
            pos += 4;
            break;
        default:
            return 0; // unsupported wire type
    }
    return pos;
}

void protocol_cleanup(void) {
    // Clean up any resources here
    protocol_initialized = false;
}

void protocol_get_version(uint8_t* major, uint8_t* minor, uint8_t* patch) {
    if (major) *major = PROTOCOL_VERSION_MAJOR;
    if (minor) *minor = PROTOCOL_VERSION_MINOR;
    if (patch) *patch = PROTOCOL_VERSION_PATCH;
}

const char* protocol_get_version_string(void) {
    static char version_string[16];
    snprintf(version_string, sizeof(version_string), "%d.%d.%d",
             PROTOCOL_VERSION_MAJOR, PROTOCOL_VERSION_MINOR, PROTOCOL_VERSION_PATCH);
    return version_string;
}

const char* protocol_get_error_string(ProtocolError error) {
    static const char* error_strings[] = {
        "Success",
        "Invalid argument",
        "Memory allocation failed",
        "Buffer too small",
        "Invalid data",
        "Unknown command",
        "Field not found",
        "Type mismatch",
        "JSON parsing error",
        /* 新增错误描述 */
        "Invalid frame header",
        "Invalid frame footer", 
        "Checksum failed"
    };
    
    if (error >= PROTOCOL_SUCCESS && error <= PROTOCOL_ERROR_CHECKSUM_FAILED) {
        return error_strings[error];
    }
    return "Unknown error";
}

ProtocolDefinition* protocol_load_from_file(const char* filename) {
    if (filename == NULL) return NULL;

    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Failed to open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* json_str = (char*)malloc(file_size + 1);
    if (!json_str) {
        fclose(file);
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    fread(json_str, 1, file_size, file);
    json_str[file_size] = '\0';
    fclose(file);

    ProtocolDefinition* protocol = parse_protocol(json_str);
    free(json_str);
    return protocol;
}

ProtocolDefinition* protocol_load_from_string(const char* json_str) {
    if (json_str == NULL) return NULL;
    return parse_protocol(json_str);
}

void protocol_free_definition(ProtocolDefinition* protocol) {
    free_protocol(protocol);
}

Command* protocol_create_command(const ProtocolDefinition* protocol, uint32_t cmd_id) {
    if (protocol == NULL) return NULL;

    const CommandDefinition* cmd_def = find_command_by_id(protocol, cmd_id);
    if (cmd_def == NULL) return NULL;

    return create_command(cmd_def);
}

void protocol_free_command(Command* cmd) {
    free_command(cmd);
}
static bool encode_field_value(FieldType type, const FieldValue* value, uint8_t* buffer, uint32_t buffer_size, uint32_t* offset) {
    switch (type) {
        case TYPE_UINT8:
            if (*offset + 1 > buffer_size) return false;
            buffer[(*offset)++] = value->uint8_val;
            break;

        case TYPE_UINT16:
            if (*offset + 2 > buffer_size) return false;
            buffer[(*offset)++] = (value->uint16_val >> 8) & 0xFF;
            buffer[(*offset)++] = value->uint16_val & 0xFF;
            break;

        case TYPE_UINT32:
            if (*offset + 4 > buffer_size) return false;
            buffer[(*offset)++] = (value->uint32_val >> 24) & 0xFF;
            buffer[(*offset)++] = (value->uint32_val >> 16) & 0xFF;
            buffer[(*offset)++] = (value->uint32_val >> 8) & 0xFF;
            buffer[(*offset)++] = value->uint32_val & 0xFF;
            break;

        case TYPE_UINT64:
            if (*offset + 8 > buffer_size) return false;
            buffer[(*offset)++] = (value->uint64_val >> 56) & 0xFF;
            buffer[(*offset)++] = (value->uint64_val >> 48) & 0xFF;
            buffer[(*offset)++] = (value->uint64_val >> 40) & 0xFF;
            buffer[(*offset)++] = (value->uint64_val >> 32) & 0xFF;
            buffer[(*offset)++] = (value->uint64_val >> 24) & 0xFF;
            buffer[(*offset)++] = (value->uint64_val >> 16) & 0xFF;
            buffer[(*offset)++] = (value->uint64_val >> 8) & 0xFF;
            buffer[(*offset)++] = value->uint64_val & 0xFF;
            break;

        case TYPE_INT8:
            if (*offset + 1 > buffer_size) return false;
            buffer[(*offset)++] = (uint8_t)value->int8_val;
            break;

        case TYPE_INT16:
            if (*offset + 2 > buffer_size) return false;
            buffer[(*offset)++] = (value->int16_val >> 8) & 0xFF;
            buffer[(*offset)++] = value->int16_val & 0xFF;
            break;

        case TYPE_INT32:
            if (*offset + 4 > buffer_size) return false;
            buffer[(*offset)++] = (value->int32_val >> 24) & 0xFF;
            buffer[(*offset)++] = (value->int32_val >> 16) & 0xFF;
            buffer[(*offset)++] = (value->int32_val >> 8) & 0xFF;
            buffer[(*offset)++] = value->int32_val & 0xFF;
            break;

        case TYPE_INT64:
            if (*offset + 8 > buffer_size) return false;
            buffer[(*offset)++] = (value->int64_val >> 56) & 0xFF;
            buffer[(*offset)++] = (value->int64_val >> 48) & 0xFF;
            buffer[(*offset)++] = (value->int64_val >> 40) & 0xFF;
            buffer[(*offset)++] = (value->int64_val >> 32) & 0xFF;
            buffer[(*offset)++] = (value->int64_val >> 24) & 0xFF;
            buffer[(*offset)++] = (value->int64_val >> 16) & 0xFF;
            buffer[(*offset)++] = (value->int64_val >> 8) & 0xFF;
            buffer[(*offset)++] = value->int64_val & 0xFF;
            break;

        case TYPE_FLOAT:
            if (*offset + 4 > buffer_size) return false;
            memcpy(buffer + *offset, &value->float_val, 4);
            *offset += 4;
            break;

        case TYPE_DOUBLE:
            if (*offset + 8 > buffer_size) return false;
            memcpy(buffer + *offset, &value->double_val, 8);
            *offset += 8;
            break;

        case TYPE_BOOL:
            if (*offset + 1 > buffer_size) return false;
            buffer[(*offset)++] = value->bool_val ? 1 : 0;
            break;

        case TYPE_STRING:
            if (*offset + 4 + strlen(value->string_val) > buffer_size) return false;
            buffer[(*offset)++] = (uint8_t)(strlen(value->string_val) >> 24);
            buffer[(*offset)++] = (uint8_t)(strlen(value->string_val) >> 16);
            buffer[(*offset)++] = (uint8_t)(strlen(value->string_val) >> 8);
            buffer[(*offset)++] = (uint8_t)strlen(value->string_val);
            memcpy(buffer + *offset, value->string_val, strlen(value->string_val));
            *offset += strlen(value->string_val);
            break;

        case TYPE_BYTES:
            if (*offset + 4 + value->bytes_val.length > buffer_size) return false;
            buffer[(*offset)++] = (value->bytes_val.length >> 24) & 0xFF;
            buffer[(*offset)++] = (value->bytes_val.length >> 16) & 0xFF;
            buffer[(*offset)++] = (value->bytes_val.length >> 8) & 0xFF;
            buffer[(*offset)++] = value->bytes_val.length & 0xFF;
            memcpy(buffer + *offset, value->bytes_val.data, value->bytes_val.length);
            *offset += value->bytes_val.length;
            break;

        default:
            return false;
    }

    return true;
}
static bool encode_varint(uint32_t value, uint8_t* buffer, uint32_t buffer_size, uint32_t* offset) {
    while (value >= 0x80) {
        if (*offset >= buffer_size) return false;
        buffer[(*offset)++] = (uint8_t)((value & 0x7F) | 0x80);
        value >>= 7;
    }
    if (*offset >= buffer_size) return false;
    buffer[(*offset)++] = (uint8_t)(value & 0x7F);
    return true;
}

bool encode_payload_from_command(const Command* cmd, uint8_t* payload_buf, uint32_t buf_size, uint32_t* payload_len) {
    if (!cmd || !payload_buf || !payload_len) return false;

    uint32_t offset = 0;
    for (uint16_t i = 0; i < cmd->field_count; i++) {
        const CommandField* field = &cmd->fields[i];
        uint8_t wire_type;

        // 判断 wire_type
        switch (field->type) {
            case TYPE_UINT8: case TYPE_UINT16: case TYPE_UINT32:
            case TYPE_UINT64: case TYPE_INT8: case TYPE_INT16:
            case TYPE_INT32: case TYPE_INT64: case TYPE_BOOL:
                wire_type = 0;  // varint
                break;
            case TYPE_DOUBLE:
                wire_type = 1;
                break;
            case TYPE_FLOAT:
                wire_type = 5;  // fixed32 or fixed64，示例中统一用 fixed32
                break;
            case TYPE_STRING: case TYPE_BYTES:
                wire_type = 2;  // length-delimited
                break;
            default:
                return false;
        }

        // 编码 tag (field_num << 3 | wire_type)
        uint8_t tag = (field->field_num << 3) | wire_type;
        if (offset + 1 > buf_size) return false;
        payload_buf[offset++] = tag;

        // 编码值（你需要实现 encode_field_value，根据类型写入 payload_buf 并更新 offset）
        if (!encode_field_value(field->type, &field->value, payload_buf, buf_size, &offset)) {
            return false;
        }
    }

    *payload_len = offset;
    return true;
}
ProtocolError protocol_encode_response(const Command* cmd,
                                       uint8_t* buffer,
                                       uint32_t buffer_size,
                                       uint32_t* encoded_size)
{
    if (!cmd || !buffer || !encoded_size) 
        return PROTOCOL_ERROR_INVALID_ARGUMENT;

    uint8_t temp_buffer[256];
    uint32_t pos = 0;

    // 1. CMD_ID_TAG (field_num=1, wire_type=varint => tag=CMD_ID_TAG)
    temp_buffer[pos++] = current_frame_cfg.CMD_ID_TAG;
    uint32_t id = cmd->id;
    while (id > 0x7F) {
        temp_buffer[pos++] = (id & 0x7F) | 0x80;
        id >>= 7;
    }
    temp_buffer[pos++] = id & 0x7F;

    // 2. SUCCESS_TAG (field_num=2, wire_type=varint => tag=0x10)
    temp_buffer[pos++] = current_frame_cfg.SUCCESS_TAG;
    temp_buffer[pos++] = true ? 1 : 0; // 这里假设 cmd 结构体有 success 字段

    // 3. RESPONSE_DATA_TAG (field_num=3, wire_type=length-delimited => tag=0x1A)
    uint8_t payload_buf[256];
    uint32_t payload_len = 0;

    // 把 response_fields 编码成 payload
    if (!encode_payload_from_command(cmd, payload_buf, sizeof(payload_buf), &payload_len)) {
        return PROTOCOL_ERROR_INVALID_ARGUMENT;
    }

    temp_buffer[pos++] = current_frame_cfg.RESPONSE_DATA_TAG; // RESPONSE_DATA_TAG
    // 写 payload 长度 (varint)
    uint32_t len = payload_len;
    while (len > 0x7F) {
        temp_buffer[pos++] = (len & 0x7F) | 0x80;
        len >>= 7;
    }
    temp_buffer[pos++] = len & 0x7F;

    // 写 payload 内容
    memcpy(temp_buffer + pos, payload_buf, payload_len);
    pos += payload_len;

    // 4. 添加帧头帧尾和 CRC
    return protocol_wrap_frame(temp_buffer, pos, buffer, buffer_size, encoded_size);
}
ProtocolError protocol_encode_command(const Command* cmd,
                                    uint8_t* buffer,
                                    uint32_t buffer_size,
                                    uint32_t* encoded_size) {
    if (!cmd || !buffer || !encoded_size) return PROTOCOL_ERROR_INVALID_ARGUMENT;

    // 先编码命令内容
    uint8_t temp_buffer[256];
    uint32_t temp_size;
    uint8_t payload_buf[256];
    uint32_t payload_len = 0;
    encode_payload_from_command(cmd,payload_buf,sizeof(payload_buf),&payload_len);

    bool flag = encode_command(cmd->id, cmd->name, payload_buf, payload_len, temp_buffer, sizeof(temp_buffer), &temp_size);
    if (!flag) return PROTOCOL_ERROR_INVALID_ARGUMENT;
    
    // 添加帧头帧尾
    return protocol_wrap_frame(temp_buffer, temp_size, buffer, buffer_size, encoded_size);
}
ProtocolError protocol_decode_command(const uint8_t* data,
                                    uint32_t data_size,
                                    const ProtocolDefinition* protocol,
                                    Command** cmd) {
    // 先去除帧头帧尾
    uint8_t temp_buffer[256];
    uint32_t data_len;
    ProtocolError err = protocol_unwrap_frame(data, data_size, temp_buffer, 
                                            sizeof(temp_buffer), &data_len);
    if (err != PROTOCOL_SUCCESS)
        return err;

    printf("decode buffer:");
    for (size_t i = 0; i < data_len; i++)
    {
        printf("%02X ", temp_buffer[i]); // %02X 表示两位大写十六进制
        if ((i + 1) % 16 == 0)
        { // 每16个字节换行
            printf("\n");
        }
    }
    printf("\n");

    uint8_t payload_buf[256];
    uint32_t payload_len = 0;
    uint32_t offset = 0;
    uint32_t cmd_id;

    bool ok = decode_protocol_frame(temp_buffer, data_len, &offset, &cmd_id,
                                    payload_buf, sizeof(payload_buf), &payload_len,
                                protocol,cmd);
    if (ok)
    {
        printf("CMD_ID: %u\n", cmd_id);
        printf("CMD_ID: %u\n", (*cmd)->id);
        printf("CMD_NAME: %s\n", (*cmd)->name);
        printf("Payload (%u bytes):\n", payload_len);
        for (uint32_t i = 0; i < payload_len; ++i)
        {
            printf("%02X ", payload_buf[i]);
        }
        printf("\n");
        return PROTOCOL_SUCCESS;
    }
}

ProtocolError protocol_set_field(Command* cmd, uint8_t field_num, FieldType type, FieldValue value) {
    if (cmd == NULL) {
        return PROTOCOL_ERROR_INVALID_ARGUMENT;
    }

    if (!set_command_field(cmd, field_num, type, value)) {
        return PROTOCOL_ERROR_FIELD_NOT_FOUND;
    }

    return PROTOCOL_SUCCESS;
}

ProtocolError protocol_get_field(const Command* cmd, uint8_t field_num, FieldType* type, FieldValue* value) {
    if (cmd == NULL || type == NULL || value == NULL) {
        return PROTOCOL_ERROR_INVALID_ARGUMENT;
    }

    if (!get_command_field(cmd, field_num, type, value)) {
        return PROTOCOL_ERROR_FIELD_NOT_FOUND;
    }

    return PROTOCOL_SUCCESS;
}

const char* protocol_field_type_to_string(FieldType type) {
    switch (type) {
        case TYPE_UINT8: return "uint8";
        case TYPE_UINT16: return "uint16";
        case TYPE_UINT32: return "uint32";
        case TYPE_UINT64: return "uint64";
        case TYPE_INT8: return "int8";
        case TYPE_INT16: return "int16";
        case TYPE_INT32: return "int32";
        case TYPE_INT64: return "int64";
        case TYPE_FLOAT: return "float";
        case TYPE_DOUBLE: return "double";
        case TYPE_BOOL: return "bool";
        case TYPE_STRING: return "string";
        case TYPE_BYTES: return "bytes";
        case TYPE_UNKNOWN: return "unknown";
        default: return "invalid";
    }
}

bool protocol_init(const FrameConfig* frame_cfg) {
    if (frame_cfg) {
        protocol_set_frame_config(frame_cfg);
    }
    return true;
}

void protocol_set_frame_config(const FrameConfig* frame_cfg) {
    // 释放旧的配置内存
    if (current_frame_cfg.header) free(current_frame_cfg.header);
    if (current_frame_cfg.footer) free(current_frame_cfg.footer);
    
    // 复制新的帧头
    if (frame_cfg->header && frame_cfg->header_size > 0) {
        current_frame_cfg.header = malloc(frame_cfg->header_size);
        memcpy(current_frame_cfg.header, frame_cfg->header, frame_cfg->header_size);
        current_frame_cfg.header_size = frame_cfg->header_size;
    } else {
        current_frame_cfg.header = NULL;
        current_frame_cfg.header_size = 0;
    }
    
    // 复制新的帧尾
    if (frame_cfg->footer && frame_cfg->footer_size > 0) {
        current_frame_cfg.footer = malloc(frame_cfg->footer_size);
        memcpy(current_frame_cfg.footer, frame_cfg->footer, frame_cfg->footer_size);
        current_frame_cfg.footer_size = frame_cfg->footer_size;
    } else {
        current_frame_cfg.footer = NULL;
        current_frame_cfg.footer_size = 0;
    }
    current_frame_cfg.CMD_ID_TAG = frame_cfg->CMD_ID_TAG;
    current_frame_cfg.CMD_NAME_TAG = frame_cfg->CMD_NAME_TAG;
    current_frame_cfg.PAYLOAD_TAG = frame_cfg->PAYLOAD_TAG;
    current_frame_cfg.RESPONSE_DATA_TAG = frame_cfg->RESPONSE_DATA_TAG;
    current_frame_cfg.SUCCESS_TAG = frame_cfg->SUCCESS_TAG;
}

const FrameConfig* protocol_get_frame_config(void) {
    return &current_frame_cfg;
}

// 封装数据帧（添加头尾）
ProtocolError protocol_wrap_frame(const uint8_t* payload, uint32_t payload_size,
                                  uint8_t* out_buffer, uint32_t buffer_size,
                                  uint32_t* out_size) {
    const FrameConfig* cfg = protocol_get_frame_config();

    const uint32_t LENGTH_FIELD_SIZE = 4;
    const uint32_t CRC_FIELD_SIZE = 2;
    uint32_t required_size = cfg->header_size + LENGTH_FIELD_SIZE + payload_size + CRC_FIELD_SIZE + cfg->footer_size;

    if (buffer_size < required_size) {
        return PROTOCOL_ERROR_BUFFER_TOO_SMALL;
    }

    uint32_t offset = 0;

    // 帧头
    if (cfg->header_size > 0) {
        memcpy(out_buffer + offset, cfg->header, cfg->header_size);
        offset += cfg->header_size;
    }

    // 写入 4 字节 payload 长度（小端)
    out_buffer[offset++] = payload_size & 0xFF;
    out_buffer[offset++] = (payload_size >> 8) & 0xFF;
    out_buffer[offset++] = (payload_size >> 16) & 0xFF;
    out_buffer[offset++] = (payload_size >> 24) & 0xFF;

    // 拷贝 payload 数据
    memcpy(out_buffer + offset, payload, payload_size);
    offset += payload_size;

    // 计算 CRC（仅对长度字段 + 数据进行计算）
    uint16_t crc = calculateChecksum(out_buffer, offset);

    // 写入 2 字节 CRC（小端)
    out_buffer[offset++] = crc & 0xFF;
    out_buffer[offset++] = (crc >> 8) & 0xFF;

    // 帧尾
    if (cfg->footer_size > 0) {
        memcpy(out_buffer + offset, cfg->footer, cfg->footer_size);
        offset += cfg->footer_size;
    }

    *out_size = offset;
    return PROTOCOL_SUCCESS;
}
// CRC-16-IBM（Modbus）实现
uint16_t calculateChecksum(const uint8_t* data, size_t length) {
    const uint16_t POLY = 0xA001;
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= POLY;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}
// 解封装数据帧（去除头尾）
ProtocolError protocol_unwrap_frame(const uint8_t* data,
                                    uint32_t data_size,
                                    uint8_t* out_payload,
                                    uint32_t out_buffer_size,
                                    uint32_t* out_payload_size) {
    const FrameConfig* cfg = protocol_get_frame_config();
    if (!data || !out_payload || !out_payload_size) return PROTOCOL_ERROR_INVALID_ARGUMENT;

    // 最小长度 = header + 4字节长度 + CRC(2) + footer
    uint32_t min_frame_size = cfg->header_size + 4 + 2 + cfg->footer_size;
    if (data_size < min_frame_size) return PROTOCOL_ERROR_FRAME_TOO_SHORT;

    uint32_t offset = 0;

    // 跳过帧头
    if (cfg->header_size > 0) {
        if (memcmp(data, cfg->header, cfg->header_size) != 0) return PROTOCOL_ERROR_INVALID_HEADER;
        offset += cfg->header_size;
    }

    // 读取长度字段（小端）
    uint32_t payload_len = data[offset] |
                          (data[offset + 1] << 8) |
                          (data[offset + 2] << 16) |
                          (data[offset + 3] << 24);
    offset += 4;

    if (offset + payload_len + 2 + cfg->footer_size > data_size) return PROTOCOL_ERROR_LENGTH_MISMATCH;

    // 拿出 payload 数据（CRC 校验前）
    if (payload_len > out_buffer_size) return PROTOCOL_ERROR_BUFFER_TOO_SMALL;
    memcpy(out_payload, data + offset, payload_len);

    // 校验 CRC
    uint16_t expected_crc = data[offset + payload_len] |
                           (data[offset + payload_len + 1] << 8);
    uint16_t actual_crc = calculateChecksum(data, offset + payload_len);  // 包括header+length+payload部分
    if (expected_crc != actual_crc) return PROTOCOL_ERROR_CRC_MISMATCH;

    // 验证帧尾
    if (cfg->footer_size > 0) {
        const uint8_t* actual_footer = data + offset + payload_len + 2;
        if (memcmp(actual_footer, cfg->footer, cfg->footer_size) != 0) return PROTOCOL_ERROR_INVALID_FOOTER;
    }

    *out_payload_size = payload_len;
    return PROTOCOL_SUCCESS;
}
Command* create_response_command(const CommandDefinition* cmd_def) {
    if (cmd_def == NULL) return NULL;

    Command* cmd = malloc(sizeof(Command));
    if (cmd == NULL) return NULL;

    cmd->id = cmd_def->id;
    cmd->name = strdup(cmd_def->name);
    cmd->field_count = cmd_def->response_fields_count;

    if (cmd->field_count > 0) {
        cmd->fields = malloc(cmd->field_count * sizeof(CommandField));
        if (cmd->fields == NULL) {
            free(cmd);
            return NULL;
        }

        // Initialize with default values
        for (uint16_t i = 0; i < cmd->field_count; i++) {
            cmd->fields[i].field_num = cmd_def->response_fields[i].field_num;
            cmd->fields[i].type = cmd_def->response_fields[i].type;

            if (cmd_def->response_fields[i].has_default) {
                switch (cmd->fields[i].type) {
                    case TYPE_UINT8: cmd->fields[i].value.uint8_val = cmd_def->response_fields[i].default_value.uint8_val; break;
                    case TYPE_UINT16: cmd->fields[i].value.uint16_val = cmd_def->response_fields[i].default_value.uint16_val; break;
                    case TYPE_UINT32: cmd->fields[i].value.uint32_val = cmd_def->response_fields[i].default_value.uint32_val; break;
                    case TYPE_UINT64: cmd->fields[i].value.uint64_val = cmd_def->response_fields[i].default_value.uint64_val; break;
                    case TYPE_INT8: cmd->fields[i].value.int8_val = cmd_def->response_fields[i].default_value.int8_val; break;
                    case TYPE_INT16: cmd->fields[i].value.int16_val = cmd_def->response_fields[i].default_value.int16_val; break;
                    case TYPE_INT32: cmd->fields[i].value.int32_val = cmd_def->response_fields[i].default_value.int32_val; break;
                    case TYPE_INT64: cmd->fields[i].value.int64_val = cmd_def->response_fields[i].default_value.int64_val; break;
                    case TYPE_FLOAT: cmd->fields[i].value.float_val = cmd_def->response_fields[i].default_value.float_val; break;
                    case TYPE_DOUBLE: cmd->fields[i].value.double_val = cmd_def->response_fields[i].default_value.double_val; break;
                    case TYPE_BOOL: cmd->fields[i].value.bool_val = cmd_def->response_fields[i].default_value.bool_val; break;
                    case TYPE_STRING:
                        if (cmd_def->response_fields[i].default_value.string_val != NULL) {
                            cmd->fields[i].value.string_val = strdup(cmd_def->response_fields[i].default_value.string_val);
                        }
                        break;
                    case TYPE_BYTES:
                        // No default for bytes
                        break;
                    default: break;
                }
            } else {
                // Zero-initialize
                memset(&cmd->fields[i].value, 0, sizeof(FieldValue));
            }
        }
    } else {
        cmd->fields = NULL;
    }

    return cmd;
}
Command* create_command(const CommandDefinition* cmd_def) {
    if (cmd_def == NULL) return NULL;

    Command* cmd = malloc(sizeof(Command));
    if (cmd == NULL) return NULL;

    cmd->id = cmd_def->id;
    cmd->name = strdup(cmd_def->name);
    cmd->field_count = cmd_def->request_fields_count;

    if (cmd->field_count > 0) {
        cmd->fields = malloc(cmd->field_count * sizeof(CommandField));
        if (cmd->fields == NULL) {
            free(cmd);
            return NULL;
        }

        // Initialize with default values
        for (uint16_t i = 0; i < cmd->field_count; i++) {
            cmd->fields[i].field_num = cmd_def->request_fields[i].field_num;
            cmd->fields[i].type = cmd_def->request_fields[i].type;

            if (cmd_def->request_fields[i].has_default) {
                switch (cmd->fields[i].type) {
                    case TYPE_UINT8: cmd->fields[i].value.uint8_val = cmd_def->request_fields[i].default_value.uint8_val; break;
                    case TYPE_UINT16: cmd->fields[i].value.uint16_val = cmd_def->request_fields[i].default_value.uint16_val; break;
                    case TYPE_UINT32: cmd->fields[i].value.uint32_val = cmd_def->request_fields[i].default_value.uint32_val; break;
                    case TYPE_UINT64: cmd->fields[i].value.uint64_val = cmd_def->request_fields[i].default_value.uint64_val; break;
                    case TYPE_INT8: cmd->fields[i].value.int8_val = cmd_def->request_fields[i].default_value.int8_val; break;
                    case TYPE_INT16: cmd->fields[i].value.int16_val = cmd_def->request_fields[i].default_value.int16_val; break;
                    case TYPE_INT32: cmd->fields[i].value.int32_val = cmd_def->request_fields[i].default_value.int32_val; break;
                    case TYPE_INT64: cmd->fields[i].value.int64_val = cmd_def->request_fields[i].default_value.int64_val; break;
                    case TYPE_FLOAT: cmd->fields[i].value.float_val = cmd_def->request_fields[i].default_value.float_val; break;
                    case TYPE_DOUBLE: cmd->fields[i].value.double_val = cmd_def->request_fields[i].default_value.double_val; break;
                    case TYPE_BOOL: cmd->fields[i].value.bool_val = cmd_def->request_fields[i].default_value.bool_val; break;
                    case TYPE_STRING:
                        if (cmd_def->request_fields[i].default_value.string_val != NULL) {
                            cmd->fields[i].value.string_val = strdup(cmd_def->request_fields[i].default_value.string_val);
                        }
                        break;
                    case TYPE_BYTES:
                        // No default for bytes
                        break;
                    default: break;
                }
            } else {
                // Zero-initialize
                memset(&cmd->fields[i].value, 0, sizeof(FieldValue));
            }
        }
    } else {
        cmd->fields = NULL;
    }

    return cmd;
}

void free_command(Command* cmd) {
    if (cmd == NULL) return;

    if (cmd->fields != NULL) {
        for (uint16_t i = 0; i < cmd->field_count; i++) {
            if (cmd->fields[i].type == TYPE_STRING && cmd->fields[i].value.string_val != NULL) {
                free(cmd->fields[i].value.string_val);
            }
            if (cmd->fields[i].type == TYPE_BYTES && cmd->fields[i].value.bytes_val.data != NULL) {
                free(cmd->fields[i].value.bytes_val.data);
            }
        }
        free(cmd->fields);
    }

    free(cmd);
}

bool set_command_field(Command* cmd, uint8_t field_num, FieldType type, FieldValue value) {
    if (cmd == NULL) return false;

    for (uint16_t i = 0; i < cmd->field_count; i++) {
        if (cmd->fields[i].field_num == field_num) {
            if (cmd->fields[i].type != type) return false;

            // Free existing string or bytes if needed
            if (cmd->fields[i].type == TYPE_STRING && cmd->fields[i].value.string_val != NULL) {
                free(cmd->fields[i].value.string_val);
            }
            if (cmd->fields[i].type == TYPE_BYTES && cmd->fields[i].value.bytes_val.data != NULL) {
                free(cmd->fields[i].value.bytes_val.data);
            }

            // Copy the new value
            if (type == TYPE_STRING) {
                cmd->fields[i].value.string_val = strdup(value.string_val);
                if (cmd->fields[i].value.string_val == NULL) return false;
            } else if (type == TYPE_BYTES) {
                cmd->fields[i].value.bytes_val.data = malloc(value.bytes_val.length);
                if (cmd->fields[i].value.bytes_val.data == NULL) return false;
                memcpy(cmd->fields[i].value.bytes_val.data, value.bytes_val.data, value.bytes_val.length);
                cmd->fields[i].value.bytes_val.length = value.bytes_val.length;
            } else {
                cmd->fields[i].value = value;
            }

            return true;
        }
    }

    return false;
}

bool get_command_field(const Command* cmd, uint8_t field_num, FieldType* type, FieldValue* value) {
    if (cmd == NULL || type == NULL || value == NULL) return false;

    for (uint16_t i = 0; i < cmd->field_count; i++) {
        if (cmd->fields[i].field_num == field_num) {
            *type = cmd->fields[i].type;

            if (cmd->fields[i].type == TYPE_STRING) {
                value->string_val = strdup(cmd->fields[i].value.string_val);
                if (value->string_val == NULL) return false;
            } else if (cmd->fields[i].type == TYPE_BYTES) {
                value->bytes_val.data = malloc(cmd->fields[i].value.bytes_val.length);
                if (value->bytes_val.data == NULL) return false;
                memcpy(value->bytes_val.data, cmd->fields[i].value.bytes_val.data, cmd->fields[i].value.bytes_val.length);
                value->bytes_val.length = cmd->fields[i].value.bytes_val.length;
            } else {
                *value = cmd->fields[i].value;
            }

            return true;
        }
    }

    return false;
}

// Helper function to encode a field value into the buffer

static bool encode_length_delimited(const uint8_t* data, uint32_t length, uint8_t* buffer, uint32_t buffer_size, uint32_t* offset) {
    if (!encode_varint(length, buffer, buffer_size, offset)) return false;
    if (*offset + length > buffer_size) return false;
    memcpy(buffer + *offset, data, length);
    *offset += length;
    return true;
}

bool encode_command(uint32_t cmd_id, const char* cmd_name, const uint8_t* payload, uint32_t payload_len,
                    uint8_t* buffer, uint32_t buffer_size, uint32_t* encoded_size) {
    if (!buffer || !encoded_size) return false;
    const FrameConfig* cfg = protocol_get_frame_config();
    uint32_t offset = 0;

    // CMD_ID_TAG
    if (offset + 1 > buffer_size) return false;
    buffer[offset++] = cfg->CMD_ID_TAG;
    if (!encode_varint(cmd_id, buffer, buffer_size, &offset)) return false;

    // CMD_NAME_TAG
    if (cmd_name && cmd_name[0] != '\0') {
        size_t name_len = strlen(cmd_name);
        if (offset + 1 > buffer_size) return false;
        buffer[offset++] = cfg->CMD_NAME_TAG;
        if (!encode_length_delimited((const uint8_t*)cmd_name, (uint32_t)name_len, buffer, buffer_size, &offset)) {
            return false;
        }
    }

    // PAYLOAD_TAG
    if (payload && payload_len > 0) {
        if (offset + 1 > buffer_size) return false;
        buffer[offset++] = cfg->PAYLOAD_TAG;
        if (!encode_length_delimited(payload, payload_len, buffer, buffer_size, &offset)) {
            return false;
        }
    }

    *encoded_size = offset;
    return true;
}

// Helper function to decode a field value from the buffer
static bool decode_field_value(FieldType type, uint8_t* value, const uint8_t* buffer, uint32_t buffer_size, uint32_t* offset) {
    switch (type) {
        case TYPE_UINT8:
            if (*offset + 1 > buffer_size) return false;
            *value = buffer[(*offset)++];
            break;

        case TYPE_UINT16:
            if (*offset + 2 > buffer_size) return false;
            *((uint16_t*)value) = ((uint16_t)buffer[(*offset)++] << 8) | buffer[(*offset)++];
            break;

        case TYPE_UINT32:
            if (*offset + 4 > buffer_size) return false;
            *((uint32_t*)value) = ((uint32_t)buffer[(*offset)++] << 24) |
                                 ((uint32_t)buffer[(*offset)++] << 16) |
                                 ((uint32_t)buffer[(*offset)++] << 8) |
                                 buffer[(*offset)++];
            break;

        case TYPE_UINT64:
            if (*offset + 8 > buffer_size) return false;
            *((uint64_t*)value) = ((uint64_t)buffer[(*offset)++] << 56) |
                                 ((uint64_t)buffer[(*offset)++] << 48) |
                                 ((uint64_t)buffer[(*offset)++] << 40) |
                                 ((uint64_t)buffer[(*offset)++] << 32) |
                                 ((uint64_t)buffer[(*offset)++] << 24) |
                                 ((uint64_t)buffer[(*offset)++] << 16) |
                                 ((uint64_t)buffer[(*offset)++] << 8) |
                                 buffer[(*offset)++];
            break;

        case TYPE_INT8:
            if (*offset + 1 > buffer_size) return false;
            *((int8_t*)value) = (int8_t)buffer[(*offset)++];
            break;

        case TYPE_INT16:
            if (*offset + 2 > buffer_size) return false;
            *((int16_t*)value) = ((int16_t)buffer[(*offset)++] << 8) | buffer[(*offset)++];
            break;

        case TYPE_INT32:
            if (*offset + 4 > buffer_size) return false;
            *((int32_t*)value) = ((int32_t)buffer[(*offset)++] << 24) |
                                ((int32_t)buffer[(*offset)++] << 16) |
                                ((int32_t)buffer[(*offset)++] << 8) |
                                buffer[(*offset)++];
            break;

        case TYPE_INT64:
            if (*offset + 8 > buffer_size) return false;
            *((int64_t*)value) = ((int64_t)buffer[(*offset)++] << 56) |
                                ((int64_t)buffer[(*offset)++] << 48) |
                                ((int64_t)buffer[(*offset)++] << 40) |
                                ((int64_t)buffer[(*offset)++] << 32) |
                                ((int64_t)buffer[(*offset)++] << 24) |
                                ((int64_t)buffer[(*offset)++] << 16) |
                                ((int64_t)buffer[(*offset)++] << 8) |
                                buffer[(*offset)++];
            break;

        case TYPE_FLOAT:
            if (*offset + 4 > buffer_size) return false;
            memcpy(value, buffer + *offset, 4);
            *offset += 4;
            break;

        case TYPE_DOUBLE:
            if (*offset + 8 > buffer_size) return false;
            memcpy(value, buffer + *offset, 8);
            *offset += 8;
            break;

        case TYPE_BOOL:
            if (*offset + 1 > buffer_size) return false;
            *((bool*)value) = buffer[(*offset)++] != 0;
            break;

        case TYPE_STRING:
            if (*offset + 1 > buffer_size) return false;
            {
                uint8_t len = buffer[(*offset)++];
                if (*offset + len > buffer_size) return false;
                char* str = malloc(len + 1);
                if (str == NULL) return false;
                memcpy(str, buffer + *offset, len);
                str[len] = '\0';
                *((char**)value) = str;
                *offset += len;
            }
            break;

        case TYPE_BYTES:
            if (*offset + 4 > buffer_size) return false;
            {
                uint32_t len = ((uint32_t)buffer[(*offset)++] << 24) |
                              ((uint32_t)buffer[(*offset)++] << 16) |
                              ((uint32_t)buffer[(*offset)++] << 8) |
                              buffer[(*offset)++];
                if (*offset + len > buffer_size) return false;
                uint8_t* bytes = malloc(len);
                if (bytes == NULL) return false;
                memcpy(bytes, buffer + *offset, len);
                *((uint8_t**)value) = bytes;
                *((uint32_t*)(value + sizeof(uint8_t*))) = len;
                *offset += len;
            }
            break;

        default:
            return false;
    }

    return true;
}
bool decode_protocol_frame(const uint8_t *data,
                           uint32_t data_size,
                           uint32_t *offset,
                           uint32_t *cmd_id,
                           uint8_t *payload_buf,
                           uint32_t payload_buf_size,
                           uint32_t *payload_len_out,
                           const ProtocolDefinition *protocol,
                           Command **cmd)
{
    if (!data || !offset || !cmd_id || !payload_buf || !payload_len_out)
    {
        return false;
    }

    uint32_t pos = *offset;
    if (pos >= data_size)
        return false;

    // === 解析 cmd_id ===
    if (data[pos++] != current_frame_cfg.CMD_ID_TAG)
    { // tag for field_num=1 (CMD_ID_TAG = 1 << 3 | 0)
        return false;
    }

    // Decode varint cmd_id
    *cmd_id = 0;
    int shift = 0;
    while (true)
    {
        if (pos >= data_size)
            return false;
        uint8_t byte = data[pos++];
        *cmd_id |= (byte & 0x7F) << shift;
        shift += 7;
        if (!(byte & 0x80))
            break;
    }

    // find command definition
    const CommandDefinition *cmd_def = find_command_by_id(protocol, *cmd_id);
    if (cmd_def == NULL)
        return false;

    Command *cmd_tmp = create_command(cmd_def);
    if (cmd_tmp == NULL)
        return false;

    // === 区分 request / response ===
    bool is_response = false;
    if (pos < data_size)
    {
        uint8_t peek_tag = data[pos];
        if (peek_tag == current_frame_cfg.SUCCESS_TAG)
        { // SUCCESS_TAG
            is_response = true;
            cmd_tmp->is_response = true;
        }
        else if (peek_tag == current_frame_cfg.CMD_NAME_TAG)
        { // CMD_NAME_TAG
            is_response = false;
            cmd_tmp->is_response = false;
        }
        else
        {
            return false; // 格式不对
        }
    }
    *cmd = cmd_tmp;
    // === 继续解析字段 ===
    while (pos < data_size)
    {
        uint8_t tag = data[pos++];
        uint8_t field_num = tag >> 3;
        uint8_t wire_type = tag & 0x07;

        if (!is_response)
        {
            // -------- Request 帧 --------
            switch (field_num)
            {
            case 2:
            { // cmd_name
                if (wire_type != 2)
                    return false;
                // 读取长度（varint）
                uint32_t len = 0;
                shift = 0;
                while (true)
                {
                    if (pos >= data_size)
                        return false;
                    uint8_t byte = data[pos++];
                    len |= (byte & 0x7F) << shift;
                    shift += 7;
                    if (!(byte & 0x80))
                        break;
                }

                if (pos + len > data_size)
                    return false;
                pos += len; // 跳过 cmd_name
                break;
            }
            case 3:
            { // payload
                if (wire_type != 2)
                    return false;

                // 读取 payload 长度
                uint32_t len = 0;
                shift = 0;
                while (true)
                {
                    if (pos >= data_size)
                        return false;
                    uint8_t byte = data[pos++];
                    len |= (byte & 0x7F) << shift;
                    shift += 7;
                    if (!(byte & 0x80))
                        break;
                }

                if (len > payload_buf_size || pos + len > data_size)
                    return false;

                memcpy(payload_buf, data + pos, len);
                *payload_len_out = len;
                pos += len;
                *offset = pos;
                return decode_command(payload_buf, len, cmd_def, cmd);
            }
            default:
            { // skip unknown field
                pos = skip_unknown_field(data,pos,data_size,wire_type);
                if (pos == 0) return false;
                break;
            }
            }
        }
        else
        {
            // -------- Response 帧 --------
            switch (field_num)
            {
            case 2:
            { // success
                if (wire_type != 0)
                    return false;
                if (pos >= data_size)
                    return false;
                uint8_t val = data[pos++];
                protocol_set_field(*cmd, 1, TYPE_BOOL, (FieldValue){.bool_val = val ? true : false});
                break;
            }
            case 3:
            { // response_data
                if (wire_type != 2)
                    return false;
                uint32_t len = 0;
                shift = 0;
                while (true)
                {
                    if (pos >= data_size)
                        return false;
                    uint8_t byte = data[pos++];
                    len |= (byte & 0x7F) << shift;
                    shift += 7;
                    if (!(byte & 0x80))
                        break;
                }
                if (len > payload_buf_size || pos + len > data_size)
                    return false;
                memcpy(payload_buf, data + pos, len);
                *payload_len_out = len;
                pos += len;
                *offset = pos;
                return decode_command(payload_buf, len, cmd_def, cmd);
            }
            default:
                pos = skip_unknown_field(data, pos, data_size, wire_type);
                if (pos == 0)
                    return false;
                break;
            }
        }
    }

    return false; // 没有 payload 字段则失败
}

bool decode_command(const uint8_t *data, uint32_t data_size, const CommandDefinition *cmd_def, Command **cmd)
{
    if (data == NULL || cmd_def == NULL || cmd == NULL)
        return false;
    printf("Payload (%u bytes):\n", data_size);
    for (uint32_t i = 0; i < data_size; ++i)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");

    printf("CMD_ID: %u\n", (*cmd)->id);
    printf("CMD_NAME: %s\n", (*cmd)->name);

    uint32_t offset = 0;
    int shift = 0;

    // Decode fields
    while (offset < data_size) {
        if (offset + 1 > data_size) {
            free_command(*cmd);
            *cmd = NULL;
            return false;
        }

        uint8_t tag = data[offset++];
        uint8_t field_num = tag >> 3;
        uint8_t wire_type = tag & 0x07;
        uint16_t field_index;
        
        // Find field definition
        const FieldDefinition* field_def = NULL;
        for (uint16_t i = 0; i < cmd_def->request_fields_count; i++) {
            if (cmd_def->request_fields[i].field_num == field_num) {
                field_def = &cmd_def->request_fields[i];
                field_index = i;
                break;
            }
        }

        if (field_def == NULL) {
            // Skip unknown field
            switch (wire_type) {
                case 0: // varint
                    while (offset < data_size && (data[offset++] & 0x80));
                    break;
                case 2: // length-delimited
                    if (offset + 1 > data_size) {
                        free_command(*cmd);
                        *cmd = NULL;
                        return false;
                    }
                    {
                        uint8_t len = data[offset++];
                        if (offset + len > data_size) {
                            free_command(*cmd);
                            *cmd = NULL;
                            return false;
                        }
                        offset += len;
                    }
                    break;
                case 5: // 32-bit
                    if (offset + 4 > data_size) {
                        free_command(*cmd);
                        *cmd = NULL;
                        return false;
                    }
                    offset += 4;
                    break;
                default:
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
            }
            continue;
        }

        // Verify wire type matches expected type
        uint8_t expected_wire_type;
        switch (field_def->type) {
            case TYPE_UINT8:
            case TYPE_UINT16:
            case TYPE_UINT32:
            case TYPE_UINT64:
            case TYPE_INT8:
            case TYPE_INT16:
            case TYPE_INT32:
            case TYPE_INT64:
            case TYPE_BOOL:
                expected_wire_type = 0; // varint
                break;
            case TYPE_FLOAT:
                expected_wire_type = 5; // 32-bit
                break;
            case TYPE_DOUBLE:
                expected_wire_type = 1; // 64-bit
                break;
            case TYPE_STRING:
            case TYPE_BYTES:
                expected_wire_type = 2; // length-delimited
                break;
            default:
                free_command(*cmd);
                *cmd = NULL;
                return false;
        }

        if (wire_type != expected_wire_type) {
            free_command(*cmd);
            *cmd = NULL;
            return false;
        }

        // Decode field value
        switch (field_def->type) {
            case TYPE_UINT8:{
                if (offset + 1 > data_size)
                {
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                } 
                uint8_t val8 = data[offset];
                offset += 1;
                (*cmd)->fields[field_index].value.uint8_val = val8;
                printf("uint8_val: %d\n",val8);
                break;
            }
            case TYPE_UINT16:{
                if (offset + 2 > data_size){
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                } 
                uint16_t val16 = (data[offset] << 8) | data[offset + 1];
                offset += 2;
                (*cmd)->fields[field_index].value.uint16_val = val16;
                printf("uint16: %d\n", val16);
                break;
            }
            case TYPE_UINT32:{
                if (offset + 4 > data_size){
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                }
                uint32_t val32 = 0;
                for (int i=0;i<4;i++){
                    val32 = (val32 << 8) | data[offset + i];
                }
                offset += 4;
                (*cmd)->fields[field_index].value.uint32_val = val32;
                printf("uint32: %d\n", val32);
                break;
            }
            case TYPE_UINT64: {
                if (offset + 8 > data_size){
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                }
                uint64_t val64 = 0;
                for (int i=0;i<8;i++){
                    val64 = (val64 << 8) | data[offset + i];
                }
                offset += 8;
                (*cmd)->fields[field_index].value.uint64_val = val64;
                printf("uint64_t: %d\n", val64);
                break;
            }
            case TYPE_INT8:{
                if (offset + 1 > data_size)
                {
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                }
                int8_t val8 = (int8_t)data[offset];
                offset += 1;
                (*cmd)->fields[field_index].value.int8_val = val8;
                printf("int8_t:%d\n",val8);
                break;
            }
            case TYPE_INT16:{
                if (offset + 2 > data_size){
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                } 
                int16_t val16 = (data[offset] << 8) | data[offset + 1];
                offset += 2;
                (*cmd)->fields[field_index].value.int16_val = val16;
                printf("int16_t:%d\n",val16);
                break;
            }
            case TYPE_INT32:{
                if (offset + 4 > data_size){
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                }
                int32_t val32 = 0;
                for (int i=0;i<4;i++){
                    val32 = (val32 << 8) | data[offset + i];
                }
                offset += 4;
                (*cmd)->fields[field_index].value.int32_val = val32;
                printf("int32_t:%d\n",val32);
                break;
            }
            case TYPE_INT64: {
                if (offset + 8 > data_size){
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                }
                int64_t val64 = 0;
                for (int i=0;i<8;i++){
                    val64 = (val64 << 8) | data[offset + i];
                }
                offset += 8;
                (*cmd)->fields[field_index].value.uint64_val = val64;
                printf("int64_t:%d\n",val64);
                break;
            }
            case TYPE_BOOL: {
                uint64_t value = 0;
                shift = 0;
                while (true) {
                    if (offset > data_size) {
                        free_command(*cmd);
                        *cmd = NULL;
                        return false;
                    }
                    uint8_t byte = data[offset++];
                    value |= (byte & 0x7F) << shift;
                    shift += 7;
                    if (!(byte & 0x80)) break;
                }
                (*cmd)->fields[field_index].value.bool_val = (bool)value;
                printf("bool: %s\n", (*cmd)->fields[field_index].value.bool_val ? "true" : "false");
                break;
            }
            case TYPE_FLOAT: {
                if (offset + 4 > data_size) {
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                }
                uint32_t bits;
                memcpy(&bits, data + offset, 4);
                offset += 4;
                float value;
                memcpy(&value, &bits, 4);
                (*cmd)->fields[field_index].value.float_val = value;
                printf("float:%.2f\n", value);
                break;
            }
            case TYPE_DOUBLE: {
                if (offset + 8 > data_size) {
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                }
                uint64_t bits;
                memcpy(&bits, data + offset, 8);
                offset += 8;
                double value;
                memcpy(&value, &bits, 8);
                (*cmd)->fields[field_index].value.double_val = value;
                printf("double:%.2f\n",value);
                break;
            }
            case TYPE_STRING:
            case TYPE_BYTES: {
                // Decode length
                uint32_t len = 0;
                if (offset + 4 > data_size) {
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                }
                for (int i = 0; i < 4; i++)
                {
                    len = (len << 8) | data[offset + i];
                }
                offset += 4;
                if (offset + len > data_size) {
                    free_command(*cmd);
                    *cmd = NULL;
                    return false;
                }
                // Allocate memory for string/bytes
                if (field_def->type == TYPE_STRING)
                {
                    (*cmd)->fields[field_index].value.string_val = malloc(len + 1);
                    memcpy((*cmd)->fields[field_index].value.string_val, data + offset, len);
                    (*cmd)->fields[field_index].value.string_val[len] = '\0';
                }
                else if (field_def->type == TYPE_BYTES)
                {
                    (*cmd)->fields[field_index].value.bytes_val.data = malloc(len);
                    (*cmd)->fields[field_index].value.bytes_val.length = len;
                    memcpy((*cmd)->fields[field_index].value.bytes_val.data, data + offset, len);
                }
                offset += len;
                break;
            }
            default:
                free_command(*cmd);
                *cmd = NULL;
                return false;
        }
    }

    return true;
}
