################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../compiler_specific/asm/access.s \
../compiler_specific/asm/bsp_cache.s \
../compiler_specific/asm/cstartup.s \
../compiler_specific/asm/initsct.s \
../compiler_specific/asm/irqfiq_handler.s \
../compiler_specific/asm/isr.s \
../compiler_specific/asm/l1_cache_init.s \
../compiler_specific/asm/reset_handler.s \
../compiler_specific/asm/ttb_init.s 

S_DEPS += \
./compiler_specific/asm/access.d \
./compiler_specific/asm/bsp_cache.d \
./compiler_specific/asm/cstartup.d \
./compiler_specific/asm/initsct.d \
./compiler_specific/asm/irqfiq_handler.d \
./compiler_specific/asm/isr.d \
./compiler_specific/asm/l1_cache_init.d \
./compiler_specific/asm/reset_handler.d \
./compiler_specific/asm/ttb_init.d 

OBJS += \
./compiler_specific/asm/access.o \
./compiler_specific/asm/bsp_cache.o \
./compiler_specific/asm/cstartup.o \
./compiler_specific/asm/initsct.o \
./compiler_specific/asm/irqfiq_handler.o \
./compiler_specific/asm/isr.o \
./compiler_specific/asm/l1_cache_init.o \
./compiler_specific/asm/reset_handler.o \
./compiler_specific/asm/ttb_init.o 


# Each subdirectory must supply rules for building sources it contributes
compiler_specific/asm/%.o: ../compiler_specific/asm/%.s
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -fdiagnostics-parseable-fixits -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

