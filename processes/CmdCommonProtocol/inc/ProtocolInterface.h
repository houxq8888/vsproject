/*
 * ProtocolInterface.h
 *
 *  Created on: Jul 7, 2025
 *      Author: xiaoqin.hou
 */

#ifndef INC_PROTOCOLINTERFACE_H_
#define INC_PROTOCOLINTERFACE_H_

/*!
\mainpage CommonCommunication 文档

## 📌 项目简介
CommonCommunication 是一个通用通讯框架，支持 cJson 驱动，具备通用通讯数据收发编解码等特性。

---

## 📚 文档版本说明

| 版本号   | 日期       | 作者      | 说明                                                         |
|--------|------------|----------|--------------------------------------------------------------|
| v1.0.0 | 2025-08-14 | Xiaoqin Hou | 🎉 初始版本。生成了基础的 API 文档，涵盖全部 .h/.c 接口说明 |


---
*/

#ifdef __cplusplus
extern "C"
{
#endif


#include <stdint.h>
#include "ProtocolCommand.h"

typedef enum {
    CMD_READ = 0,
    CMD_WRITE = 1
} CommandMode;

typedef struct {
    uint32_t field_num;   // 字段号
    FieldType type;       // 数据类型，比如 TYPE_UINT8, TYPE_STRING
    FieldValue value;     // 实际的值
} FieldInput;


/**
 * @brief 初始化协议接口模块。
 *
 * 该函数完成协议接口相关资源的初始化工作，例如内存分配、
 * 缓冲区清零、通信参数设置等。调用此函数后，才能使用
 * 发送和接收命令的相关接口。
 */
void ProtocolInterface_Init(void);

/**
 * @brief 生成待发送的协议命令数据。
 *
 * 根据指定的命令 ID、模式和输入字段，打包生成完整的
 * 协议发送缓冲区内容。
 *
 * @param[in]  cmd_id      命令 ID。
 * @param[in]  mode        命令模式（发送/接收）。
 * @param[in]  fields      输入字段数组指针。
 * @param[in]  field_count 输入字段数量。
 * @param[out] tx_buffer   输出的发送缓冲区指针。
 * @param[out] tx_size     输出的发送数据长度（字节）。
 */
void getSendCommand(uint32_t cmd_id, 
                    CommandMode mode, 
                    const FieldInput *fields, 
                    uint32_t field_count,
                    uint8_t *tx_buffer, 
                    uint32_t *tx_size);
/**
 * @brief 解析接收到的协议命令数据。
 *
 * 从接收缓冲区中解析出完整的命令结构体，并根据需要生成
 * 应答数据。
 *
 * @param[in]  rx_buffer   接收缓冲区指针。
 * @param[in]  rx_size     接收数据长度（字节）。
 * @param[out] tx_buffer   应答数据输出缓冲区指针（可选）。
 * @param[out] tx_size     应答数据长度输出（字节，可选）。
 *
 * @return 返回解析出的命令结构体指针；如解析失败，返回 NULL。
 */
Command* getReceiveCommand(uint8_t *rx_buffer, uint32_t *rx_size, uint8_t *tx_buffer, 
                    uint32_t *tx_size);

/**
 * @brief 反初始化协议接口模块。
 *
 * 释放协议接口使用的资源，清理内部状态。
 * 调用此函数后，不应再使用协议收发相关接口。
 */
void ProtocolInterface_deInit(void);


#ifdef __cplusplus
}
#endif

#endif