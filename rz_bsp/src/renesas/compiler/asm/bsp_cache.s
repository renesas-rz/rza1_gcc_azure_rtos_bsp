//
//********************************************************************************************************
//                                          CACHE UTILITY FUNCTIONS
//
// File      : bsp_cache.s
// For       : Cortex A9 on the Renesas RZ
// Toolchain : ARMCC
//
// Notes : none.
//********************************************************************************************************
//

//********************************************************************************************************
//                                           MACROS AND DEFINIITIONS
//********************************************************************************************************

	// MMU C1
	.equ	CTRL_C1_M    , 0x0001
	.equ	CTRL_C1_A    , 0x0002
	.equ	CTRL_C1_C    , 0x0004
	.equ	CTRL_C1_W    , 0x0008
	.equ	CTRL_C1_S    , 0x0100
	.equ	CTRL_C1_R    , 0x0200
	.equ	CTRL_C1_Z    , 0x0800
	.equ	CTRL_C1_I    , 0x1000
	.equ	CTRL_C1_RR   , 0x4000

//    PRESERVE8

//    SECTION  BSP_Cache:CODE:NOROOT(2)

	.equ	CPU_ARM_REG_SCU_CTRL			, 0xF8F00000
	.equ	CPU_ARM_REG_SCU_INV     		, 0xF8F0000C

	.equ	CPU_ARM_L2C_REG1_CTRL   		, 0x3FFFF100
	.equ	CPU_ARM_L2C_REG1_AUX_CTRL		, 0x3FFFF104
	.equ	CPU_ARM_L2C_REG1_TAG_RAM_CTRL   , 0x3FFFF108
	.equ	CPU_ARM_L2C_REG1_DATA_RAM_CTRL  , 0x3FFFF10C
	.equ	CPU_ARM_L2C_REG15_PWR_CTRL      , 0x3FFFFF80

	.equ	CPU_ARM_L2C_REG7_CACHE_SYNC     , 0x3FFFF730
	.equ	CPU_ARM_L2C_REG7_CACHE_INV_PA   , 0x3FFFF770
	.equ	CPU_ARM_L2C_REG7_CACHE_INV_WAY  , 0x3FFFF77C
	.equ	CPU_ARM_L2C_REG7_CACHE_CLEAN_PA , 0x3FFFF7B0

//********************************************************************************************************
//                                      BSP_DCacheInvalidateAll()
//
// Description : Invalidate L1 & L2 data cache. Used during initialisation sequence.
//
// Prototypes  : void  BSP_DCacheInvalidateAll (void)
//
// Argument(s) : none.
//********************************************************************************************************

    .global BSP_DCacheInvalidateAll

BSP_DCacheInvalidateAll:

                                    // Invalidate L1 data cache
  LDR     r0, =0x1FE                // Load set index
BSP_DCacheInvalidateAll_loop_1:
  MOV     r1, #0x00000003           // Load number of ways
BSP_DCacheInvalidateAll_loop_2:
  MOV     r2, r1, LSL #30
  ADD     r2, r2, r0, LSL #4
  MCR     p15, 0, r2, c7, c6, 2
  SUBS    r1, r1, #1
  BGE     BSP_DCacheInvalidateAll_loop_2
  SUBS    r0, r0, #1
  BGE     BSP_DCacheInvalidateAll_loop_1
  DSB
                                    // Invalidate L2 unified cache
  LDR     r0, =CPU_ARM_L2C_REG7_CACHE_INV_WAY
  MOV     r1, #0xFF00
  STR     r1, [r0]
  LDR     r0, =CPU_ARM_L2C_REG7_CACHE_SYNC
  MOV     r1, #1
  STR     r1, [r0]
BSP_DCacheInvalidateAll_loop_3:
  LDR     r2, [r0]
  ANDS    r2, r2, #1
  BNE     BSP_DCacheInvalidateAll_loop_3
  DSB
  ISB

  BX      lr

//********************************************************************************************************
//                                         BSP_L2C310Config()
//
// Description : Set tag and data RAM access latency. Used during initialisation sequence.
//
// Prototypes  : void  BSP_L2C310Config (void)
//
// Argument(s) : none.
//********************************************************************************************************

    .global BSP_L2C310Config

BSP_L2C310Config:
    LDR     r0, =CPU_ARM_L2C_REG1_TAG_RAM_CTRL
    LDR     r1, =0x00000111
    STR     r1, [r0]

    LDR     r0, =CPU_ARM_L2C_REG1_DATA_RAM_CTRL
    LDR     r1, =0x00000121
    STR     r1, [r0]

    LDR     r0, =CPU_ARM_L2C_REG1_AUX_CTRL
    LDR     r1, =(0x02060000 | 0x40000000 | 0x10000000 | 0x20000000 | 0x1)
    STR     r1, [r0]

    LDR     r0, =CPU_ARM_L2C_REG15_PWR_CTRL
    MOV     r1, #0x00000000
    STR     r1, [r0]

    DSB

    BX      lr

//********************************************************************************************************
//                                           BSP_CachesEn()
//
// Description : Enable L1 Data and Instruction cache as well as the L2 unified cache.
//
// Prototypes  : void  BSP_L2C310Config (void)
//
// Argument(s) : None
//********************************************************************************************************

    .global BSP_CachesEn

BSP_CachesEn:
    MRC     p15, 0, r0, c1, c0, 1
    ORR     r0, r0, #0xE
    MCR     p15, 0, r0, c1, c0, 1

    MRC     p15, 0, r0, c1, c0, 0
    ORR     r0, r0, #CTRL_C1_C
    ORR     r0, r0, #CTRL_C1_I
    MCR     p15, 0, r0, c1, c0, 0
    DSB
    ISB

    LDR     r0, =CPU_ARM_L2C_REG1_CTRL
    MOV     r1, #1
    STR     r1, [r0]

    DSB

	BX      lr

	.global BSP_CachesIEn

BSP_CachesIEn:
    MRC     p15, 0, r0, c1, c0, 0
    ORR     r0, r0, #CTRL_C1_I
    MCR     p15, 0, r0, c1, c0, 0
    DSB
    ISB

    BX      lr
//********************************************************************************************************
//                                       BSP_BranchPredictorEn()
//
// Description : Enable branch prediction.
//
// Prototypes  : void  BSP_BranchPredictorEn (void)
//
// Argument(s) : None
//********************************************************************************************************

    .global BSP_BranchPredictorEn

BSP_BranchPredictorEn:
   MRC     p15, 0, r0, c1, c0, 0
   ORR     r0, r0, #CTRL_C1_Z
   MCR     p15, 0,r0, c1, c0, 0
   DSB

   BX      lr

//********************************************************************************************************
//                                      INVALIDATE DATA CACHE RANGE
//
// Description : Invalidate a range of data cache by MVA.
//
// Prototypes  : void  BSP_DCache_InvalidateRange  (void       *p_mem,
//                                                  CPU_SIZE_T  range);
//
// Argument(s) : p_mem    Start address of the region to invalidate.
//
//               range    Size of the region to invalidate in bytes.
//
// Note(s)     : (1) p_mem value not aligned to 32 bytes will be truncated to the next lowest aligned
//                   address.
//
//               (2) range value not a multiple of 32 will be the next multiple of 32 that includes
//                   the start address and the end of the specified range.
//********************************************************************************************************

    .global BSP_DCache_InvalidateRange

BSP_DCache_InvalidateRange:
    MOV 	r3, r0
    ADD 	r1, r1, r0
    BIC 	r3, r3, #31

    LDR    	r2, =CPU_ARM_L2C_REG7_CACHE_INV_PA
Invalidate_RangeL2:
    STR 	r3, [r2]
    ADD 	r3, r3, #32
    CMP 	r3, r1
    BLT 	Invalidate_RangeL2
    DSB

    MOV 	r3, r0
    BIC 	r3, r3, #31
Invalidate_RangeL1:
    MCR 	p15,0, r3, c7, c6, 1
    ADD 	r3, r3, #32
    CMP 	r3, r1
    BLT 	Invalidate_RangeL1
    DSB
    BX 		LR

//********************************************************************************************************
//                                       FLUSH DATA CACHE RANGE
//
// Description : Flush (clean) a range of data cache by MVA.
//
// Prototypes  : void  BSP_DCache_FlushRange  (void       *p_mem,
//                                             CPU_SIZE_T  range);
//
// Argument(s) : p_mem    Start address of the region to flush.
//
//               range    Size of the region to invalidate in bytes.
//
// Note(s)     : (1) p_mem value not aligned to 32 bytes will be truncated to the next lowest aligned
//                   address.
//
//               (2) range value not a multiple of 32 will be the next multiple of 32 that includes
//                   the start address and the end of the specified range.
//********************************************************************************************************

    .global BSP_DCache_FlushRange

BSP_DCache_FlushRange:
    DSB
    MOV 	r3, r0
    ADD 	r1, r1, r0
    BIC 	r3, r3, #31

Flush_RangeL1:
    MCR 	p15, 0, r3, c7, c14, 1
    ADD 	r3, r3, #32
    CMP 	r3, r1
    BLT 	Flush_RangeL1
    DSB

    MOV 	r3, r0
    BIC 	r3, r3, #31
    LDR    	r2, =CPU_ARM_L2C_REG7_CACHE_CLEAN_PA
Flush_RangeL2:
    STR 	r3, [r2]
    ADD 	r3, r3, #32
    CMP 	r3, r1
    BLT 	Flush_RangeL2
    DSB

    BX 		LR

//    END
