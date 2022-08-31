ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/base_tools 
TARGET	:=	$(shell basename $(CURDIR))
SOURCES		:=	. common fatfs disk
INCLUDES	:=	. common font fatfs disk

ARCH	:=	-mthumb -mthumb-interwork 

CFLAGS	:=	-g -Wall -Wextra -Wpedantic -Wcast-align -Wno-main -O2\
			-march=armv5te -mtune=arm946e-s -fomit-frame-pointer -ffast-math -std=gnu11\
			-fno-builtin-memcpy $(ARCH) -fdata-sections -ffunction-sections

CFLAGS	+=	$(INCLUDE) -DARM9

CFLAGS	+=	-DBUILD_NAME="\"$(TARGET) (`date +'%Y/%m/%d'`)\""

ifeq ($(FONT),ORIG)
CFLAGS	+=	-DFONT_ORIGINAL
else ifeq ($(FONT),6X10)
CFLAGS	+=	-DFONT_6X10
else ifeq ($(FONT),ACORN)
CFLAGS	+=	-DFONT_ACORN
else ifeq ($(FONT),GB)
CFLAGS	+=	-DFONT_GB
else
FONT = 6x10
CFLAGS	+=	-DFONT_6X10
endif

ifeq ($(OPEN),1)
	CFLAGS += -DOPEN_INSTALLER
endif

ASFLAGS	:=	-g -mcpu=arm946e-s $(ARCH)
LDFLAGS	=	-Tlink.ld -nostartfiles -g $(ARCH) -Wl,-Map,$(TARGET).map

SFILES	= $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.s))
CFILES	= $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.c))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

LD	:=	$(CC)

OFILES	:= $(addsuffix .o,$(BINFILES)) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) $(foreach dir,$(LIBDIRS),-I$(dir)/include) 
LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)
OBJS	= $(SFILES:.s=.o) $(CFILES:.c=.o)
DEPS	= $(OBJS:.o=.d)
.PHONY: clean all firm 
#---------------------------------------------------------------------------------
all: $(TARGET).elf

$(TARGET).elf: $(OBJS)
	@echo [LD] $(TARGET).elf
	@$(CC) $(LDFLAGS) $^ -o $@ -nostartfiles
%.o: %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@
%.o: %.s
	@echo [CC] $<
	@$(CC) $(ASFLAGS) -MMD -MP -c $< -o $@

firm: $(TARGET).elf
	@firmtool build $(TARGET).firm -D $(TARGET).elf -C NDMA -i


#---------------------------------------------------------------------------------
clean:
	@echo clean $(TARGET)...
	@find ./ -name "*.o" -exec rm "{}" \;
	@find ./ -name "*.d" -exec rm "{}" \;
	@rm -f $(TARGET).elf
	@rm -f $(TARGET).bin
	@rm -f $(TARGET).firm
	@rm -f $(TARGET).map
