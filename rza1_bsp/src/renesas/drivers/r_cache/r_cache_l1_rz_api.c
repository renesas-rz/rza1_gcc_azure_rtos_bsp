/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_cache_l1_rz_api.c
* Version      : 1.0
* Description  : Declaration of the ARMv7 L1 cache API function.
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
//#include "tmc_switch.h"
#include "Renesas_RZ_A1.h"
#include "core_ca.h"
#include "core_caFunc.h"

#define L1_CACHE_LINE_SIZE           (0x00000020u)
#define L1_CACHE_LINE_MASK           (0xFFFFFFE0u)


/*******************************************************************************
* Function Name: R_CACHE_L1_InvalidAll
* Description  : Invalidate whole of the PL310 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1_InvalidAll(void)
{
    __v7_inv_dcache_all();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1_InvalidAll
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1_CleanAll
* Description  : Clean&invalidate whole of the PL310 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1_CleanAll(void)
{
    __v7_clean_dcache_all();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1_CleanAll
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1_CleanInvalidAll
* Description  : Clean&invalidate whole of the PL310 cache.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_CACHE_L1_CleanInvalidAll(void)
{
    __v7_clean_inv_dcache_all();
    return;
}
/*******************************************************************************
End of function R_CACHE_L1_CleanInvalidAll
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1_InvalidLine
* Description  : Apply invalidate operation to the cache lines which is included
*                in the specified address range.
* Arguments    : line_addr -
*                    Starting address of cache operation (physical address).
*                size -
*                    The byte size from line_addr.
* Return Value : none
*******************************************************************************/
void R_CACHE_L1_InvalidLine(uint32_t line_addr, uint32_t size)
{
    uint32_t addr =     (line_addr & L1_CACHE_LINE_MASK);
    uint32_t end_addr = (line_addr + size);

    for ( ; addr < end_addr; addr += L1_CACHE_LINE_SIZE)
    {
        __v7_inv_dcache_mva(addr);
    }
    return;
}
/*******************************************************************************
End of function R_CACHE_L1_InvalidLine
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1_CleanLine
* Description  : Apply clean operation to the cache lines which is included
*                in the specified address range.
* Arguments    : line_addr -
*                    Starting address of cache operation (physical address).
*                size -
*                    The byte size from line_addr.
* Return Value : none
*******************************************************************************/
void R_CACHE_L1_CleanLine(uint32_t line_addr, uint32_t size)
{
    uint32_t addr =     (line_addr & L1_CACHE_LINE_MASK);
    uint32_t end_addr = (line_addr + size);

    for ( ; addr < end_addr; addr += L1_CACHE_LINE_SIZE)
    {
        __v7_clean_dcache_mva(addr);
    }
    return;
}
/*******************************************************************************
End of function R_CACHE_L1_CleanLine
*******************************************************************************/

/*******************************************************************************
* Function Name: R_CACHE_L1_CleanInvalidLine
* Description  : Apply clean&invalidate operation to the cache lines which is
*                included in the specified address range.
* Arguments    : line_addr -
*                    Starting address of cache operation (physical address).
*                size -
*                    The byte size from line_addr.
* Return Value : none
*******************************************************************************/
void R_CACHE_L1_CleanInvalidLine(uint32_t line_addr, uint32_t size)
{
    uint32_t addr =     (line_addr & L1_CACHE_LINE_MASK);
    uint32_t end_addr = (line_addr + size);

    for ( ; addr < end_addr; addr += L1_CACHE_LINE_SIZE)
    {
        __v7_clean_inv_dcache_mva(addr);
    }
    return;
}
/*******************************************************************************
End of function R_CACHE_L1_CleanInvalidLine
*******************************************************************************/

#if 1//SAWABE_20181121E
void R_CACHE_disable()
{
    __disable_caches();
    return;
}
void R_CACHE_enable()
{
    __enable_caches();
    return;
}

#endif
