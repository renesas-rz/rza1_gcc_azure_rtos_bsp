/*
 * r_intc_monitor.c
 *
 *  Created on: 5 Jun 2018
 *      Author: b3800117
 */


/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 *******************************************************************************/
/* definition of I/O register */
#include "iodefine_cfg.h"
#include "intc_iodefine.h"
#include "r_intc.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wild_compare.h>

#include "compiler_settings.h"

#include "console.h"
#include "control.h"
#include "version.h"

/******************************************************************************
 Prototypes
 ******************************************************************************/
void R_INTC_Display_TaskTable(FILE *p_out);
void R_INTC_Update_Isr_Log_Entry(uint16_t entry, uint8_t priority);


typedef struct _intc_tbl_t
{
    uint8_t level;
    const char   *const  strname;
} st_intc_tbl_t ;

static uint16_t sorted_tbl[INTC_ID_TOTAL]  = {};

static st_intc_tbl_t monitor [INTC_ID_TOTAL]  =
{
    { ISR_ENTRY_UNUSED, "SW0           " }, /* 0   : SW0           */
    { ISR_ENTRY_UNUSED, "SW1           " }, /* 1   : SW1           */
    { ISR_ENTRY_UNUSED, "SW2           " }, /* 2   : SW2           */
    { ISR_ENTRY_UNUSED, "SW3           " }, /* 3   : SW3           */
    { ISR_ENTRY_UNUSED, "SW4           " }, /* 4   : SW4           */
    { ISR_ENTRY_UNUSED, "SW5           " }, /* 5   : SW5           */
    { ISR_ENTRY_UNUSED, "SW6           " }, /* 6   : SW6           */
    { ISR_ENTRY_UNUSED, "SW7           " }, /* 7   : SW7           */
    { ISR_ENTRY_UNUSED, "SW8           " }, /* 8   : SW8           */
    { ISR_ENTRY_UNUSED, "SW9           " }, /* 9   : SW9           */
    { ISR_ENTRY_UNUSED, "SW10          " }, /* 10  : SW10          */
    { ISR_ENTRY_UNUSED, "SW11          " }, /* 11  : SW11          */
    { ISR_ENTRY_UNUSED, "SW12          " }, /* 12  : SW12          */
    { ISR_ENTRY_UNUSED, "SW13          " }, /* 13  : SW13          */
    { ISR_ENTRY_UNUSED, "SW14          " }, /* 14  : SW14          */
    { ISR_ENTRY_UNUSED, "SW15          " }, /* 15  : SW15          */
    { ISR_ENTRY_UNUSED, "PMUIRQ0       " }, /* 16  : PMUIRQ0       */
    { ISR_ENTRY_UNUSED, "COMMRX0       " }, /* 17  : COMMRX0       */
    { ISR_ENTRY_UNUSED, "COMMTX0       " }, /* 18  : COMMTX0       */
    { ISR_ENTRY_UNUSED, "CTIIRQ0       " }, /* 19  : CTIIRQ0       */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 20  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 21  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 22  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 23  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 24  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 25  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 26  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 27  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 28  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 29  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 30  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 31  : <reserved>    */
    { ISR_ENTRY_UNUSED, "IRQ0          " }, /* 32  : IRQ0          */
    { ISR_ENTRY_UNUSED, "IRQ1          " }, /* 33  : IRQ1          */
    { ISR_ENTRY_UNUSED, "IRQ2          " }, /* 34  : IRQ2          */
    { ISR_ENTRY_UNUSED, "IRQ3          " }, /* 35  : IRQ3          */
    { ISR_ENTRY_UNUSED, "IRQ4          " }, /* 36  : IRQ4          */
    { ISR_ENTRY_UNUSED, "IRQ5          " }, /* 37  : IRQ5          */
    { ISR_ENTRY_UNUSED, "IRQ6          " }, /* 38  : IRQ6          */
    { ISR_ENTRY_UNUSED, "IRQ7          " }, /* 39  : IRQ7          */
    { ISR_ENTRY_UNUSED, "PL310ERR      " }, /* 40  : PL310ERR      */
    { ISR_ENTRY_UNUSED, "DMAINT0       " }, /* 41  : DMAINT0       */
    { ISR_ENTRY_UNUSED, "DMAINT1       " }, /* 42  : DMAINT1       */
    { ISR_ENTRY_UNUSED, "DMAINT2       " }, /* 43  : DMAINT2       */
    { ISR_ENTRY_UNUSED, "DMAINT3       " }, /* 44  : DMAINT3       */
    { ISR_ENTRY_UNUSED, "DMAINT4       " }, /* 45  : DMAINT4       */
    { ISR_ENTRY_UNUSED, "DMAINT5       " }, /* 46  : DMAINT5       */
    { ISR_ENTRY_UNUSED, "DMAINT6       " }, /* 47  : DMAINT6       */
    { ISR_ENTRY_UNUSED, "DMAINT7       " }, /* 48  : DMAINT7       */
    { ISR_ENTRY_UNUSED, "DMAINT8       " }, /* 49  : DMAINT8       */
    { ISR_ENTRY_UNUSED, "DMAINT9       " }, /* 50  : DMAINT9       */
    { ISR_ENTRY_UNUSED, "DMAINT10      " }, /* 51  : DMAINT10      */
    { ISR_ENTRY_UNUSED, "DMAINT11      " }, /* 52  : DMAINT11      */
    { ISR_ENTRY_UNUSED, "DMAINT12      " }, /* 53  : DMAINT12      */
    { ISR_ENTRY_UNUSED, "DMAINT13      " }, /* 54  : DMAINT13      */
    { ISR_ENTRY_UNUSED, "DMAINT14      " }, /* 55  : DMAINT14      */
    { ISR_ENTRY_UNUSED, "DMAINT15      " }, /* 56  : DMAINT15      */
    { ISR_ENTRY_UNUSED, "DMAERR        " }, /* 57  : DMAERR        */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 58  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 59  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 60  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 61  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 62  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 63  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 64  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 65  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 66  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 67  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 68  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 69  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 70  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 71  : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 72  : <reserved>    */
    { ISR_ENTRY_UNUSED, "USBI0         " }, /* 73  : USBI0         */
    { ISR_ENTRY_UNUSED, "USBI1         " }, /* 74  : USBI1         */
    { ISR_ENTRY_UNUSED, "S0_VI_VSYNC0  " }, /* 75  : S0_VI_VSYNC0  */
    { ISR_ENTRY_UNUSED, "S0_LO_VSYNC0  " }, /* 76  : S0_LO_VSYNC0  */
    { ISR_ENTRY_UNUSED, "S0_VSYNCERR0  " }, /* 77  : S0_VSYNCERR0  */
    { ISR_ENTRY_UNUSED, "GR3_VLINE0    " }, /* 78  : GR3_VLINE0    */
    { ISR_ENTRY_UNUSED, "S0_VFIELD0    " }, /* 79  : S0_VFIELD0    */
    { ISR_ENTRY_UNUSED, "IV1_VBUFERR0  " }, /* 80  : IV1_VBUFERR0  */
    { ISR_ENTRY_UNUSED, "IV3_VBUFERR0  " }, /* 81  : IV3_VBUFERR0  */
    { ISR_ENTRY_UNUSED, "IV5_VBUFERR0  " }, /* 82  : IV5_VBUFERR0  */
    { ISR_ENTRY_UNUSED, "IV6_VBUFERR0  " }, /* 83  : IV6_VBUFERR0  */
    { ISR_ENTRY_UNUSED, "S0_WLINE0     " }, /* 84  : S0_WLINE0     */
    { ISR_ENTRY_UNUSED, "S1_VI_VSYNC0  " }, /* 85  : S1_VI_VSYNC0  */
    { ISR_ENTRY_UNUSED, "S1_LO_VSYNC0  " }, /* 86  : S1_LO_VSYNC0  */
    { ISR_ENTRY_UNUSED, "S1_VSYNCERR0  " }, /* 87  : S1_VSYNCERR0  */
    { ISR_ENTRY_UNUSED, "S1_VFIELD0    " }, /* 88  : S1_VFIELD0    */
    { ISR_ENTRY_UNUSED, "IV2_VBUFERR0  " }, /* 89  : IV2_VBUFERR0  */
    { ISR_ENTRY_UNUSED, "IV4_VBUFERR0  " }, /* 90  : IV4_VBUFERR0  */
    { ISR_ENTRY_UNUSED, "S1_WLINE0     " }, /* 91  : S1_WLINE0     */
    { ISR_ENTRY_UNUSED, "OIR_VI_VSYNC0 " }, /* 92  : OIR_VI_VSYNC0 */
    { ISR_ENTRY_UNUSED, "OIR_LO_VSYNC0 " }, /* 93  : OIR_LO_VSYNC0 */
    { ISR_ENTRY_UNUSED, "OIR_VSYNCERR0 " }, /* 94  : OIR_VSYNCERR0 */
    { ISR_ENTRY_UNUSED, "OIR_VFIELD0   " }, /* 95  : OIR_VFIELD0   */
    { ISR_ENTRY_UNUSED, "IV7_VBUFERR0  " }, /* 96  : IV7_VBUFERR0  */
    { ISR_ENTRY_UNUSED, "IV8_VBUFERR0  " }, /* 97  : IV8_VBUFERR0  */
    { ISR_ENTRY_UNUSED, "OIR_WLINE0    " }, /* 98  : OIR_WLINE0    */
    { ISR_ENTRY_UNUSED, "S0_VI_VSYNC1  " }, /* 99  : S0_VI_VSYNC1  */
    { ISR_ENTRY_UNUSED, "S0_LO_VSYNC1  " }, /* 100 : S0_LO_VSYNC1  */
    { ISR_ENTRY_UNUSED, "S0_VSYNCERR1  " }, /* 101 : S0_VSYNCERR1  */
    { ISR_ENTRY_UNUSED, "GR3_VLINE1    " }, /* 102 : GR3_VLINE1    */
    { ISR_ENTRY_UNUSED, "S0_VFIELD1    " }, /* 103 : S0_VFIELD1    */
    { ISR_ENTRY_UNUSED, "IV1_VBUFERR1  " }, /* 104 : IV1_VBUFERR1  */
    { ISR_ENTRY_UNUSED, "IV3_VBUFERR1  " }, /* 105 : IV3_VBUFERR1  */
    { ISR_ENTRY_UNUSED, "IV5_VBUFERR1  " }, /* 106 : IV5_VBUFERR1  */
    { ISR_ENTRY_UNUSED, "IV6_VBUFERR1  " }, /* 107 : IV6_VBUFERR1  */
    { ISR_ENTRY_UNUSED, "S0_WLINE1     " }, /* 108 : S0_WLINE1     */
    { ISR_ENTRY_UNUSED, "S1_VI_VSYNC1  " }, /* 109 : S1_VI_VSYNC1  */
    { ISR_ENTRY_UNUSED, "S1_LO_VSYNC1  " }, /* 110 : S1_LO_VSYNC1  */
    { ISR_ENTRY_UNUSED, "S1_VSYNCERR1  " }, /* 111 : S1_VSYNCERR1  */
    { ISR_ENTRY_UNUSED, "S1_VFIELD1    " }, /* 112 : S1_VFIELD1    */
    { ISR_ENTRY_UNUSED, "IV2_VBUFERR1  " }, /* 113 : IV2_VBUFERR1  */
    { ISR_ENTRY_UNUSED, "IV4_VBUFERR1  " }, /* 114 : IV4_VBUFERR1  */
    { ISR_ENTRY_UNUSED, "S1_WLINE1     " }, /* 115 : S1_WLINE1     */
    { ISR_ENTRY_UNUSED, "OIR_VI_VSYNC1 " }, /* 116 : OIR_VI_VSYNC1 */
    { ISR_ENTRY_UNUSED, "OIR_LO_VSYNC1 " }, /* 117 : OIR_LO_VSYNC1 */
    { ISR_ENTRY_UNUSED, "OIR_VLINE1    " }, /* 118 : OIR_VLINE1    */
    { ISR_ENTRY_UNUSED, "OIR_VFIELD1   " }, /* 119 : OIR_VFIELD1   */
    { ISR_ENTRY_UNUSED, "IV7_VBUFERR1  " }, /* 120 : IV7_VBUFERR1  */
    { ISR_ENTRY_UNUSED, "IV8_VBUFERR1  " }, /* 121 : IV8_VBUFERR1  */
    { ISR_ENTRY_UNUSED, "OIR_WLINE1    " }, /* 122 : OIR_WLINE1    */
    { ISR_ENTRY_UNUSED, "IMRDI         " }, /* 123 : IMRDI         */
    { ISR_ENTRY_UNUSED, "IMR2I0        " }, /* 124 : IMR2I0        */
    { ISR_ENTRY_UNUSED, "IMR2I1        " }, /* 125 : IMR2I1        */
    { ISR_ENTRY_UNUSED, "JEDI          " }, /* 126 : JEDI          */
    { ISR_ENTRY_UNUSED, "JDTI          " }, /* 127 : JDTI          */
    { ISR_ENTRY_UNUSED, "CMP0          " }, /* 128 : CMP0          */
    { ISR_ENTRY_UNUSED, "CMP1          " }, /* 129 : CMP1          */
    { ISR_ENTRY_UNUSED, "INT0          " }, /* 130 : INT0          */
    { ISR_ENTRY_UNUSED, "INT1          " }, /* 131 : INT1          */
    { ISR_ENTRY_UNUSED, "INT2          " }, /* 132 : INT2          */
    { ISR_ENTRY_UNUSED, "INT3          " }, /* 133 : INT3          */
    { ISR_ENTRY_UNUSED, "OSTM0TINT     " }, /* 134 : OSTM0TINT     */
    { ISR_ENTRY_UNUSED, "OSTM1TINT     " }, /* 135 : OSTM1TINT     */
    { ISR_ENTRY_UNUSED, "CMI           " }, /* 136 : CMI           */
    { ISR_ENTRY_UNUSED, "WTOUT         " }, /* 137 : WTOUT         */
    { ISR_ENTRY_UNUSED, "ITI           " }, /* 138 : ITI           */
    { ISR_ENTRY_UNUSED, "TGI0A         " }, /* 139 : TGI0A         */
    { ISR_ENTRY_UNUSED, "TGI0B         " }, /* 140 : TGI0B         */
    { ISR_ENTRY_UNUSED, "TGI0C         " }, /* 141 : TGI0C         */
    { ISR_ENTRY_UNUSED, "TGI0D         " }, /* 142 : TGI0D         */
    { ISR_ENTRY_UNUSED, "TGI0V         " }, /* 143 : TGI0V         */
    { ISR_ENTRY_UNUSED, "TGI0E         " }, /* 144 : TGI0E         */
    { ISR_ENTRY_UNUSED, "TGI0F         " }, /* 145 : TGI0F         */
    { ISR_ENTRY_UNUSED, "TGI1A         " }, /* 146 : TGI1A         */
    { ISR_ENTRY_UNUSED, "TGI1B         " }, /* 147 : TGI1B         */
    { ISR_ENTRY_UNUSED, "TGI1V         " }, /* 148 : TGI1V         */
    { ISR_ENTRY_UNUSED, "TGI1U         " }, /* 149 : TGI1U         */
    { ISR_ENTRY_UNUSED, "TGI2A         " }, /* 150 : TGI2A         */
    { ISR_ENTRY_UNUSED, "TGI2B         " }, /* 151 : TGI2B         */
    { ISR_ENTRY_UNUSED, "TGI2V         " }, /* 152 : TGI2V         */
    { ISR_ENTRY_UNUSED, "TGI2U         " }, /* 153 : TGI2U         */
    { ISR_ENTRY_UNUSED, "TGI3A         " }, /* 154 : TGI3A         */
    { ISR_ENTRY_UNUSED, "TGI3B         " }, /* 155 : TGI3B         */
    { ISR_ENTRY_UNUSED, "TGI3C         " }, /* 156 : TGI3C         */
    { ISR_ENTRY_UNUSED, "TGI3D         " }, /* 157 : TGI3D         */
    { ISR_ENTRY_UNUSED, "TGI3V         " }, /* 158 : TGI3V         */
    { ISR_ENTRY_UNUSED, "TGI4A         " }, /* 159 : TGI4A         */
    { ISR_ENTRY_UNUSED, "TGI4B         " }, /* 160 : TGI4B         */
    { ISR_ENTRY_UNUSED, "TGI4C         " }, /* 161 : TGI4C         */
    { ISR_ENTRY_UNUSED, "TGI4D         " }, /* 162 : TGI4D         */
    { ISR_ENTRY_UNUSED, "TGI4V         " }, /* 163 : TGI4V         */
    { ISR_ENTRY_UNUSED, "CMI1          " }, /* 164 : CMI1          */
    { ISR_ENTRY_UNUSED, "CMI2          " }, /* 165 : CMI2          */
    { ISR_ENTRY_UNUSED, "SGDEI0        " }, /* 166 : SGDEI0        */
    { ISR_ENTRY_UNUSED, "SGDEI1        " }, /* 167 : SGDEI1        */
    { ISR_ENTRY_UNUSED, "SGDEI2        " }, /* 168 : SGDEI2        */
    { ISR_ENTRY_UNUSED, "SGDEI3        " }, /* 169 : SGDEI3        */
    { ISR_ENTRY_UNUSED, "ADI           " }, /* 170 : ADI           */
    { ISR_ENTRY_UNUSED, "LMTI          " }, /* 171 : LMTI          */
    { ISR_ENTRY_UNUSED, "SSII0         " }, /* 172 : SSII0         */
    { ISR_ENTRY_UNUSED, "SSIRXI0       " }, /* 173 : SSIRXI0       */
    { ISR_ENTRY_UNUSED, "SSITXI0       " }, /* 174 : SSITXI0       */
    { ISR_ENTRY_UNUSED, "SSII1         " }, /* 175 : SSII1         */
    { ISR_ENTRY_UNUSED, "SSIRXI1       " }, /* 176 : SSIRXI1       */
    { ISR_ENTRY_UNUSED, "SSITXI1       " }, /* 177 : SSITXI1       */
    { ISR_ENTRY_UNUSED, "SSII2         " }, /* 178 : SSII2         */
    { ISR_ENTRY_UNUSED, "SSIRTI2       " }, /* 179 : SSIRTI2       */
    { ISR_ENTRY_UNUSED, "SSII3         " }, /* 180 : SSII3         */
    { ISR_ENTRY_UNUSED, "SSIRXI3       " }, /* 181 : SSIRXI3       */
    { ISR_ENTRY_UNUSED, "SSITXI3       " }, /* 182 : SSITXI3       */
    { ISR_ENTRY_UNUSED, "SSII4         " }, /* 183 : SSII4         */
    { ISR_ENTRY_UNUSED, "SSIRTI4       " }, /* 184 : SSIRTI4       */
    { ISR_ENTRY_UNUSED, "SSII5         " }, /* 185 : SSII5         */
    { ISR_ENTRY_UNUSED, "SSIRXI5       " }, /* 186 : SSIRXI5       */
    { ISR_ENTRY_UNUSED, "SSITXI5       " }, /* 187 : SSITXI5       */
    { ISR_ENTRY_UNUSED, "SPDIFI        " }, /* 188 : SPDIFI        */
    { ISR_ENTRY_UNUSED, "INTIICTEI0    " }, /* 189 : INTIICTEI0    */
    { ISR_ENTRY_UNUSED, "INTIICRI0     " }, /* 190 : INTIICRI0     */
    { ISR_ENTRY_UNUSED, "INTIICTI0     " }, /* 191 : INTIICTI0     */
    { ISR_ENTRY_UNUSED, "INTIICSPI0    " }, /* 192 : INTIICSPI0    */
    { ISR_ENTRY_UNUSED, "INTIICSTI0    " }, /* 193 : INTIICSTI0    */
    { ISR_ENTRY_UNUSED, "INTIICNAKI0   " }, /* 194 : INTIICNAKI0   */
    { ISR_ENTRY_UNUSED, "INTIICALI0    " }, /* 195 : INTIICALI0    */
    { ISR_ENTRY_UNUSED, "INTIICTMOI0   " }, /* 196 : INTIICTMOI0   */
    { ISR_ENTRY_UNUSED, "INTIICTEI1    " }, /* 197 : INTIICTEI1    */
    { ISR_ENTRY_UNUSED, "INTIICRI1     " }, /* 198 : INTIICRI1     */
    { ISR_ENTRY_UNUSED, "INTIICTI1     " }, /* 199 : INTIICTI1     */
    { ISR_ENTRY_UNUSED, "INTIICSPI1    " }, /* 200 : INTIICSPI1    */
    { ISR_ENTRY_UNUSED, "INTIICSTI1    " }, /* 201 : INTIICSTI1    */
    { ISR_ENTRY_UNUSED, "INTIICNAKI1   " }, /* 202 : INTIICNAKI1   */
    { ISR_ENTRY_UNUSED, "INTIICALI1    " }, /* 203 : INTIICALI1    */
    { ISR_ENTRY_UNUSED, "INTIICTMOI1   " }, /* 204 : INTIICTMOI1   */
    { ISR_ENTRY_UNUSED, "INTIICTEI2    " }, /* 205 : INTIICTEI2    */
    { ISR_ENTRY_UNUSED, "INTIICRI2     " }, /* 206 : INTIICRI2     */
    { ISR_ENTRY_UNUSED, "INTIICTI2     " }, /* 207 : INTIICTI2     */
    { ISR_ENTRY_UNUSED, "INTIICSPI2    " }, /* 208 : INTIICSPI2    */
    { ISR_ENTRY_UNUSED, "INTIICSTI2    " }, /* 209 : INTIICSTI2    */
    { ISR_ENTRY_UNUSED, "INTIICNAKI2   " }, /* 210 : INTIICNAKI2   */
    { ISR_ENTRY_UNUSED, "INTIICALI2    " }, /* 211 : INTIICALI2    */
    { ISR_ENTRY_UNUSED, "INTIICTMOI2   " }, /* 212 : INTIICTMOI2   */
    { ISR_ENTRY_UNUSED, "INTIICTEI3    " }, /* 213 : INTIICTEI3    */
    { ISR_ENTRY_UNUSED, "INTIICRI3     " }, /* 214 : INTIICRI3     */
    { ISR_ENTRY_UNUSED, "INTIICTI3     " }, /* 215 : INTIICTI3     */
    { ISR_ENTRY_UNUSED, "INTIICSPI3    " }, /* 216 : INTIICSPI3    */
    { ISR_ENTRY_UNUSED, "INTIICSTI3    " }, /* 217 : INTIICSTI3    */
    { ISR_ENTRY_UNUSED, "INTIICNAKI3   " }, /* 218 : INTIICNAKI3   */
    { ISR_ENTRY_UNUSED, "INTIICALI3    " }, /* 219 : INTIICALI3    */
    { ISR_ENTRY_UNUSED, "INTIICTMOI3   " }, /* 220 : INTIICTMOI3   */
    { ISR_ENTRY_UNUSED, "BRI0          " }, /* 221 : BRI0          */
    { ISR_ENTRY_UNUSED, "ERI0          " }, /* 222 : ERI0          */
    { ISR_ENTRY_UNUSED, "RXI0          " }, /* 223 : RXI0          */
    { ISR_ENTRY_UNUSED, "TXI0          " }, /* 224 : TXI0          */
    { ISR_ENTRY_UNUSED, "BRI1          " }, /* 225 : BRI1          */
    { ISR_ENTRY_UNUSED, "ERI1          " }, /* 226 : ERI1          */
    { ISR_ENTRY_UNUSED, "RXI1          " }, /* 227 : RXI1          */
    { ISR_ENTRY_UNUSED, "TXI1          " }, /* 228 : TXI1          */
    { ISR_ENTRY_UNUSED, "BRI2          " }, /* 229 : BRI2          */
    { ISR_ENTRY_UNUSED, "ERI2          " }, /* 230 : ERI2          */
    { ISR_ENTRY_UNUSED, "RXI2          " }, /* 231 : RXI2          */
    { ISR_ENTRY_UNUSED, "TXI2          " }, /* 232 : TXI2          */
    { ISR_ENTRY_UNUSED, "BRI3          " }, /* 233 : BRI3          */
    { ISR_ENTRY_UNUSED, "ERI3          " }, /* 234 : ERI3          */
    { ISR_ENTRY_UNUSED, "RXI3          " }, /* 235 : RXI3          */
    { ISR_ENTRY_UNUSED, "TXI3          " }, /* 236 : TXI3          */
    { ISR_ENTRY_UNUSED, "BRI4          " }, /* 237 : BRI4          */
    { ISR_ENTRY_UNUSED, "ERI4          " }, /* 238 : ERI4          */
    { ISR_ENTRY_UNUSED, "RXI4          " }, /* 239 : RXI4          */
    { ISR_ENTRY_UNUSED, "TXI4          " }, /* 240 : TXI4          */
    { ISR_ENTRY_UNUSED, "BRI5          " }, /* 241 : BRI5          */
    { ISR_ENTRY_UNUSED, "ERI5          " }, /* 242 : ERI5          */
    { ISR_ENTRY_UNUSED, "RXI5          " }, /* 243 : RXI5          */
    { ISR_ENTRY_UNUSED, "TXI5          " }, /* 244 : TXI5          */
    { ISR_ENTRY_UNUSED, "BRI6          " }, /* 245 : BRI6          */
    { ISR_ENTRY_UNUSED, "ERI6          " }, /* 246 : ERI6          */
    { ISR_ENTRY_UNUSED, "RXI6          " }, /* 247 : RXI6          */
    { ISR_ENTRY_UNUSED, "TXI6          " }, /* 248 : TXI6          */
    { ISR_ENTRY_UNUSED, "BRI7          " }, /* 249 : BRI7          */
    { ISR_ENTRY_UNUSED, "ERI7          " }, /* 250 : ERI7          */
    { ISR_ENTRY_UNUSED, "RXI7          " }, /* 251 : RXI7          */
    { ISR_ENTRY_UNUSED, "TXI7          " }, /* 252 : TXI7          */
    { ISR_ENTRY_UNUSED, "INTRCANGERR   " }, /* 253 : INTRCANGERR   */
    { ISR_ENTRY_UNUSED, "INTRCANGRECC  " }, /* 254 : INTRCANGRECC  */
    { ISR_ENTRY_UNUSED, "INTRCAN0REC   " }, /* 255 : INTRCAN0REC   */
    { ISR_ENTRY_UNUSED, "INTRCAN0ERR   " }, /* 256 : INTRCAN0ERR   */
    { ISR_ENTRY_UNUSED, "INTRCAN0TRX   " }, /* 257 : INTRCAN0TRX   */
    { ISR_ENTRY_UNUSED, "INTRCAN1REC   " }, /* 258 : INTRCAN1REC   */
    { ISR_ENTRY_UNUSED, "INTRCAN1ERR   " }, /* 259 : INTRCAN1ERR   */
    { ISR_ENTRY_UNUSED, "INTRCAN1TRX   " }, /* 260 : INTRCAN1TRX   */
    { ISR_ENTRY_UNUSED, "INTRCAN2REC   " }, /* 261 : INTRCAN2REC   */
    { ISR_ENTRY_UNUSED, "INTRCAN2ERR   " }, /* 262 : INTRCAN2ERR   */
    { ISR_ENTRY_UNUSED, "INTRCAN2TRX   " }, /* 263 : INTRCAN2TRX   */
    { ISR_ENTRY_UNUSED, "INTRCAN3REC   " }, /* 264 : INTRCAN3REC   */
    { ISR_ENTRY_UNUSED, "INTRCAN3ERR   " }, /* 265 : INTRCAN3ERR   */
    { ISR_ENTRY_UNUSED, "INTRCAN3TRX   " }, /* 266 : INTRCAN3TRX   */
    { ISR_ENTRY_UNUSED, "INTRCAN4REC   " }, /* 267 : INTRCAN4REC   */
    { ISR_ENTRY_UNUSED, "INTRCAN4ERR   " }, /* 268 : INTRCAN4ERR   */
    { ISR_ENTRY_UNUSED, "INTRCAN4TRX   " }, /* 269 : INTRCAN4TRX   */
    { ISR_ENTRY_UNUSED, "SPEI0         " }, /* 270 : SPEI0         */
    { ISR_ENTRY_UNUSED, "SPRI0         " }, /* 271 : SPRI0         */
    { ISR_ENTRY_UNUSED, "SPTI0         " }, /* 272 : SPTI0         */
    { ISR_ENTRY_UNUSED, "SPEI1         " }, /* 273 : SPEI1         */
    { ISR_ENTRY_UNUSED, "SPRI1         " }, /* 274 : SPRI1         */
    { ISR_ENTRY_UNUSED, "SPTI1         " }, /* 275 : SPTI1         */
    { ISR_ENTRY_UNUSED, "SPEI2         " }, /* 276 : SPEI2         */
    { ISR_ENTRY_UNUSED, "SPRI2         " }, /* 277 : SPRI2         */
    { ISR_ENTRY_UNUSED, "SPTI2         " }, /* 278 : SPTI2         */
    { ISR_ENTRY_UNUSED, "SPEI3         " }, /* 279 : SPEI3         */
    { ISR_ENTRY_UNUSED, "SPRI3         " }, /* 280 : SPRI3         */
    { ISR_ENTRY_UNUSED, "SPTI3         " }, /* 281 : SPTI3         */
    { ISR_ENTRY_UNUSED, "SPEI4         " }, /* 282 : SPEI4         */
    { ISR_ENTRY_UNUSED, "SPRI4         " }, /* 283 : SPRI4         */
    { ISR_ENTRY_UNUSED, "SPTI4         " }, /* 284 : SPTI4         */
    { ISR_ENTRY_UNUSED, "IEBBTD        " }, /* 285 : IEBBTD        */
    { ISR_ENTRY_UNUSED, "IEBBTERR      " }, /* 286 : IEBBTERR      */
    { ISR_ENTRY_UNUSED, "IEBBTSTA      " }, /* 287 : IEBBTSTA      */
    { ISR_ENTRY_UNUSED, "IEBBTV        " }, /* 288 : IEBBTV        */
    { ISR_ENTRY_UNUSED, "ISY           " }, /* 289 : ISY           */
    { ISR_ENTRY_UNUSED, "IERR          " }, /* 290 : IERR          */
    { ISR_ENTRY_UNUSED, "ITARG         " }, /* 291 : ITARG         */
    { ISR_ENTRY_UNUSED, "ISEC          " }, /* 292 : ISEC          */
    { ISR_ENTRY_UNUSED, "IBUF          " }, /* 293 : IBUF          */
    { ISR_ENTRY_UNUSED, "IREADY        " }, /* 294 : IREADY        */
    { ISR_ENTRY_UNUSED, "FLSTE         " }, /* 295 : FLSTE         */
    { ISR_ENTRY_UNUSED, "FLTENDI       " }, /* 296 : FLTENDI       */
    { ISR_ENTRY_UNUSED, "FLTREQ0I      " }, /* 297 : FLTREQ0I      */
    { ISR_ENTRY_UNUSED, "FLTREQ1I      " }, /* 298 : FLTREQ1I      */
    { ISR_ENTRY_UNUSED, "MMC0          " }, /* 299 : MMC0          */
    { ISR_ENTRY_UNUSED, "MMC1          " }, /* 300 : MMC1          */
    { ISR_ENTRY_UNUSED, "MMC2          " }, /* 301 : MMC2          */
    { ISR_ENTRY_UNUSED, "SDHI0_3       " }, /* 302 : SDHI0_3       */
    { ISR_ENTRY_UNUSED, "SDHI0_0       " }, /* 303 : SDHI0_0       */
    { ISR_ENTRY_UNUSED, "SDHI0_1       " }, /* 304 : SDHI0_1       */
    { ISR_ENTRY_UNUSED, "SDHI1_3       " }, /* 305 : SDHI1_3       */
    { ISR_ENTRY_UNUSED, "SDHI1_0       " }, /* 306 : SDHI1_0       */
    { ISR_ENTRY_UNUSED, "SDHI1_1       " }, /* 307 : SDHI1_1       */
    { ISR_ENTRY_UNUSED, "ARM           " }, /* 308 : ARM           */
    { ISR_ENTRY_UNUSED, "PRD           " }, /* 309 : PRD           */
    { ISR_ENTRY_UNUSED, "CUP           " }, /* 310 : CUP           */
    { ISR_ENTRY_UNUSED, "SCUAI0        " }, /* 311 : SCUAI0        */
    { ISR_ENTRY_UNUSED, "SCUAI1        " }, /* 312 : SCUAI1        */
    { ISR_ENTRY_UNUSED, "SCUFDI0       " }, /* 313 : SCUFDI0       */
    { ISR_ENTRY_UNUSED, "SCUFDI1       " }, /* 314 : SCUFDI1       */
    { ISR_ENTRY_UNUSED, "SCUFDI2       " }, /* 315 : SCUFDI2       */
    { ISR_ENTRY_UNUSED, "SCUFDI3       " }, /* 316 : SCUFDI3       */
    { ISR_ENTRY_UNUSED, "SCUFUI0       " }, /* 317 : SCUFUI0       */
    { ISR_ENTRY_UNUSED, "SCUFUI1       " }, /* 318 : SCUFUI1       */
    { ISR_ENTRY_UNUSED, "SCUFUI2       " }, /* 319 : SCUFUI2       */
    { ISR_ENTRY_UNUSED, "SCUFUI3       " }, /* 320 : SCUFUI3       */
    { ISR_ENTRY_UNUSED, "SCUDVI0       " }, /* 321 : SCUDVI0       */
    { ISR_ENTRY_UNUSED, "SCUDVI1       " }, /* 322 : SCUDVI1       */
    { ISR_ENTRY_UNUSED, "SCUDVI2       " }, /* 323 : SCUDVI2       */
    { ISR_ENTRY_UNUSED, "SCUDVI3       " }, /* 324 : SCUDVI3       */
    { ISR_ENTRY_UNUSED, "MLB_CINT      " }, /* 325 : MLB_CINT      */
    { ISR_ENTRY_UNUSED, "MLB_SINT      " }, /* 326 : MLB_SINT      */
    { ISR_ENTRY_UNUSED, "DRC0          " }, /* 327 : DRC0          */
    { ISR_ENTRY_UNUSED, "DRC1          " }, /* 328 : DRC1          */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 329 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 330 : <reserved>    */
    { ISR_ENTRY_UNUSED, "LINI0_INT_T   " }, /* 331 : LINI0_INT_T   */
    { ISR_ENTRY_UNUSED, "LINI0_INT_R   " }, /* 332 : LINI0_INT_R   */
    { ISR_ENTRY_UNUSED, "LINI0_INT_S   " }, /* 333 : LINI0_INT_S   */
    { ISR_ENTRY_UNUSED, "LINI0_INT_M   " }, /* 334 : LINI0_INT_M   */
    { ISR_ENTRY_UNUSED, "LINI1_INT_T   " }, /* 335 : LINI1_INT_T   */
    { ISR_ENTRY_UNUSED, "LINI1_INT_R   " }, /* 336 : LINI1_INT_R   */
    { ISR_ENTRY_UNUSED, "LINI1_INT_S   " }, /* 337 : LINI1_INT_S   */
    { ISR_ENTRY_UNUSED, "LINI1_INT_M   " }, /* 338 : LINI1_INT_M   */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 339 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 340 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 341 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 342 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 343 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 344 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 345 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 346 : <reserved>    */
    { ISR_ENTRY_UNUSED, "ERI0          " }, /* 347 : ERI0          */
    { ISR_ENTRY_UNUSED, "RXI0          " }, /* 348 : RXI0          */
    { ISR_ENTRY_UNUSED, "TXI0          " }, /* 349 : TXI0          */
    { ISR_ENTRY_UNUSED, "TEI0          " }, /* 350 : TEI0          */
    { ISR_ENTRY_UNUSED, "ERI1          " }, /* 351 : ERI1          */
    { ISR_ENTRY_UNUSED, "RXI1          " }, /* 352 : RXI1          */
    { ISR_ENTRY_UNUSED, "TXI1          " }, /* 353 : TXI1          */
    { ISR_ENTRY_UNUSED, "TEI1          " }, /* 354 : TEI1          */
    { ISR_ENTRY_UNUSED, "AVBI_DATA     " }, /* 355 : AVBI_DATA     */
    { ISR_ENTRY_UNUSED, "AVBI_ERROR    " }, /* 356 : AVBI_ERROR    */
    { ISR_ENTRY_UNUSED, "AVBI_MANAGE   " }, /* 357 : AVBI_MANAGE   */
    { ISR_ENTRY_UNUSED, "AVBI_MAC      " }, /* 358 : AVBI_MAC      */
    { ISR_ENTRY_UNUSED, "ETHERI        " }, /* 359 : ETHERI        */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 360 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 361 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 362 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 363 : <reserved>    */
    { ISR_ENTRY_UNUSED, "CEUI          " }, /* 364 : CEUI          */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 365 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 366 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 367 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 368 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 369 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 370 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 371 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 372 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 373 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 374 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 375 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 376 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 377 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 378 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 379 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 380 : <reserved>    */
    { ISR_ENTRY_UNUSED, "H2XMLB_ERRINT " }, /* 381 : H2XMLB_ERRINT */
    { ISR_ENTRY_UNUSED, "H2XIC1_ERRINT " }, /* 382 : H2XIC1_ERRINT */
    { ISR_ENTRY_UNUSED, "X2HPERI1_ERRINT" }, /* 383 : X2HPERI1_ERRINT*/
    { ISR_ENTRY_UNUSED, "X2HPERI2_ERRINT" }, /* 384 : X2HPERI2_ERRINT*/
    { ISR_ENTRY_UNUSED, "X2HPERI34_ERRINT" }, /* 385 : X2HPERI34_ERRINT*/
    { ISR_ENTRY_UNUSED, "X2HPERI5_ERRINT" }, /* 386 : X2HPERI5_ERRINT*/
    { ISR_ENTRY_UNUSED, "X2HPERI67_ERRINT" }, /* 387 : X2HPERI67_ERRINT*/
    { ISR_ENTRY_UNUSED, "X2HDBGR_ERRINT" }, /* 388 : X2HDBGR_ERRINT*/
    { ISR_ENTRY_UNUSED, "X2HBSC_ERRINT " }, /* 389 : X2HBSC_ERRINT */
    { ISR_ENTRY_UNUSED, "X2HSPI1_ERRINT" }, /* 390 : X2HSPI1_ERRINT*/
    { ISR_ENTRY_UNUSED, "X2HSPI2_ERRINT" }, /* 391 : X2HSPI2_ERRINT*/
    { ISR_ENTRY_UNUSED, "PRRI          " }, /* 392 : PRRI          */
    { ISR_ENTRY_UNUSED, "IFEI0         " }, /* 393 : IFEI0         */
    { ISR_ENTRY_UNUSED, "OFFI0         " }, /* 394 : OFFI0         */
    { ISR_ENTRY_UNUSED, "PFVEI0        " }, /* 395 : PFVEI0        */
    { ISR_ENTRY_UNUSED, "IFEI1         " }, /* 396 : IFEI1         */
    { ISR_ENTRY_UNUSED, "OFFI1         " }, /* 397 : OFFI1         */
    { ISR_ENTRY_UNUSED, "PFVEI1        " }, /* 398 : PFVEI1        */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 399 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 400 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 401 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 402 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 403 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 404 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 405 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 406 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 407 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 408 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 409 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 410 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 411 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 412 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 413 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 414 : <reserved>    */
    { ISR_ENTRY_UNUSED, "<reserved>    " }, /* 415 : <reserved>    */
    { ISR_ENTRY_UNUSED, "TINT0         " }, /* 416 : TINT0         */
    { ISR_ENTRY_UNUSED, "TINT1         " }, /* 417 : TINT1         */
    { ISR_ENTRY_UNUSED, "TINT2         " }, /* 418 : TINT2         */
    { ISR_ENTRY_UNUSED, "TINT3         " }, /* 419 : TINT3         */
    { ISR_ENTRY_UNUSED, "TINT4         " }, /* 420 : TINT4         */
    { ISR_ENTRY_UNUSED, "TINT5         " }, /* 421 : TINT5         */
    { ISR_ENTRY_UNUSED, "TINT6         " }, /* 422 : TINT6         */
    { ISR_ENTRY_UNUSED, "TINT7         " }, /* 423 : TINT7         */
    { ISR_ENTRY_UNUSED, "TINT8         " }, /* 424 : TINT8         */
    { ISR_ENTRY_UNUSED, "TINT9         " }, /* 425 : TINT9         */
    { ISR_ENTRY_UNUSED, "TINT10        " }, /* 426 : TINT10        */
    { ISR_ENTRY_UNUSED, "TINT11        " }, /* 427 : TINT11        */
    { ISR_ENTRY_UNUSED, "TINT12        " }, /* 428 : TINT12        */
    { ISR_ENTRY_UNUSED, "TINT13        " }, /* 429 : TINT13        */
    { ISR_ENTRY_UNUSED, "TINT14        " }, /* 430 : TINT14        */
    { ISR_ENTRY_UNUSED, "TINT15        " }, /* 431 : TINT15        */
    { ISR_ENTRY_UNUSED, "TINT16        " }, /* 432 : TINT16        */
    { ISR_ENTRY_UNUSED, "TINT17        " }, /* 433 : TINT17        */
    { ISR_ENTRY_UNUSED, "TINT18        " }, /* 434 : TINT18        */
    { ISR_ENTRY_UNUSED, "TINT19        " }, /* 435 : TINT19        */
    { ISR_ENTRY_UNUSED, "TINT20        " }, /* 436 : TINT20        */
    { ISR_ENTRY_UNUSED, "TINT21        " }, /* 437 : TINT21        */
    { ISR_ENTRY_UNUSED, "TINT22        " }, /* 438 : TINT22        */
    { ISR_ENTRY_UNUSED, "TINT23        " }, /* 439 : TINT23        */
    { ISR_ENTRY_UNUSED, "TINT24        " }, /* 440 : TINT24        */
    { ISR_ENTRY_UNUSED, "TINT25        " }, /* 441 : TINT25        */
    { ISR_ENTRY_UNUSED, "TINT26        " }, /* 442 : TINT26        */
    { ISR_ENTRY_UNUSED, "TINT27        " }, /* 443 : TINT27        */
    { ISR_ENTRY_UNUSED, "TINT28        " }, /* 444 : TINT28        */
    { ISR_ENTRY_UNUSED, "TINT29        " }, /* 445 : TINT29        */
    { ISR_ENTRY_UNUSED, "TINT30        " }, /* 446 : TINT30        */
    { ISR_ENTRY_UNUSED, "TINT31        " }, /* 447 : TINT31        */
    { ISR_ENTRY_UNUSED, "TINT32        " }, /* 448 : TINT32        */
    { ISR_ENTRY_UNUSED, "TINT33        " }, /* 449 : TINT33        */
    { ISR_ENTRY_UNUSED, "TINT34        " }, /* 450 : TINT34        */
    { ISR_ENTRY_UNUSED, "TINT35        " }, /* 451 : TINT35        */
    { ISR_ENTRY_UNUSED, "TINT36        " }, /* 452 : TINT36        */
    { ISR_ENTRY_UNUSED, "TINT37        " }, /* 453 : TINT37        */
    { ISR_ENTRY_UNUSED, "TINT38        " }, /* 454 : TINT38        */
    { ISR_ENTRY_UNUSED, "TINT39        " }, /* 455 : TINT39        */
    { ISR_ENTRY_UNUSED, "TINT40        " }, /* 456 : TINT40        */
    { ISR_ENTRY_UNUSED, "TINT41        " }, /* 457 : TINT41        */
    { ISR_ENTRY_UNUSED, "TINT42        " }, /* 458 : TINT42        */
    { ISR_ENTRY_UNUSED, "TINT43        " }, /* 459 : TINT43        */
    { ISR_ENTRY_UNUSED, "TINT44        " }, /* 460 : TINT44        */
    { ISR_ENTRY_UNUSED, "TINT45        " }, /* 461 : TINT45        */
    { ISR_ENTRY_UNUSED, "TINT46        " }, /* 462 : TINT46        */
    { ISR_ENTRY_UNUSED, "TINT47        " }, /* 463 : TINT47        */
    { ISR_ENTRY_UNUSED, "TINT48        " }, /* 464 : TINT48        */
    { ISR_ENTRY_UNUSED, "TINT49        " }, /* 465 : TINT49        */
    { ISR_ENTRY_UNUSED, "TINT50        " }, /* 466 : TINT50        */
    { ISR_ENTRY_UNUSED, "TINT51        " }, /* 467 : TINT51        */
    { ISR_ENTRY_UNUSED, "TINT52        " }, /* 468 : TINT52        */
    { ISR_ENTRY_UNUSED, "TINT53        " }, /* 469 : TINT53        */
    { ISR_ENTRY_UNUSED, "TINT54        " }, /* 470 : TINT54        */
    { ISR_ENTRY_UNUSED, "TINT55        " }, /* 471 : TINT55        */
    { ISR_ENTRY_UNUSED, "TINT56        " }, /* 472 : TINT56        */
    { ISR_ENTRY_UNUSED, "TINT57        " }, /* 473 : TINT57        */
    { ISR_ENTRY_UNUSED, "TINT58        " }, /* 474 : TINT58        */
    { ISR_ENTRY_UNUSED, "TINT59        " }, /* 475 : TINT59        */
    { ISR_ENTRY_UNUSED, "TINT60        " }, /* 476 : TINT60        */
    { ISR_ENTRY_UNUSED, "TINT61        " }, /* 477 : TINT61        */
    { ISR_ENTRY_UNUSED, "TINT62        " }, /* 478 : TINT62        */
    { ISR_ENTRY_UNUSED, "TINT63        " }, /* 479 : TINT63        */
    { ISR_ENTRY_UNUSED, "TINT64        " }, /* 480 : TINT64        */
    { ISR_ENTRY_UNUSED, "TINT65        " }, /* 481 : TINT65        */
    { ISR_ENTRY_UNUSED, "TINT66        " }, /* 482 : TINT66        */
    { ISR_ENTRY_UNUSED, "TINT67        " }, /* 483 : TINT67        */
    { ISR_ENTRY_UNUSED, "TINT68        " }, /* 484 : TINT68        */
    { ISR_ENTRY_UNUSED, "TINT69        " }, /* 485 : TINT69        */
    { ISR_ENTRY_UNUSED, "TINT70        " }, /* 486 : TINT70        */
    { ISR_ENTRY_UNUSED, "TINT71        " }, /* 487 : TINT71        */
    { ISR_ENTRY_UNUSED, "TINT72        " }, /* 488 : TINT72        */
    { ISR_ENTRY_UNUSED, "TINT73        " }, /* 489 : TINT73        */
    { ISR_ENTRY_UNUSED, "TINT74        " }, /* 490 : TINT74        */
    { ISR_ENTRY_UNUSED, "TINT75        " }, /* 491 : TINT75        */
    { ISR_ENTRY_UNUSED, "TINT76        " }, /* 492 : TINT76        */
    { ISR_ENTRY_UNUSED, "TINT77        " }, /* 493 : TINT77        */
    { ISR_ENTRY_UNUSED, "TINT78        " }, /* 494 : TINT78        */
    { ISR_ENTRY_UNUSED, "TINT79        " }, /* 495 : TINT79        */
    { ISR_ENTRY_UNUSED, "TINT80        " }, /* 496 : TINT80        */
    { ISR_ENTRY_UNUSED, "TINT81        " }, /* 497 : TINT81        */
    { ISR_ENTRY_UNUSED, "TINT82        " }, /* 498 : TINT82        */
    { ISR_ENTRY_UNUSED, "TINT83        " }, /* 499 : TINT83        */
    { ISR_ENTRY_UNUSED, "TINT84        " }, /* 500 : TINT84        */
    { ISR_ENTRY_UNUSED, "TINT85        " }, /* 501 : TINT85        */
    { ISR_ENTRY_UNUSED, "TINT86        " }, /* 502 : TINT86        */
    { ISR_ENTRY_UNUSED, "TINT87        " }, /* 503 : TINT87        */
    { ISR_ENTRY_UNUSED, "TINT88        " }, /* 504 : TINT88        */
    { ISR_ENTRY_UNUSED, "TINT89        " }, /* 505 : TINT89        */
    { ISR_ENTRY_UNUSED, "TINT90        " }, /* 506 : TINT90        */
    { ISR_ENTRY_UNUSED, "TINT91        " }, /* 507 : TINT91        */
    { ISR_ENTRY_UNUSED, "TINT92        " }, /* 508 : TINT92        */
    { ISR_ENTRY_UNUSED, "TINT93        " }, /* 509 : TINT93        */
    { ISR_ENTRY_UNUSED, "TINT94        " }, /* 510 : TINT94        */
    { ISR_ENTRY_UNUSED, "TINT95        " }, /* 511 : TINT95        */
    { ISR_ENTRY_UNUSED, "TINT96        " }, /* 512 : TINT96        */
    { ISR_ENTRY_UNUSED, "TINT97        " }, /* 513 : TINT97        */
    { ISR_ENTRY_UNUSED, "TINT98        " }, /* 514 : TINT98        */
    { ISR_ENTRY_UNUSED, "TINT99        " }, /* 515 : TINT99        */
    { ISR_ENTRY_UNUSED, "TINT100       " }, /* 516 : TINT100       */
    { ISR_ENTRY_UNUSED, "TINT101       " }, /* 517 : TINT101       */
    { ISR_ENTRY_UNUSED, "TINT102       " }, /* 518 : TINT102       */
    { ISR_ENTRY_UNUSED, "TINT103       " }, /* 519 : TINT103       */
    { ISR_ENTRY_UNUSED, "TINT104       " }, /* 520 : TINT104       */
    { ISR_ENTRY_UNUSED, "TINT105       " }, /* 521 : TINT105       */
    { ISR_ENTRY_UNUSED, "TINT106       " }, /* 522 : TINT106       */
    { ISR_ENTRY_UNUSED, "TINT107       " }, /* 523 : TINT107       */
    { ISR_ENTRY_UNUSED, "TINT108       " }, /* 524 : TINT108       */
    { ISR_ENTRY_UNUSED, "TINT109       " }, /* 525 : TINT109       */
    { ISR_ENTRY_UNUSED, "TINT110       " }, /* 526 : TINT110       */
    { ISR_ENTRY_UNUSED, "TINT111       " }, /* 527 : TINT111       */
    { ISR_ENTRY_UNUSED, "TINT112       " }, /* 528 : TINT112       */
    { ISR_ENTRY_UNUSED, "TINT113       " }, /* 529 : TINT113       */
    { ISR_ENTRY_UNUSED, "TINT114       " }, /* 530 : TINT114       */
    { ISR_ENTRY_UNUSED, "TINT115       " }, /* 531 : TINT115       */
    { ISR_ENTRY_UNUSED, "TINT116       " }, /* 532 : TINT116       */
    { ISR_ENTRY_UNUSED, "TINT117       " }, /* 533 : TINT117       */
    { ISR_ENTRY_UNUSED, "TINT118       " }, /* 534 : TINT118       */
    { ISR_ENTRY_UNUSED, "TINT119       " }, /* 535 : TINT119       */
    { ISR_ENTRY_UNUSED, "TINT120       " }, /* 536 : TINT120       */
    { ISR_ENTRY_UNUSED, "TINT121       " }, /* 537 : TINT121       */
    { ISR_ENTRY_UNUSED, "TINT122       " }, /* 538 : TINT122       */
    { ISR_ENTRY_UNUSED, "TINT123       " }, /* 539 : TINT123       */
    { ISR_ENTRY_UNUSED, "TINT124       " }, /* 540 : TINT124       */
    { ISR_ENTRY_UNUSED, "TINT125       " }, /* 541 : TINT125       */
    { ISR_ENTRY_UNUSED, "TINT126       " }, /* 542 : TINT126       */
    { ISR_ENTRY_UNUSED, "TINT127       " }, /* 543 : TINT127       */
    { ISR_ENTRY_UNUSED, "TINT128       " }, /* 544 : TINT128       */
    { ISR_ENTRY_UNUSED, "TINT129       " }, /* 545 : TINT129       */
    { ISR_ENTRY_UNUSED, "TINT130       " }, /* 546 : TINT130       */
    { ISR_ENTRY_UNUSED, "TINT131       " }, /* 547 : TINT131       */
    { ISR_ENTRY_UNUSED, "TINT132       " }, /* 548 : TINT132       */
    { ISR_ENTRY_UNUSED, "TINT133       " }, /* 549 : TINT133       */
    { ISR_ENTRY_UNUSED, "TINT134       " }, /* 550 : TINT134       */
    { ISR_ENTRY_UNUSED, "TINT135       " }, /* 551 : TINT135       */
    { ISR_ENTRY_UNUSED, "TINT136       " }, /* 552 : TINT136       */
    { ISR_ENTRY_UNUSED, "TINT137       " }, /* 553 : TINT137       */
    { ISR_ENTRY_UNUSED, "TINT138       " }, /* 554 : TINT138       */
    { ISR_ENTRY_UNUSED, "TINT139       " }, /* 555 : TINT139       */
    { ISR_ENTRY_UNUSED, "TINT140       " }, /* 556 : TINT140       */
    { ISR_ENTRY_UNUSED, "TINT141       " }, /* 557 : TINT141       */
    { ISR_ENTRY_UNUSED, "TINT142       " }, /* 558 : TINT142       */
    { ISR_ENTRY_UNUSED, "TINT143       " }, /* 559 : TINT143       */
    { ISR_ENTRY_UNUSED, "TINT144       " }, /* 560 : TINT144       */
    { ISR_ENTRY_UNUSED, "TINT145       " }, /* 561 : TINT145       */
    { ISR_ENTRY_UNUSED, "TINT146       " }, /* 562 : TINT146       */
    { ISR_ENTRY_UNUSED, "TINT147       " }, /* 563 : TINT147       */
    { ISR_ENTRY_UNUSED, "TINT148       " }, /* 564 : TINT148       */
    { ISR_ENTRY_UNUSED, "TINT149       " }, /* 565 : TINT149       */
    { ISR_ENTRY_UNUSED, "TINT150       " }, /* 566 : TINT150       */
    { ISR_ENTRY_UNUSED, "TINT151       " }, /* 567 : TINT151       */
    { ISR_ENTRY_UNUSED, "TINT152       " }, /* 568 : TINT152       */
    { ISR_ENTRY_UNUSED, "TINT153       " }, /* 569 : TINT153       */
    { ISR_ENTRY_UNUSED, "TINT154       " }, /* 570 : TINT154       */
    { ISR_ENTRY_UNUSED, "TINT155       " }, /* 571 : TINT155       */
    { ISR_ENTRY_UNUSED, "TINT156       " }, /* 572 : TINT156       */
    { ISR_ENTRY_UNUSED, "TINT157       " }, /* 573 : TINT157       */
    { ISR_ENTRY_UNUSED, "TINT158       " }, /* 574 : TINT158       */
    { ISR_ENTRY_UNUSED, "TINT159       " }, /* 575 : TINT159       */
    { ISR_ENTRY_UNUSED, "TINT160       " }, /* 576 : TINT160       */
    { ISR_ENTRY_UNUSED, "TINT161       " }, /* 577 : TINT161       */
    { ISR_ENTRY_UNUSED, "TINT162       " }, /* 578 : TINT162       */
    { ISR_ENTRY_UNUSED, "TINT163       " }, /* 579 : TINT163       */
    { ISR_ENTRY_UNUSED, "TINT164       " }, /* 580 : TINT164       */
    { ISR_ENTRY_UNUSED, "TINT165       " }, /* 581 : TINT165       */
    { ISR_ENTRY_UNUSED, "TINT166       " }, /* 582 : TINT166       */
    { ISR_ENTRY_UNUSED, "TINT167       " }, /* 583 : TINT167       */
    { ISR_ENTRY_UNUSED, "TINT168       " }, /* 584 : TINT168       */
    { ISR_ENTRY_UNUSED, "TINT169       " }, /* 585 : TINT169       */
    { ISR_ENTRY_UNUSED, "TINT170       " }, /* 586 : TINT170       */
};

void R_INTC_Update_Isr_Log_Entry(uint16_t entry, uint8_t priority)
{
    uint16_t size = (sizeof(monitor)/sizeof(st_intc_tbl_t));

    if(entry < size)
    {
        if (ISR_ENTRY_UNUSED == priority)
        {
            monitor[entry].level = ISR_ENTRY_UNUSED;
        }
        else
        {
            monitor[entry].level = priority;
        }
    }
}


void R_INTC_Display_TaskTable(FILE *p_out)
{
    uint16_t entry = 0;
    uint16_t index = 0;
    uint16_t size = (sizeof(monitor)/sizeof(st_intc_tbl_t));

    memset(sorted_tbl, 0xFF, INTC_ID_TOTAL * sizeof (uint16_t));

    /* populate the unsorted table */
    for(entry = 0; entry < size; entry++)
    {
        if(ISR_ENTRY_UNUSED != monitor[entry].level)
        {
            sorted_tbl[index++] = entry;
        }
    }


    /* sort the data */
    {
        uint16_t sort_incomplete = 1;

        while(sort_incomplete)
        {
            sort_incomplete = 0;

             /* perform sorting pass */
            for(entry = 0; entry < size; entry++)
            {
                if(0xFFFF != sorted_tbl[entry+1])
                {
                    if(monitor[sorted_tbl[entry+1]].level < monitor[sorted_tbl[entry]].level)
                    {
                        uint16_t t1 = sorted_tbl[entry];

                        /* swap the entries */
                        sorted_tbl[entry]   = sorted_tbl[entry+1];
                        sorted_tbl[entry+1] = t1;

                        /* record pass has made changes */
                        sort_incomplete++;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    fprintf(p_out, "\r\nHardware ISR Table\r\n");

    fprintf(p_out, " Pr  Interrupt Name\r\n");
    fprintf(p_out, "=== ===============\r\n");

    for(entry = 0; entry < size; entry++)
    {
        if(0xFFFF != sorted_tbl[entry])
        {
            fprintf(p_out, "[%02d][%s]\r\n", monitor[sorted_tbl[entry]].level, monitor[sorted_tbl[entry]].strname);
            fflush(p_out);
        }
        else
        {
            break;
        }
    }
}
