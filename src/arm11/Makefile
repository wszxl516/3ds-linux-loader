ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/base_tools 
TARGET	:=	$(shell basename $(CURDIR))
SOURCES	= ./
INCLUDES = ./

SFILES	= $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.S))
CFILES	= $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.c))

OBJS	= $(SFILES:.S=.o) $(CFILES:.c=.o)
DEPS	= $(OBJS:.o=.d)
INCLUDE	= $(foreach dir, $(INCLUDES), -I$(CURDIR)/$(dir))

ARCH	= -mcpu=mpcore -march=armv6k -mlittle-endian -mthumb-interwork
ASFLAGS	= $(ARCH) $(COMMON_ARCH) $(INCLUDE)
CFLAGS	= -Wall -Os -fno-builtin $(ARCH) $(INCLUDE) -fno-stack-protector -D__LOGO__


.PHONY: all clean copy

all: $(TARGET).elf

$(TARGET).elf: $(OBJS)
	@echo [LD] $(TARGET).elf
	@$(CC) -T link.ld -Wl,--nmagic $^ -o $@ -nostartfiles -Wl,-Map,$(TARGET).map
%.o: %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
%.o: %.S
	@echo [CC] $<
	@$(CC) $(ASFLAGS) -MMD -MP -c $< -o $@ 

clean:
	@echo clean $(TARGET)...
	@rm -f $(OBJS) $(TARGET).elf $(DEPS) *.map

-include $(DEPS)
