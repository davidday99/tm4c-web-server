PROJECT = main
SRCS = $(wildcard src/*.c)
OBJ = obj/
OBJS = $(addprefix $(OBJ), $(notdir $(SRCS:.c=.o)))
INC = inc/
LD_SCRIPT = TM4C123GH6PM.ld

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld 
OBJCOPY = arm-none-eabi-objcopy
RM      = rm -rf
MKDIR   = @mkdir -p $(@D)

OPT += -O0

#GCC FLAGS
CFLAGS = -ggdb3 -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 
CFLAGS += -mfloat-abi=softfp -MD -std=c99 -c -Wextra -Wall -Wno-missing-braces
CFLAGS += $(OPT)

#LINKER FLAGS
LDFLAGS = -T $(LD_SCRIPT) -e Reset_Handler 

all: bin/$(PROJECT).bin

$(OBJ)%.o: src/%.c          
	$(MKDIR)              
	$(CC) -o $@ $^ -I$(INC) $(CFLAGS)

bin/$(PROJECT).elf: $(OBJS)      #make contains debug symbols for GNU GDB
	$(MKDIR)           
	$(CC) -o $@ $^ $(LDFLAGS) -nostdlib -lc

bin/$(PROJECT).bin: bin/$(PROJECT).elf    #debug symbols for GNU GDB stripped by objcopy,finished binary ready for flashing
	$(OBJCOPY) -O binary $< $@


clean:
	-$(RM) obj
	-$(RM) bin