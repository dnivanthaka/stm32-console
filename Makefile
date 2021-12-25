# Makefile for blink demo

#TOOLS = arm-linux-gnu
IDIR = include
ODIR = obj
TOOLS = arm-none-eabi

# Assembling with gcc makes it want crt0 at link time.
#AS = $(TOOLS)-gcc
AS = $(TOOLS)-as
# Use the -g flag if you intend to use gdb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb, added -Os flag to optimize code generation
CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb -g -Os -I$(IDIR)

#LD = $(TOOLS)-gcc
LD = $(TOOLS)-ld.bfd
OBJCOPY = $(TOOLS)-objcopy
DUMP = $(TOOLS)-objdump -d
GDB = $(TOOLS)-gdb

OBJS = start.o startup.o gpio.o adc.o rcc.o interrupts.o usart.o system.o spi.o i2c.o pcf8574.o st7565r.o keypad.o sound.o eventq.o test.o

all: main.bin main.dump

main.dump:	main.elf
	$(DUMP) main.elf >main.dump

main.bin:	main.elf
	$(OBJCOPY) main.elf main.bin -O binary

main.elf: 	$(OBJS)
	$(LD) -T main.lds -o main.elf $(OBJS)

start.o:	start.s
	$(AS) start.s -o start.o

startup.o:	startup.c
	$(CC) -c startup.c

system.o:	system.c 
	$(CC) -c system.c 

test.o:	test.c 
	$(CC) -c test.c 

rcc.o:	rcc.c
	$(CC) -c rcc.c

interrupts.o:	interrupts.c
	$(CC) -c interrupts.c

spi.o:	spi.c
	$(CC) -c spi.c

usart.o: usart.c
	$(CC) -c usart.c

gpio.o:	gpio.c
	$(CC) -c gpio.c

adc.o:	adc.c
	$(CC) -c adc.c

i2c.o:	i2c.c
	$(CC) -c i2c.c

pcf8574.o:	pcf8574.c
	$(CC) -c pcf8574.c

st7565r.o:	st7565r.c
	$(CC) -c st7565r.c

keypad.o:	keypad.c
	$(CC) -c keypad.c

sound.o:	sound.c
	$(CC) -c sound.c

eventq.o:	eventq.c
	$(CC) -c eventq.c

clean:
	rm -f *.o main.elf main.bin main.dump
