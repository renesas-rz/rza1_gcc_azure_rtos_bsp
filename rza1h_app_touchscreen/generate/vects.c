                                                                          
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
/************************************************************************/
/*    File Version: V1.01                                               */
/*    Date Generated: 19/04/2019                                        */
/************************************************************************/

#include "interrupt_handlers.h"

typedef void (*fp) (void);

#define RVECT_SECT          __attribute__ ((section (".rvectors")))

const fp RelocatableVectors[] RVECT_SECT  = {

/*;0x0000 GIC0*/
    (fp)INT_Excep_GIC0,
/*;0x0004 GIC1*/
    (fp)INT_Excep_GIC1,
/*;0x0008 GIC2*/
    (fp)INT_Excep_GIC2,
/*;0x000C GIC3*/
    (fp)INT_Excep_GIC3,
/*;0x0010 GIC4*/
    (fp)INT_Excep_GIC4,
/*;0x0014  GIC5*/
    (fp)INT_Excep_GIC5,
/*;0x0018  GIC6*/
    (fp)INT_Excep_GIC6,
/*;0x001C  GIC7*/
    (fp)INT_Excep_GIC7,
/*;0x0020  GIC8*/
    (fp)INT_Excep_GIC8,
/*;0x0024  GIC9*/
    (fp)INT_Excep_GIC9,
/*;0x0028  GIC10*/
    (fp)INT_Excep_GIC10,
/*;0x002C  GIC11*/
    (fp)INT_Excep_GIC11,
/*;0x0030  GIC12*/
    (fp)INT_Excep_GIC12,
/*;0x0034  GIC13*/
    (fp)INT_Excep_GIC13,
/*;0x0038  GIC14*/
    (fp)INT_Excep_GIC14,
/*;0x003C  GIC15*/
    (fp)INT_Excep_GIC15,
/*;0x0040  CPU_PMUIRQ0*/
    (fp)INT_Excep_CPU_PMUIRQ0,
/*;0x0044  CPU_COMMRX0*/
    (fp)INT_Excep_CPU_COMMRX0,
/*;0x0048  CPU_COMMTX0*/
    (fp)INT_Excep_CPU_COMMTX0,
/*;0x004C  CPU_CTIIRQ0*/
    (fp)INT_Excep_CPU_CTIIRQ0,
/*;0x0050 Reserved*/
    (fp)0,
/*;0x0054 Reserved*/
    (fp)0,
/*;0x0058 Reserved*/
    (fp)0,
/*;0x005C Reserved*/
    (fp)0,
/*;0x0060 Reserved*/
    (fp)0,
/*;0x0064 Reserved*/
    (fp)0,
/*;0x0068 Reserved*/
    (fp)0,
/*;0x006C Reserved*/
    (fp)0,
/*;0x0070 Reserved*/
    (fp)0,
/*;0x0074 Reserved*/
    (fp)0,
/*;0x0078 Reserved*/
    (fp)0,
/*;0x007C Reserved*/
    (fp)0,
/*;0x0080  IRQ0*/
    (fp)INT_Excep_IRQ0,
/*;0x0084  IRQ1*/
    (fp)INT_Excep_IRQ1,
/*;0x0088  IRQ2*/
    (fp)INT_Excep_IRQ2,
/*;0x008C  IRQ3*/
    (fp)INT_Excep_IRQ3,
/*;0x0090  IRQ4*/
    (fp)INT_Excep_IRQ4,
/*;0x0094  IRQ5*/
    (fp)INT_Excep_IRQ5,
/*;0x0098  IRQ6*/
    (fp)INT_Excep_IRQ6,
/*;0x009C  IRQ7*/
    (fp)INT_Excep_IRQ7,
/*;0x00A0  PL310ERR*/
    (fp)INT_Excep_PL310ERR,
/*;0x00A4  DMAINT0*/
    (fp)INT_Excep_DMAINT0,
/*;0x00A8  DMAINT1*/
    (fp)INT_Excep_DMAINT1,
/*;0x00AC  DMAINT2*/
    (fp)INT_Excep_DMAINT2,
/*;0x00B0  DMAINT3*/
    (fp)INT_Excep_DMAINT3,
/*;0x00B4  DMAINT4*/
    (fp)INT_Excep_DMAINT4,
/*;0x00B8  DMAINT5*/
    (fp)INT_Excep_DMAINT5,
/*;0x00BC  DMAINT6*/
    (fp)INT_Excep_DMAINT6,
/*;0x00C0  DMAINT7*/
    (fp)INT_Excep_DMAINT7,
/*;0x00C4  DMAINT8*/
    (fp)INT_Excep_DMAINT8,
/*;0x00C8  DMAINT9*/
    (fp)INT_Excep_DMAINT9,
/*;0x00CC  DMAINT10*/
    (fp)INT_Excep_DMAINT10,
/*;0x00D0  DMAINT11*/
    (fp)INT_Excep_DMAINT11,
/*;0x00D4  DMAINT12*/
    (fp)INT_Excep_DMAINT12,
/*;0x00D8  DMAINT13*/
    (fp)INT_Excep_DMAINT13,
/*;0x00DC  DMAINT14*/
    (fp)INT_Excep_DMAINT14,
/*;0x00E0  DMAINT15*/
    (fp)INT_Excep_DMAINT15,
/*;0x00E4  DMAERR*/
    (fp)INT_Excep_DMAERR,
/*;0x00E8 Reserved*/
    (fp)0,
/*;0x00EC Reserved*/
    (fp)0,
/*;0x00F0 Reserved*/
    (fp)0,
/*;0x00F4 Reserved*/
    (fp)0,
/*;0x00F8 Reserved*/
    (fp)0,
/*;0x00FC Reserved*/
    (fp)0,
/*;0x0100 Reserved*/
    (fp)0,
/*;0x0104 Reserved*/
    (fp)0,
/*;0x0108 Reserved*/
    (fp)0,
/*;0x010C Reserved*/
    (fp)0,
/*;0x0110 Reserved*/
    (fp)0,
/*;0x0114 Reserved*/
    (fp)0,
/*;0x0118 Reserved*/
    (fp)0,
/*;0x011C Reserved*/
    (fp)0,
/*;0x0120 Reserved*/
    (fp)0,
/*;0x0124 USBI0*/
    (fp)INT_Excep_USBI0,
/*;0x0128 USBI1*/
    (fp)INT_Excep_USBI1,
/*;0x012C S0_VI_VSYNC0*/
    (fp)INT_Excep_S0_VI_VSYNC0,
/*;0x0130 S0_LO_VSYNC0*/
    (fp)INT_Excep_S0_LO_VSYNC0,
/*;0x0134 S0_VSYNCERR0*/
    (fp)INT_Excep_S0_VSYNCERR0,
/*;0x0138 GR3_VLINE0*/
    (fp)INT_Excep_GR3_VLINE0,
/*;0x013C S0_VFIELD0*/
    (fp)INT_Excep_S0_VFIELD0,
/*;0x0140 IV1_VBUFERR0*/
    (fp)INT_Excep_IV1_VBUFERR0,
/*;0x0144 IV3_VBUFERR0*/
    (fp)INT_Excep_IV3_VBUFERR0,
/*;0x0148 IV5_VBUFERR0*/
    (fp)INT_Excep_IV5_VBUFERR0,
/*;0x014C IV6_VBUFERR0*/
    (fp)INT_Excep_IV6_VBUFERR0,
/*;0x0150 S0_WLINE0*/
    (fp)INT_Excep_S0_WLINE0,
/*;0x0154 Reserved*/
    (fp)0,
/*;0x0158 Reserved*/
    (fp)0,
/*;0x015C Reserved*/
    (fp)0,
/*;0x0160 Reserved*/
    (fp)0,
/*;0x0164 Reserved*/
    (fp)0,
/*;0x0168 Reserved*/
    (fp)0,
/*;0x016C Reserved*/
    (fp)0,
/*;0x0170 Reserved*/
    (fp)0,
/*;0x0174 Reserved*/
    (fp)0,
/*;0x0178 Reserved*/
    (fp)0,
/*;0x017C Reserved*/
    (fp)0,
/*;0x0180 Reserved*/
    (fp)0,
/*;0x0184 Reserved*/
    (fp)0,
/*;0x0188 Reserved*/
    (fp)0,
/*;0x018C Reserved*/
    (fp)0,
/*;0x0190 Reserved*/
    (fp)0,
/*;0x0194 Reserved*/
    (fp)0,
/*;0x0198 Reserved*/
    (fp)0,
/*;0x019C Reserved*/
    (fp)0,
/*;0x01A0 Reserved*/
    (fp)0,
/*;0x01A4 Reserved*/
    (fp)0,
/*;0x01A8 Reserved*/
    (fp)0,
/*;0x01AC Reserved*/
    (fp)0,
/*;0x01B0 Reserved*/
    (fp)0,
/*;0x01B4 Reserved*/
    (fp)0,
/*;0x01B8 Reserved*/
    (fp)0,
/*;0x01BC Reserved*/
    (fp)0,
/*;0x01C0 Reserved*/
    (fp)0,
/*;0x01C4 Reserved*/
    (fp)0,
/*;0x01C8 Reserved*/
    (fp)0,
/*;0x01CC Reserved*/
    (fp)0,
/*;0x01D0 Reserved*/
    (fp)0,
/*;0x01D4 Reserved*/
    (fp)0,
/*;0x01D8 Reserved*/
    (fp)0,
/*;0x01DC Reserved*/
    (fp)0,
/*;0x01E0 Reserved*/
    (fp)0,
/*;0x01E4 Reserved*/
    (fp)0,
/*;0x01E8 Reserved*/
    (fp)0,
/*;0x01EC Reserved*/
    (fp)0,
/*;0x01F0 Reserved*/
    (fp)0,
/*;0x01F4 Reserved*/
    (fp)0,
/*;0x01F8 JEDI*/
    (fp)INT_Excep_JEDI,
/*;0x01FC JDTI*/
    (fp)INT_Excep_JDTI,
/*;0x0200 Reserved*/
    (fp)0,
/*;0x0204 Reserved*/
    (fp)0,
/*;0x0208 Reserved*/
    (fp)0,
/*;0x020C Reserved*/
    (fp)0,
/*;0x0210 Reserved*/
    (fp)0,
/*;0x0214 Reserved*/
    (fp)0,
/*;0x0218 OSTMI0*/
    (fp)INT_Excep_OSTMI0,
/*;0x021C OSTMI1*/
    (fp)INT_Excep_OSTMI1,
/*;0x0220 CMI*/
    (fp)INT_Excep_CMI,
/*;0x0224 WTOUT*/
    (fp)INT_Excep_WTOUT,
/*;0x0228 ITI*/
    (fp)INT_Excep_ITI,
/*;0x022C TGI0A*/
    (fp)INT_Excep_TGI0A,
/*;0x0230 TGI0B*/
    (fp)INT_Excep_TGI0B,
/*;0x0234 TGI0C*/
    (fp)INT_Excep_TGI0C,
/*;0x0238 TGI0D*/
    (fp)INT_Excep_TGI0D,
/*;0x023C TGI0V*/
    (fp)INT_Excep_TGI0V,
/*;0x0240 TGI0E*/
    (fp)INT_Excep_TGI0E,
/*;0x0244 TGI0F*/
    (fp)INT_Excep_TGI0F,
/*;0x0248 TGI1A*/
    (fp)INT_Excep_TGI1A,
/*;0x024C TGI1B*/
    (fp)INT_Excep_TGI1B,
/*;0x0250 TGI1V*/
    (fp)INT_Excep_TGI1V,
/*;0x0254 TGI1U*/
    (fp)INT_Excep_TGI1U,
/*;0x0258 TGI2A*/
    (fp)INT_Excep_TGI2A,
/*;0x025C TGI2B*/
    (fp)INT_Excep_TGI2B,
/*;0x0260 TGI2V*/
    (fp)INT_Excep_TGI2V,
/*;0x0264 TGI2U*/
    (fp)INT_Excep_TGI2U,
/*;0x0268 TGI3A*/
    (fp)INT_Excep_TGI3A,
/*;0x026C TGI3B*/
    (fp)INT_Excep_TGI3B,
/*;0x0270 TGI3C*/
    (fp)INT_Excep_TGI3C,
/*;0x0274 TGI3D*/
    (fp)INT_Excep_TGI3D,
/*;0x0278 TGI3V*/
    (fp)INT_Excep_TGI3V,
/*;0x027C TGI4A*/
    (fp)INT_Excep_TGI4A,
/*;0x0280 TGI4B*/
    (fp)INT_Excep_TGI4B,
/*;0x0284 TGI4C*/
    (fp)INT_Excep_TGI4C,
/*;0x0288 TGI4D*/
    (fp)INT_Excep_TGI4D,
/*;0x028C TGI4V*/
    (fp)INT_Excep_TGI4V,
/*;0x0290 Reserved*/
    (fp)0,
/*;0x0294 Reserved*/
    (fp)0,
/*;0x0298 Reserved*/
    (fp)0,
/*;0x029C Reserved*/
    (fp)0,
/*;0x02A0 Reserved*/
    (fp)0,
/*;0x02A4 Reserved*/
    (fp)0,
/*;0x02A8 ADI*/
    (fp)INT_Excep_ADI,
/*;0x02AC ADWAR*/
    (fp)INT_Excep_ADWAR,
/*;0x02B0 SSII0*/
    (fp)INT_Excep_SSII0,
/*;0x02B4 SSIRXI0*/
    (fp)INT_Excep_SSIRXI0,
/*;0x02B8 SSITXI0*/
    (fp)INT_Excep_SSITXI0,
/*;0x02BC SSII1*/
    (fp)INT_Excep_SSII1,
/*;0x02C0 SSIRXI1*/
    (fp)INT_Excep_SSIRXI1,
/*;0x02C4 SSITXI1*/
    (fp)INT_Excep_SSITXI1,
/*;0x02C8 SSII2*/
    (fp)INT_Excep_SSII2,
/*;0x02CC SSIRTI2*/
    (fp)INT_Excep_SSIRTI2,
/*;0x02D0 SSII3*/
    (fp)INT_Excep_SSII3,
/*;0x02D4 SSIRXI3*/
    (fp)INT_Excep_SSIRXI3,
/*;0x02D8 SSITXI3*/
    (fp)INT_Excep_SSITXI3,
/*;0x02DC Reserved*/
    (fp)0,
/*;0x02E0 Reserved*/
    (fp)0,
/*;0x02E4 Reserved*/
    (fp)0,
/*;0x02E8 Reserved*/
    (fp)0,
/*;0x02EC Reserved*/
    (fp)0,
/*;0x02F0 SPDIFI*/
    (fp)INT_Excep_SPDIFI,
/*;0x02F4 I2C_TEI0*/
    (fp)INT_Excep_I2C_TEI0,
/*;0x02F8 RI0*/
    (fp)INT_Excep_RI0,
/*;0x02FC TI0*/
    (fp)INT_Excep_TI0,
/*;0x0300 SPI0*/
    (fp)INT_Excep_SPI0,
/*;0x0304 STI0*/
    (fp)INT_Excep_STI0,
/*;0x0308 NAKI0*/
    (fp)INT_Excep_NAKI0,
/*;0x030C ALI0*/
    (fp)INT_Excep_ALI0,
/*;0x0310 TMOI0*/
    (fp)INT_Excep_TMOI0,
/*;0x0314 I2C_TEI1*/
    (fp)INT_Excep_I2C_TEI1,
/*;0x0318 RI1*/
    (fp)INT_Excep_RI1,
/*;0x031C TI1*/
    (fp)INT_Excep_TI1,
/*;0x0320 SPI1*/
    (fp)INT_Excep_SPI1,
/*;0x0324 STI1*/
    (fp)INT_Excep_STI1,
/*;0x0328 NAKI1*/
    (fp)INT_Excep_NAKI1,
/*;0x032C ALI1*/
    (fp)INT_Excep_ALI1,
/*;0x0330 TMOI1*/
    (fp)INT_Excep_TMOI1,
/*;0x0334 TEI2*/
    (fp)INT_Excep_TEI2,
/*;0x0338 RI2*/
    (fp)INT_Excep_RI2,
/*;0x033C TI2*/
    (fp)INT_Excep_TI2,
/*;0x0340 SPI2*/
    (fp)INT_Excep_SPI2,
/*;0x0344 STI2*/
    (fp)INT_Excep_STI2,
/*;0x0348 NAKI2*/
    (fp)INT_Excep_NAKI2,
/*;0x034C ALI2*/
    (fp)INT_Excep_ALI2,
/*;0x0350 TMOI2*/
    (fp)INT_Excep_TMOI2,
/*;0x0354 TEI3*/
    (fp)INT_Excep_TEI3,
/*;0x0358 RI3*/
    (fp)INT_Excep_RI3,
/*;0x035C TI3*/
    (fp)INT_Excep_TI3,
/*;0x0360 SPI3*/
    (fp)INT_Excep_SPI3,
/*;0x0364 STI3*/
    (fp)INT_Excep_STI3,
/*;0x0368 NAKI3*/
    (fp)INT_Excep_NAKI3,
/*;0x036C ALI3*/
    (fp)INT_Excep_ALI3,
/*;0x0370 TMOI3*/
    (fp)INT_Excep_TMOI3,
/*;0x0374 FIFO_BRI0*/
    (fp)INT_Excep_FIFO_BRI0,
/*;0x0378 FIFO_ERI0*/
    (fp)INT_Excep_FIFO_ERI0,
/*;0x037C FIFO_RXI0*/
    (fp)INT_Excep_FIFO_RXI0,
/*;0x0380 FIFO_TXI0*/
    (fp)INT_Excep_FIFO_TXI0,
/*;0x0384 FIFO_BRI1*/
    (fp)INT_Excep_FIFO_BRI1,
/*;0x0388 FIFO_ERI1*/
    (fp)INT_Excep_FIFO_ERI1,
/*;0x038C FIFO_RXI1*/
    (fp)INT_Excep_FIFO_RXI1,
/*;0x0390 FIFO_TXI1*/
    (fp)INT_Excep_FIFO_TXI1,
/*;0x0394 BRI2*/
    (fp)INT_Excep_BRI2,
/*;0x0398 ERI2*/
    (fp)INT_Excep_ERI2,
/*;0x039C RXI2*/
    (fp)INT_Excep_RXI2,
/*;0x03A0 TXI2*/
    (fp)INT_Excep_TXI2,
/*;0x03A4 BRI3*/
    (fp)INT_Excep_BRI3,
/*;0x03A8 ERI3*/
    (fp)INT_Excep_ERI3,
/*;0x03AC RXI3*/
    (fp)INT_Excep_RXI3,
/*;0x03B0 TXI3*/
    (fp)INT_Excep_TXI3,
/*;0x03B4 BRI4*/
    (fp)INT_Excep_BRI4,
/*;0x03B8 ERI4*/
    (fp)INT_Excep_ERI4,
/*;0x03BC RXI4*/
    (fp)INT_Excep_RXI4,
/*;0x03C0 TXI4*/
    (fp)INT_Excep_TXI4,
/*;0x03C4 Reserved*/
    (fp)0,
/*;0x03C8 Reserved*/
    (fp)0,
/*;0x03CC Reserved*/
    (fp)0,
/*;0x03D0 Reserved*/
    (fp)0,
/*;0x03D4 Reserved*/
    (fp)0,
/*;0x03D8 Reserved*/
    (fp)0,
/*;0x03DC Reserved*/
    (fp)0,
/*;0x03E0 Reserved*/
    (fp)0,
/*;0x03E4 Reserved*/
    (fp)0,
/*;0x03E8 Reserved*/
    (fp)0,
/*;0x03EC Reserved*/
    (fp)0,
/*;0x03F0 Reserved*/
    (fp)0,
/*;0x03F4 GERI*/
    (fp)INT_Excep_GERI,
/*;0x03F8 RFI*/
    (fp)INT_Excep_RFI,
/*;0x03FC CFRXI0*/
    (fp)INT_Excep_CFRXI0,
/*;0x0400 CERI0*/
    (fp)INT_Excep_CERI0,
/*;0x0404 CTXI0*/
    (fp)INT_Excep_CTXI0,
/*;0x0408 CFRXI1*/
    (fp)INT_Excep_CFRXI1,
/*;0x040C CERI1*/
    (fp)INT_Excep_CERI1,
/*;0x0410 CTXI1*/
    (fp)INT_Excep_CTXI1,
/*;0x0414 Reserved*/
    (fp)0,
/*;0x0418 Reserved*/
    (fp)0,
/*;0x041C Reserved*/
    (fp)0,
/*;0x0420 Reserved*/
    (fp)0,
/*;0x0424 Reserved*/
    (fp)0,
/*;0x0428 Reserved*/
    (fp)0,
/*;0x042C Reserved*/
    (fp)0,
/*;0x0430 Reserved*/
    (fp)0,
/*;0x0434 Reserved*/
    (fp)0,
/*;0x0438 SPEI0*/
    (fp)INT_Excep_SPEI0,
/*;0x043C SPRI0*/
    (fp)INT_Excep_SPRI0,
/*;0x0440 SPTI0*/
    (fp)INT_Excep_SPTI0,
/*;0x0444 SPEI1*/
    (fp)INT_Excep_SPEI1,
/*;0x0448 SPRI1*/
    (fp)INT_Excep_SPRI1,
/*;0x044C SPTI1*/
    (fp)INT_Excep_SPTI1,
/*;0x0450 SPEI2*/
    (fp)INT_Excep_SPEI2,
/*;0x0454 SPRI2*/
    (fp)INT_Excep_SPRI2,
/*;0x0458 SPTI2*/
    (fp)INT_Excep_SPTI2,
/*;0x045C Reserved*/
    (fp)0,
/*;0x0460 Reserved*/
    (fp)0,
/*;0x0464 Reserved*/
    (fp)0,
/*;0x0468 Reserved*/
    (fp)0,
/*;0x046C Reserved*/
    (fp)0,
/*;0x0470 Reserved*/
    (fp)0,
/*;0x0474 Reserved*/
    (fp)0,
/*;0x0478 Reserved*/
    (fp)0,
/*;0x047C Reserved*/
    (fp)0,
/*;0x0480 Reserved*/
    (fp)0,
/*;0x0484 Reserved*/
    (fp)0,
/*;0x0488 Reserved*/
    (fp)0,
/*;0x048C Reserved*/
    (fp)0,
/*;0x0490 Reserved*/
    (fp)0,
/*;0x0494 Reserved*/
    (fp)0,
/*;0x0498 Reserved*/
    (fp)0,
/*;0x049C Reserved*/
    (fp)0,
/*;0x04A0 Reserved*/
    (fp)0,
/*;0x04A4 Reserved*/
    (fp)0,
/*;0x04A8 Reserved*/
    (fp)0,
/*;0x04AC MMC0*/
    (fp)INT_Excep_MMC0,
/*;0x04B0 MMC1*/
    (fp)INT_Excep_MMC1,
/*;0x04B4 MMC2*/
    (fp)INT_Excep_MMC2,
/*;0x04B8 SDHI0_3*/
    (fp)INT_Excep_SDHI0_3,
/*;0x04BC SDHI0_0*/
    (fp)INT_Excep_SDHI0_0,
/*;0x04C0 SDHI0_1*/
    (fp)INT_Excep_SDHI0_1,
/*;0x04C4 SDHI1_3*/
    (fp)INT_Excep_SDHI1_3,
/*;0x04C8 SDHI1_0*/
    (fp)INT_Excep_SDHI1_0,
/*;0x04CC SDHI1_1*/
    (fp)INT_Excep_SDHI1_1,
/*;0x04D0 ARM*/
    (fp)INT_Excep_ARM,
/*;0x04D4 PRD*/
    (fp)INT_Excep_PRD,
/*;0x04D8 CUP*/
    (fp)INT_Excep_CUP,
/*;0x04DC SCUAI0*/
    (fp)INT_Excep_SCUAI0,
/*;0x04E0 SCUAI1*/
    (fp)INT_Excep_SCUAI1,
/*;0x04E4 SCUFDI0*/
    (fp)INT_Excep_SCUFDI0,
/*;0x04E8 SCUFDI1*/
    (fp)INT_Excep_SCUFDI1,
/*;0x04EC SCUFDI2*/
    (fp)INT_Excep_SCUFDI2,
/*;0x04F0 SCUFDI3*/
    (fp)INT_Excep_SCUFDI3,
/*;0x04F4 SCUFUI0*/
    (fp)INT_Excep_SCUFUI0,
/*;0x04F8 SCUFUI1*/
    (fp)INT_Excep_SCUFUI1,
/*;0x04FC SCUFUI2*/
    (fp)INT_Excep_SCUFUI2,
/*;0x0500 SCUFUI3*/
    (fp)INT_Excep_SCUFUI3,
/*;0x0504 SCUDVI0*/
    (fp)INT_Excep_SCUDVI0,
/*;0x0508 SCUDVI1*/
    (fp)INT_Excep_SCUDVI1,
/*;0x050C SCUDVI2*/
    (fp)INT_Excep_SCUDVI2,
/*;0x0510 SCUDVI3*/
    (fp)INT_Excep_SCUDVI3,
/*;0x0514 Reserved*/
    (fp)0,
/*;0x0518 Reserved*/
    (fp)0,
/*;0x051C Reserved*/
    (fp)0,
/*;0x0520 Reserved*/
    (fp)0,
/*;0x0524 Reserved*/
    (fp)0,
/*;0x0528 Reserved*/
    (fp)0,
/*;0x052C Reserved*/
    (fp)0,
/*;0x0530 Reserved*/
    (fp)0,
/*;0x0534 Reserved*/
    (fp)0,
/*;0x0538 Reserved*/
    (fp)0,
/*;0x053C Reserved*/
    (fp)0,
/*;0x0540 Reserved*/
    (fp)0,
/*;0x0544 Reserved*/
    (fp)0,
/*;0x0548 Reserved*/
    (fp)0,
/*;0x054C Reserved*/
    (fp)0,
/*;0x0550 Reserved*/
    (fp)0,
/*;0x0554 Reserved*/
    (fp)0,
/*;0x0558 Reserved*/
    (fp)0,
/*;0x055C Reserved*/
    (fp)0,
/*;0x0560 Reserved*/
    (fp)0,
/*;0x0564 Reserved*/
    (fp)0,
/*;0x0568 Reserved*/
    (fp)0,
/*;0x056C ERI0*/
    (fp)INT_Excep_ERI0,
/*;0x0570 RXI0*/
    (fp)INT_Excep_RXI0,
/*;0x0574 TXI0*/
    (fp)INT_Excep_TXI0,
/*;0x0578 TEI0*/
    (fp)INT_Excep_TEI0,
/*;0x057C ERI1*/
    (fp)INT_Excep_ERI1,
/*;0x0580 RXI1*/
    (fp)INT_Excep_RXI1,
/*;0x0584 TXI1*/
    (fp)INT_Excep_TXI1,
/*;0x0588 TEI1*/
    (fp)INT_Excep_TEI1,
/*;0x058C  AVBI_DATA */
    (fp)INT_Excep_AVBI_DATA,
/*;0x0590  AVBI_ERROR */
    (fp)INT_Excep_AVBI_ERROR,
/*;0x0594  AVBI_MANAGE */
    (fp)INT_Excep_AVBI_MANAGE,
/*;0x0598  AVBI_MAC */
    (fp)INT_Excep_AVBI_MAC,
/*;0x059C ETHERI*/
    (fp)INT_Excep_ETHERI,
/*;0x05A0 Reserved*/
    (fp)0,
/*;0x05A4 Reserved*/
    (fp)0,
/*;0x05A8 Reserved*/
    (fp)0,
/*;0x05AC Reserved*/
    (fp)0,
/*;0x05B0 CEUI*/
    (fp)INT_Excep_CEUI,
/*;0x05B4 Reserved*/
    (fp)0,
/*;0x05B8 Reserved*/
    (fp)0,
/*;0x05BC Reserved*/
    (fp)0,
/*;0x05C0 Reserved*/
    (fp)0,
/*;0x05C4 Reserved*/
    (fp)0,
/*;0x05C8 Reserved*/
    (fp)0,
/*;0x05CC Reserved*/
    (fp)0,
/*;0x05D0 Reserved*/
    (fp)0,
/*;0x05D4 Reserved*/
    (fp)0,
/*;0x05D8 Reserved*/
    (fp)0,
/*;0x05DC Reserved*/
    (fp)0,
/*;0x05E0 Reserved*/
    (fp)0,
/*;0x05E4 Reserved*/
    (fp)0,
/*;0x05E8 Reserved*/
    (fp)0,
/*;0x05EC Reserved*/
    (fp)0,
/*;0x05F0 Reserved*/
    (fp)0,
/*;0x05F4 H2XMLB_ERRINT*/
    (fp)INT_Excep_H2XMLB_ERRINT,
/*;0x05F8 H2XIC1_ERRINT*/
    (fp)INT_Excep_H2XIC1_ERRINT,
/*;0x05FC X2HPERI1_ERRINT*/
    (fp)INT_Excep_X2HPERI1_ERRINT,
/*;0x0600 X2HPERI2_ERRINT*/
    (fp)INT_Excep_X2HPERI2_ERRINT,
/*;0x0604 X2HPERI34_ERRINT*/
    (fp)INT_Excep_X2HPERI34_ERRINT,
/*;0x0608 X2HPERI5_ERRINT*/
    (fp)INT_Excep_X2HPERI5_ERRINT,
/*;0x060C X2HPERI67_ERRINT*/
    (fp)INT_Excep_X2HPERI67_ERRINT,
/*;0x0610 X2HDBGR_ERRINT*/
    (fp)INT_Excep_X2HDBGR_ERRINT,
/*;0x0614  X2HBSC_ERRINT */
    (fp)INT_Excep_X2HBSC_ERRINT,
/*;0x0618  X2HSPI1_ERRINT */
    (fp)INT_Excep_X2HSPI1_ERRINT,
/*;0x061C  X2HSPI2_ERRINT */
    (fp)INT_Excep_X2HSPI2_ERRINT,
/*;0x0620  PRRI */
    (fp)INT_Excep_PRRI,
/*;0x0624 Reserved*/
    (fp)0,
/*;0x0628 Reserved*/
    (fp)0,
/*;0x062C Reserved*/
    (fp)0,
/*;0x0630 Reserved*/
    (fp)0,
/*;0x0634 Reserved*/
    (fp)0,
/*;0x0638 Reserved*/
    (fp)0,
/*;0x063C Reserved*/
    (fp)0,
/*;0x0640 Reserved*/
    (fp)0,
/*;0x0644 Reserved*/
    (fp)0,
/*;0x0648 Reserved*/
    (fp)0,
/*;0x064C Reserved*/
    (fp)0,
/*;0x0650 Reserved*/
    (fp)0,
/*;0x0654 Reserved*/
    (fp)0,
/*;0x0658 Reserved*/
    (fp)0,
/*;0x065C Reserved*/
    (fp)0,
/*;0x0660 Reserved*/
    (fp)0,
/*;0x0664 Reserved*/
    (fp)0,
/*;0x0668 Reserved*/
    (fp)0,
/*;0x066C Reserved*/
    (fp)0,
/*;0x0670 Reserved*/
    (fp)0,
/*;0x0674 Reserved*/
    (fp)0,
/*;0x0678 Reserved*/
    (fp)0,
/*;0x067C Reserved*/
    (fp)0,
/*;0x0680 TINT0*/
    (fp)INT_Excep_TINT0,
/*;0x0684 TINT1*/
    (fp)INT_Excep_TINT1,
/*;0x0688 TINT2*/
    (fp)INT_Excep_TINT2,
/*;0x068C TINT3*/
    (fp)INT_Excep_TINT3,
/*;0x0690 TINT4*/
    (fp)INT_Excep_TINT4,
/*;0x0694 TINT5*/
    (fp)INT_Excep_TINT5,
/*;0x0698 TINT6*/
    (fp)INT_Excep_TINT6,
/*;0x069C TINT7*/
    (fp)INT_Excep_TINT7,
/*;0x06A0 TINT8*/
    (fp)INT_Excep_TINT8,
/*;0x06A4 TINT9*/
    (fp)INT_Excep_TINT9,
/*;0x06A8 TINT10*/
    (fp)INT_Excep_TINT10,
/*;0x06AC TINT11*/
    (fp)INT_Excep_TINT11,
/*;0x06B0 TINT12*/
    (fp)INT_Excep_TINT12,
/*;0x06B4 TINT13*/
    (fp)INT_Excep_TINT13,
/*;0x06B8 TINT14*/
    (fp)INT_Excep_TINT14,
/*;0x06BC TINT15*/
    (fp)INT_Excep_TINT15,
/*;0x06C0 TINT16*/
    (fp)INT_Excep_TINT16,
/*;0x06C4 TINT17*/
    (fp)INT_Excep_TINT17,
/*;0x06C8 TINT18*/
    (fp)INT_Excep_TINT18,
/*;0x06CC TINT19*/
    (fp)INT_Excep_TINT19,
/*;0x06D0 TINT20*/
    (fp)INT_Excep_TINT20,
/*;0x06D4 TINT21*/
    (fp)INT_Excep_TINT21,
/*;0x06D8 TINT22*/
    (fp)INT_Excep_TINT22,
/*;0x06DC TINT23*/
    (fp)INT_Excep_TINT23,
/*;0x06E0 TINT24*/
    (fp)INT_Excep_TINT24,
/*;0x06E4 TINT25*/
    (fp)INT_Excep_TINT25,
/*;0x06E8 TINT26*/
    (fp)INT_Excep_TINT26,
/*;0x06EC TINT27*/
    (fp)INT_Excep_TINT27,
/*;0x06F0 TINT28*/
    (fp)INT_Excep_TINT28,
/*;0x06F4 TINT29*/
    (fp)INT_Excep_TINT29,
/*;0x06F8 TINT30*/
    (fp)INT_Excep_TINT30,
/*;0x06FC TINT31*/
    (fp)INT_Excep_TINT31,
/*;0x0700 TINT32*/
    (fp)INT_Excep_TINT32,
/*;0x0704 TINT33*/
    (fp)INT_Excep_TINT33,
/*;0x0708 TINT34*/
    (fp)INT_Excep_TINT34,
/*;0x070C TINT35*/
    (fp)INT_Excep_TINT35,
/*;0x0710 TINT36*/
    (fp)INT_Excep_TINT36,
/*;0x0714 TINT37*/
    (fp)INT_Excep_TINT37,
/*;0x0718 TINT38*/
    (fp)INT_Excep_TINT38,
/*;0x071C TINT39*/
    (fp)INT_Excep_TINT39,
/*;0x0720 TINT40*/
    (fp)INT_Excep_TINT40,
/*;0x0724 TINT41*/
    (fp)INT_Excep_TINT41,
/*;0x0728 TINT42*/
    (fp)INT_Excep_TINT42,
/*;0x072C TINT43*/
    (fp)INT_Excep_TINT43,
/*;0x0730 TINT44*/
    (fp)INT_Excep_TINT44,
/*;0x0734 TINT45*/
    (fp)INT_Excep_TINT45,
/*;0x0738 TINT46*/
    (fp)INT_Excep_TINT46,
/*;0x073C TINT47*/
    (fp)INT_Excep_TINT47,
/*;0x0740 TINT48*/
    (fp)INT_Excep_TINT48,
/*;0x0744 TINT49*/
    (fp)INT_Excep_TINT49,
/*;0x0748 TINT50*/
    (fp)INT_Excep_TINT50,
/*;0x074C TINT51*/
    (fp)INT_Excep_TINT51,
/*;0x0750 TINT52*/
    (fp)INT_Excep_TINT52,
/*;0x0754 TINT53*/
    (fp)INT_Excep_TINT53,
/*;0x0758 TINT54*/
    (fp)INT_Excep_TINT54,
/*;0x075C TINT55*/
    (fp)INT_Excep_TINT55,
/*;0x0760 TINT56*/
    (fp)INT_Excep_TINT56,
/*;0x0764 TINT57*/
    (fp)INT_Excep_TINT57,
/*;0x0768 TINT58*/
    (fp)INT_Excep_TINT58,
/*;0x076C TINT59*/
    (fp)INT_Excep_TINT59,
/*;0x0770 TINT60*/
    (fp)INT_Excep_TINT60,
/*;0x0774 TINT61*/
    (fp)INT_Excep_TINT61,
/*;0x0778 TINT62*/
    (fp)INT_Excep_TINT62,
/*;0x077C TINT63*/
    (fp)INT_Excep_TINT63,
/*;0x0780 TINT64*/
    (fp)INT_Excep_TINT64,
/*;0x0784 TINT65*/
    (fp)INT_Excep_TINT65,
/*;0x0788 TINT66*/
    (fp)INT_Excep_TINT66,
/*;0x078C TINT67*/
    (fp)INT_Excep_TINT67,
/*;0x0790 TINT68*/
    (fp)INT_Excep_TINT68,
/*;0x0794 TINT69*/
    (fp)INT_Excep_TINT69,
/*;0x0798 TINT70*/
    (fp)INT_Excep_TINT70,
/*;0x079C TINT71*/
    (fp)INT_Excep_TINT71,
/*;0x07A0 TINT72*/
    (fp)INT_Excep_TINT72,
/*;0x07A4 TINT73*/
    (fp)INT_Excep_TINT73,
/*;0x07A8 TINT74*/
    (fp)INT_Excep_TINT74,
/*;0x07AC TINT75*/
    (fp)INT_Excep_TINT75,
/*;0x07B0 TINT76*/
    (fp)INT_Excep_TINT76,
/*;0x07B4 TINT77*/
    (fp)INT_Excep_TINT77,
/*;0x07B8 TINT78*/
    (fp)INT_Excep_TINT78,
/*;0x07BC TINT79*/
    (fp)INT_Excep_TINT79,
/*;0x07C0 TINT80*/
    (fp)INT_Excep_TINT80,
/*;0x07C4 TINT81*/
    (fp)INT_Excep_TINT81,
/*;0x07C8 TINT82*/
    (fp)INT_Excep_TINT82,
/*;0x07CC TINT83*/
    (fp)INT_Excep_TINT83,
/*;0x07D0 TINT84*/
    (fp)INT_Excep_TINT84,
/*;0x07D4 TINT85*/
    (fp)INT_Excep_TINT85,
/*;0x07D8 TINT86*/
    (fp)INT_Excep_TINT86,
/*;0x07DC TINT87*/
    (fp)INT_Excep_TINT87,
/*;0x07E0 TINT88*/
    (fp)INT_Excep_TINT88,
/*;0x07E4 TINT89*/
    (fp)INT_Excep_TINT89,
/*;0x07E8 TINT90*/
    (fp)INT_Excep_TINT90,
/*;0x07EC TINT91*/
    (fp)INT_Excep_TINT91,
/*;0x07F0 TINT92*/
    (fp)INT_Excep_TINT92,
/*;0x07F4 TINT93*/
    (fp)INT_Excep_TINT93,
/*;0x07F8 TINT94*/
    (fp)INT_Excep_TINT94,
/*;0x07FC TINT95*/
    (fp)INT_Excep_TINT95,
/*;0x0800 TINT96*/
    (fp)INT_Excep_TINT96,
/*;0x0804 TINT97*/
    (fp)INT_Excep_TINT97,
/*;0x0808 TINT98*/
    (fp)INT_Excep_TINT98,
/*;0x080C TINT99*/
    (fp)INT_Excep_TINT99,
/*;0x0810 TINT100*/
    (fp)INT_Excep_TINT100,
/*;0x0814 TINT101*/
    (fp)INT_Excep_TINT101,
/*;0x0818 TINT102*/
    (fp)INT_Excep_TINT102,
/*;0x081C TINT103*/
    (fp)INT_Excep_TINT103,
/*;0x0820 TINT104*/
    (fp)INT_Excep_TINT104,
/*;0x0824 TINT105*/
    (fp)INT_Excep_TINT105,
/*;0x0828 TINT106*/
    (fp)INT_Excep_TINT106,
/*;0x082C TINT107*/
    (fp)INT_Excep_TINT107,
/*;0x0830 TINT108*/
    (fp)INT_Excep_TINT108,
/*;0x0834 TINT109*/
    (fp)INT_Excep_TINT109,
/*;0x0838 TINT110*/
    (fp)INT_Excep_TINT110,
/*;0x083C TINT111*/
    (fp)INT_Excep_TINT111,
/*;0x0840 TINT112*/
    (fp)INT_Excep_TINT112,
/*;0x0844 TINT113*/
    (fp)INT_Excep_TINT113,
/*;0x0848 TINT114*/
    (fp)INT_Excep_TINT114,
/*;0x084C TINT115*/
    (fp)INT_Excep_TINT115,
/*;0x0850 TINT116*/
    (fp)INT_Excep_TINT116,
/*;0x0854 TINT117*/
    (fp)INT_Excep_TINT117,
/*;0x0858 TINT118*/
    (fp)INT_Excep_TINT118,
/*;0x085C TINT119*/
    (fp)INT_Excep_TINT119,
/*;0x0860 TINT120*/
    (fp)INT_Excep_TINT120,
/*;0x0864 TINT121*/
    (fp)INT_Excep_TINT121,
};
