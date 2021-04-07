################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LINKER_SCRIPT += \
../generate/linker_script.ld 

S_UPPER_SRCS += \
../generate/start.S \
../generate/vect_table.S 

C_SRCS += \
../generate/hwsetup.c \
../generate/inthandler.c \
../generate/vects.c 

C_DEPS += \
./generate/hwsetup.d \
./generate/inthandler.d \
./generate/vects.d 

OBJS += \
./generate/hwsetup.o \
./generate/inthandler.o \
./generate/start.o \
./generate/vect_table.o \
./generate/vects.o 

S_UPPER_DEPS += \
./generate/start.d \
./generate/vect_table.d 


# Each subdirectory must supply rules for building sources it contributes
generate/%.o: ../generate/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\threadx\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\azure\ports\cortex_a9\gnu\inc" -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
generate/%.o: ../generate/%.S
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

