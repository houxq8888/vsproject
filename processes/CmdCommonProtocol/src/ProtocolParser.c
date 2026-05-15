/*
 * ProtocolParser.c
 *
 *  Created on: Jul 7, 2025
 *      Author: xiaoqin.hou
 */

#include "ProtocolParser.h"
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

FieldType string_to_field_type(const char* type_str) {
    if (strcmp(type_str, "uint8") == 0) return TYPE_UINT8;
    if (strcmp(type_str, "uint16") == 0) return TYPE_UINT16;
    if (strcmp(type_str, "uint32") == 0) return TYPE_UINT32;
    if (strcmp(type_str, "uint64") == 0) return TYPE_UINT64;
    if (strcmp(type_str, "int8") == 0) return TYPE_INT8;
    if (strcmp(type_str, "int16") == 0) return TYPE_INT16;
    if (strcmp(type_str, "int32") == 0) return TYPE_INT32;
    if (strcmp(type_str, "int64") == 0) return TYPE_INT64;
    if (strcmp(type_str, "float") == 0 || strcmp(type_str, "float32") == 0) return TYPE_FLOAT;
    if (strcmp(type_str, "double") == 0) return TYPE_DOUBLE;
    if (strcmp(type_str, "bool") == 0) return TYPE_BOOL;
    if (strcmp(type_str, "string") == 0) return TYPE_STRING;
    if (strcmp(type_str, "bytes") == 0) return TYPE_BYTES;
    return TYPE_UNKNOWN;
}

static void parse_field(cJSON* field_json, FieldDefinition* field) {
    cJSON* item;

    item = cJSON_GetObjectItemCaseSensitive(field_json, "name");
    if (cJSON_IsString(item)) {
        field->name = strdup(item->valuestring);
    }

    item = cJSON_GetObjectItemCaseSensitive(field_json, "type");
    if (cJSON_IsString(item)) {
        field->type = string_to_field_type(item->valuestring);
    }

    item = cJSON_GetObjectItemCaseSensitive(field_json, "field_num");
    if (cJSON_IsNumber(item)) {
        field->field_num = (uint8_t)item->valueint;
    }

    item = cJSON_GetObjectItemCaseSensitive(field_json, "description");
    if (cJSON_IsString(item)) {
        field->description = strdup(item->valuestring);
    }

    item = cJSON_GetObjectItemCaseSensitive(field_json, "default");
    if (item != NULL) {
        field->has_default = true;
        switch (field->type) {
            case TYPE_UINT8: field->default_value.uint8_val = (uint8_t)item->valueint; break;
            case TYPE_UINT16: field->default_value.uint16_val = (uint16_t)item->valueint; break;
            case TYPE_UINT32: field->default_value.uint32_val = (uint32_t)item->valueint; break;
            case TYPE_UINT64: field->default_value.uint64_val = (uint64_t)item->valueint; break;
            case TYPE_INT8: field->default_value.int8_val = (int8_t)item->valueint; break;
            case TYPE_INT16: field->default_value.int16_val = (int16_t)item->valueint; break;
            case TYPE_INT32: field->default_value.int32_val = (int32_t)item->valueint; break;
            case TYPE_INT64: field->default_value.int64_val = (int64_t)item->valueint; break;
            case TYPE_FLOAT: field->default_value.float_val = (float)item->valuedouble; break;
            case TYPE_DOUBLE: field->default_value.double_val = item->valuedouble; break;
            case TYPE_BOOL: field->default_value.bool_val = cJSON_IsTrue(item); break;
            case TYPE_STRING:
                if (cJSON_IsString(item)) {
                    field->default_value.string_val = strdup(item->valuestring);
                }
                break;
            default: break;
        }
    } else {
        field->has_default = false;
    }
}

static void parse_metadata(cJSON* metadata_json, CommandMetadata* metadata) {
    cJSON* item = cJSON_GetObjectItemCaseSensitive(metadata_json, "require_ack");
    if (cJSON_IsBool(item)) {
        metadata->require_ack = cJSON_IsTrue(item);
    }

    item = cJSON_GetObjectItemCaseSensitive(metadata_json, "timeout_ms");
    if (cJSON_IsNumber(item)) {
        metadata->timeout_ms = (uint32_t)item->valueint;
    }
}

static void parse_command(cJSON* command_json, CommandDefinition* command) {
    cJSON* item;

    item = cJSON_GetObjectItemCaseSensitive(command_json, "id");
    if (cJSON_IsNumber(item)) {
        command->id = (uint32_t)item->valueint;
    }

    item = cJSON_GetObjectItemCaseSensitive(command_json, "name");
    if (cJSON_IsString(item)) {
        command->name = strdup(item->valuestring);
    }

    item = cJSON_GetObjectItemCaseSensitive(command_json, "metadata");
    if (cJSON_IsObject(item)) {
        parse_metadata(item, &command->metadata);
    }

    // Parse request fields
    item = cJSON_GetObjectItemCaseSensitive(command_json, "request_fields");
    if (cJSON_IsArray(item)) {
        command->request_fields_count = (uint16_t)cJSON_GetArraySize(item);
        command->request_fields = malloc(command->request_fields_count * sizeof(FieldDefinition));

        for (uint16_t i = 0; i < command->request_fields_count; i++) {
            cJSON* field_json = cJSON_GetArrayItem(item, i);
            parse_field(field_json, &command->request_fields[i]);
        }
    }

    // Parse response fields
    item = cJSON_GetObjectItemCaseSensitive(command_json, "response_fields");
    if (cJSON_IsArray(item)) {
        command->response_fields_count = (uint16_t)cJSON_GetArraySize(item);
        command->response_fields = malloc(command->response_fields_count * sizeof(FieldDefinition));

        for (uint16_t i = 0; i < command->response_fields_count; i++) {
            cJSON* field_json = cJSON_GetArrayItem(item, i);
            parse_field(field_json, &command->response_fields[i]);
        }
    }
}

ProtocolDefinition* parse_protocol(const char* json_str) {
    if (json_str == NULL) return NULL;

    // 2. 解析 JSON
    cJSON* root = cJSON_Parse(json_str);
    if (!root) {
        const char* error_ptr = cJSON_GetErrorPtr();
        printf("JSON parsing error before: %s\n", error_ptr);
        printf("Problematic JSON snippet:\n%.200s\n", error_ptr ? error_ptr : "Unknown");
        return;
    }

    ProtocolDefinition* protocol = malloc(sizeof(ProtocolDefinition));
    if (protocol == NULL) {
        cJSON_Delete(root);
        return NULL;
    }
    memset(protocol, 0, sizeof(ProtocolDefinition));

    cJSON* item = cJSON_GetObjectItemCaseSensitive(root, "protocol_version");
    if (cJSON_IsString(item)) {
        protocol->version = strdup(item->valuestring);
    }

    item = cJSON_GetObjectItemCaseSensitive(root, "commands");
    if (cJSON_IsArray(item)) {
        protocol->command_count = (uint16_t)cJSON_GetArraySize(item);
        protocol->commands = malloc(protocol->command_count * sizeof(CommandDefinition));

        for (uint16_t i = 0; i < protocol->command_count; i++) {
            cJSON* command_json = cJSON_GetArrayItem(item, i);
            parse_command(command_json, &protocol->commands[i]);
        }
    }

    cJSON_Delete(root);
    return protocol;
}

void free_protocol(ProtocolDefinition* protocol) {
    if (protocol == NULL) return;

    free(protocol->version);

    for (uint16_t i = 0; i < protocol->command_count; i++) {
        CommandDefinition* cmd = &protocol->commands[i];
        free(cmd->name);

        for (uint16_t j = 0; j < cmd->request_fields_count; j++) {
            free(cmd->request_fields[j].name);
            free(cmd->request_fields[j].description);
            if (cmd->request_fields[j].type == TYPE_STRING && cmd->request_fields[j].has_default) {
                free(cmd->request_fields[j].default_value.string_val);
            }
        }
        free(cmd->request_fields);

        for (uint16_t j = 0; j < cmd->response_fields_count; j++) {
            free(cmd->response_fields[j].name);
            free(cmd->response_fields[j].description);
            if (cmd->response_fields[j].type == TYPE_STRING && cmd->response_fields[j].has_default) {
                free(cmd->response_fields[j].default_value.string_val);
            }
        }
        free(cmd->response_fields);
    }

    free(protocol->commands);
    free(protocol);
}

const CommandDefinition* find_command_by_id(const ProtocolDefinition* protocol, uint32_t cmd_id) {
    if (protocol == NULL) return NULL;

    for (uint16_t i = 0; i < protocol->command_count; i++) {
        if (protocol->commands[i].id == cmd_id) {
            return &protocol->commands[i];
        }
    }

    return NULL;
}
