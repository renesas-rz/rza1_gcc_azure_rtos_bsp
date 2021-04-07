################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../src/renesas/compiler/asm/access.S \
../src/renesas/compiler/asm/fiq_handler.S \
../src/renesas/compiler/asm/initsct.S \
../src/renesas/compiler/asm/isr.S \
../src/renesas/compiler/asm/l1_cache_init.S \
../src/renesas/compiler/asm/reset_handler.S \
../src/renesas/compiler/asm/start.S \
../src/renesas/compiler/asm/ttb_init.S 

SRC_SRCS += \
../src/renesas/compiler/asm/cache-v7.src 

SRC_DEPS += \
./src/renesas/compiler/asm/cache-v7.d 

OBJS += \
./src/renesas/compiler/asm/access.o \
./src/renesas/compiler/asm/cache-v7.o \
./src/renesas/compiler/asm/fiq_handler.o \
./src/renesas/compiler/asm/initsct.o \
./src/renesas/compiler/asm/isr.o \
./src/renesas/compiler/asm/l1_cache_init.o \
./src/renesas/compiler/asm/reset_handler.o \
./src/renesas/compiler/asm/start.o \
./src/renesas/compiler/asm/ttb_init.o 

S_UPPER_DEPS += \
./src/renesas/compiler/asm/access.d \
./src/renesas/compiler/asm/fiq_handler.d \
./src/renesas/compiler/asm/initsct.d \
./src/renesas/compiler/asm/isr.d \
./src/renesas/compiler/asm/l1_cache_init.d \
./src/renesas/compiler/asm/reset_handler.d \
./src/renesas/compiler/asm/start.d \
./src/renesas/compiler/asm/ttb_init.d 


# Each subdirectory must supply rules for building sources it contributes
src/renesas/compiler/asm/%.o: ../src/renesas/compiler/asm/%.S
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -x assembler-with-cpp -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
src/renesas/compiler/asm/%.o: ../src/renesas/compiler/asm/%.src
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -x assembler-with-cpp -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

