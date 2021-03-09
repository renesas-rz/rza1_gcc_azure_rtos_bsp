/**
 * @file compiler_port.h
 * @brief Compiler specific definitions
 *
 * @section License
 *
 * Copyright (C) 2010-2015 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.6.1
 **/

#ifndef _COMPILER_PORT_H
#define _COMPILER_PORT_H

//Dependencies
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

#include "r_typedefs.h"

//Types
typedef char char_t;
typedef signed int int_t;
typedef unsigned int uint_t;


#if !defined(RENESAS_APPLICATION_SYSTEM_INC_R_TYPEDEFS_H_) && !defined(USE_CHIBIOS)
   typedef int bool_t;
#endif

#if defined(__CC_ARM)
   #undef PRIu8
   #define PRIu8 "u"
   #undef PRIu16
   #define PRIu16 "u"
#endif

//GCC compiler?
#if defined(__GNUC__)
   #undef __start_packed
   #define __start_packed
   #undef __end_packed
   #define __end_packed __attribute__((__packed__))
//Keil MDK-ARM compiler?
#elif defined(__CC_ARM)
   #pragma anon_unions
   #undef __start_packed
   #define __start_packed __packed
   #undef __end_packed
   #define __end_packed
#endif
#endif
