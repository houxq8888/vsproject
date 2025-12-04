/*
    @ 
    @file   structUpperPcToLowerPC.cpp
    @brief  Implementation file
    @author xiaoqin.hou
    @date   2025/04/16
    @version 1.0.0
*/
#include "structUpperPcToLowerPC.h"
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include <sstream>

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

#define sizeofSwirl (1+1+2+1+1+2)
#define sizeofInjectSample (1+2+2+1+2+1+1+2)
#define sizeofDetectSample (1+2+2+2+4+1+2+2+1+2+1+1+2)
#define sizeofGetSample (1+2+2+1+2+1+1+2)
#define sizeofBalance (1+2+1+1+2)

/**
 * ***********
 * @Brief Single byte data inversion
 * @param
 *      @Desbuf:destination buffer
 *      @Srcbuf:source buffer
 * @retval None
 * @note (MSB)0101_0101----> 1010_1010(LSB)
 */
void InvertUint8(unsigned char *DesBuf,unsigned char *SrcBuf){
    int i;
    unsigned char temp=0;
    for (i=0;i<8;i++){
        if (SrcBuf[0] & (1 <<i)){
            temp=temp|(1<<(7-i));
        }
    }
    DesBuf[0]=temp;
}
/**
 * ***********
 * @brief double byte data inversion
 * @param
 *      @Desbuf:destination buffer
 *      @Srcbuf:source buffer
 * @retval None
 * @note (MSB)0101_0101_1010_1010 --> 0101_0101_1010_1010 (LSB)
 */
void InverUint16(unsigned short *DesBuf,unsigned short *SrcBuf){
    int i;
    unsigned short temp=0;
    for (i=0;i<16;i++){
        if (SrcBuf[0] & (1 <<i)){
            temp=temp|(1<<(15-i));
        }
    }
    DesBuf[0]=temp;
}
unsigned short CRC16_MODBUS(unsigned char *puchMsg,unsigned int usDataLen){
    unsigned short wCRCin=0xFFFF;
    unsigned short wCPoly=0x8005;
    unsigned char wChar=0;
    while (usDataLen--){
        wChar=*(puchMsg++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^=(wChar<<8);
        for (int i=0;i<8;i++){
            if (wCRCin & 0x8000){
                wCRCin=(wCRCin<<1)^wCPoly;
            } else {
                wCRCin=wCRCin<<1;
            }
        }
    }
    InverUint16(&wCRCin,&wCRCin);
    return (wCRCin);
}
ResSerialize serializeSwirl(swirlStruct swirlS){
    ResSerialize resSerialize;
    uint32_t swirlStructSize = 1+1+2+1+1+2;
    resSerialize.count = swirlStructSize+4;
    int cnt = -1;
    if (SMALLER_SAVE)
    {
        // swirl
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&swirlStructSize + i));
        resSerialize.data[++cnt] = (swirlS.swirlSpeed);
        resSerialize.data[++cnt]=(swirlS.index);
        for (int i=0;i<2;i++)
            resSerialize.data[++cnt]=(*((uint8_t*)&swirlS.continueTime+i));
        resSerialize.data[++cnt]=(swirlS.CanId);
        resSerialize.data[++cnt]=(swirlS.curFaceTypeIndex);
        for (int i=0;i<2;i++)
            resSerialize.data[++cnt]=(*((uint8_t*)&swirlS.Type+i));
    }
    else
    {
        // swirl
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&swirlStructSize + i));
        resSerialize.data[++cnt] = (swirlS.swirlSpeed);
        resSerialize.data[++cnt]=(swirlS.index);
        for (int i=1;i>=0;i--)
            resSerialize.data[++cnt]=(*((uint8_t*)&swirlS.continueTime+i));
        resSerialize.data[++cnt]=(swirlS.CanId);
        resSerialize.data[++cnt]=(swirlS.curFaceTypeIndex);
        for (int i=1;i>=0;i--)
            resSerialize.data[++cnt]=(*((uint8_t*)&swirlS.Type+i));
    }
    return resSerialize;
}
swirlStruct deSerializeSwirl(ResSerialize contents){
    swirlStruct swirlS=initSwirlStruct();
    union Data32
    {
        uint8_t u8_32[4]; 
        uint32_t u32;
    };
    union DataFloat
    {
        uint8_t u8f[4];
        float f;
    };
    union DataDouble
    {
        uint8_t u8d[8]; 
        double d;
    };
    union Data16
    {
        uint8_t u8_16[2]; 
        uint16_t u16;
    };

    union Data32 data32;
    union DataFloat datafloat;
    union DataDouble dataDouble;
    union Data16 data16;

    if (SMALLER_SAVE){
        swirlS.swirlSpeed=contents.data[0];
        swirlS.index=contents.data[1];
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[2 + i];
        swirlS.continueTime = data16.u16;
        swirlS.CanId=contents.data[4];
        swirlS.curFaceTypeIndex=contents.data[5];
        for (int i =0 ; i<2;i++)
            data16.u8_16[i] = contents.data[6 + i];
        swirlS.Type=data16.u16;
    } else {
        swirlS.swirlSpeed=contents.data[0];
        swirlS.index=contents.data[1];
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[2 + i];
        swirlS.continueTime = data16.u16;
        swirlS.CanId=contents.data[4];
        swirlS.curFaceTypeIndex=contents.data[5];
        for (int i =0 ; i<2;i++)
            data16.u8_16[1-i] = contents.data[6 + i];
        swirlS.Type=data16.u16;
    }
    return swirlS;
}
ResSerialize serializeGetSample(getSampleStruct getSampleS){
    ResSerialize resSerialize;
    uint32_t getSampleStructSize = 1+2+2+1+2+1+1+2;
    resSerialize.count = getSampleStructSize+4;
    int cnt = -1;
    if (SMALLER_SAVE)
    {
        // GetSample
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleStructSize + i));
        resSerialize.data[++cnt] = (getSampleS.mode);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleS.speed + i));
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleS.time + i));
        resSerialize.data[++cnt] = (getSampleS.index);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleS.continueTime + i));
        resSerialize.data[++cnt] = (getSampleS.CanId);
        resSerialize.data[++cnt] = (getSampleS.curFaceTypeIndex);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleS.Type + i));
    }
    else
    {
        // GetSample
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleStructSize + i));
        resSerialize.data[++cnt] = (getSampleS.mode);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleS.speed + i));
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleS.time + i));
        resSerialize.data[++cnt] = (getSampleS.index);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleS.continueTime + i));
        resSerialize.data[++cnt] = (getSampleS.CanId);
        resSerialize.data[++cnt] = (getSampleS.curFaceTypeIndex);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleS.Type + i));
    }
    return resSerialize;
}
getSampleStruct deSerializeGetSample(ResSerialize contents){
    getSampleStruct getSampleS = initGetSampleStruct();
    union Data32
    {
        uint8_t u8_32[4]; 
        uint32_t u32;
    };
    union DataFloat
    {
        uint8_t u8f[4];
        float f;
    };
    union DataDouble
    {
        uint8_t u8d[8]; 
        double d;
    };
    union Data16
    {
        uint8_t u8_16[2]; 
        uint16_t u16;
    };

    union Data32 data32;
    union DataFloat datafloat;
    union DataDouble dataDouble;
    union Data16 data16;

    if (SMALLER_SAVE){
        getSampleS.mode=contents.data[0];
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[1 + i];
        getSampleS.speed=data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[3 + i];
        getSampleS.time=data16.u16;
        getSampleS.index=contents.data[5];
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[6 + i];
        getSampleS.continueTime = data16.u16;
        getSampleS.CanId=contents.data[8];
        getSampleS.curFaceTypeIndex=contents.data[9];
        for (int i =0 ; i<2;i++)
            data16.u8_16[i] = contents.data[10 + i];
        getSampleS.Type=data16.u16;
    } else {
        getSampleS.mode=contents.data[0];
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[1 + i];
        getSampleS.speed=data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[3 + i];
        getSampleS.time=data16.u16;
        getSampleS.index=contents.data[5];
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[6 + i];
        getSampleS.continueTime = data16.u16;
        getSampleS.CanId=contents.data[8];
        getSampleS.curFaceTypeIndex=contents.data[9];
        for (int i =0 ; i<2;i++)
            data16.u8_16[1-i] = contents.data[10 + i];
        getSampleS.Type=data16.u16;
    }
    return getSampleS;
}
ResSerialize serializeInjectSample(injectSampleStruct injectSampleS){
    ResSerialize resSerialize;
     uint32_t getSampleStructSize = 1+2+2+1+2+1+1+2;
    resSerialize.count = getSampleStructSize+4;
    int cnt = -1;
    if (SMALLER_SAVE)
    {
        // InjectSample
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleStructSize + i));
        resSerialize.data[++cnt] = (injectSampleS.mode);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&injectSampleS.speed + i));
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&injectSampleS.time + i));
        resSerialize.data[++cnt] = (injectSampleS.index);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&injectSampleS.continueTime + i));
        resSerialize.data[++cnt] = (injectSampleS.CanId);
        resSerialize.data[++cnt] = (injectSampleS.curFaceTypeIndex);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&injectSampleS.Type + i));
    }
    else
    {
        // InjectSample
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&getSampleStructSize + i));
        resSerialize.data[++cnt] = (injectSampleS.mode);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&injectSampleS.speed + i));
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&injectSampleS.time + i));
        resSerialize.data[++cnt] = (injectSampleS.index);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&injectSampleS.continueTime + i));
        resSerialize.data[++cnt] = (injectSampleS.CanId);
        resSerialize.data[++cnt] = (injectSampleS.curFaceTypeIndex);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&injectSampleS.Type + i));
    }
    return resSerialize;
}
injectSampleStruct deSerializeInjectSample(ResSerialize contents){
    injectSampleStruct injectSampleS = initInjectSampleStruct();
    union Data32
    {
        uint8_t u8_32[4]; 
        uint32_t u32;
    };
    union DataFloat
    {
        uint8_t u8f[4];
        float f;
    };
    union DataDouble
    {
        uint8_t u8d[8]; 
        double d;
    };
    union Data16
    {
        uint8_t u8_16[2]; 
        uint16_t u16;
    };

    union Data32 data32;
    union DataFloat datafloat;
    union DataDouble dataDouble;
    union Data16 data16;
    if (SMALLER_SAVE){
        injectSampleS.mode=contents.data[0];
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[1 + i];
        injectSampleS.speed=data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[3 + i];
        injectSampleS.time=data16.u16;
        injectSampleS.index=contents.data[5];
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[6 + i];
        injectSampleS.continueTime = data16.u16;
        injectSampleS.CanId=contents.data[8];
        injectSampleS.curFaceTypeIndex=contents.data[9];
        for (int i =0 ; i<2;i++)
            data16.u8_16[i] = contents.data[10 + i];
        injectSampleS.Type=data16.u16;
    } else {
        injectSampleS.mode=contents.data[0];
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[1 + i];
        injectSampleS.speed=data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[3 + i];
        injectSampleS.time=data16.u16;
        injectSampleS.index=contents.data[5];
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[6 + i];
        injectSampleS.continueTime = data16.u16;
        injectSampleS.CanId=contents.data[8];
        injectSampleS.curFaceTypeIndex=contents.data[9];
        for (int i =0 ; i<2;i++)
            data16.u8_16[1-i] = contents.data[10 + i];
        injectSampleS.Type=data16.u16;
    }
    return injectSampleS;
}
ResSerialize serializeDetectSample(detectStruct detectS){
    ResSerialize resSerialize;
     uint32_t detectStructSize = 1+2+2+2+4+1+2+2+1+2+1+1+2;
    resSerialize.count = detectStructSize+4;
    int cnt = -1;
    if (SMALLER_SAVE)
    {
        // detect
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectStructSize + i));
        resSerialize.data[++cnt] = (detectS.dddj);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.ddTypeMin + i));
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.ddTypeMax + i));
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.ddCuiQuTime + i));
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.targetDW + i));
        resSerialize.data[++cnt] = (detectS.targetType);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.targetElapsed + i));
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.targetRange + i));
        resSerialize.data[++cnt] = (detectS.index);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.continueTime + i));
        resSerialize.data[++cnt] = (detectS.CanId);
        resSerialize.data[++cnt] = (detectS.curFaceTypeIndex);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.Type + i));
    }
    else
    {
        // detect
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectStructSize + i));
        resSerialize.data[++cnt] = (detectS.dddj);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.ddTypeMin + i));
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.ddTypeMax + i));
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.ddCuiQuTime + i));
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.targetDW + i));
        resSerialize.data[++cnt] = (detectS.targetType);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.targetElapsed + i));
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.targetRange + i));
        resSerialize.data[++cnt] = (detectS.index);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.continueTime + i));
        resSerialize.data[++cnt] = (detectS.CanId);
        resSerialize.data[++cnt] = (detectS.curFaceTypeIndex);
        for (int i = 1; i >=0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&detectS.Type + i));
    }
    return resSerialize;
}
detectStruct deSerializeDetectSample(ResSerialize contents){
    detectStruct detectS;
    union Data32
    {
        uint8_t u8_32[4]; 
        uint32_t u32;
    };
    union DataFloat
    {
        uint8_t u8f[4];
        float f;
    };
    union DataDouble
    {
        uint8_t u8d[8]; 
        double d;
    };
    union Data16
    {
        uint8_t u8_16[2]; 
        uint16_t u16;
    };

    union Data32 data32;
    union DataFloat datafloat;
    union DataDouble dataDouble;
    union Data16 data16;
    if (SMALLER_SAVE){
        detectS.dddj=contents.data[0];
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[1 + i];
        detectS.ddTypeMin=data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[3 + i];
        detectS.ddTypeMax=data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[5 + i];
        detectS.ddCuiQuTime=data16.u16;
        for (int i = 0; i < 4; i++)
            datafloat.u8f[i] = contents.data[7 + i];
        detectS.targetDW=datafloat.f;
        detectS.targetType=contents.data[11];
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[12 + i];
        detectS.targetElapsed = data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[14 + i];
        detectS.targetRange = data16.u16;
        detectS.index=contents.data[16];
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[17 + i];
        detectS.continueTime=data16.u16;
        detectS.CanId=contents.data[19];
        detectS.curFaceTypeIndex=contents.data[20];
        for (int i =0 ; i<2;i++)
            data16.u8_16[i] = contents.data[21 + i];
        detectS.Type=data16.u16;
    } else {
        detectS.dddj=contents.data[0];
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[1 + i];
        detectS.ddTypeMin=data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[3 + i];
        detectS.ddTypeMax=data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[5 + i];
        detectS.ddCuiQuTime=data16.u16;
        for (int i = 0; i < 4; i++)
            datafloat.u8f[3-i] = contents.data[7 + i];
        detectS.targetDW=datafloat.f;
        detectS.targetType=contents.data[11];
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[12 + i];
        detectS.targetElapsed = data16.u16;
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[14 + i];
        detectS.targetRange = data16.u16;
        detectS.index=contents.data[16];
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[17 + i];
        detectS.continueTime=data16.u16;
        detectS.CanId=contents.data[19];
        detectS.curFaceTypeIndex=contents.data[20];
        for (int i =0 ; i<2;i++)
            data16.u8_16[1-i] = contents.data[21 + i];
        detectS.Type=data16.u16;
    }
    return detectS;
}
ResSerialize serializeBalance(balanceStruct balanceS){
    ResSerialize resSerialize;
    uint32_t balanceStructSize = 1+2+1+1+2;
    resSerialize.count = balanceStructSize+4;
    int cnt = -1;
    if (SMALLER_SAVE)
    {
        // balance
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&balanceStructSize + i));
        resSerialize.data[++cnt] = balanceS.index;
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&balanceS.continueTime + i));
        resSerialize.data[++cnt] = (balanceS.CanId);
        resSerialize.data[++cnt] = (balanceS.curFaceTypeIndex);
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&balanceS.Type + i));
    }
    else
    {
        // balance
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&balanceStructSize + i));
        resSerialize.data[++cnt] = balanceS.index;
        for (int i = 1; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&balanceS.continueTime + i));
        resSerialize.data[++cnt] = (balanceS.CanId);
        resSerialize.data[++cnt] = (balanceS.curFaceTypeIndex);
        for (int i = 1; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&balanceS.Type + i));
    }
    return resSerialize;
}
balanceStruct deSerializeBalance(ResSerialize contents){
    balanceStruct balanceS;
    union Data32
    {
        uint8_t u8_32[4]; 
        uint32_t u32;
    };
    union DataFloat
    {
        uint8_t u8f[4];
        float f;
    };
    union DataDouble
    {
        uint8_t u8d[8]; 
        double d;
    };
    union Data16
    {
        uint8_t u8_16[2]; 
        uint16_t u16;
    };

    union Data32 data32;
    union DataFloat datafloat;
    union DataDouble dataDouble;
    union Data16 data16;
    if (SMALLER_SAVE){
        balanceS.index=contents.data[0];
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[1 + i];
        balanceS.continueTime=data16.u16;
        balanceS.CanId=contents.data[3];
        balanceS.curFaceTypeIndex=contents.data[4];
        for (int i =0 ; i<2;i++)
            data16.u8_16[i] = contents.data[5 + i];
        balanceS.Type=data16.u16;
    } else {
        balanceS.index=contents.data[0];
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[1 + i];
        balanceS.continueTime=data16.u16;
        balanceS.CanId=contents.data[3];
        balanceS.curFaceTypeIndex=contents.data[4];
        for (int i =0 ; i<2;i++)
            data16.u8_16[1-i] = contents.data[5 + i];
        balanceS.Type=data16.u16;
    }
    return balanceS;
}
ResSerialize serializeAnalysisData(analysisData analysisS){
    ResSerialize resSerialize;
    uint32_t analysisStructSize = 2+4+4+2+4;
    resSerialize.count = analysisStructSize+4;
    int cnt = -1;
    if (SMALLER_SAVE)
    {
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&analysisStructSize + i));
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&analysisS.TimeSpan + i));
        for (int i=0;i<4;i++)
            resSerialize.data[++cnt] = (*((uint8_t*)&analysisS.Drift+i));
        for (int i=0;i<4;i++)
            resSerialize.data[++cnt] = (*((uint8_t*)&analysisS.WaterWeigth+i));
        for (int i=0;i<2;i++)
            resSerialize.data[++cnt] = (*((uint8_t*)&analysisS.Voltage+i));
        for (int i=0;i<4;i++)
            resSerialize.data[++cnt] = (*((uint8_t*)&analysisS.Balance_Speed+i));
    }
    else
    {
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&analysisStructSize + i));
        for (int i = 1; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&analysisS.TimeSpan + i));
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t*)&analysisS.Drift+i));
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t*)&analysisS.WaterWeigth+i));
        for (int i = 1; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t*)&analysisS.Voltage+i));
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t*)&analysisS.Balance_Speed+i));
    }
    return resSerialize;
}
analysisData deSerializeAnalysisData(ResSerialize contents)
{
    analysisData analysisS;
    union Data32
    {
        uint8_t u8_32[4]; 
        uint32_t u32;
    };
    union DataFloat
    {
        uint8_t u8f[4];
        float f;
    };
    union DataDouble
    {
        uint8_t u8d[8]; 
        double d;
    };
    union Data16
    {
        uint8_t u8_16[2]; 
        uint16_t u16;
    };

    union Data32 data32;
    union DataFloat datafloat;
    union DataDouble dataDouble;
    union Data16 data16;
    if (SMALLER_SAVE){
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[0 + i];
        analysisS.TimeSpan=data16.u16;
        for (int i =0 ; i<4;i++)
            datafloat.u8f[i] = contents.data[2 + i];
        analysisS.Drift=datafloat.f;
        for (int i =0 ; i<4;i++)
            datafloat.u8f[i] = contents.data[6 + i];
        analysisS.WaterWeigth=datafloat.f;
        for (int i =0 ; i<2;i++)
            data16.u8_16[i] = contents.data[10 + i];
        analysisS.Voltage=data16.u16;
        for (int i =0 ; i<4;i++)
            datafloat.u8f[i] = contents.data[12 + i];
        analysisS.Balance_Speed=datafloat.f;
    } else {
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[0 + i];
        analysisS.TimeSpan=data16.u16;
        for (int i =0 ; i<4;i++)
            datafloat.u8f[3-i] = contents.data[2 + i];
        analysisS.Drift=datafloat.f;
        for (int i =0 ; i<4;i++)
            datafloat.u8f[3-i] = contents.data[6 + i];
        analysisS.WaterWeigth=datafloat.f;
        for (int i =0 ; i<2;i++)
            data16.u8_16[1-i] = contents.data[10 + i];
        analysisS.Voltage=data16.u16;
        for (int i =0 ; i<4;i++)
            datafloat.u8f[3-i] = contents.data[12 + i];
        analysisS.Balance_Speed=datafloat.f;
    }
    return analysisS;
}
checkDevice deSerializeCheckDevice(ResSerialize contents)
{
    union Data32
    {
        uint8_t u8_32[4];
        uint32_t u32;
    };
    union DataFloat
    {
        uint8_t u8f[4];
        float f;
    };
    union DataDouble
    {
        uint8_t u8d[8];
        double d;
    };
    union Data16
    {
        uint8_t u8_16[2];
        uint16_t u16;
    };

    union Data32 data32;
    union DataFloat datafloat;
    union DataDouble dataDouble;
    union Data16 data16;
    uint32_t lengthAll,lengthInstrumetId,lengthFaceType,lengthFaceName,lengthFaceRun,lengthFaceState;
    checkDevice checkDeviceS;
    checkDeviceS.FaceNumber = contents.data[0];

    if (checkDeviceS.FaceNumber >= MAX_NUM)
    {
        checkDeviceS = initCheckDevice();
        return checkDeviceS;
    }

    if (SMALLER_SAVE)
    {
        for (int i = 0; i < 2; i++)
            data16.u8_16[i] = contents.data[1 + i];
        checkDeviceS.Type = data16.u16;
        for (int i = 0; i < 4; i++)
            data32.u8_32[i] = contents.data[3 + i];
        lengthInstrumetId = data32.u32;

        if (lengthInstrumetId > 20)
        {
            checkDeviceS.CanId = 0;
            return checkDeviceS;
        }

        for (int i = 0; i < (lengthInstrumetId); i++)
            checkDeviceS.Instrument_Id[i] = (contents.data[7 + i]);
        checkDeviceS.Instrument_Id[lengthInstrumetId] = '\0';
        checkDeviceS.CanId = contents.data[7 + lengthInstrumetId];
        for (int i = 0; i < 4; i++)
            data32.u8_32[i] = contents.data[8 + lengthInstrumetId + i];
        lengthFaceType = data32.u32;
        for (int i = 0; i < checkDeviceS.FaceNumber; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                data16.u8_16[j] = contents.data[12 + lengthInstrumetId + i * 2 + j];
            }
            checkDeviceS.FaceType[i] = data16.u16;
        }
        for (int i = 0; i < 4; i++)
            data32.u8_32[i] = contents.data[12 + lengthInstrumetId + 2 * checkDeviceS.FaceNumber + i];
        lengthFaceName = data32.u32;
        uint32_t lengthFaceNameCountLength = 0;
        for (int i = 0; i < checkDeviceS.FaceNumber; i++)
        {
            uint32_t lengthFaceNameIndex = 0;
            for (int j = 0; j < 4; j++)
            {
                data32.u8_32[j] = contents.data[16 + lengthInstrumetId + 2 * checkDeviceS.FaceNumber + lengthFaceNameCountLength + i * 4 + j];
            }
            lengthFaceNameIndex = data32.u32;

            // if (lengthFaceNameIndex >= MAX_NUM)
            // {
            //     return checkDeviceS;
            // }
            for (int j = 0; j < (lengthFaceNameIndex); j++)
                checkDeviceS.Face_Name[i][j] = (contents.data[16 + lengthInstrumetId + 2 * checkDeviceS.FaceNumber + lengthFaceNameCountLength + (i + 1) * 4 + j]);
            checkDeviceS.Face_Name[i][lengthFaceNameIndex] = '\0';
            lengthFaceNameCountLength += lengthFaceNameIndex;
        }
        for (int i = 0; i < 4; i++)
            data32.u8_32[i] = contents.data[16 + lengthInstrumetId + 2 * checkDeviceS.FaceNumber + lengthFaceName + i];
        lengthFaceRun = data32.u32;
        for (int i = 0; i < checkDeviceS.FaceNumber; i++)
        {
            checkDeviceS.FaceRun[i] = contents.data[20 + lengthInstrumetId + 2 * checkDeviceS.FaceNumber + lengthFaceName + i];
        }
        for (int i = 0; i < 4; i++)
            data32.u8_32[i] = contents.data[20 + lengthInstrumetId + 3 * checkDeviceS.FaceNumber + lengthFaceName + i];
        lengthFaceState = data32.u32;
        for (int i = 0; i < checkDeviceS.FaceNumber; i++)
        {
            checkDeviceS.FaceState[i] = contents.data[24 + lengthInstrumetId + 3 * checkDeviceS.FaceNumber + lengthFaceName + i];
        }
    }
    else
    {
        for (int i = 0; i < 2; i++)
            data16.u8_16[1-i] = contents.data[1 + i];
        checkDeviceS.Type = data16.u16;
        for (int i = 0; i < 4; i++)
            data32.u8_32[3-i] = contents.data[3 + i];
        lengthInstrumetId = data32.u32;
        if (lengthInstrumetId > 20){     
            checkDeviceS.CanId = 0;
            return checkDeviceS;
        }
        for (int i = 0; i < (lengthInstrumetId); i++)
            checkDeviceS.Instrument_Id[i] = (contents.data[7 + i]);
        checkDeviceS.Instrument_Id[lengthInstrumetId]='\0';
        checkDeviceS.CanId=contents.data[7+lengthInstrumetId];
        for (int i = 0; i < 4; i++)
            data32.u8_32[3-i] = contents.data[8+lengthInstrumetId + i];
        lengthFaceType=data32.u32;
        for (int i=0;i<checkDeviceS.FaceNumber;i++){
            for (int j=0;j<2;j++){
                data16.u8_16[1-j]=contents.data[12+lengthInstrumetId+i*2+j];
            }
            checkDeviceS.FaceType[i]=data16.u16;
        }

        for (int i=0;i<4;i++)
            data32.u8_32[3-i]=contents.data[12+lengthInstrumetId+2*checkDeviceS.FaceNumber+i];
        lengthFaceName=data32.u32;

        uint32_t lengthFaceNameCountLength=0;
        for (int i=0;i<checkDeviceS.FaceNumber;i++){
            uint32_t lengthFaceNameIndex=0;
            for (int j=0;j<4;j++){
                int pos=16+lengthInstrumetId+2*checkDeviceS.FaceNumber+lengthFaceNameCountLength+i*4+j;
                data32.u8_32[3-j]=contents.data[pos];
            }
            lengthFaceNameIndex=data32.u32;
            // if (lengthFaceNameIndex >= MAX_NUM)
            // {
            //     return checkDeviceS;
            // }
            for (int j = 0; j < (lengthFaceNameIndex); j++){
                int pos=16 + lengthInstrumetId+2*checkDeviceS.FaceNumber+lengthFaceNameCountLength+(i+1)*4+j;
                checkDeviceS.Face_Name[i][j] = (contents.data[pos]);
            }
            checkDeviceS.Face_Name[i][lengthFaceNameIndex]='\0';
            lengthFaceNameCountLength+=lengthFaceNameIndex;
        }
        for (int i = 0; i < 4; i++)
            data32.u8_32[3-i] = contents.data[16 + lengthInstrumetId+2*checkDeviceS.FaceNumber+lengthFaceName + i];
        lengthFaceRun=data32.u32;
        for (int i=0;i<checkDeviceS.FaceNumber;i++){
            checkDeviceS.FaceRun[i]=contents.data[20 + lengthInstrumetId+2*checkDeviceS.FaceNumber+lengthFaceName+i];
        }
        for (int i = 0; i < 4; i++)
            data32.u8_32[3-i] = contents.data[20 + lengthInstrumetId+3*checkDeviceS.FaceNumber+lengthFaceName + i];
        lengthFaceState=data32.u32;
        for (int i=0;i<checkDeviceS.FaceNumber;i++){
            checkDeviceS.FaceState[i]=contents.data[24 + lengthInstrumetId+3*checkDeviceS.FaceNumber+lengthFaceName+i];
        }
    }
    return checkDeviceS;
}
ResSerialize serializeCheckDevice(checkDevice checkDeviceS){
    ResSerialize resSerialize;
    uint32_t lengthInstrumetId = strlen(checkDeviceS.Instrument_Id);
    if (checkDeviceS.FaceNumber > MAX_NUM)
    {
        checkDeviceS.FaceNumber = MAX_NUM;
    }
    uint32_t lengthFaceName = 0;
    for (int i = 0; i < checkDeviceS.FaceNumber; i++)
    {
        lengthFaceName += strlen(checkDeviceS.Face_Name[i]);
        lengthFaceName += 4;
    }
    uint32_t structSize = 1+2+(4 + lengthInstrumetId)+1 + (4 + checkDeviceS.FaceNumber * 2) +
                             (4 + lengthFaceName) + /*1 + */(4 + checkDeviceS.FaceNumber) + 
                             (4 + checkDeviceS.FaceNumber) + 1;

    resSerialize.count = structSize+4;
    int cnt = -1;
    if (SMALLER_SAVE)
    {
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&structSize + i));
        // FaceNumber
        resSerialize.data[++cnt] = (checkDeviceS.FaceNumber);
        // Type
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&checkDeviceS.Type + i)); // Type;
        // Instrument_Id
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = ((*((uint8_t *)&lengthInstrumetId + i)));
        for (int i = 0; i < (lengthInstrumetId); i++)
        {
            resSerialize.data[++cnt] = ((*((uint8_t *)&checkDeviceS.Instrument_Id[i])));
        }
        // -------CanId
        resSerialize.data[++cnt] = (checkDeviceS.CanId);
        //-------- FaceType
        uint32_t FaceTypeNumber = checkDeviceS.FaceNumber * 2;
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&FaceTypeNumber + i));
        for (int i = 0; i < checkDeviceS.FaceNumber; i++)
        {
            for (int j = 0; j < 2; j++)
                resSerialize.data[++cnt] = (*((uint8_t *)&checkDeviceS.FaceType[i] + j));
        }
        // -------Face_Name
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = ((*((uint8_t *)&lengthFaceName + i)));
        for (int i = 0; i < (checkDeviceS.FaceNumber); i++)
        {
            uint32_t lengthFaceNameIndex = strlen(checkDeviceS.Face_Name[i]);
            for (int j = 0; j < 4; j++)
                resSerialize.data[++cnt] = ((*((uint8_t *)&lengthFaceNameIndex + j)));
            for (int j = 0; j < (lengthFaceNameIndex); j++)
            {
                resSerialize.data[++cnt] = ((*((uint8_t *)&checkDeviceS.Face_Name[i][j])));
            }
        }
        //-------FaceRun
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&checkDeviceS.FaceNumber + i));
        for (int i = 0; i < checkDeviceS.FaceNumber; i++)
            resSerialize.data[++cnt] = (checkDeviceS.FaceRun[i]);
        // --------FaceState
        for (int i = 0; i < 4; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&checkDeviceS.FaceNumber + i));
        for (int i = 0; i < checkDeviceS.FaceNumber; i++)
            resSerialize.data[++cnt] = (checkDeviceS.FaceState[i]);
        resSerialize.data[++cnt] = checkDeviceS.FaceReceiveStatus;
    }
    else
    {
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&structSize + i));
        // FaceNumber
        resSerialize.data[++cnt] = (checkDeviceS.FaceNumber);
        // Type
        for (int i = 1; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&checkDeviceS.Type + i)); // Type;
        // Instrument_Id
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = ((*((uint8_t *)&lengthInstrumetId + i)));
        if (lengthInstrumetId > 0)
        {
            for (int i = 0; i < (lengthInstrumetId); i++)
            {
                resSerialize.data[++cnt] = ((*((uint8_t *)&checkDeviceS.Instrument_Id[i])));
            }
        }
        // -------CanId
        resSerialize.data[++cnt] = (checkDeviceS.CanId);
        //-------- FaceType
        uint32_t FaceTypeNumber = checkDeviceS.FaceNumber * 2;
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&FaceTypeNumber + i));
        if (checkDeviceS.FaceNumber > 0)
        {
            for (int i = 0; i < checkDeviceS.FaceNumber; i++)
            {
                for (int j = 1; j >= 0; j--)
                    resSerialize.data[++cnt] = (*((uint8_t *)&checkDeviceS.FaceType[i] + j));
            }
        }
        // -------Face_Name
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = ((*((uint8_t *)&lengthFaceName + i)));
        if (checkDeviceS.FaceNumber > 0)
        {
            for (int i = 0; i < checkDeviceS.FaceNumber; i++)
            {
                uint32_t lengthFaceNameIndex = strlen(checkDeviceS.Face_Name[i]);
                for (int j = 3; j >= 0; j--)
                    resSerialize.data[++cnt] = ((*((uint8_t *)&lengthFaceNameIndex + j)));
                if (lengthFaceNameIndex > 0)
                {
                    for (int j = 0; j < (lengthFaceNameIndex); j++)
                    {
                        resSerialize.data[++cnt] = ((*((uint8_t *)&checkDeviceS.Face_Name[i][j])));
                    }
                }
            }
        }
        //-------FaceRun
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&checkDeviceS.FaceNumber + i));
        if (checkDeviceS.FaceNumber > 0)
        {
            for (int i = 0; i < checkDeviceS.FaceNumber; i++)
                resSerialize.data[++cnt] = (checkDeviceS.FaceRun[i]);
        }
        // --------FaceState
        for (int i = 3; i >= 0; i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&checkDeviceS.FaceNumber + i));
        if (checkDeviceS.FaceNumber > 0)
        {
            for (int i = 0; i < checkDeviceS.FaceNumber; i++)
                resSerialize.data[++cnt] = (checkDeviceS.FaceState[i]);
        }
        resSerialize.data[++cnt] = checkDeviceS.FaceReceiveStatus;
    }
    return resSerialize;
}

ResSerialize serialize(UpperPCToLowerPC stuctS)
{
    ResSerialize resSerialize;
    int cnt = 0;
    resSerialize.data[cnt] = COM_HEAD;

    if (stuctS.CurPhase==CHECK_DEVICE){
        ResSerialize checkDeviceSerialize = serializeCheckDevice(stuctS.CheckDevice);
        uint32_t lengthAll=4+1+checkDeviceSerialize.count;
        resSerialize.count=lengthAll+1+4;
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        } else {
            for (int i=3;i>=0;i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        }
        resSerialize.data[++cnt] = stuctS.CurPhase;
        for (int i=0;i<checkDeviceSerialize.count;i++)
            resSerialize.data[++cnt] = checkDeviceSerialize.data[i];
    } else if (stuctS.CurPhase == RUN_FLOW)
    {
        int countSwril=0, countBalance=0,countGetSample=0,countInjectSample=0,countDetectSample=0;
        for (int i=0;i<MAX_NUM;i++){
            if (stuctS.Swirl[i].index==255) continue;
            countSwril++;
        }
        for (int i=0;i<MAX_NUM;i++){
            if (stuctS.BalanceSample[i].index==255) continue;
            countBalance++;
        }
        for (int i=0;i<MAX_NUM;i++){
            if (stuctS.GetSample[i].index==255) continue;
            countGetSample++;
        }
        for (int i=0;i<MAX_NUM;i++){
            if (stuctS.InjectSample[i].index==255) continue;
            countInjectSample++;
        }
        for (int i=0;i<MAX_NUM;i++){
            if (stuctS.DetectSample[i].index==255) continue;
            countDetectSample++;
        }
        uint32_t lengthAll=4+1;
        if (countSwril>0) lengthAll+=countSwril*(sizeofSwirl+4);
        lengthAll+=4;
        if (countBalance>0) lengthAll+=countBalance*(sizeofBalance+4);
        lengthAll+=4;
        if (countGetSample>0) lengthAll+=countGetSample*(sizeofGetSample+4);
        lengthAll+=4;
        if (countInjectSample>0) lengthAll+=countInjectSample*(sizeofInjectSample+4);
        lengthAll+=4;
        if (countDetectSample>0) lengthAll+=countDetectSample*(sizeofDetectSample+4);
        lengthAll+=4;

        resSerialize.count = lengthAll + 1 + 4;
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        }
        else
        {
            for (int i = 3; i >= 0; i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        }
        resSerialize.data[++cnt] = stuctS.CurPhase;
        //--------swirl--------
        uint32_t sizeSwirl = countSwril * (sizeofSwirl+4);
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeSwirl + i));
        }
        else
        {
            for (int i = 3; i >= 0; i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeSwirl + i));
        }
        if (countSwril > 0)
        {
            for (int i = 0; i < MAX_NUM; i++)
            {
                if (stuctS.Swirl[i].index == 255)
                    continue;
                ResSerialize serializeTemp = serializeSwirl(stuctS.Swirl[i]);
                for (int j = 0; j < serializeTemp.count; j++)
                    resSerialize.data[++cnt] = serializeTemp.data[j];
            }
        }
        //------balance--------
        uint32_t sizeBalance = countBalance * (sizeofBalance+4);
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeBalance + i));
        }
        else
        {
            for (int i = 3; i >= 0; i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeBalance + i));
        }
        if (countBalance > 0)
        {
            for (int i = 0; i < MAX_NUM; i++)
            {
                if (stuctS.BalanceSample[i].index == 255)
                    continue;
                ResSerialize serializeTemp = serializeBalance(stuctS.BalanceSample[i]);
                for (int j = 0; j < serializeTemp.count; j++)
                    resSerialize.data[++cnt] = serializeTemp.data[j];
            }
        }
        //------getsample--------
        uint32_t sizeGetSample = countGetSample * (sizeofGetSample+4);
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeGetSample + i));
        }
        else
        {
            for (int i = 3; i >= 0; i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeGetSample + i));
        }
        if (countGetSample > 0)
        {
            for (int i = 0; i < MAX_NUM; i++)
            {
                if (stuctS.GetSample[i].index == 255)
                    continue;
                ResSerialize serializeTemp = serializeGetSample(stuctS.GetSample[i]);
                for (int j = 0; j < serializeTemp.count; j++)
                    resSerialize.data[++cnt] = serializeTemp.data[j];
            }
        }
        //------injectsample--------
        uint32_t sizeInjectSample = countInjectSample * (sizeofInjectSample+4);
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeInjectSample + i));
        }
        else
        {
            for (int i = 3; i >= 0; i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeInjectSample + i));
        }
        if (countInjectSample > 0)
        {
            for (int i = 0; i < MAX_NUM; i++)
            {
                if (stuctS.InjectSample[i].index == 255)
                    continue;
                ResSerialize serializeTemp = serializeInjectSample(stuctS.InjectSample[i]);
                for (int j = 0; j < serializeTemp.count; j++)
                    resSerialize.data[++cnt] = serializeTemp.data[j];
            }
        }
        //------detect--------
        uint32_t sizeDetect = countDetectSample * (sizeofDetectSample+4);
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeDetect + i));
        }
        else
        {
            for (int i = 3; i >= 0; i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&sizeDetect + i));
        }
        if (countDetectSample > 0)
        {
            for (int i = 0; i < MAX_NUM; i++)
            {
                if (stuctS.DetectSample[i].index == 255)
                    continue;
                ResSerialize serializeTemp = serializeDetectSample(stuctS.DetectSample[i]);
                for (int j = 0; j < serializeTemp.count; j++)
                    resSerialize.data[++cnt] = serializeTemp.data[j];
            }
        }
    } else if (stuctS.CurPhase==RESPONSE_FLOW) {
        uint32_t lengthAll=4+1+1;
        resSerialize.count=lengthAll+1+4;
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        } else {
            for (int i=3;i>=0;i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        }
        resSerialize.data[++cnt] = stuctS.CurPhase;
        resSerialize.data[++cnt] = stuctS.Instrument_Run;
    } else if (stuctS.CurPhase == SERIALIZE_INIT) {
        uint32_t lengthSyncTime = strlen(stuctS.SyncTime);
        uint32_t lengthAll=4+1+1+4+lengthSyncTime;
        resSerialize.count=lengthAll+1+4;
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        } else {
            for (int i=3;i>=0;i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        }
        resSerialize.data[++cnt] = stuctS.CurPhase;
        resSerialize.data[++cnt] = stuctS.Instrument_Run;
            
        // SyncTime
        if (SMALLER_SAVE){
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t*)&lengthSyncTime+i));
        } else {
            for (int i=3;i>=0;i--){
                resSerialize.data[++cnt]=(*((uint8_t*)&lengthSyncTime+i));
            }
        }
        for (int i = 0; i < (lengthSyncTime); i++)
        {
            resSerialize.data[++cnt] = ((*((uint8_t *)&stuctS.SyncTime[i])));
        }
    } else if (stuctS.CurPhase==INTERACTE_DATA){
        ResSerialize analysisDataSerialize = serializeAnalysisData(stuctS.AnalysisData);
        uint32_t lengthAll=4+1+analysisDataSerialize.count;
        resSerialize.count = lengthAll + 1 + 4;
        if (SMALLER_SAVE) 
        {
            for (int i = 0; i < 4; i++)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        } else {
            for (int i=3;i>=0;i--)
                resSerialize.data[++cnt] = (*((uint8_t *)&lengthAll + i));
        }
        resSerialize.data[++cnt] = stuctS.CurPhase;
        for (int i=0;i<analysisDataSerialize.count;i++)
            resSerialize.data[++cnt]=analysisDataSerialize.data[i];
    }  
    //校验位
    unsigned char* ptr = (unsigned char*)&resSerialize.data[1];
    unsigned short CRC16=CRC16_MODBUS(ptr,resSerialize.count-5);

    if (SMALLER_SAVE)
    {
        for (int i = 0; i < 2; i++)
            resSerialize.data[++cnt] = (*((uint8_t *)&CRC16 + i));
    } else {
        for (int i=1;i>=0;i--)
            resSerialize.data[++cnt] = (*((uint8_t *)&CRC16 + i));
    }
    resSerialize.data[++cnt] = (COM_TAIL1);
    resSerialize.data[++cnt] = (COM_TAIL2);
    resSerialize.data[++cnt] = (COM_TAIL3);
    resSerialize.data[++cnt] = (COM_TAIL4);
    resSerialize.count=cnt+1;
    return resSerialize;
}
checkDevice initCheckDevice(){
    checkDevice device;
    device.FaceNumber=0;
    device.Type=0;
    device.Instrument_Id[0]='\0';
    device.CanId=0;
    for (int i=0;i<MAX_NUM;i++){
        device.FaceType[i]=0;
        device.Face_Name[i][0]='\0';
        device.FaceRun[i]=0;
        device.FaceState[i]=0;
    }
    return device;
}

swirlStruct initSwirlStruct(){
    swirlStruct swirl;
    swirl.swirlSpeed=0;
    swirl.index=255;
    swirl.continueTime=65535;
    return swirl;
}
getSampleStruct initGetSampleStruct(){
    getSampleStruct sample;
    sample.mode=0;
    sample.speed=0;
    sample.time=0;
    sample.index=255;
    sample.continueTime=65535;
    return sample;
}
injectSampleStruct initInjectSampleStruct(){
    injectSampleStruct sample;
    sample.mode=0;
    sample.speed=0;
    sample.time=0;
    sample.index=255;
    sample.continueTime=65535;
    return sample;
}
detectStruct initDetectStruct(){
    detectStruct detect;
    detect.ddCuiQuTime=0;
    detect.ddTypeMax=0;
    detect.ddTypeMin=0;
    detect.dddj=0;
    detect.index=255;
    detect.continueTime=65535;
    return detect;
}
balanceStruct initBalanceStruct(){
    balanceStruct balance;
    balance.index=255;
    balance.continueTime=65535;
    balance.curFaceTypeIndex=0;
    return balance;
}
analysisData initAnalysisData(){
    analysisData analysis;
    analysis.Balance_Speed=0;
    analysis.Drift=0;
    analysis.TimeSpan=0;
    analysis.Voltage=0;
    analysis.WaterWeigth=0;
    return analysis;
}
UpperPCToLowerPC initUpperPCToLowerPC(){
    UpperPCToLowerPC upper;
    upper.CurPhase=SERIALIZE_INIT;
    upper.CheckDevice=initCheckDevice();
    for (int i=0;i<MAX_NUM;i++){
        upper.Swirl[i]=initSwirlStruct();
        upper.GetSample[i]=initGetSampleStruct();
        upper.InjectSample[i]=initInjectSampleStruct();
        upper.DetectSample[i]=initDetectStruct();
        upper.BalanceSample[i]=initBalanceStruct();
    }
    upper.Instrument_Run=0;
    upper.AnalysisData=initAnalysisData();
    return upper;
}
swirlStruct copySwirl(swirlStruct swirlS)
{
    swirlStruct temp;
    temp.CanId=swirlS.CanId;
    temp.Type=swirlS.Type;
    temp.continueTime=swirlS.continueTime;
    temp.curFaceTypeIndex=swirlS.curFaceTypeIndex;
    temp.index=swirlS.index;
    temp.swirlSpeed=swirlS.swirlSpeed;
    return temp;
}
balanceStruct copyBalance(balanceStruct balanceS)
{
    balanceStruct temp;
    temp.CanId=balanceS.CanId;
    temp.Type=balanceS.Type;
    temp.continueTime=balanceS.continueTime;
    temp.curFaceTypeIndex=balanceS.curFaceTypeIndex;
    temp.index=balanceS.index;
    return temp;
}
getSampleStruct copyGetSample(getSampleStruct getSampleS)
{
    getSampleStruct temp;
    temp.mode=getSampleS.mode;
    temp.speed=getSampleS.speed;
    temp.time=getSampleS.time;
    temp.index =getSampleS.index;
    temp.continueTime=getSampleS.continueTime;
    temp.CanId=getSampleS.CanId;
    temp.curFaceTypeIndex=getSampleS.curFaceTypeIndex;
    temp.Type=getSampleS.Type;
    return temp;
}
injectSampleStruct copyInjectSample(injectSampleStruct injectSampleS)
{
    injectSampleStruct temp;
    temp.mode=injectSampleS.mode;
    temp.speed=injectSampleS.speed;
    temp.time=injectSampleS.time;
    temp.index =injectSampleS.index;
    temp.continueTime=injectSampleS.continueTime;
    temp.CanId=injectSampleS.CanId;
    temp.curFaceTypeIndex=injectSampleS.curFaceTypeIndex;
    temp.Type=injectSampleS.Type;
    return temp;
}
checkDevice copyCheckDevice(checkDevice checkDeviceS)
{
    checkDevice temp;
    temp.CanId=checkDeviceS.CanId;
    temp.FaceNumber=checkDeviceS.FaceNumber;
    temp.Type=checkDeviceS.Type;
    memcpy(temp.Instrument_Id,checkDeviceS.Instrument_Id,20);
    for (int i=0;i<MAX_NUM;i++){
        temp.FaceType[i]=checkDeviceS.FaceType[i];
        memcpy(temp.Face_Name[i],checkDeviceS.Face_Name[i],20);
        temp.FaceRun[i]=checkDeviceS.FaceRun[i];
        temp.FaceState[i]=checkDeviceS.FaceState[i];
    }
    temp.FaceReceiveStatus=checkDeviceS.FaceReceiveStatus;
    return temp;
}
detectStruct copyDetect(detectStruct detectS)
{
    detectStruct temp;
    temp.dddj=detectS.dddj;
    temp.ddTypeMin=detectS.ddTypeMin;
    temp.ddTypeMax=detectS.ddTypeMax;
    temp.ddCuiQuTime=detectS.ddCuiQuTime;
    temp.targetDW=detectS.targetDW;
    temp.targetType=detectS.targetType;
    temp.targetElapsed=detectS.targetElapsed;
    temp.targetRange=detectS.targetRange;
    temp.index=detectS.index;
    temp.continueTime=detectS.continueTime;
    temp.CanId=detectS.CanId;
    temp.curFaceTypeIndex=detectS.curFaceTypeIndex;
    temp.Type=detectS.Type;
    return temp;
}
UpperPCToLowerPC deSerialize(ResSerialize contents){
    UpperPCToLowerPC upperToLower=initUpperPCToLowerPC();
    union Data32
    {
        uint8_t u8_32[4]; 
        uint32_t u32;
    };
    union DataFloat
    {
        uint8_t u8f[4];
        float f;
    };
    union DataDouble
    {
        uint8_t u8d[8]; 
        double d;
    };
    union Data16
    {
        uint8_t u8_16[2]; 
        uint16_t u16;
    };
    union DataUS16
    {
        uint8_t u8_us16[2];
        unsigned short us16;
    };

    union Data32 data32;
    union DataFloat datafloat;
    union DataDouble dataDouble;
    union Data16 data16;
    union DataUS16 dataUS16;

    uint32_t lengthAll,lengthCheckDevice;
    uint32_t lengthSwirl,lengthBalance,lengthGetSample,lengthInjectSample,lengthDetect=0,lengthAnalysisData=0;
    if (contents.count < 4)
    {
        upperToLower=initUpperPCToLowerPC();
        return upperToLower;
    } 
    if (contents.data[0]==COM_HEAD&&
        contents.data[contents.count-1]==COM_TAIL4&&
        contents.data[contents.count-2]==COM_TAIL3&&
        contents.data[contents.count-3]==COM_TAIL2&&
        contents.data[contents.count-4]==COM_TAIL1){
    }else {
        upperToLower=initUpperPCToLowerPC();
        return upperToLower;
    }
    if (SMALLER_SAVE)
    {
        for (int i = 0; i < 4; i++)
            data32.u8_32[i] = contents.data[1 + i];
        lengthAll = data32.u32;
    }
    else
    {
        for (int i = 0; i < 4; i++)
            data32.u8_32[3-i] = contents.data[1 + i];
        lengthAll = data32.u32;
    }

    if ((lengthAll + 7) != contents.count)
    {
        upperToLower=initUpperPCToLowerPC();
        return upperToLower;
    }
    //校验位
    unsigned char* ptr = (unsigned char*)&contents.data[1];
    unsigned short CRC16=CRC16_MODBUS(ptr,contents.count-7);
    unsigned short CRC16_RES=0;
    if (SMALLER_SAVE)
    {
        for (int i = 0; i < 2; i++)
            dataUS16.u8_us16[i] = contents.data[contents.count-6+i];
    } else {
        for (int i = 0; i < 2; i++)
            dataUS16.u8_us16[1-i] = contents.data[contents.count-6+i];
    }
    CRC16_RES=dataUS16.us16;
    if (CRC16_RES!=CRC16)
    {
        upperToLower=initUpperPCToLowerPC();
        return upperToLower;
    }
    upperToLower.CurPhase=contents.data[5];

    if (upperToLower.CurPhase == SERIALIZE_INIT ||
        upperToLower.CurPhase == RESPONSE_FLOW)
    {
        upperToLower.Instrument_Run=contents.data[6];
        if (upperToLower.CurPhase == SERIALIZE_INIT)
        {
            uint32_t lengthSyncTime;
            if (SMALLER_SAVE)
            {
                for (int i = 0; i < 4; i++)
                    data32.u8_32[i] = contents.data[7 + i];
                lengthSyncTime = data32.u32;
            }
            else
            {
                for (int i = 0; i < 4; i++)
                    data32.u8_32[3 - i] = contents.data[7 + i];
                lengthSyncTime = data32.u32;
            }
            int lengthSyncTimeAdjust=(lengthSyncTime>128?128:lengthSyncTime);
            for (int i=0;i<lengthSyncTimeAdjust;i++){
                upperToLower.SyncTime[i]=contents.data[11+i];
            }
            if (lengthSyncTimeAdjust<128) upperToLower.SyncTime[lengthSyncTimeAdjust]='\0';
        }
    }
    else if (upperToLower.CurPhase == CHECK_DEVICE)
    {
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[i] = contents.data[6 + i];
            lengthCheckDevice = data32.u32;
        }
        else
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[3 - i] = contents.data[6 + i];
            lengthCheckDevice = data32.u32;
        }
        ResSerialize contentsTemp;
        contentsTemp.count=0;
        for (int i=0;i<lengthCheckDevice;i++){
            contentsTemp.data[i]=contents.data[i+10];
            contentsTemp.count++;
        }
        upperToLower.CheckDevice = copyCheckDevice(deSerializeCheckDevice(contentsTemp));
    }
    else if (upperToLower.CurPhase == RUN_FLOW)
    {
        //--------swirl--------
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[i] = contents.data[6 + i];
            lengthSwirl = data32.u32;
        }
        else
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[3 - i] = contents.data[6 + i];
            lengthSwirl = data32.u32;
        }
        if (lengthSwirl>0){
            for (int i = 0; i < lengthSwirl/(4+sizeofSwirl); i++)
            {
                ResSerialize contentsTemp;
                contentsTemp.count=0;
                for (int j=10+i*(4+sizeofSwirl)+4;j<contents.count;j++){
                    contentsTemp.data[j-(10+i*(4+sizeofSwirl)+4)]=contents.data[j];
                    contentsTemp.count++;
                }
                if (contentsTemp.count<=sizeofSwirl) continue;
                upperToLower.Swirl[i] = copySwirl(deSerializeSwirl(contentsTemp));
            }
        }
        //------balance--------
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[i] = contents.data[10+lengthSwirl + i];
            lengthBalance = data32.u32;
        }
        else
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[3 - i] = contents.data[10+lengthSwirl + i];
            lengthBalance = data32.u32;
        }
        if (lengthBalance>0){
            for (int i=0;i<lengthBalance/(4+sizeofBalance);i++){
                ResSerialize contentsTemp;
                contentsTemp.count=0;
                for (int j=14+lengthSwirl+i*(4+sizeofBalance)+4;j<contents.count;j++){
                    contentsTemp.data[j-(14+lengthSwirl+i*(4+sizeofBalance)+4)]=contents.data[j];
                    contentsTemp.count++;  
                }
                if (contentsTemp.count<=sizeofBalance) continue;
                upperToLower.BalanceSample[i]=copyBalance(deSerializeBalance(contentsTemp));
            }
        }
        //------getsample--------
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[i] = contents.data[14+lengthSwirl+lengthBalance + i];
            lengthGetSample = data32.u32;
        }
        else
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[3 - i] = contents.data[14+lengthSwirl+lengthBalance + i];
            lengthGetSample = data32.u32;
        }
        if (lengthGetSample>0){
            for (int i=0;i<lengthGetSample/(4+sizeofGetSample);i++){
                ResSerialize contentsTemp;
                contentsTemp.count=0;
                for (int j=18+lengthSwirl+lengthBalance+i*(4+sizeofGetSample)+4;j<contents.count;j++){
                    contentsTemp.data[j-(18+lengthSwirl+lengthBalance+i*(4+sizeofGetSample)+4)]=contents.data[j];
                    contentsTemp.count++;  
                }
                if (contentsTemp.count<=sizeofGetSample) continue;
                upperToLower.GetSample[i]=copyGetSample(deSerializeGetSample(contentsTemp));
            }
        }
        //------injectsample--------
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[i] = contents.data[18+lengthSwirl+lengthBalance+lengthGetSample + i];
            lengthInjectSample = data32.u32;
        }
        else
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[3 - i] = contents.data[18+lengthSwirl+lengthBalance+lengthGetSample + i];
            lengthInjectSample = data32.u32;
        }
        if (lengthInjectSample>0){
            for (int i=0;i<lengthInjectSample/(4+sizeofInjectSample);i++){
                ResSerialize contentsTemp;
                contentsTemp.count=0;
                for (int j=22+lengthSwirl+lengthBalance+lengthGetSample+i*(4+sizeofInjectSample)+4;j<contents.count;j++){
                    contentsTemp.data[j-(22+lengthSwirl+lengthBalance+lengthGetSample+i*(4+sizeofInjectSample)+4)]=contents.data[j];
                    contentsTemp.count++;  
                }
                if (contentsTemp.count<=sizeofInjectSample) continue;
                upperToLower.InjectSample[i]=copyInjectSample(deSerializeInjectSample(contentsTemp));
            }
        }
        //------detect--------
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[i] = contents.data[22+lengthSwirl+lengthBalance+lengthGetSample+lengthInjectSample + i];
            lengthDetect = data32.u32;
        }
        else
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[3 - i] = contents.data[22+lengthSwirl+lengthBalance+lengthGetSample+lengthInjectSample + i];
            lengthDetect = data32.u32;
        }
        if (lengthDetect>0){
            for (int i=0;i<lengthDetect/(4+sizeofDetectSample);i++){
                ResSerialize contentsTemp;
                contentsTemp.count=0;
                for (int j=26+lengthSwirl+lengthBalance+lengthGetSample+lengthInjectSample+i*(4+sizeofDetectSample)+4;j<contents.count;j++){
                    contentsTemp.data[j-(26+lengthSwirl+lengthBalance+lengthGetSample+lengthInjectSample+i*(4+sizeofDetectSample)+4)]=contents.data[j];
                    contentsTemp.count++;  
                }
                if (contentsTemp.count<=sizeofDetectSample) continue;
                upperToLower.DetectSample[i]=copyDetect(deSerializeDetectSample(contentsTemp));
            }
        }
    }
    else if (upperToLower.CurPhase == INTERACTE_DATA)
    {   
        if (SMALLER_SAVE)
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[i] = contents.data[6 + i];
            lengthAnalysisData = data32.u32;
        }
        else
        {
            for (int i = 0; i < 4; i++)
                data32.u8_32[3 - i] = contents.data[6 + i];
            lengthAnalysisData = data32.u32;
        }
        ResSerialize contentsTemp;
        contentsTemp.count=0;
        for (int i=0;i<lengthAnalysisData;i++){
            contentsTemp.data[i]=contents.data[i+10];
            contentsTemp.count++;
        }
        upperToLower.AnalysisData=deSerializeAnalysisData(contentsTemp);
    }
    return upperToLower;
}
