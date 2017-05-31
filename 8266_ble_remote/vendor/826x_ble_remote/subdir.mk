################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/826x_ble_remote/app.c \
../vendor/826x_ble_remote/app_att.c \
../vendor/826x_ble_remote/main.c 

OBJS += \
./vendor/826x_ble_remote/app.o \
./vendor/826x_ble_remote/app_att.o \
./vendor/826x_ble_remote/main.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/826x_ble_remote/%.o: ../vendor/826x_ble_remote/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu" -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu_spec" -D__PROJECT_8266_BLE_REMOTE__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


