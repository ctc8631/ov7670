EXECUTABLE=main.elf
BIN_IMAGE=main.bin

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-g -O0 -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-ffreestanding -nostdlib

# to run from FLASH
STMLIB = ./inc

CFLAGS+=-Wl,-T,$(STMLIB)/stm32_flash.ld

CFLAGS+=-I./

# stm32f4_discovery lib
CFLAGS+=-I$(STMLIB)/STM32F4xx_StdPeriph_Driver/inc
CFLAGS+=-I$(STMLIB)/STM32F4xx
CFLAGS+=-I$(STMLIB)/CMSIS
CFLAGS+=-I$(STMLIB)

all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@
	arm-none-eabi-objdump -S main.elf > main.list

$(EXECUTABLE): \
	src/main.c \
	src/OV7670.c \
	src/stm32f4xx_it.c \
	src/OV7670config.c \
	inc/system_stm32f4xx.c \
	inc/startup_stm32f4xx.S \
	inc/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
	inc/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
	inc/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dcmi.c \
	inc/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c \
	inc/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fsmc.c \
	inc/STM32F4xx_StdPeriph_Driver/src/misc.c
	$(CC) $(CFLAGS) $^ -o $@  

run : main.bin gdbscript
	./run.sh $<

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
	rm -rf main.list

.PHONY: all clean

flash: $(BIN_IMAGE)
	st-flash write $< 0x8000000

openocd:
	openocd -f ~/embedded/P-ctc8631/Lab-6/stm32f4discovery.cfg
	
gdbauto: $(BIN_IMAGE)
	arm-none-eabi-gdb -x gdbauto.sh

