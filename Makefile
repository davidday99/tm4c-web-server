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

OPT += -O0

#GCC FLAGS
CFLAGS = -ggdb3 -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 
CFLAGS += -mfloat-abi=softfp -MD -std=c99 -c -Wextra -Wall -Wno-missing-braces
CFLAGS += $(OPT)

#LINKER FLAGS
LDFLAGS = -T $(LD_SCRIPT) -e Reset_Handler 

case1: bin/case1.elf
case2: bin/case2.elf
case3: bin/case3.elf

$(OBJ)%.o: src/%.c          
	$(MKDIR)              
	$(CC) -o $@ $^ -I$(INC) $(CFLAGS)

# ********** CASE 1 **********
# undefined memcpy error if you uncomment lines 12-14 in main
bin/case1.elf: $(OBJS)
	$(MKDIR)           
	$(LD) -o $@ $^ $(LDFLAGS)

# ********** CASE 2 **********
# no more error for lines 12-14 in main
bin/case2.elf: $(OBJS)
	$(MKDIR)           
	$(LD) -o $@ $^ /usr/arm-none-eabi/lib/libc.a $(LDFLAGS)

# ********** CASE 3 **********
# no more errors for lines 12-14 in main.
# use -nostdlib because we're not using crt0
# use -lc because to reinclude std c functions, namely memcpy
bin/case3.elf: $(OBJS) 
	$(MKDIR)           
	$(CC) -o $@ $^ $(LDFLAGS) -nostdlib -lc

clean:
	-$(RM) obj
	-$(RM) bin