################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/db.c \
../APP/io_app.c \
../APP/modbus_app.c \
../APP/net_app.c \
../APP/rtc_app.c \
../APP/usbdisk_app.c 

OBJS += \
./APP/db.o \
./APP/io_app.o \
./APP/modbus_app.o \
./APP/net_app.o \
./APP/rtc_app.o \
./APP/usbdisk_app.o 

C_DEPS += \
./APP/db.d \
./APP/io_app.d \
./APP/modbus_app.d \
./APP/net_app.d \
./APP/rtc_app.d \
./APP/usbdisk_app.d 


# Each subdirectory must supply rules for building sources it contributes
APP/%.o: ../APP/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wuninitialized  -g -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\fal\inc" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\FlashDB\inc" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\fal\samples\porting" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\Debug" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\agile_modbus-1.1.3\util" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\agile_modbus-1.1.3\inc" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\Core" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\User" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\SRC\Peripheral\inc" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\drivers" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\include" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\include\libc" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\libcpu\risc-v" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\libcpu\risc-v\common" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\src" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\components\drivers\include" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\components\drivers\misc" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\components\drivers\serial" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\components\drivers\spi" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\components\drivers\spi\sfud" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\components\drivers\spi\sfud\inc" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\components\drivers\spi\sfud\src" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\rtthread\components\finsh" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\NetLib" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\mqtt_src" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\Udisk_Lib" -I"H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\APP" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

