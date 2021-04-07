################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

-include makefile.init

RM := rm -rf *.lst *.lis *.lpp *.map

# All of the sources participating in the build are defined here
-include sources.mk
-include src/renesas/middleware/video/src/subdir.mk
-include src/renesas/middleware/touch/src/touch/subdir.mk
-include src/renesas/middleware/touch/src/lcd_controller/FT5x06/subdir.mk
-include src/renesas/middleware/touch/src/lcd_controller/FT5216/subdir.mk
-include src/renesas/middleware/touch/src/lcd_controller/subdir.mk
-include src/renesas/middleware/sound/src/subdir.mk
-include src/renesas/middleware/riic_max9856/src/subdir.mk
-include src/renesas/middleware/riic_cat9554/src/subdir.mk
-include src/renesas/middleware/pmod_lcd/src/subdir.mk
-include src/renesas/middleware/ethernet/src/subdir.mk
-include src/renesas/middleware/cmos/src/subdir.mk
-include src/renesas/drivers/r_wdt/src/lld/subdir.mk
-include src/renesas/drivers/r_wdt/src/hld/subdir.mk
-include src/renesas/drivers/r_vdc_vdec/vdec/src/subdir.mk
-include src/renesas/drivers/r_vdc_vdec/vdc_l/src/subdir.mk
-include src/renesas/drivers/r_vdc_vdec/vdc_h/src/subdir.mk
-include src/renesas/drivers/r_vdc_vdec/porting/subdir.mk
-include src/renesas/drivers/r_switch/src/subdir.mk
-include src/renesas/drivers/r_ssif/src/lld/subdir.mk
-include src/renesas/drivers/r_ssif/src/hld/subdir.mk
-include src/renesas/drivers/r_sci/src/lld/subdir.mk
-include src/renesas/drivers/r_sci/src/hld/subdir.mk
-include src/renesas/drivers/r_rtc/src/lld/subdir.mk
-include src/renesas/drivers/r_rtc/src/hld/subdir.mk
-include src/renesas/drivers/r_rspi/src/subdir.mk
-include src/renesas/drivers/r_led/src/subdir.mk
-include src/renesas/drivers/r_l2cc/subdir.mk
-include src/renesas/drivers/r_jcu/userdef/subdir.mk
-include src/renesas/drivers/r_jcu/src/subdir.mk
-include src/renesas/drivers/r_jcu/porting/subdir.mk
-include src/renesas/drivers/r_i2c/src/lld/subdir.mk
-include src/renesas/drivers/r_i2c/src/hld/subdir.mk
-include src/renesas/drivers/r_eeprom/src/subdir.mk
-include src/renesas/drivers/r_dmac/src/lld/subdir.mk
-include src/renesas/drivers/r_dmac/src/hld/subdir.mk
-include src/renesas/drivers/r_ceu/src/subdir.mk
-include src/renesas/drivers/r_ceu/porting/subdir.mk
-include src/renesas/drivers/r_cache/subdir.mk
-include src/renesas/drivers/ostm/src/sc/subdir.mk
-include src/renesas/drivers/ostm/src/lld/subdir.mk
-include src/renesas/drivers/ostm/src/hld/subdir.mk
-include src/renesas/drivers/intc/subdir.mk
-include src/renesas/drivers/adc/subdir.mk
-include src/renesas/configuration/sc/subdir.mk
-include src/renesas/configuration/os_abstraction/src/subdir.mk
-include src/renesas/configuration/os_abstraction/doc/subdir.mk
-include src/renesas/compiler/init/subdir.mk
-include src/renesas/compiler/asm/subdir.mk
-include src/renesas/compiler/subdir.mk
-include src/renesas/application/system/reference/inc/subdir.mk
-include src/renesas/application/system/subdir.mk
-include src/renesas/application/graphics/subdir.mk
-include src/renesas/application/subdir.mk
-include src/renesas/subdir.mk
-include src/arm/cmsis/src/subdir.mk
-include azure/ports/cortex_a9/gnu/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(P_UPPER_DEPS)),)
-include $(P_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(PP_UPPER_DEPS)),)
-include $(PP_UPPER_DEPS)
endif
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif
ifneq ($(strip $(FSY_UPPER_DEPS)),)
-include $(FSY_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(JMP_UPPER_DEPS)),)
-include $(JMP_UPPER_DEPS)
endif
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: librz_bsp.a

# Tool invocations
librz_bsp.a: $(OBJS) $(USER_OBJS)
	@	arm-none-eabi-ar -r  "librz_bsp.a" $(OBJS) $(USER_OBJS) $(LIBS)
# Other Targets
clean:
ifneq ($(strip $(JMP_DEPS)),)
	-$(file > clean_JMP_DEPS.tmp, $(JMP_DEPS))
	-@ xargs -s 6000 -t -a clean_JMP_DEPS.tmp $(RM)
	-@$(RM) clean_JMP_DEPS.tmp
endif
ifneq ($(strip $(FSY_DEPS)),)
	-$(file > clean_FSY_DEPS.tmp, $(FSY_DEPS))
	-@ xargs -s 6000 -t -a clean_FSY_DEPS.tmp $(RM)
	-@$(RM) clean_FSY_DEPS.tmp
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
	-$(file > clean_C_UPPER_DEPS.tmp, $(C_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_C_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_C_UPPER_DEPS.tmp
endif
ifneq ($(strip $(ARCHIVES)),)
	-$(file > clean_ARCHIVES.tmp, $(ARCHIVES))
	-@ xargs -s 6000 -t -a clean_ARCHIVES.tmp $(RM)
	-@$(RM) clean_ARCHIVES.tmp
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
	-$(file > clean_SRC_UPPER_DEPS.tmp, $(SRC_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_SRC_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_SRC_UPPER_DEPS.tmp
endif
ifneq ($(strip $(P_UPPER_DEPS)),)
	-$(file > clean_P_UPPER_DEPS.tmp, $(P_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_P_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_P_UPPER_DEPS.tmp
endif
ifneq ($(strip $(S_DEPS)),)
	-$(file > clean_S_DEPS.tmp, $(S_DEPS))
	-@ xargs -s 6000 -t -a clean_S_DEPS.tmp $(RM)
	-@$(RM) clean_S_DEPS.tmp
endif
ifneq ($(strip $(PP_UPPER_DEPS)),)
	-$(file > clean_PP_UPPER_DEPS.tmp, $(PP_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_PP_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_PP_UPPER_DEPS.tmp
endif
ifneq ($(strip $(P_DEPS)),)
	-$(file > clean_P_DEPS.tmp, $(P_DEPS))
	-@ xargs -s 6000 -t -a clean_P_DEPS.tmp $(RM)
	-@$(RM) clean_P_DEPS.tmp
endif
ifneq ($(strip $(FSY_UPPER_DEPS)),)
	-$(file > clean_FSY_UPPER_DEPS.tmp, $(FSY_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_FSY_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_FSY_UPPER_DEPS.tmp
endif
ifneq ($(strip $(C_DEPS)),)
	-$(file > clean_C_DEPS.tmp, $(C_DEPS))
	-@ xargs -s 6000 -t -a clean_C_DEPS.tmp $(RM)
	-@$(RM) clean_C_DEPS.tmp
endif
ifneq ($(strip $(SRC_DEPS)),)
	-$(file > clean_SRC_DEPS.tmp, $(SRC_DEPS))
	-@ xargs -s 6000 -t -a clean_SRC_DEPS.tmp $(RM)
	-@$(RM) clean_SRC_DEPS.tmp
endif
ifneq ($(strip $(JMP_UPPER_DEPS)),)
	-$(file > clean_JMP_UPPER_DEPS.tmp, $(JMP_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_JMP_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_JMP_UPPER_DEPS.tmp
endif
ifneq ($(strip $(PP_DEPS)),)
	-$(file > clean_PP_DEPS.tmp, $(PP_DEPS))
	-@ xargs -s 6000 -t -a clean_PP_DEPS.tmp $(RM)
	-@$(RM) clean_PP_DEPS.tmp
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
	-$(file > clean_ASM_UPPER_DEPS.tmp, $(ASM_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_ASM_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_ASM_UPPER_DEPS.tmp
endif
ifneq ($(strip $(SX_DEPS)),)
	-$(file > clean_SX_DEPS.tmp, $(SX_DEPS))
	-@ xargs -s 6000 -t -a clean_SX_DEPS.tmp $(RM)
	-@$(RM) clean_SX_DEPS.tmp
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
	-$(file > clean_SX_UPPER_DEPS.tmp, $(SX_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_SX_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_SX_UPPER_DEPS.tmp
endif
ifneq ($(strip $(OBJS)),)
	-$(file > clean_OBJS.tmp, $(OBJS))
	-@ xargs -s 6000 -t -a clean_OBJS.tmp $(RM)
	-@$(RM) clean_OBJS.tmp
endif
ifneq ($(strip $(ASM_DEPS)),)
	-$(file > clean_ASM_DEPS.tmp, $(ASM_DEPS))
	-@ xargs -s 6000 -t -a clean_ASM_DEPS.tmp $(RM)
	-@$(RM) clean_ASM_DEPS.tmp
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
	-$(file > clean_S_UPPER_DEPS.tmp, $(S_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_S_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_S_UPPER_DEPS.tmp
endif
	-$(RM)  librz_bsp.a
	-@echo ' '

.PHONY: all clean dependents 

-include ../makefile.targets
