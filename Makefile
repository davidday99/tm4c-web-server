PROJECT = main
SRCS = $(wildcard src/*.c)
OBJ = obj/
OBJS = $(addprefix $(OBJ), $(notdir $(SRCS:.c=.o)))
INC = inc/
LD_SCRIPT = TM4C123GH6PM.ld

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld 
OBJCOPY = arm-none-eabi-objcopy
RM = rm -rf
MKDIR = @mkdir -p $(@D)

CFLAGS = -ggdb3 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 
CFLAGS += -mfloat-abi=softfp -MD -std=c99 -Wextra -Wall -Wno-missing-braces

all: bin/$(PROJECT).elf


$(OBJ)%.o: src/%.c          
	$(MKDIR)              
	$(CC) -o $@ $^ -c -I$(INC) $(CFLAGS)
	
bin/$(PROJECT).elf: $(OBJS) 
	$(MKDIR)           
	$(CC) -o $@ $^ $(CFLAGS) -Wl,-T $(LD_SCRIPT) -Wl,-e Reset_Handler
	$(OBJCOPY) -O binary $@ bin/$(PROJECT).bin 

clean:
	-$(RM) obj
	-$(RM) bin