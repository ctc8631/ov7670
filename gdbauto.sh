file main.elf
target remote :3333
monitor poll
monitor reset halt
monitor flash probe 0
monitor flash write_image erase main.bin 0x08000000
monitor reset 
monitor exit
# x/1bt &RAM_Buffer[320*240-1]
# x/100bt RAM_Buffer
# dump memory pic1.raw RAM_Buffer &RAM_Buffer[320*120*2-1]
# b DMA2_Stream7_IRQHandler
# c
# b DCMI_IRQHandler
# b 77
# b 102
# c

