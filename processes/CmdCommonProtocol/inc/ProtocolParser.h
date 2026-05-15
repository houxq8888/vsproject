/*
 * ProtocolParser.h
 *
 *  Created on: Jul 7, 2025
 *      Author: xiaoqin.hou
 */

#ifndef INC_PROTOCOLPARSER_H_
#define INC_PROTOCOLPARSER_H_


#include "cJSON.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_UINT64,
    TYPE_INT8,
    TYPE_INT16,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_BYTES,
    TYPE_UNKNOWN
} FieldType;

typedef struct {
    char* name;
    FieldType type;
    uint8_t field_num;
    char* description;
    bool has_default;
    union {
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
    } default_value;
} FieldDefinition;

typedef struct {
    bool require_ack;
    uint32_t timeout_ms;
} CommandMetadata;

typedef struct {
    uint32_t id;
    char* name;
    CommandMetadata metadata;
    FieldDefinition* request_fields;
    uint16_t request_fields_count;
    FieldDefinition* response_fields;
    uint16_t response_fields_count;
} CommandDefinition;

typedef struct {
    char* version;
    CommandDefinition* commands;
    uint16_t command_count;
} ProtocolDefinition;

// Function prototypes
ProtocolDefinition* parse_protocol(const char* json_str);
void free_protocol(ProtocolDefinition* protocol);
const CommandDefinition* find_command_by_id(const ProtocolDefinition* protocol, uint32_t cmd_id);
FieldType string_to_field_type(const char* type_str);


#endif /* INC_PROTOCOLPARSER_H_ */
