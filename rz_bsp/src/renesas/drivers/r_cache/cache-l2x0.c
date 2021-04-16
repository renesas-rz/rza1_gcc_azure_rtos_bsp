/*
 * arch/arm/mm/cache-l2x0.c - L210/L220 cache controller support
 *
 * Copyright (C) 2007 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
  */


//#include <asm/cacheflush.h>
#include "cache-l2x0.h"
#include "pl310_l2cc.h"
#include "r_typedefs.h"

/*
 * min() macros
 */
#define min(x, y) ({                \
    x < y ? x : y; })



static inline void __raw_writel(unsigned long val, volatile void *addr)
{
    __asm__ volatile ("str %1, %0"
             : "+Qo" (*(volatile unsigned long *)addr)
             : "r" (val)
             );
}

#define writel_relaxed(v,c)    __raw_writel(v,c)


#define CACHE_LINE_SIZE        32

static void *l2x0_base = (void*)0x3FFFF000;

static unsigned long l2x0_way_mask = (1 << 8) - 1;/* Bitmask of active ways */
static unsigned long l2x0_size = 0x20000l; /* 128 KBytes L2C-310 */
static unsigned long sync_reg_offset = L2X0_CACHE_SYNC;

static inline void cache_sync(void)
{
    uint8_t *base = l2x0_base;
    writel_relaxed(0, base + sync_reg_offset);
}

static inline void l2x0_clean_line(unsigned long addr)
{
    uint8_t *base = l2x0_base;
    writel_relaxed(addr, base + L2X0_CLEAN_LINE_PA);
}

static inline void l2x0_inv_line(unsigned long addr)
{
    uint8_t  *base = l2x0_base;
    writel_relaxed(addr, base + L2X0_INV_LINE_PA);
}

#ifdef CONFIG_PL310_ERRATA_588369
static inline void l2x0_flush_line(unsigned long addr)
{
    uint8_t *base = l2x0_base;

    /* Clean by PA followed by Invalidate by PA */
    writel_relaxed(addr, base + L2X0_CLEAN_LINE_PA);
    writel_relaxed(addr, base + L2X0_INV_LINE_PA);
}
#else
static inline void l2x0_flush_line(unsigned long addr)
{
    uint8_t *base = l2x0_base;
    writel_relaxed(addr, base + L2X0_CLEAN_INV_LINE_PA);
}
#endif


void l2x0_flush_all(void)
{
    /* clean all ways */
    writel_relaxed(l2x0_way_mask, (uint8_t *) l2x0_base + L2X0_CLEAN_INV_WAY);
    while(get_pl310_l2cc_InvalByWay() & 0xFF);
    cache_sync();
}


void l2x0_clean_all(void)
{
    /* clean all ways */
    writel_relaxed(l2x0_way_mask, (uint8_t *) l2x0_base + L2X0_CLEAN_WAY);
    while(get_pl310_l2cc_InvalByWay() & 0xFF);
    cache_sync();
}

void l2x0_inv_all(void)
{
    /* invalidate all ways */
    /* Invalidating when L2 is enabled is a nono */
    //BUG_ON(readl(l2x0_base + L2X0_CTRL) & L2X0_CTRL_EN);
    writel_relaxed(l2x0_way_mask, (uint8_t *) l2x0_base + L2X0_INV_WAY);
    while(get_pl310_l2cc_InvalByWay() & 0xFF);
    cache_sync();
}

void l2x0_inv_range(unsigned long start, unsigned long end)
{
    if (start & (CACHE_LINE_SIZE - 1)) {
        start &= (unsigned long)~(CACHE_LINE_SIZE - 1);
        l2x0_flush_line(start);
        start += CACHE_LINE_SIZE;
    }

    if (end & (CACHE_LINE_SIZE - 1)) {
        end &= (unsigned long)~(CACHE_LINE_SIZE - 1);
        l2x0_flush_line(end);
    }

    while (start < end) {
        unsigned long blk_end = start + min(end - start, 4096UL);

        while (start < blk_end) {
            l2x0_inv_line(start);
            start += CACHE_LINE_SIZE;
        }
    }
    cache_sync();
}

void l2x0_clean_range(unsigned long start, unsigned long end)
{
    if ((end - start) >= l2x0_size) {
        l2x0_clean_all();
        return;
    }

    start &= (unsigned long)~(CACHE_LINE_SIZE - 1);
    while (start < end) {
        unsigned long blk_end = start + min(end - start, 4096UL);

        while (start < blk_end) {
            l2x0_clean_line(start);
            start += CACHE_LINE_SIZE;
        }
    }
    cache_sync();
}

void l2x0_flush_range(unsigned long start, unsigned long end)
{
    if ((end - start) >= l2x0_size) {
        l2x0_flush_all();
        return;
    }

    start &= (unsigned long)~(CACHE_LINE_SIZE - 1);
    while (start < end) {
        unsigned long blk_end = start + min(end - start, 4096UL);

        while (start < blk_end) {
            l2x0_flush_line(start);
            start += CACHE_LINE_SIZE;
        }

    }
    cache_sync();
}

void l2x0_disable(void)
{
    l2x0_flush_all();
    writel_relaxed(0, (uint8_t *) l2x0_base + L2X0_CTRL);
}
