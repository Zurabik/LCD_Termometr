###############################################################################
# Makefile for the project LCD_T
###############################################################################

## General Flags
PROJECT = LCD_T
MCU = atmega328p
TARGET = LCD_T.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99   -DF_CPU=18432000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=LCD_T.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = LCD_T.o bcd.o lcd_lib_2.o OWIcrc.o OWIHighLevelFunctions.o OWISWBitFunctions.o OWIUARTBitFunctions.o radio.o spi.o Temper.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) LCD_T.hex LCD_T.eep LCD_T.lss size

## Compile
LCD_T.o: ../LCD_T.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bcd.o: ../bcd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_lib_2.o: ../lcd_lib_2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

OWIcrc.o: ../OWIcrc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

OWIHighLevelFunctions.o: ../OWIHighLevelFunctions.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

OWISWBitFunctions.o: ../OWISWBitFunctions.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

OWIUARTBitFunctions.o: ../OWIUARTBitFunctions.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

radio.o: ../radio.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi.o: ../spi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Temper.o: ../Temper.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) LCD_T.elf dep/* LCD_T.hex LCD_T.eep LCD_T.lss LCD_T.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

