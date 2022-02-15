// *****************************************************************************//
// 版    权 : 深圳市高科润电子有限公司所有(2030)
// 文件名称 : GCE_Data_Type_V2.h
// 功能描述 : 数据类型定义
// 作    者 : 曾海更
// 创建日期 : 2021.6.30
// 版    本 : V1
// 变更记录 : V1/曾海更/2021.6.30
//            1.首次创建
//            V2/曾海更/2021.08.13
//            1.由部分宏定义与灵动芯片重定义,在重定义加前缀GCE_,比如
//              MAX(_x, _y改成GCE_MIN(_x, _y)
//******************************************************************************
//#include "General.h"

//******************************************************************************
//                                 macro definitions
//******************************************************************************
#ifndef _GCE_DATA_TYPE_V2_H_
#define _GCE_DATA_TYPE_V2_H_

//******************************************************************************
//                                 Includes
//******************************************************************************
#include "GCE_MCU_Type_V1.h" // 芯片类型配置(只读文件)

//---<< 字节基础数据类型定义 >>-------------------------------------------------
typedef signed long SI32;
typedef unsigned long UI32;
typedef signed short SI16;
typedef unsigned short UI16;
typedef signed char SI08;
typedef unsigned char UI08;

// 小端模式是指数据的高字节保存在内存的高地址,而数据的低字节保存在内存的低地址
#if (MCU_RAM_TYPE == LITTLE_ENDIAD)
//---<< 位基础数据类型定义 >>---------------------------------------------------
typedef struct Bit32_Def
{
    UI08 b0 : 1; // 8bit access LL
    UI08 b1 : 1; // 8bit access LH
    UI08 b2 : 1; // 8bit access HL
    UI08 b3 : 1; // 8bit access HL
    UI08 b4 : 1; // 8bit access HL
    UI08 b5 : 1; // 8bit access HL
    UI08 b6 : 1; // 8bit access HL
    UI08 b7 : 1; // 8bit access HL

    UI08 b8 : 1;  // 8bit access HL
    UI08 b9 : 1;  // 8bit access HL
    UI08 b10 : 1; // 8bit access HL
    UI08 b11 : 1; // 8bit access HL
    UI08 b12 : 1; // 8bit access HL
    UI08 b13 : 1; // 8bit access HL
    UI08 b14 : 1; // 8bit access HL
    UI08 b15 : 1; // 8bit access HL

    UI08 b16 : 1; // 8bit access LL
    UI08 b17 : 1; // 8bit access LH
    UI08 b18 : 1; // 8bit access HL
    UI08 b19 : 1; // 8bit access HL
    UI08 b20 : 1; // 8bit access HL
    UI08 b21 : 1; // 8bit access HL
    UI08 b22 : 1; // 8bit access HL
    UI08 b23 : 1; // 8bit access HL

    UI08 b24 : 1; // 8bit access HL
    UI08 b25 : 1; // 8bit access HL
    UI08 b26 : 1; // 8bit access HL
    UI08 b27 : 1; // 8bit access HL
    UI08 b28 : 1; // 8bit access HL
    UI08 b29 : 1; // 8bit access HL
    UI08 b30 : 1; // 8bit access HL
    UI08 b31 : 1;
} TS_Bit32_Def;

typedef struct Bit16_Def
{
    UI08 b0 : 1; // 8bit access LL
    UI08 b1 : 1; // 8bit access LH
    UI08 b2 : 1; // 8bit access HL
    UI08 b3 : 1; // 8bit access HL
    UI08 b4 : 1; // 8bit access HL
    UI08 b5 : 1; // 8bit access HL
    UI08 b6 : 1; // 8bit access HL
    UI08 b7 : 1; // 8bit access HL

    UI08 b8 : 1;  // 8bit access HL
    UI08 b9 : 1;  // 8bit access HL
    UI08 b10 : 1; // 8bit access HL
    UI08 b11 : 1; // 8bit access HL
    UI08 b12 : 1; // 8bit access HL
    UI08 b13 : 1; // 8bit access HL
    UI08 b14 : 1; // 8bit access HL
    UI08 b15 : 1; // 8bit access HL
} TS_Bit16_Def;

typedef struct Bit8_Def
{
    UI08 b0 : 1; // 8bit access LL
    UI08 b1 : 1; // 8bit access LH
    UI08 b2 : 1; // 8bit access HL
    UI08 b3 : 1; // 8bit access HL
    UI08 b4 : 1; // 8bit access HL
    UI08 b5 : 1; // 8bit access HL
    UI08 b6 : 1; // 8bit access HL
    UI08 b7 : 1; // 8bit access HL
} TS_Bit8_Def;

//---<< 无符号联合体基础数据类型定义 >>-----------------------------------------
typedef union UIbyte_Def
{
    UI08 byte;
    TS_Bit8_Def bit_;
} TU_UIbyte_Def;

typedef union UIword_Def
{
    UI16 word;
    struct
    {
        UI08 lo; // 16bit access L
        UI08 hi; // 16bit access H
    } byte;
    TS_Bit16_Def bit_;
} TU_UIword_Def;

typedef union UIlword_Def
{
    UI32 lword;
    struct
    {
        UI08 ll; // 16bit access L
        UI08 lh; // 16bit access H
        UI08 hl; // 16bit access L
        UI08 hh; // 16bit access H
    } byte;

    struct
    {
        UI16 lo; // 16bit access L
        UI16 hi; // 16bit access H
    } word;

    TS_Bit32_Def bit_;
} TU_UIlword_Def;

//---<< 有符号联合体基础数据类型定义 >>-----------------------------------------
typedef union SIbyte_Def
{
    SI08 byte;
    TS_Bit8_Def bit_;
} TU_SIbyte_Def;

typedef union SIword_Def
{
    SI16 word;
    struct
    {
        UI08 lo; // 16bit access L
        SI08 hi; // 16bit access H
    } byte;

    TS_Bit16_Def bit_;
} TU_SIword_Def;

typedef union SIlword_Def
{
    SI32 lword;

    struct
    {
        UI08 ll; // 16bit access L
        UI08 lh; // 16bit access H
        UI08 hl; // 16bit access L
        SI08 hh; // 16bit access H
    } byte;

    struct
    {
        UI16 lo; // 16bit access L
        SI16 hi; // 16bit access H
    } word;

    TS_Bit32_Def bit_;

} TU_SIlword_Def;

// 大端模式是指数据的高字节保存在内存的低地址,而数据的低字节保存在内存的高地址.
#else //#elif (MCU_RAM_TYPE == BIG_ENDIAD)
//---<< 位基础数据类型定义 >>---------------------------------------------------
typedef struct Bit32_Def
{
    UI08 b24 : 1; // 8bit access HL
    UI08 b25 : 1; // 8bit access HL
    UI08 b26 : 1; // 8bit access HL
    UI08 b27 : 1; // 8bit access HL
    UI08 b28 : 1; // 8bit access HL
    UI08 b29 : 1; // 8bit access HL
    UI08 b30 : 1; // 8bit access HL
    UI08 b31 : 1; // 8bit access HL

    UI08 b16 : 1; // 8bit access LL
    UI08 b17 : 1; // 8bit access LH
    UI08 b18 : 1; // 8bit access HL
    UI08 b19 : 1; // 8bit access HL
    UI08 b20 : 1; // 8bit access HL
    UI08 b21 : 1; // 8bit access HL
    UI08 b22 : 1; // 8bit access HL
    UI08 b23 : 1; // 8bit access HL

    UI08 b8 : 1;  // 8bit access HL
    UI08 b9 : 1;  // 8bit access HL
    UI08 b10 : 1; // 8bit access HL
    UI08 b11 : 1; // 8bit access HL
    UI08 b12 : 1; // 8bit access HL
    UI08 b13 : 1; // 8bit access HL
    UI08 b14 : 1; // 8bit access HL
    UI08 b15 : 1; // 8bit access HL

    UI08 b0 : 1; // 8bit access LL
    UI08 b1 : 1; // 8bit access LH
    UI08 b2 : 1; // 8bit access HL
    UI08 b3 : 1; // 8bit access HL
    UI08 b4 : 1; // 8bit access HL
    UI08 b5 : 1; // 8bit access HL
    UI08 b6 : 1; // 8bit access HL
    UI08 b7 : 1; // 8bit access HL

} TS_Bit32_Def;

typedef struct Bit16_Def
{
    UI08 b8 : 1;  // 8bit access HL
    UI08 b9 : 1;  // 8bit access HL
    UI08 b10 : 1; // 8bit access HL
    UI08 b11 : 1; // 8bit access HL
    UI08 b12 : 1; // 8bit access HL
    UI08 b13 : 1; // 8bit access HL
    UI08 b14 : 1; // 8bit access HL
    UI08 b15 : 1; // 8bit access HL

    UI08 b0 : 1; // 8bit access LL
    UI08 b1 : 1; // 8bit access LH
    UI08 b2 : 1; // 8bit access HL
    UI08 b3 : 1; // 8bit access HL
    UI08 b4 : 1; // 8bit access HL
    UI08 b5 : 1; // 8bit access HL
    UI08 b6 : 1; // 8bit access HL
    UI08 b7 : 1; // 8bit access HL
} TS_Bit16_Def;

typedef struct Bit8_Def
{
    UI08 b0 : 1; // 8bit access LL
    UI08 b1 : 1; // 8bit access LH
    UI08 b2 : 1; // 8bit access HL
    UI08 b3 : 1; // 8bit access HL
    UI08 b4 : 1; // 8bit access HL
    UI08 b5 : 1; // 8bit access HL
    UI08 b6 : 1; // 8bit access HL
    UI08 b7 : 1; // 8bit access HL
} TS_Bit8_Def;

//---<< 无符号联合体基础数据类型定义 >>-----------------------------------------
typedef union UIbyte_Def
{
    UI08 byte;
    TS_Bit8_Def bit_;
} TU_UIbyte_Def;

typedef union UIword_Def
{
    UI16 word;
    struct
    {
        UI08 hi; // 16bit access H
        UI08 lo; // 16bit access L
    } byte;
    TS_Bit16_Def bit_;
} TU_UIword_Def;

typedef union UIlword_Def
{
    UI32 lword;
    struct
    {
        UI08 hh; // 16bit access H
        UI08 hl; // 16bit access L
        UI08 lh; // 16bit access H
        UI08 ll; // 16bit access L
    } byte;

    struct
    {
        UI16 hi; // 16bit access H
        UI16 lo; // 16bit access L
    } word;

    TS_Bit32_Def bit_;
} TU_UIlword_Def;

//---<< 有符号联合体基础数据类型定义 >>-----------------------------------------
typedef union SIbyte_Def
{
    SI08 byte;
    TS_Bit8_Def bit_;
} TU_SIbyte_Def;

typedef union SIword_Def
{
    SI16 word;
    struct
    {
        SI08 hi; // 16bit access H
        UI08 lo; // 16bit access L
    } byte;

    TS_Bit16_Def bit_;
} TU_SIword_Def;

typedef union SIlword_Def
{
    SI32 lword;

    struct
    {
        SI08 hh; // 16bit access H
        UI08 hl; // 16bit access L
        UI08 lh; // 16bit access H
        UI08 ll; // 16bit access L
    } byte;

    struct
    {
        SI16 hi; // 16bit access H
        UI16 lo; // 16bit access L
    } word;

    TS_Bit32_Def bit_;

} TU_SIlword_Def;
#endif
//---<< 联合体数据类型定义 >>---------------------------------------------------
typedef TU_UIbyte_Def UUI08;
typedef TU_UIword_Def UUI16;
typedef TU_UIlword_Def UUI32;

typedef TU_SIbyte_Def USI08;
typedef TU_SIword_Def USI16;
typedef TU_SIlword_Def USI32;

//---<< 内核数据类型定义 >>-----------------------------------------------------
#if (_CORE_TYPE == _CORE_C51)
#define GCE_CONST code
#define GCE_XDATA xdata
#define GCE_IDATA idata
#else
#define GCE_CONST const
#define GCE_XDATA
#define GCE_IDATA
#endif

//---<< 位宏定义 >>-------------------------------------------------------------
#define bit0 0x00000001
#define bit1 0x00000002
#define bit2 0x00000004
#define bit3 0x00000008

#define bit4 0x00000010
#define bit5 0x00000020
#define bit6 0x00000040
#define bit7 0x00000080

#define bit8 0x00000100
#define bit9 0x00000200
#define bit10 0x00000400
#define bit11 0x00000800

#define bit12 0x00001000
#define bit13 0x00002000
#define bit14 0x00004000
#define bit15 0x00008000

#define bit16 0x00010000
#define bit17 0x00020000
#define bit18 0x00040000
#define bit19 0x00080000

#define bit20 0x00100000
#define bit21 0x00200000
#define bit22 0x00400000
#define bit23 0x00800000

#define bit24 0x01000000
#define bit25 0x02000000
#define bit26 0x04000000
#define bit27 0x08000000

#define bit28 0x10000000
#define bit29 0x20000000
#define bit30 0x40000000
#define bit31 0x80000000

//---<< 二进制宏定义 >>---------------------------------------------------------
#define _0000_0000 0x00
#define _0000_0001 0x01
#define _0000_0010 0x02
#define _0000_0011 0x03
#define _0000_0100 0x04
#define _0000_0101 0x05
#define _0000_0110 0x06
#define _0000_0111 0x07
#define _0000_1000 0x08
#define _0000_1001 0x09
#define _0000_1010 0x0A
#define _0000_1011 0x0B
#define _0000_1100 0x0C
#define _0000_1101 0x0D
#define _0000_1110 0x0E
#define _0000_1111 0x0F
#define _0001_0000 0x10
#define _0001_0001 0x11
#define _0001_0010 0x12
#define _0001_0011 0x13
#define _0001_0100 0x14
#define _0001_0101 0x15
#define _0001_0110 0x16
#define _0001_0111 0x17
#define _0001_1000 0x18
#define _0001_1001 0x19
#define _0001_1010 0x1A
#define _0001_1011 0x1B
#define _0001_1100 0x1C
#define _0001_1101 0x1D
#define _0001_1110 0x1E
#define _0001_1111 0x1F
#define _0010_0000 0x20
#define _0010_0001 0x21
#define _0010_0010 0x22
#define _0010_0011 0x23
#define _0010_0100 0x24
#define _0010_0101 0x25
#define _0010_0110 0x26
#define _0010_0111 0x27
#define _0010_1000 0x28
#define _0010_1001 0x29
#define _0010_1010 0x2A
#define _0010_1011 0x2B
#define _0010_1100 0x2C
#define _0010_1101 0x2D
#define _0010_1110 0x2E
#define _0010_1111 0x2F
#define _0011_0000 0x30
#define _0011_0001 0x31
#define _0011_0010 0x32
#define _0011_0011 0x33
#define _0011_0100 0x34
#define _0011_0101 0x35
#define _0011_0110 0x36
#define _0011_0111 0x37
#define _0011_1000 0x38
#define _0011_1001 0x39
#define _0011_1010 0x3A
#define _0011_1011 0x3B
#define _0011_1100 0x3C
#define _0011_1101 0x3D
#define _0011_1110 0x3E
#define _0011_1111 0x3F
#define _0100_0000 0x40
#define _0100_0001 0x41
#define _0100_0010 0x42
#define _0100_0011 0x43
#define _0100_0100 0x44
#define _0100_0101 0x45
#define _0100_0110 0x46
#define _0100_0111 0x47
#define _0100_1000 0x48
#define _0100_1001 0x49
#define _0100_1010 0x4A
#define _0100_1011 0x4B
#define _0100_1100 0x4C
#define _0100_1101 0x4D
#define _0100_1110 0x4E
#define _0100_1111 0x4F
#define _0101_0000 0x50
#define _0101_0001 0x51
#define _0101_0010 0x52
#define _0101_0011 0x53
#define _0101_0100 0x54
#define _0101_0101 0x55
#define _0101_0110 0x56
#define _0101_0111 0x57
#define _0101_1000 0x58
#define _0101_1001 0x59
#define _0101_1010 0x5A
#define _0101_1011 0x5B
#define _0101_1100 0x5C
#define _0101_1101 0x5D
#define _0101_1110 0x5E
#define _0101_1111 0x5F
#define _0110_0000 0x60
#define _0110_0001 0x61
#define _0110_0010 0x62
#define _0110_0011 0x63
#define _0110_0100 0x64
#define _0110_0101 0x65
#define _0110_0110 0x66
#define _0110_0111 0x67
#define _0110_1000 0x68
#define _0110_1001 0x69
#define _0110_1010 0x6A
#define _0110_1011 0x6B
#define _0110_1100 0x6C
#define _0110_1101 0x6D
#define _0110_1110 0x6E
#define _0110_1111 0x6F
#define _0111_0000 0x70
#define _0111_0001 0x71
#define _0111_0010 0x72
#define _0111_0011 0x73
#define _0111_0100 0x74
#define _0111_0101 0x75
#define _0111_0110 0x76
#define _0111_0111 0x77
#define _0111_1000 0x78
#define _0111_1001 0x79
#define _0111_1010 0x7A
#define _0111_1011 0x7B
#define _0111_1100 0x7C
#define _0111_1101 0x7D
#define _0111_1110 0x7E
#define _0111_1111 0x7F
#define _1000_0000 0x80
#define _1000_0001 0x81
#define _1000_0010 0x82
#define _1000_0011 0x83
#define _1000_0100 0x84
#define _1000_0101 0x85
#define _1000_0110 0x86
#define _1000_0111 0x87
#define _1000_1000 0x88
#define _1000_1001 0x89
#define _1000_1010 0x8A
#define _1000_1011 0x8B
#define _1000_1100 0x8C
#define _1000_1101 0x8D
#define _1000_1110 0x8E
#define _1000_1111 0x8F
#define _1001_0000 0x90
#define _1001_0001 0x91
#define _1001_0010 0x92
#define _1001_0011 0x93
#define _1001_0100 0x94
#define _1001_0101 0x95
#define _1001_0110 0x96
#define _1001_0111 0x97
#define _1001_1000 0x98
#define _1001_1001 0x99
#define _1001_1010 0x9A
#define _1001_1011 0x9B
#define _1001_1100 0x9C
#define _1001_1101 0x9D
#define _1001_1110 0x9E
#define _1001_1111 0x9F
#define _1010_0000 0xA0
#define _1010_0001 0xA1
#define _1010_0010 0xA2
#define _1010_0011 0xA3
#define _1010_0100 0xA4
#define _1010_0101 0xA5
#define _1010_0110 0xA6
#define _1010_0111 0xA7
#define _1010_1000 0xA8
#define _1010_1001 0xA9
#define _1010_1010 0xAA
#define _1010_1011 0xAB
#define _1010_1100 0xAC
#define _1010_1101 0xAD
#define _1010_1110 0xAE
#define _1010_1111 0xAF
#define _1011_0000 0xB0
#define _1011_0001 0xB1
#define _1011_0010 0xB2
#define _1011_0011 0xB3
#define _1011_0100 0xB4
#define _1011_0101 0xB5
#define _1011_0110 0xB6
#define _1011_0111 0xB7
#define _1011_1000 0xB8
#define _1011_1001 0xB9
#define _1011_1010 0xBA
#define _1011_1011 0xBB
#define _1011_1100 0xBC
#define _1011_1101 0xBD
#define _1011_1110 0xBE
#define _1011_1111 0xBF
#define _1100_0000 0xC0
#define _1100_0001 0xC1
#define _1100_0010 0xC2
#define _1100_0011 0xC3
#define _1100_0100 0xC4
#define _1100_0101 0xC5
#define _1100_0110 0xC6
#define _1100_0111 0xC7
#define _1100_1000 0xC8
#define _1100_1001 0xC9
#define _1100_1010 0xCA
#define _1100_1011 0xCB
#define _1100_1100 0xCC
#define _1100_1101 0xCD
#define _1100_1110 0xCE
#define _1100_1111 0xCF
#define _1101_0000 0xD0
#define _1101_0001 0xD1
#define _1101_0010 0xD2
#define _1101_0011 0xD3
#define _1101_0100 0xD4
#define _1101_0101 0xD5
#define _1101_0110 0xD6
#define _1101_0111 0xD7
#define _1101_1000 0xD8
#define _1101_1001 0xD9
#define _1101_1010 0xDA
#define _1101_1011 0xDB
#define _1101_1100 0xDC
#define _1101_1101 0xDD
#define _1101_1110 0xDE
#define _1101_1111 0xDF
#define _1110_0000 0xE0
#define _1110_0001 0xE1
#define _1110_0010 0xE2
#define _1110_0011 0xE3
#define _1110_0100 0xE4
#define _1110_0101 0xE5
#define _1110_0110 0xE6
#define _1110_0111 0xE7
#define _1110_1000 0xE8
#define _1110_1001 0xE9
#define _1110_1010 0xEA
#define _1110_1011 0xEB
#define _1110_1100 0xEC
#define _1110_1101 0xED
#define _1110_1110 0xEE
#define _1110_1111 0xEF
#define _1111_0000 0xF0
#define _1111_0001 0xF1
#define _1111_0010 0xF2
#define _1111_0011 0xF3
#define _1111_0100 0xF4
#define _1111_0101 0xF5
#define _1111_0110 0xF6
#define _1111_0111 0xF7
#define _1111_1000 0xF8
#define _1111_1001 0xF9
#define _1111_1010 0xFA
#define _1111_1011 0xFB
#define _1111_1100 0xFC
#define _1111_1101 0xFD
#define _1111_1110 0xFE
#define _1111_1111 0xFF

//---<< 通用宏定义 >>---------------------------------------------------------
// 使能/关闭 定义
#define GCE_DISABLE (0)
#define GCE_ENABLE (1)

// 高/低 定义
#define GCE_LOW (0)
#define GCE_HIGH (1)

// 开/关 定义
#define GCE_OFF (0)
#define GCE_ON (1)

// 错误/成功 定义
#define GCE_ERROR (0)
#define GCE_SUCCESS (1)

// 假/真 定义
#define GCE_FALSE (0)
#define GCE_TRUE (1)

//--------------------------------------
// 无符号最大值
#define UI08_MAX (255)
#define UI16_MAX (65535u)
#define UI32_MAX (4294967295uL)

// 有符号最大值
#define SI08_MAX (127)
#define SI16_MAX (32767)
#define SIT32_MAX (2147483647)

// 有符号最小值
#define SI08_MIN (-128)
#define SI16_MIN (-32768)
#define SI32_MIN (-2147483648uL)

//--------------------------------------
// 时间:秒,分,小时转换成对应的秒
#define GCE_TIME_SSEC(_t) ((_t))       // 单位: 秒
#define GCE_TIME_SMIN(_t) ((_t)*60)    // 单位: 秒
#define GCE_TIME_SHOUR(_t) ((_t)*3600) // 单位: 秒

// 时间:秒转换成对应的秒,分,小时
#define GCE_TIME_CSEC(_t) ((_t))         // 单位: 秒
#define GCE_TIME_CMIN(_t) ((_t) / 60)    // 单位: 分钟
#define GCE_TIME_CHOUR(_t) ((_t) / 3600) // 单位: 小时

// 时间MS与S之间的转换
#define GCE_TIME_CS2MS(_t) ((_t)*1000)   // S->MS
#define GCE_TIME_CMS2S(_t) ((_t) / 1000) // MS->S

//--------------------------------------
// 获取最大最小值
#define GCE_MAX(_x, _y) (((_x) > (_y)) ? (_x) : (_y))
#define GCE_MIN(_x, _y) (((_x) < (_y)) ? (_x) : (_y))

// 位操作
#define GCE_SET_BIT(_var, _place) (_var) |= (1 << (_place))    // 在第_place位置1
#define GCE_CLR_BIT(_var, _place) (_var) &= (~(1 << (_place))) // 在第_place位置0
#define GCE_XOR_BIT(_var, _place) (_var) ^= (1 << (_place))    // 在第_place位取反
#define GCE_GET_BIT(_var, _place) ((_var >> (_place)) & 1)     // 返回第_place位结果
#define GCE_EQU_BIT(_var, _value) ((_var & _value) == _value)  // 判断_var是否等于_value

// 字节操作
#define GET_BYTE_0(_var) ((UI08)((_var) & (UI08)0xFF))        // 获取第1个字节
#define GET_BYTE_1(_var) ((UI08)(BYTE_0((_var) >> (UI08)8)))  // 获取第2个字节
#define GET_BYTE_2(_var) ((UI08)(BYTE_0((_var) >> (UI08)16))) // 获取第3个字节
#define GET_BYTE_3(_var) ((UI08)(BYTE_0((_var) >> (UI08)24))) // 获取第4个字节

// 检查十进制或十六进制的数据
#define GCE_DECCHK(_c) ((_c) >= '0' && (_c) <= '9')
#define GCE_HEXCHK(_c) (((_c) >= '0' && (_c) <= '9') || ((_c) >= 'A' && (_c) <= 'F') || ((_c) >= 'a' && (_c) <= 'f'))

// 数组与整型数据之间的转换
#define _2BYTE_C_16HEX(_pBuf) \
    (UI16)((*(_pBuf) << 8) | *(_pBuf + 1))

#define _3BYTE_C_24HEX(_pBuf) \
    (UI32)((*(_pBuf) << 16) | (*(_pBuf + 1) << 8) | *(_pBuf + 2))

#define _4BYTE_C_32HEX(_pBuf) \
    (UI32)((*(_pBuf) << 24) | (*(_pBuf + 1) << 16) | (*(_pBuf + 2) << 8) | *(_pBuf + 3))

#define _16HEX_C_2BYTES(_pBuf, Data16) \
    *(_pBuf) = (UI08)(Data16 >> 8);    \
    *(_pBuf + 1) = (UI08)(Data16)

#define _24HEX_C_3BYTES(_pBuf, Data24)  \
    *(_pBuf) = (UI08)(Data24 >> 16);    \
    *(_pBuf + 1) = (UI08)(Data24 >> 8); \
    *(_pBuf + 2) = (UI08)(Data24)

#define _32HEX_C_4BYTES(_pBuf, Data32)   \
    *(_pBuf) = (UI08)(Data32 >> 24);     \
    *(_pBuf + 1) = (UI08)(Data32 >> 16); \
    *(_pBuf + 2) = (UI08)(Data32 >> 8);  \
    *(_pBuf + 3) = (UI08)(Data32)

// 1字节对数函数(底数为a=2, y=log(a)(n))
#define BYTE1_LOG2(_n) \
    ((_n) == 256 ? 8 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 7 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 ? 6 : ((_n) == 2 * 2 * 2 * 2 * 2 ? 5 : ((_n) == 2 * 2 * 2 * 2 ? 4 : ((_n) == 2 * 2 * 2 ? 3 : ((_n) == 2 * 2 ? 2 : ((_n) == 2 ? 1 : ((_n) == 1 ? 0 : -1)))))))))

// 2字节对数函数(底数为a=2, y=log(a)(n))
#define BYTE2_LOG2(_n) \
    ((_n) == 65535 ? 16 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 15 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 14 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 13 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 12 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 11 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 10 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 9 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 8 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 * 2 ? 7 : ((_n) == 2 * 2 * 2 * 2 * 2 * 2 ? 6 : ((_n) == 2 * 2 * 2 * 2 * 2 ? 5 : ((_n) == 2 * 2 * 2 * 2 ? 4 : ((_n) == 2 * 2 * 2 ? 3 : ((_n) == 2 * 2 ? 2 : ((_n) == 2 ? 1 : ((_n) == 1 ? 0 : -1)))))))))))))))))

typedef enum
{
    AI_NORMAL = 0,
    AI_SHORT = 1,
    AI_CUT = 2
} TS_AISTATUS;

typedef enum
{
    OFF = 0,
    ON = !OFF
} ONOFF_STATUS;

typedef enum
{
    DI_SHORT = 0,
    DI_CUT = 1
} SHORTCUT_STATUS;

typedef struct DO_Para_Def
{
    UI16 on_time;
    UI16 off_time;
    ONOFF_STATUS BUF;
    ONOFF_STATUS OUT;
} TS_DO_Para_Def;

typedef struct DI_Para_Def
{
    SHORTCUT_STATUS Status;
    SHORTCUT_STATUS Status_buf;
    UI16 short_time;
    UI16 cut_time;
} TS_DI_Para_Def;

#endif
//******************************* End of File (H) ******************************
