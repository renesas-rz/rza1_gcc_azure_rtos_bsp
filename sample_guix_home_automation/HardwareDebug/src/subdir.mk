################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/common_hardware_code.c \
../src/controller_base.c \
../src/demo_guix_home_automation.c \
../src/demo_guix_home_automation_resources.c \
../src/demo_guix_home_automation_specifications.c \
../src/helper_c.c \
../src/lights.c \
../src/locks.c \
../src/passcode_screen.c \
../src/screen_base.c \
../src/thermostat.c \
../src/weather.c 

C_DEPS += \
./src/common_hardware_code.d \
./src/controller_base.d \
./src/demo_guix_home_automation.d \
./src/demo_guix_home_automation_resources.d \
./src/demo_guix_home_automation_specifications.d \
./src/helper_c.d \
./src/lights.d \
./src/locks.d \
./src/passcode_screen.d \
./src/screen_base.d \
./src/thermostat.d \
./src/weather.d 

OBJS += \
./src/common_hardware_code.o \
./src/controller_base.o \
./src/demo_guix_home_automation.o \
./src/demo_guix_home_automation_resources.o \
./src/demo_guix_home_automation_specifications.o \
./src/helper_c.o \
./src/lights.o \
./src/locks.o \
./src/passcode_screen.o \
./src/screen_base.o \
./src/thermostat.o \
./src/weather.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=1000 -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\guix\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\threadx\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\addons\dhcp" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/sample_guix_home_automation/src" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

