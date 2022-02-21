PROJECT = main
DEV = /dev/ttyACM0
FLASHER = lm4flash
SRCS = $(wildcard src/*.c)
OBJ = obj/
OBJS = $(addprefix $(OBJ), $(notdir $(SRCS:.c=.o)))
INC = -Iinc/ -Ilib/network-stack/inc/ -Ilib/hal/inc/ 
LD_SCRIPT = TM4C123GH6PM.ld

NETDEPS = $(wildcard lib/network-stack/eobj/*.o)
HALDEPS = $(wildcard lib/hal/lib/*.o)

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld 
DEBUGGER = arm-none-eabi-gdb
OBJCOPY = arm-none-eabi-objcopy
RM = rm -rf
MKDIR = @mkdir -p $(@D)

OPT_LEVEL = -Os
CFLAGS = -ggdb3 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -nostartfiles -nostdlib
CFLAGS += -mfloat-abi=softfp -MD -std=c99 -Wextra -Wall -Wno-missing-braces -Wno-builtin-declaration-mismatch
CFLAGS += $(OPT_LEVEL)

$(info $(OBJS))

dependencies:
	$(MAKE) embedded -C lib/network-stack
	$(MAKE) library -C lib/hal

build: bin/$(PROJECT).elf

$(OBJ)%.o: src/%.c          
	$(MKDIR)              
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)

bin/$(PROJECT).elf: $(OBJS) $(HALDEPS) $(NETDEPS)
	$(MKDIR)
	$(CC) -o $@ $^ $(CFLAGS) -Wl,-T $(LD_SCRIPT) -Wl,-eReset_Handler
	$(OBJCOPY) -O binary $@ bin/$(PROJECT).bin 

flash:
	$(FLASHER) -S $(DEV) bin/$(PROJECT).bin

debug:
	$(DEBUGGER) --tui bin/$(PROJECT).elf -ex "target remote :3333"

clean:
	-$(RM) obj
	-$(RM) bin
	-$(MAKE) clean -C lib/network-stack
	-$(MAKE) clean -C lib/hal