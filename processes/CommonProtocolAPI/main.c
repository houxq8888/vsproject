/*
 * ProtocolInterface.c
 *
 *  Created on: Jul 7, 2025
 *      Author: xiaoqin.hou
 */
#include "ProtocolInterface.h"
#include <stdio.h>

void main() {
    // init
    ProtocolInterface_Init();
    // uart_init();
    
    // Example: Send  command
    {
        printf("Example1:\n");
        uint8_t tx_buffer[256];
        uint32_t tx_size = sizeof(tx_buffer);

        getSendCommand(1, CMD_READ, NULL, 0, tx_buffer, &tx_size);
    }
    {
        printf("Example2:\n");
        uint8_t tx_buffer[256];
        uint32_t tx_size = sizeof(tx_buffer);

        FieldInput writeFields[3];
        // end_potential
        writeFields[0].field_num = 1;
        writeFields[0].type = TYPE_UINT16;
        writeFields[0].value.uint16_val = 123;
        // end_drift
        writeFields[1].field_num = 2;
        writeFields[1].type = TYPE_UINT8;
        writeFields[1].value.uint8_val = 45;
        // start_drift
        writeFields[2].field_num = 3;
        writeFields[2].type = TYPE_UINT8;
        writeFields[2].value.uint8_val = 3;
        // end_delay
        writeFields[3].field_num = 4;
        writeFields[3].type = TYPE_UINT8;
        writeFields[3].value.uint8_val = 4;
        // calibration_factor
        writeFields[4].field_num = 5;
        writeFields[4].type = TYPE_UINT8;
        writeFields[4].value.uint8_val = 5;
        // min_duration
        writeFields[5].field_num = 6;
        writeFields[5].type = TYPE_UINT8;
        writeFields[5].value.uint8_val = 6;
        // max_duration
        writeFields[6].field_num = 7;
        writeFields[6].type = TYPE_UINT8;
        writeFields[6].value.uint8_val = 7;
        // extraction_time
        writeFields[7].field_num = 8;
        writeFields[7].type = TYPE_UINT8;
        writeFields[7].value.uint8_val = 8;
        // termination_time
        writeFields[8].field_num = 9;
        writeFields[8].type = TYPE_UINT8;
        writeFields[8].value.uint8_val = 9;
        // electrolytic_electrode
        writeFields[9].field_num = 10;
        writeFields[9].type = TYPE_UINT8;
        writeFields[9].value.uint8_val = 10;
        // fan
        writeFields[10].field_num = 11;
        writeFields[10].type = TYPE_UINT8;
        writeFields[10].value.uint8_val = 11;
        // heater_tube
        writeFields[11].field_num = 12;
        writeFields[11].type = TYPE_UINT8;
        writeFields[11].value.uint8_val = 12;
        // initial_temperature
        writeFields[12].field_num = 13;
        writeFields[12].type = TYPE_UINT8;
        writeFields[12].value.uint8_val = 13;
        // constant_temperature_time
        writeFields[13].field_num = 14;
        writeFields[13].type = TYPE_UINT8;
        writeFields[13].value.uint8_val = 123;
        // program_heating
        writeFields[14].field_num = 15;
        writeFields[14].type = TYPE_UINT8;
        writeFields[14].value.uint8_val = 45;
        // rate1
        writeFields[15].field_num = 16;
        writeFields[15].type = TYPE_UINT8;
        writeFields[15].value.uint8_val = 3;
        // rate2
        writeFields[16].field_num = 17;
        writeFields[16].type = TYPE_UINT8;
        writeFields[16].value.uint8_val = 4;
        // rate3
        writeFields[17].field_num = 18;
        writeFields[17].type = TYPE_UINT8;
        writeFields[17].value.uint8_val = 3;
        // temperature1
        writeFields[18].field_num = 19;
        writeFields[18].type = TYPE_UINT8;
        writeFields[18].value.uint8_val = 3;
        // temperature2
        writeFields[19].field_num = 20;
        writeFields[19].type = TYPE_UINT8;
        writeFields[19].value.uint8_val = 3;
        // temperature3
        writeFields[20].field_num = 21;
        writeFields[20].type = TYPE_UINT8;
        writeFields[20].value.uint8_val = 3;
        // keep_warm1
        writeFields[21].field_num = 22;
        writeFields[21].type = TYPE_UINT8;
        writeFields[21].value.uint8_val = 3;
        // keep_warm2
        writeFields[22].field_num = 23;
        writeFields[22].type = TYPE_UINT8;
        writeFields[22].value.uint8_val = 3;
        // keep_warm3
        writeFields[23].field_num = 24;
        writeFields[23].type = TYPE_UINT8;
        writeFields[23].value.uint8_val = 3;

        getSendCommand(2, CMD_WRITE, writeFields, 24, tx_buffer, &tx_size);
    }

    {
        uint8_t tx_buffer[256];
        uint32_t tx_size = sizeof(tx_buffer);
        uint8_t receive_data1[]={0xEE, 0xFF, 0x43, 0x00, 0x00, 0x00, 0x08, 0x02, 0x12, 0x0C, 0x53, 0x65, 0x74, 0x50, 0x61, 0x72, 
                                 0x61, 0x6D, 0x65, 0x74, 0x65, 0x72, 0x1A, 0x31, 0x08, 0x00, 0x7B, 0x10, 0x2D, 0x18, 0x03, 0x20, 
                                 0x04, 0x28, 0x05, 0x30, 0x06, 0x38, 0x07, 0x40, 0x08, 0x48, 0x09, 0x50, 0x0A, 0x58, 0x0B, 0x60, 
                                 0x0C, 0x68, 0x0D, 0x70, 0x7B, 0x78, 0x2D, 0x80, 0x03, 0x88, 0x04, 0x90, 0x03, 0x98, 0x03, 0xA0, 
                                 0x03, 0xA8, 0x03, 0xB0, 0x03, 0xB8, 0x03, 0xC0, 0x03, 0x67, 0x67, 0xFF, 0xFC, 0xFF, 0xFF};
        printf("receive_data1:\n");
        Command* cmd = getReceiveCommand(receive_data1, sizeof(receive_data1), tx_buffer, &tx_size);

        uint8_t receive_data2[]= {
            0xEE, 0xFF, 0x0A, 0x00, 0x00, 0x00, 0x08, 0x02, 0x10, 0x01, 0x1A, 0x04, 0x08, 0x01, 0x10, 0x00, 
            0xE2, 0x8A, 0xFF, 0xFC, 0xFF, 0xFF
        };
        printf("receive_data2:\n");
        Command* cmd2 = getReceiveCommand(receive_data2, sizeof(receive_data2), tx_buffer, &tx_size);

        // received decode
        uint8_t receive_data[] = {0xEE, 0xFF, 0x5B, 0x00, 0x00, 0x00, 0x08, 0x02, 0x12, 0x0C, 0x53, 0x65, 0x74, 0x50, 0x61, 0x72,
                                  0x61, 0x6D, 0x65, 0x74, 0x65, 0x72, 0x1A, 0x49, 0x08, 0x00, 0x01, 0x15, 0x00, 0x00, 0x00, 0x40,
                                  0x18, 0x03, 0x20, 0x00, 0x00, 0x00, 0x04, 0x28, 0x01, 0x30, 0x06, 0x38, 0x00, 0x07, 0x40, 0x00,
                                  0x00, 0x00, 0x08, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x51, 0x7B, 0x14, 0xAE,
                                  0x47, 0xE1, 0x7A, 0x0C, 0x40, 0x5D, 0x60, 0xE5, 0x38, 0x40, 0x62, 0x00, 0x00, 0x00, 0x0B, 0x68,
                                  0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x6A, 0x00, 0x00, 0x00, 0x02, 0x68,
                                  0x69, 0xCC, 0x03, 0xFF, 0xFC, 0xFF, 0xFF}; //
        printf("received_data3:\n");
        Command *cmd1 = getReceiveCommand(receive_data, sizeof(receive_data), tx_buffer, &tx_size);
    }
    // Clean up
    ProtocolInterface_deInit();
}