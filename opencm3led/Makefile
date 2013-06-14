# Using the CodeSourcery Lite ARM EABI tools
COMPILER_PREFIX = arm-none-eabi-
CC   = $(COMPILER_PREFIX)gcc
LD   = $(COMPILER_PREFIX)ld
CP   = $(COMPILER_PREFIX)objcopy
DMP  = $(COMPILER_PREFIX)objdump
SZ   = $(COMPILER_PREFIX)size
HEX  = $(CP) -O ihex 
BIN  = $(CP) -O binary
MCU  = cortex-m3

# 
# project name - base part of the output file name
BINARY        = main

# Paths
# Base path to libopencm3
OPENCM3_PATH       = ../libopencm3
# include/ sub-directory
OPENCM3_INC_PATH   = $(OPENCM3_PATH)/include
# lib/ sub-directory
OPENCM3_LIB_PATH   = $(OPENCM3_PATH)/lib
# Linker path
LINKER_SCRIPT_PATH = ./linker
# Directory to store generated object files
OBJDIR = ./obj


# Sources
SRC  = main.c
#SRC += system_stm32f10x.c
#SRC += crt.c 
#SRC += vectors_stm32f10x_hd.c


# Includes to generate with -I flag
INCLUDES_LIST = $(OPENCM3_INC_PATH)

# Defines used
DEFINES = -DSTM32F1

# linker script file
LDSCRIPT = $(LINKER_SCRIPT_PATH)/stm32-f103ve.ld

INCLUDES  = $(patsubst %,-I%,$(INCLUDES_LIST))

FNAMES = $(foreach file, $(SRC), $(notdir $(file)))
OBJECTS = $(foreach file, $(FNAMES:.c=.o), $(OBJDIR)/$(file))

CFLAGS =  -mcpu=$(MCU) -fno-common -mthumb -msoft-float -g -Wall -Wextra -Wimplicit-function-declaration -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes -Wundef -Wshadow $(DEFINES)

LDFLAGS = -lopencm3_stm32f1 --static -Wl,--start-group -lc -lgcc -Wl,--end-group -L$(OPENCM3_LIB_PATH) -T$(LDSCRIPT) -nostartfiles -Wl,--gc-sections -mthumb -mcpu=$(MCU) -msoft-float -mfix-$(MCU)-ldrd -Wl,--print-gc-sections

# Generate dependency information
CFLAGS += -MD -MP -MF .dep/$(@F).d

# Macro to create a target: object file in the $(OBJDIR) directory
define make-deps
$(OBJDIR)/$(notdir $(1:%.c=%.o)): $1
endef

# First target - .elf, .hex and .bin files
all: $(BINARY).elf $(BINARY).hex $(BINARY).bin
	@echo Size of the sections:
	@$(SZ) $(BINARY).elf

# generate list of target - a target per file
$(foreach d, $(SRC), $(eval $(call make-deps,$d)))

$(OBJDIR):
	@mkdir -p $(OBJDIR)

# ensure we have $(OBJDIR) before building objects
$(OBJECTS): | $(OBJDIR)


%.o : # Dependencies declared above
	@echo Compiling $< into $@
	$(CC) -c $(CFLAGS) -I . $(INCLUDES) $< -o $@


%elf: $(OBJECTS)
	@echo Linking $@
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

%hex: %elf
	@echo Creating $@
	@$(HEX) $< $@

%bin: %elf
	@echo Creating $@
	@$(BIN) $< $@


clean:
	-rm -f $(OBJECTS)
	-rm -fr $(OBJDIR)
	-rm -f $(BINARY).elf
	-rm -f $(BINARY).map
	-rm -f $(BINARY).hex
	-rm -f $(BINARY).bin
	-rm -fR .dep

# flash the binary to the device using serial port
flash:
	stm32flash -w $(BINARY).hex -v -g 0x0 /dev/tty.PL2303-000013FA

# 
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
