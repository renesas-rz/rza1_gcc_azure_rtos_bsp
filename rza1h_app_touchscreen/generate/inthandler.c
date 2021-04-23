                                                                          
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
/************************************************************************/
/*    File Version: V1.02                                               */
/*    Date Generated: 30/05/2019                                        */
/************************************************************************/

#include "interrupt_handlers.h"

/* Exception(Undefined Instruction) */
void INT_Excep_UndefinedInst(void){/* brk(); */}

/* Exception(Supervisor Instruction)*/
void INT_Excep_SuperVisorInst(void){/* brk(); */}

/* SWI */
void INT_Excep_SWI(void){/* brk(); */}

/* PREFETCH_ABORT*/
void INT_Excep_PREFETCH_ABORT(void){/* brk(); */}

/* DATA_ABORT*/
void INT_Excep_DATA_ABORT(void){/* brk(); */}

/* Reserved*/
void INT_Excep_Reserved(void){/* brk(); */}

/* IRQ*/
void INT_Excep_IRQ(void){/* brk(); */}

/* FIQ*/
void INT_Excep_FIQ(void){/* brk(); */}

/* Dummy*/
void Dummy(void){/* brk(); */}

/* GIC0 */
void INT_Excep_GIC0(void){ }

/* GIC1*/
void INT_Excep_GIC1(void){ }

/* GIC2*/
void INT_Excep_GIC2(void){ }

/* GIC3*/
void INT_Excep_GIC3(void){ }

/* GIC4*/
void INT_Excep_GIC4(void){ }

/* GIC5*/
void INT_Excep_GIC5(void){ }

/* GIC6*/
void INT_Excep_GIC6(void){ }

/* GIC7*/
void INT_Excep_GIC7(void){ }

/* GIC8*/
void INT_Excep_GIC8(void){ }

/* GIC9*/
void INT_Excep_GIC9(void){ }

/* GIC10*/
void INT_Excep_GIC10(void){ }

/* GIC11*/
void INT_Excep_GIC11(void){ }

/* GIC12*/
void INT_Excep_GIC12(void){ }

/* GIC13*/
void INT_Excep_GIC13(void){ }

/* GIC14*/
void INT_Excep_GIC14(void){ }

/* GIC15*/
void INT_Excep_GIC15(void){ }

/* CPU_PMUIRQ0*/
void INT_Excep_CPU_PMUIRQ0(void){ }

/* CPU_COMMRX0*/
void INT_Excep_CPU_COMMRX0(void){ }

/* CPU_COMMTX0*/
void INT_Excep_CPU_COMMTX0(void){ }

/* CPU_CTIIRQ0*/
void INT_Excep_CPU_CTIIRQ0(void){ }

/* IRQ0*/
void INT_Excep_IRQ0(void){ }

/* IRQ1*/
void INT_Excep_IRQ1(void){ }

/* IRQ2*/
void INT_Excep_IRQ2(void){ }

/* IRQ3*/
void INT_Excep_IRQ3(void){ }

/* IRQ4*/
void INT_Excep_IRQ4(void){ }

/* IRQ5*/
void INT_Excep_IRQ5(void){ }

/* IRQ6*/
void INT_Excep_IRQ6(void){ }

/* IRQ7*/
void INT_Excep_IRQ7(void){ }

/* PL310ERR*/
void INT_Excep_PL310ERR(void){ }

/* DMAINT0*/
void INT_Excep_DMAINT0(void){ }

/* DMAINT1*/
void INT_Excep_DMAINT1(void){ }

/* DMAINT2*/
void INT_Excep_DMAINT2(void){ }

/* DMAINT3*/
void INT_Excep_DMAINT3(void){ }

/* DMAINT4*/
void INT_Excep_DMAINT4(void){ }

/* DMAINT5*/
void INT_Excep_DMAINT5(void){ }

/* DMAINT6*/
void INT_Excep_DMAINT6(void){ }

/* DMAINT7*/
void INT_Excep_DMAINT7(void){ }

/* DMAINT8*/
void INT_Excep_DMAINT8(void){ }

/* DMAINT9*/
void INT_Excep_DMAINT9(void){ }

/* DMAINT10*/
void INT_Excep_DMAINT10(void){ }

/* DMAINT11*/
void INT_Excep_DMAINT11(void){ }

/* DMAINT12*/
void INT_Excep_DMAINT12(void){ }

/* DMAINT13*/
void INT_Excep_DMAINT13(void){ }

/* DMAINT14*/
void INT_Excep_DMAINT14(void){ }

/* DMAINT15*/
void INT_Excep_DMAINT15(void){ }

/* DMAERR*/
void INT_Excep_DMAERR(void){ }

/* USBI0*/
void INT_Excep_USBI0(void){ }

/* USBI1*/
void INT_Excep_USBI1(void){ }

/* S0_VI_VSYNC0*/
void INT_Excep_S0_VI_VSYNC0(void){ }

/* S0_LO_VSYNC0*/
void INT_Excep_S0_LO_VSYNC0(void){ }

/* S0_VSYNCERR0*/
void INT_Excep_S0_VSYNCERR0(void){ }

/* GR3_VLINE0*/
void INT_Excep_GR3_VLINE0(void){ }

/* S0_VFIELD0*/
void INT_Excep_S0_VFIELD0(void){ }

/* IV1_VBUFERR0*/
void INT_Excep_IV1_VBUFERR0(void){ }

/* IV3_VBUFERR0*/
void INT_Excep_IV3_VBUFERR0(void){ }

/* IV5_VBUFERR0*/
void INT_Excep_IV5_VBUFERR0(void){ }

/* IV6_VBUFERR0*/
void INT_Excep_IV6_VBUFERR0(void){ }

/* S0_WLINE0*/
void INT_Excep_S0_WLINE0(void){ }

/* JEDI*/
void INT_Excep_JEDI(void){ }

/* JDTI*/
void INT_Excep_JDTI(void){ }

/* OSTMI0*/
void INT_Excep_OSTMI0(void){ }

/* OSTMI1*/
void INT_Excep_OSTMI1(void){ }

/* CMI*/
void INT_Excep_CMI(void){ }

/* WTOUT*/
void INT_Excep_WTOUT(void){ }

/* ITI*/
void INT_Excep_ITI(void){ }

/* TGI0A*/
void INT_Excep_TGI0A(void){ }

/* TGI0B*/
void INT_Excep_TGI0B(void){ }

/* TGI0C*/
void INT_Excep_TGI0C(void){ }

/* TGI0D*/
void INT_Excep_TGI0D(void){ }

/* TGI0V*/
void INT_Excep_TGI0V(void){ }

/* TGI0E*/
void INT_Excep_TGI0E(void){ }

/* TGI0F*/
void INT_Excep_TGI0F(void){ }

/* TGI1A*/
void INT_Excep_TGI1A(void){ }

/* TGI1B*/
void INT_Excep_TGI1B(void){ }

/* TGI1V*/
void INT_Excep_TGI1V(void){ }

/* TGI1U*/
void INT_Excep_TGI1U(void){ }

/* TGI2A*/
void INT_Excep_TGI2A(void){ }

/* TGI2B*/
void INT_Excep_TGI2B(void){ }

/* TGI2V*/
void INT_Excep_TGI2V(void){ }

/* TGI2U*/
void INT_Excep_TGI2U(void){ }

/* TGI3A*/
void INT_Excep_TGI3A(void){ }

/* TGI3B*/
void INT_Excep_TGI3B(void){ }

/* TGI3C*/
void INT_Excep_TGI3C(void){ }

/* TGI3D*/
void INT_Excep_TGI3D(void){ }

/* TGI3V*/
void INT_Excep_TGI3V(void){ }

/* TGI4A*/
void INT_Excep_TGI4A(void){ }

/* TGI4B*/
void INT_Excep_TGI4B(void){ }

/* TGI4C*/
void INT_Excep_TGI4C(void){ }

/* TGI4D*/
void INT_Excep_TGI4D(void){ }

/* TGI4V*/
void INT_Excep_TGI4V(void){ }

/* ADI*/
void INT_Excep_ADI(void){ }

/* ADWAR*/
void INT_Excep_ADWAR(void){ }

/* SSII0*/
void INT_Excep_SSII0(void){ }

/* SSIRXI0*/
void INT_Excep_SSIRXI0(void){ }

/* SSITXI0*/
void INT_Excep_SSITXI0(void){ }

/* SSII1*/
void INT_Excep_SSII1(void){ }

/* SSIRXI1*/
void INT_Excep_SSIRXI1(void){ }

/* SSITXI1*/
void INT_Excep_SSITXI1(void){ }

/* SSII2*/
void INT_Excep_SSII2(void){ }

/* SSIRTI2*/
void INT_Excep_SSIRTI2(void){ }

/* SSII3*/
void INT_Excep_SSII3(void){ }

/* SSIRXI3*/
void INT_Excep_SSIRXI3(void){ }

/* SSITXI3*/
void INT_Excep_SSITXI3(void){ }

/* SPDIFI*/
void INT_Excep_SPDIFI(void){ }

/* I2C_TEI0*/
void INT_Excep_I2C_TEI0(void){ }

/* RI0*/
void INT_Excep_RI0(void){ }

/* TI0*/
void INT_Excep_TI0(void){ }

/* SPI0*/
void INT_Excep_SPI0(void){ }

/* STI0*/
void INT_Excep_STI0(void){ }

/* NAKI0*/
void INT_Excep_NAKI0(void){ }

/* ALI0*/
void INT_Excep_ALI0(void){ }

/* TMOI0*/
void INT_Excep_TMOI0(void){ }

/* I2C_TEI1*/
void INT_Excep_I2C_TEI1(void){ }

/* RI1*/
void INT_Excep_RI1(void){ }

/* TI1*/
void INT_Excep_TI1(void){ }

/* SPI1*/
void INT_Excep_SPI1(void){ }

/* STI1*/
void INT_Excep_STI1(void){ }

/* NAKI1*/
void INT_Excep_NAKI1(void){ }

/* ALI1*/
void INT_Excep_ALI1(void){ }

/* TMOI1*/
void INT_Excep_TMOI1(void){ }

/* TEI2*/
void INT_Excep_TEI2(void){ }

/* RI2*/
void INT_Excep_RI2(void){ }

/* TI2*/
void INT_Excep_TI2(void){ }

/* SPI2*/
void INT_Excep_SPI2(void){ }

/* STI2*/
void INT_Excep_STI2(void){ }

/* NAKI2*/
void INT_Excep_NAKI2(void){ }

/* ALI2*/
void INT_Excep_ALI2(void){ }

/* TMOI2*/
void INT_Excep_TMOI2(void){ }

/* TEI3*/
void INT_Excep_TEI3(void){ }

/* RI3*/
void INT_Excep_RI3(void){ }

/* TI3*/
void INT_Excep_TI3(void){ }

/* SPI3*/
void INT_Excep_SPI3(void){ }

/* STI3*/
void INT_Excep_STI3(void){ }

/* NAKI3*/
void INT_Excep_NAKI3(void){ }

/* ALI3*/
void INT_Excep_ALI3(void){ }

/* TMOI3*/
void INT_Excep_TMOI3(void){ }

/* FIFO_BRI0*/
void INT_Excep_FIFO_BRI0(void){ }

/* FIFO_ERI0*/
void INT_Excep_FIFO_ERI0(void){ }

/* FIFO_RXI0*/
void INT_Excep_FIFO_RXI0(void){ }

/* FIFO_TXI0*/
void INT_Excep_FIFO_TXI0(void){ }

/* FIFO_BRI1*/
void INT_Excep_FIFO_BRI1(void){ }

/* FIFO_ERI1*/
void INT_Excep_FIFO_ERI1(void){ }

/* FIFO_RXI1*/
void INT_Excep_FIFO_RXI1(void){ }

/* FIFO_TXI1*/
void INT_Excep_FIFO_TXI1(void){ }

/* BRI2*/
void INT_Excep_BRI2(void){ }

/* ERI2*/
void INT_Excep_ERI2(void){ }

/* RXI2*/
void INT_Excep_RXI2(void){ }

/* TXI2*/
void INT_Excep_TXI2(void){ }

/* BRI3*/
void INT_Excep_BRI3(void){ }

/* ERI3*/
void INT_Excep_ERI3(void){ }

/* RXI3*/
void INT_Excep_RXI3(void){ }

/* TXI3*/
void INT_Excep_TXI3(void){ }

/* BRI4*/
void INT_Excep_BRI4(void){ }

/* ERI4*/
void INT_Excep_ERI4(void){ }

/* RXI4*/
void INT_Excep_RXI4(void){ }

/* TXI4*/
void INT_Excep_TXI4(void){ }

/* GERI*/
void INT_Excep_GERI(void){ }

/* RFI*/
void INT_Excep_RFI(void){ }

/* CFRXI0*/
void INT_Excep_CFRXI0(void){ }

/* CERI0*/
void INT_Excep_CERI0(void){ }

/* CTXI0*/
void INT_Excep_CTXI0(void){ }

/* CFRXI1*/
void INT_Excep_CFRXI1(void){ }

/* CERI1*/
void INT_Excep_CERI1(void){ }

/* CTXI1*/
void INT_Excep_CTXI1(void){ }

/* SPEI0*/
void INT_Excep_SPEI0(void){ }

/* SPRI0*/
void INT_Excep_SPRI0(void){ }

/* SPTI0*/
void INT_Excep_SPTI0(void){ }

/* SPEI1*/
void INT_Excep_SPEI1(void){ }

/* SPRI1*/
void INT_Excep_SPRI1(void){ }

/* SPTI1*/
void INT_Excep_SPTI1(void){ }

/* SPEI2*/
void INT_Excep_SPEI2(void){ }

/* SPRI2*/
void INT_Excep_SPRI2(void){ }

/* SPTI2*/
void INT_Excep_SPTI2(void){ }

/* MMC0*/
void INT_Excep_MMC0(void){ }

/* MMC1*/
void INT_Excep_MMC1(void){ }

/* MMC2*/
void INT_Excep_MMC2(void){ }

/* SDHI0_3*/
void INT_Excep_SDHI0_3(void){ }

/* SDHI0_0*/
void INT_Excep_SDHI0_0(void){ }

/* SDHI0_1*/
void INT_Excep_SDHI0_1(void){ }

/* SDHI1_3*/
void INT_Excep_SDHI1_3(void){ }

/* SDHI1_0*/
void INT_Excep_SDHI1_0(void){ }

/* SDHI1_1*/
void INT_Excep_SDHI1_1(void){ }

/* ARM*/
void INT_Excep_ARM(void){ }

/* PRD*/
void INT_Excep_PRD(void){ }

/* CUP*/
void INT_Excep_CUP(void){ }

/* SCUAI0*/
void INT_Excep_SCUAI0(void){ }

/* SCUAI1*/
void INT_Excep_SCUAI1(void){ }

/* SCUFDI0*/
void INT_Excep_SCUFDI0(void){ }

/* SCUFDI1*/
void INT_Excep_SCUFDI1(void){ }

/* SCUFDI2*/
void INT_Excep_SCUFDI2(void){ }

/* SCUFDI3*/
void INT_Excep_SCUFDI3(void){ }

/* SCUFUI0*/
void INT_Excep_SCUFUI0(void){ }

/* SCUFUI1*/
void INT_Excep_SCUFUI1(void){ }

/* SCUFUI2*/
void INT_Excep_SCUFUI2(void){ }

/* SCUFUI3*/
void INT_Excep_SCUFUI3(void){ }

/* SCUDVI0*/
void INT_Excep_SCUDVI0(void){ }

/* SCUDVI1*/
void INT_Excep_SCUDVI1(void){ }

/* SCUDVI2*/
void INT_Excep_SCUDVI2(void){ }

/* SCUDVI3*/
void INT_Excep_SCUDVI3(void){ }

/* ERI0*/
void INT_Excep_ERI0(void){ }

/* RXI0*/
void INT_Excep_RXI0(void){ }

/* TXI0*/
void INT_Excep_TXI0(void){ }

/* TEI0*/
void INT_Excep_TEI0(void){ }

/* ERI1*/
void INT_Excep_ERI1(void){ }

/* RXI1*/
void INT_Excep_RXI1(void){ }

/* TXI1*/
void INT_Excep_TXI1(void){ }

/* TEI1*/
void INT_Excep_TEI1(void){ }

/* AVBI_DATA*/
void INT_Excep_AVBI_DATA(void){ }

/* AVBI_ERROR*/
void INT_Excep_AVBI_ERROR(void){ }

/* AVBI_MANAGE*/
void INT_Excep_AVBI_MANAGE(void){ }

/* AVBI_MAC*/
void INT_Excep_AVBI_MAC(void){ }

/* ETHERI*/
void INT_Excep_ETHERI(void){ }

/* CEUI*/
void INT_Excep_CEUI(void){ }

/* H2XMLB_ERRINT*/
void INT_Excep_H2XMLB_ERRINT(void){ }

/* H2XIC1_ERRINT*/
void INT_Excep_H2XIC1_ERRINT(void){ }

/* X2HPERI1_ERRINT*/
void INT_Excep_X2HPERI1_ERRINT(void){ }

/* X2HPERI2_ERRINT*/
void INT_Excep_X2HPERI2_ERRINT(void){ }

/* X2HPERI34_ERRINT*/
void INT_Excep_X2HPERI34_ERRINT(void){ }

/* X2HPERI5_ERRINT*/
void INT_Excep_X2HPERI5_ERRINT(void){ }

/* X2HPERI67_ERRINT*/
void INT_Excep_X2HPERI67_ERRINT(void){ }

/* X2HDBGR_ERRINT*/
void INT_Excep_X2HDBGR_ERRINT(void){ }

/* X2HBSC_ERRINT */
void INT_Excep_X2HBSC_ERRINT(void){ }

/* X2HSPI1_ERRINT */
void INT_Excep_X2HSPI1_ERRINT(void){ }

/* X2HSPI2_ERRINT */
void INT_Excep_X2HSPI2_ERRINT(void){ }

/* PRRI*/
void INT_Excep_PRRI(void){ }

/* IFEI0*/
void INT_Excep_IFEI0(void){ }

/* OFFI0*/
void INT_Excep_OFFI0(void){ }

/* PFVEI0*/
void INT_Excep_PFVEI0(void){ }

/* TINT0*/
void INT_Excep_TINT0(void){ }

/* TINT1*/
void INT_Excep_TINT1(void){ }

/* TINT2*/
void INT_Excep_TINT2(void){ }

/* TINT3*/
void INT_Excep_TINT3(void){ }

/* TINT4*/
void INT_Excep_TINT4(void){ }

/* TINT5*/
void INT_Excep_TINT5(void){ }

/* TINT6*/
void INT_Excep_TINT6(void){ }

/* TINT7*/
void INT_Excep_TINT7(void){ }

/* TINT8*/
void INT_Excep_TINT8(void){ }

/* TINT9*/
void INT_Excep_TINT9(void){ }

/* TINT10*/
void INT_Excep_TINT10(void){ }

/* TINT11*/
void INT_Excep_TINT11(void){ }

/* TINT12*/
void INT_Excep_TINT12(void){ }

/* TINT13*/
void INT_Excep_TINT13(void){ }

/* TINT14*/
void INT_Excep_TINT14(void){ }

/* TINT15*/
void INT_Excep_TINT15(void){ }

/* TINT16*/
void INT_Excep_TINT16(void){ }

/* TINT17*/
void INT_Excep_TINT17(void){ }

/* TINT18*/
void INT_Excep_TINT18(void){ }

/* TINT19*/
void INT_Excep_TINT19(void){ }

/* TINT20*/
void INT_Excep_TINT20(void){ }

/* TINT21*/
void INT_Excep_TINT21(void){ }

/* TINT22*/
void INT_Excep_TINT22(void){ }

/* TINT23*/
void INT_Excep_TINT23(void){ }

/* TINT24*/
void INT_Excep_TINT24(void){ }

/* TINT25*/
void INT_Excep_TINT25(void){ }

/* TINT26*/
void INT_Excep_TINT26(void){ }

/* TINT27*/
void INT_Excep_TINT27(void){ }

/* TINT28*/
void INT_Excep_TINT28(void){ }

/* TINT29*/
void INT_Excep_TINT29(void){ }

/* TINT30*/
void INT_Excep_TINT30(void){ }

/* TINT31*/
void INT_Excep_TINT31(void){ }

/* TINT32*/
void INT_Excep_TINT32(void){ }

/* TINT33*/
void INT_Excep_TINT33(void){ }

/* TINT34*/
void INT_Excep_TINT34(void){ }

/* TINT35*/
void INT_Excep_TINT35(void){ }

/* TINT36*/
void INT_Excep_TINT36(void){ }

/* TINT37*/
void INT_Excep_TINT37(void){ }

/* TINT38*/
void INT_Excep_TINT38(void){ }

/* TINT39*/
void INT_Excep_TINT39(void){ }

/* TINT40*/
void INT_Excep_TINT40(void){ }

/* TINT41*/
void INT_Excep_TINT41(void){ }

/* TINT42*/
void INT_Excep_TINT42(void){ }

/* TINT43*/
void INT_Excep_TINT43(void){ }

/* TINT44*/
void INT_Excep_TINT44(void){ }

/* TINT45*/
void INT_Excep_TINT45(void){ }

/* TINT46*/
void INT_Excep_TINT46(void){ }

/* TINT47*/
void INT_Excep_TINT47(void){ }

/* TINT48*/
void INT_Excep_TINT48(void){ }

/* TINT49*/
void INT_Excep_TINT49(void){ }

/* TINT50*/
void INT_Excep_TINT50(void){ }

/* TINT51*/
void INT_Excep_TINT51(void){ }

/* TINT52*/
void INT_Excep_TINT52(void){ }

/* TINT53*/
void INT_Excep_TINT53(void){ }

/* TINT54*/
void INT_Excep_TINT54(void){ }

/* TINT55*/
void INT_Excep_TINT55(void){ }

/* TINT56*/
void INT_Excep_TINT56(void){ }

/* TINT57*/
void INT_Excep_TINT57(void){ }

/* TINT58*/
void INT_Excep_TINT58(void){ }

/* TINT59*/
void INT_Excep_TINT59(void){ }

/* TINT60*/
void INT_Excep_TINT60(void){ }

/* TINT61*/
void INT_Excep_TINT61(void){ }

/* TINT62*/
void INT_Excep_TINT62(void){ }

/* TINT63*/
void INT_Excep_TINT63(void){ }

/* TINT64*/
void INT_Excep_TINT64(void){ }

/* TINT65*/
void INT_Excep_TINT65(void){ }

/* TINT66*/
void INT_Excep_TINT66(void){ }

/* TINT67*/
void INT_Excep_TINT67(void){ }

/* TINT68*/
void INT_Excep_TINT68(void){ }

/* TINT69*/
void INT_Excep_TINT69(void){ }

/* TINT70*/
void INT_Excep_TINT70(void){ }

/* TINT71*/
void INT_Excep_TINT71(void){ }

/* TINT72*/
void INT_Excep_TINT72(void){ }

/* TINT73*/
void INT_Excep_TINT73(void){ }

/* TINT74*/
void INT_Excep_TINT74(void){ }

/* TINT75*/
void INT_Excep_TINT75(void){ }

/* TINT76*/
void INT_Excep_TINT76(void){ }

/* TINT77*/
void INT_Excep_TINT77(void){ }

/* TINT78*/
void INT_Excep_TINT78(void){ }

/* TINT79*/
void INT_Excep_TINT79(void){ }

/* TINT80*/
void INT_Excep_TINT80(void){ }

/* TINT81*/
void INT_Excep_TINT81(void){ }

/* TINT82*/
void INT_Excep_TINT82(void){ }

/* TINT83*/
void INT_Excep_TINT83(void){ }

/* TINT84*/
void INT_Excep_TINT84(void){ }

/* TINT85*/
void INT_Excep_TINT85(void){ }

/* TINT86*/
void INT_Excep_TINT86(void){ }

/* TINT87*/
void INT_Excep_TINT87(void){ }

/* TINT88*/
void INT_Excep_TINT88(void){ }

/* TINT89*/
void INT_Excep_TINT89(void){ }

/* TINT90*/
void INT_Excep_TINT90(void){ }

/* TINT91*/
void INT_Excep_TINT91(void){ }

/* TINT92*/
void INT_Excep_TINT92(void){ }

/* TINT93*/
void INT_Excep_TINT93(void){ }

/* TINT94*/
void INT_Excep_TINT94(void){ }

/* TINT95*/
void INT_Excep_TINT95(void){ }

/* TINT96*/
void INT_Excep_TINT96(void){ }

/* TINT97*/
void INT_Excep_TINT97(void){ }

/* TINT98*/
void INT_Excep_TINT98(void){ }

/* TINT99*/
void INT_Excep_TINT99(void){ }

/* TINT100*/
void INT_Excep_TINT100(void){ }

/* TINT101*/
void INT_Excep_TINT101(void){ }

/* TINT102*/
void INT_Excep_TINT102(void){ }

/* TINT103*/
void INT_Excep_TINT103(void){ }

/* TINT104*/
void INT_Excep_TINT104(void){ }

/* TINT105*/
void INT_Excep_TINT105(void){ }

/* TINT106*/
void INT_Excep_TINT106(void){ }

/* TINT107*/
void INT_Excep_TINT107(void){ }

/* TINT108*/
void INT_Excep_TINT108(void){ }

/* TINT109*/
void INT_Excep_TINT109(void){ }

/* TINT110*/
void INT_Excep_TINT110(void){ }

/* TINT111*/
void INT_Excep_TINT111(void){ }

/* TINT112*/
void INT_Excep_TINT112(void){ }

/* TINT113*/
void INT_Excep_TINT113(void){ }

/* TINT114*/
void INT_Excep_TINT114(void){ }

/* TINT115*/
void INT_Excep_TINT115(void){ }

/* TINT116*/
void INT_Excep_TINT116(void){ }

/* TINT117*/
void INT_Excep_TINT117(void){ }

/* TINT118*/
void INT_Excep_TINT118(void){ }

/* TINT119*/
void INT_Excep_TINT119(void){ }

/* TINT120*/
void INT_Excep_TINT120(void){ }

/* TINT121*/
void INT_Excep_TINT121(void){ }

