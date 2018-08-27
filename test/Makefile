#===============================================================
#Makefile for building MSP Code Examples in command line
#environement using the GCC Open Source Compiler for MSP430
#===============================================================

# refer to this for compiler options:
# 	http://www.ti.com/lit/ug/slau646c/slau646c.pdf

# generic devices: msp430 or msp430x
# MSP430afe253 -- msp430, 16KB flash, 512B RAM
# MSP430FR5994 -- msp430, 256KB flash, 512000B RAM
DEVICE:=MSP430FR5994
EXAMPLE:=main
FOLDER:= crc
# Require DEVICE to be specified
ifndef DEVICE
$(info Please specify a device, e.g. DEVICE=MSP430F5529)
$(error unspecified device)
endif
# Require EXAMPLE to be specified
ifndef EXAMPLE
$(info Please specify an example, e.g. EXAMPLE=MSP430F55xx_adc_01)
$(error unspecified example)
endif

###################### Windows OS ######################
ifeq ($(OS),Windows_NT)
	################## GCC Root Variable ###################
	GCC_DIR     ?= '$(HOMEDRIVE)$(HOMEPATH)/ti/gcc'
	GCC_MSP_INC_DIR ?= $(GCC_DIR)/include
	LDDIR       := $(GCC_MSP_INC_DIR)/$(DEVICE)
	RM          := rd /s /q
################### Linux or Mac OS ####################
else
	################## GCC Root Variable ###################
	GCC_DIR     ?= /home/d/Downloads/msp430-gcc-6.4.0.32_linux64
	GCC_MSP_INC_DIR ?= $(GCC_DIR)/include
	LDDIR       := $(GCC_MSP_INC_DIR)/$(shell echo $(DEVICE) | tr A-Z a-z)
	RM          := rm -rf
endif

######################################
GCC_BIN_DIR     ?= $(GCC_DIR)/bin
GCC_INC_DIR     ?= $(GCC_DIR)/msp430-elf/include
######################################
CC              := $(GCC_BIN_DIR)/msp430-elf-gcc
OBJDUMP         := $(GCC_BIN_DIR)/msp430-elf-objdump
AS		        := $(GCC_BIN_DIR)/msp430-elf-as
GDB			    := $(GCC_BIN_DIR)/msp430-elf-gdb
######################################
#CFLAGS          := -Os -D__$(DEVICE)__ -mmcu=$(DEVICE) -g -ffunction-sections -fdata-sections -DDEPRECATED
#LDFLAGS         := -T $(LDDIR).ld -L $(GCC_MSP_INC_DIR) -mmcu=$(DEVICE) -g -Wl,--gc-sections 
CFLAGS          := -O0 -D__$(DEVICE)__ -mmcu=$(DEVICE) -ffunction-sections -fdata-sections -DDEPRECATED -nostartfiles -ffreestanding -std=c99 -fomit-frame-pointer -fno-optimize-sibling-calls 
LDFLAGS         := -T $(LDDIR).ld -L $(GCC_MSP_INC_DIR) -mmcu=$(DEVICE) -Wl,--gc-sections 
INCLUDES        := -I $(GCC_MSP_INC_DIR) -I $(GCC_INC_DIR)
######################################
SRC             := $(FOLDER)/$(EXAMPLE).c
EXOBJECT        := $(EXAMPLE)/$(EXAMPLE).o
EXOUTPUT        := $(EXAMPLE)/$(EXAMPLE).out

SRC2             := $(FOLDER)/crc.c
EXOBJECT2        := $(EXAMPLE)/crc.o
EXOUTPUT2        := $(EXAMPLE)/crc.out

SRC3             := $(FOLDER)/isqrt.c
EXOBJECT3        := $(EXAMPLE)/isqrt.o
EXOUTPUT3        := $(EXAMPLE)/isqrt.out

SRC4             := $(FOLDER)/rad2deg.c
EXOBJECT4        := $(EXAMPLE)/rad2deg.o
EXOUTPUT4        := $(EXAMPLE)/rad2deg.out


######################################

all: $(EXOUTPUT)

$(EXAMPLE):
	@mkdir $(EXAMPLE)

$(EXOBJECT): ${SRC}
	@echo ============================================
	@echo Compiling $(EXAMPLE).c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@
	$(CC) $(INCLUDES) $(CFLAGS) -S $< -o $@.asm

$(EXOBJECT2): ${SRC2}
	@echo ============================================
	@echo Compiling crc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@
	$(CC) $(INCLUDES) $(CFLAGS) -S $< -o $@.asm

#$(EXOBJECT3): ${SRC3}
	#@echo ============================================
	#@echo Compiling isqrt.c
	#$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@
	#$(CC) $(INCLUDES) $(CFLAGS) -S $< -o $@.asm

#$(EXOBJECT4): ${SRC4}
	#@echo ============================================
	#@echo Compiling rad2deg.c
	#$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@
	#$(CC) $(INCLUDES) $(CFLAGS) -S $< -o $@.asm

$(EXOUTPUT): $(EXAMPLE) $(EXOBJECT) $(EXOBJECT2) 
	@echo ============================================
	@echo Linking objects and generating output binary
	$(CC) $(LDFLAGS) $(EXOBJECT) $(EXOBJECT2) -o $@ -lm
	$(OBJDUMP) --source -D -g -S -x -z -a $@ > $@.objdump
	hexdump -e '16/1 "%02x " "\n"' -v $@ > $@.hexdump

debug: all
	$(GDB) $(EXAMPLE)/$(EXAMPLE).out

clean:
	@$(RM) $(EXAMPLE)
