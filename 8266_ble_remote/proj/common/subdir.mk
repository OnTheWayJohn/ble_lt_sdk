################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../proj/common/breakpoint.c \
../proj/common/compatibility.c \
../proj/common/list.c \
../proj/common/log.c \
../proj/common/mempool.c \
../proj/common/mmem.c \
../proj/common/printf.c \
../proj/common/qsort.c \
../proj/common/selection_sort.c \
../proj/common/string.c \
../proj/common/utility.c 

OBJS += \
./proj/common/breakpoint.o \
./proj/common/compatibility.o \
./proj/common/list.o \
./proj/common/log.o \
./proj/common/mempool.o \
./proj/common/mmem.o \
./proj/common/printf.o \
./proj/common/qsort.o \
./proj/common/selection_sort.o \
./proj/common/string.o \
./proj/common/utility.o 


# Each subdirectory must supply rules for building sources it contributes
proj/common/%.o: ../proj/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu" -I"C:\Users\john\Desktop\telink_ble_sdk_release_v3.1.0_20170509\telink_ble_sdk_release_v3.1.0_20170509\ble_lt_sdk\proj\mcu_spec" -D__PROJECT_8266_BLE_REMOTE__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


