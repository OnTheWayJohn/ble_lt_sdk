################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../proj/mcu_spec/adc_8266.c \
../proj/mcu_spec/adc_8267.c \
../proj/mcu_spec/gpio_8266.c \
../proj/mcu_spec/gpio_8267.c \
../proj/mcu_spec/pwm_8266.c \
../proj/mcu_spec/pwm_8267.c 

S_UPPER_SRCS += \
../proj/mcu_spec/cstartup_8261.S \
../proj/mcu_spec/cstartup_8266.S \
../proj/mcu_spec/cstartup_8266_ram.S \
../proj/mcu_spec/cstartup_8267.S \
../proj/mcu_spec/cstartup_8267_ram.S \
../proj/mcu_spec/cstartup_8269.S \
../proj/mcu_spec/cstartup_8269_ram.S 

OBJS += \
./proj/mcu_spec/adc_8266.o \
./proj/mcu_spec/adc_8267.o \
./proj/mcu_spec/cstartup_8261.o \
./proj/mcu_spec/cstartup_8266.o \
./proj/mcu_spec/cstartup_8266_ram.o \
./proj/mcu_spec/cstartup_8267.o \
./proj/mcu_spec/cstartup_8267_ram.o \
./proj/mcu_spec/cstartup_8269.o \
./proj/mcu_spec/cstartup_8269_ram.o \
./proj/mcu_spec/gpio_8266.o \
./proj/mcu_spec/gpio_8267.o \
./proj/mcu_spec/pwm_8266.o \
./proj/mcu_spec/pwm_8267.o 


# Each subdirectory must supply rules for building sources it contributes
proj/mcu_spec/%.o: ../proj/mcu_spec/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu" -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu_spec" -D__PROJECT_8266_BLE_REMOTE__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

proj/mcu_spec/%.o: ../proj/mcu_spec/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 CC/Assembler'
	tc32-elf-gcc -DMCU_CORE_8266 -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


