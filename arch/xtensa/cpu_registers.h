#include "cpu-defs.h"

// REMARK: we use #ifdef/#endif, #ifdef/#endif instead of #ifdef/#else/#endif notation due to the limitation of `RegisterEnumParser.cs`
typedef enum {
    PC_32           = 0,
    AR_0_32         = 1,
    AR_1_32         = 2,
    AR_2_32         = 3,
    AR_3_32         = 4,
    AR_4_32         = 5,
    AR_5_32         = 6,
    AR_6_32         = 7,
    AR_7_32         = 8,
    AR_8_32         = 9,
    AR_9_32         = 10,
    AR_10_32        = 11,
    AR_11_32        = 12,
    AR_12_32        = 13,
    AR_13_32        = 14,
    AR_14_32        = 15,
    AR_15_32        = 16,
    AR_16_32        = 17,
    AR_17_32        = 18,
    AR_18_32        = 19,
    AR_19_32        = 20,
    AR_20_32        = 21,
    AR_21_32        = 22,
    AR_22_32        = 23,
    AR_23_32        = 24,
    AR_24_32        = 25,
    AR_25_32        = 26,
    AR_26_32        = 27,
    AR_27_32        = 28,
    AR_28_32        = 29,
    AR_29_32        = 30,
    AR_30_32        = 31,
    AR_31_32        = 32,
    SAR_32          = 33,
    WINDOWBASE_32   = 34,
    WINDOWSTART_32  = 35,
    CONFIGID_0_32   = 36,
    CONFIGID_1_32   = 37,
    PS_32           = 38,
    SCOMPARE_1_32   = 39,
    EXPSTATE_32     = 40,
    MMID_32         = 41,
    IBREAKENABLE_32 = 42,
    ATOMCTL_32      = 43,
    DDR_32          = 44,
    IBREAKA_0_32    = 45,
    IBREAKA_1_32    = 46,
    DBREAKA_0_32    = 47,
    DBREAKA_1_32    = 48,
    DBREAKC_0_32    = 49,
    DBREAKC_1_32    = 50,
    EPC_1_32        = 51,
    EPC_2_32        = 52,
    EPC_3_32        = 53,
    EPC_4_32        = 54,
    EPC_5_32        = 55,
    EPC_6_32        = 56,
    EPC_7_32        = 57,
    DEPC_32         = 58,
    EPS_2_32        = 59,
    EPS_3_32        = 60,
    EPS_4_32        = 61,
    EPS_5_32        = 62,
    EPS_6_32        = 63,
    EPS_7_32        = 64,
    EXCSAVE_1_32    = 65,
    EXCSAVE_2_32    = 66,
    EXCSAVE_3_32    = 67,
    EXCSAVE_4_32    = 68,
    EXCSAVE_5_32    = 69,
    EXCSAVE_6_32    = 70,
    EXCSAVE_7_32    = 71,
    INTERRUPT_32    = 72,
    INTSET_32       = 73,
    INTCLEAR_32     = 74,
    INTENABLE_32    = 75,
    VECBASE_32      = 76,
    EXCCAUSE_32     = 77,
    DEBUGCAUSE_32   = 78,
    CCOUNT_32       = 79,
    PRID_32         = 80,
    ICOUNT_32       = 81,
    ICOUNTLEVEL_32  = 82,
    EXCVADDR_32     = 83,
    CCOMPARE_0_32   = 84,
    CCOMPARE_1_32   = 85,
    CCOMPARE_2_32   = 86,
    MISC_0_32       = 87,
    MISC_1_32       = 88,
    A_0_32          = 89,
    A_1_32          = 90,
    A_2_32          = 91,
    A_3_32          = 92,
    A_4_32          = 93,
    A_5_32          = 94,
    A_6_32          = 95,
    A_7_32          = 96,
    A_8_32          = 97,
    A_9_32          = 98,
    A_10_32         = 99,
    A_11_32         = 100,
    A_12_32         = 101,
    A_13_32         = 102,
    A_14_32         = 103,
    A_15_32         = 104,
} Registers;