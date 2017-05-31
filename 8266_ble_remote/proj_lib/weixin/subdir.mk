################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../proj_lib/weixin/weixin_epb.c 

OBJS += \
./proj_lib/weixin/weixin_epb.o 


# Each subdirectory must supply rules for building sources it contributes
proj_lib/weixin/%.o: ../proj_lib/weixin/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu" -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu_spec" -D__PROJECT_8266_BLE_REMOTE__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


