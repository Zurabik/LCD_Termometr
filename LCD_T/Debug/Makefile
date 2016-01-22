################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../bcd.c \
../n3310.c \
../twim.c \
../UART.c \
../ExternLoad.c \
../lcd_lib_2.c \
../LCD_T.c \
../Message.c \
../nec.c \
../OWIcrc.c \
../OWIHighLevelFunctions.c \
../OWISWBitFunctions.c \
../OWIUARTBitFunctions.c \
../radio.c \
../shift_register.c \
../spi.c \
../Temper.c \
../Timer.c \
../TX_RX_command.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
bcd.o \
n3310.o \
twim.o \
UART.o \
ExternLoad.o \
lcd_lib_2.o \
LCD_T.o \
Message.o \
nec.o \
OWIcrc.o \
OWIHighLevelFunctions.o \
OWISWBitFunctions.o \
OWIUARTBitFunctions.o \
radio.o \
shift_register.o \
spi.o \
Temper.o \
Timer.o \
TX_RX_command.o

OBJS_AS_ARGS +=  \
bcd.o \
n3310.o \
twim.o \
UART.o \
ExternLoad.o \
lcd_lib_2.o \
LCD_T.o \
Message.o \
nec.o \
OWIcrc.o \
OWIHighLevelFunctions.o \
OWISWBitFunctions.o \
OWIUARTBitFunctions.o \
radio.o \
shift_register.o \
spi.o \
Temper.o \
Timer.o \
TX_RX_command.o

C_DEPS +=  \
bcd.d \
n3310.d \
twim.d \
UART.d \
ExternLoad.d \
lcd_lib_2.d \
LCD_T.d \
Message.d \
nec.d \
OWIcrc.d \
OWIHighLevelFunctions.d \
OWISWBitFunctions.d \
OWIUARTBitFunctions.d \
radio.d \
shift_register.d \
spi.d \
Temper.d \
Timer.d \
TX_RX_command.d

C_DEPS_AS_ARGS +=  \
bcd.d \
n3310.d \
twim.d \
UART.d \
ExternLoad.d \
lcd_lib_2.d \
LCD_T.d \
Message.d \
nec.d \
OWIcrc.d \
OWIHighLevelFunctions.d \
OWISWBitFunctions.d \
OWIUARTBitFunctions.d \
radio.d \
shift_register.d \
spi.d \
Temper.d \
Timer.d \
TX_RX_command.d

OUTPUT_FILE_PATH +=LCD_Termometr.elf

OUTPUT_FILE_PATH_AS_ARGS +=LCD_Termometr.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler







































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="LCD_Termometr.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega328p"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "LCD_Termometr.elf" "LCD_Termometr.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "LCD_Termometr.elf" "LCD_Termometr.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "LCD_Termometr.elf" > "LCD_Termometr.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "LCD_Termometr.elf" "LCD_Termometr.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "LCD_Termometr.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "LCD_Termometr.elf" "LCD_Termometr.a" "LCD_Termometr.hex" "LCD_Termometr.lss" "LCD_Termometr.eep" "LCD_Termometr.map" "LCD_Termometr.srec" "LCD_Termometr.usersignatures"
	