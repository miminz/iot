################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include rtthread/src/subdir.mk
-include rtthread/libcpu/risc-v/common/subdir.mk
-include rtthread/components/finsh/subdir.mk
-include rtthread/components/drivers/spi/sfud/src/subdir.mk
-include rtthread/components/drivers/spi/subdir.mk
-include rtthread/components/drivers/serial/subdir.mk
-include rtthread/components/drivers/misc/subdir.mk
-include rtthread/subdir.mk
-include mqtt_src/subdir.mk
-include fal/src/subdir.mk
-include fal/samples/porting/subdir.mk
-include drivers/subdir.mk
-include agile_modbus-1.1.3/util/subdir.mk
-include agile_modbus-1.1.3/src/subdir.mk
-include User/subdir.mk
-include Udisk_Lib/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include NetLib/subdir.mk
-include FlashDB/src/subdir.mk
-include FlashDB/samples/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include APP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
rt-thread_usb_net_driver.hexAndbin \

SECONDARY_LIST += \
rt-thread_usb_net_driver.lst \

SECONDARY_SIZE += \
rt-thread_usb_net_driver.siz \


# 所有目标
all: rt-thread_usb_net_driver.elf secondary-outputs

# 工具调用
rt-thread_usb_net_driver.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wuninitialized  -g -T "H:\Users\sc\Desktop\rtt_net_usb_drivers\rtt_net_usb_drivers\RT-Thread\rt-thread_usb_net_driver\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"rt-thread_usb_net_driver.map" --specs=nano.specs --specs=nosys.specs -o "rt-thread_usb_net_driver.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
rt-thread_usb_net_driver.hexAndbin: rt-thread_usb_net_driver.elf
	@	riscv-none-embed-objcopy -O ihex "rt-thread_usb_net_driver.elf"  "rt-thread_usb_net_driver.hex" && riscv-none-embed-objcopy -O binary "rt-thread_usb_net_driver.elf"  "rt-thread_usb_net_driver.bin"
	@	@
rt-thread_usb_net_driver.lst: rt-thread_usb_net_driver.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble "rt-thread_usb_net_driver.elf" > "rt-thread_usb_net_driver.lst"
	@	@
rt-thread_usb_net_driver.siz: rt-thread_usb_net_driver.elf
	@	riscv-none-embed-size --format=berkeley "rt-thread_usb_net_driver.elf"
	@	@
	$(MAKE) --no-print-directory post-build

# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) rt-thread_usb_net_driver.elf
	-@
post-build:
	-riscv-none-embed-objcopy -O ihex "rt-thread_usb_net_driver.elf" "rt-thread_usb_net_driver.hex" && riscv-none-embed-objcopy -O binary "rt-thread_usb_net_driver.elf" "rt-thread_usb_net_driver.bin"
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents post-build

-include ../makefile.targets
