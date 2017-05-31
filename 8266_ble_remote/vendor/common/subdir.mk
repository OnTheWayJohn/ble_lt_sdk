################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/common/blt_led.c \
../vendor/common/blt_soft_timer.c \
../vendor/common/e2prom.c \
../vendor/common/myprintf.c \
../vendor/common/tl_audio.c 

OBJS += \
./vendor/common/blt_led.o \
./vendor/common/blt_soft_timer.o \
./vendor/common/e2prom.o \
./vendor/common/myprintf.o \
./vendor/common/tl_audio.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/%.o: ../vendor/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu" -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu_spec" -D__PROJECT_8266_BLE_REMOTE__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


