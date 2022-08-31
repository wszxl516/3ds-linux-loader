MAKE = make --no-print-directory
TARGET	:=	$(shell basename $(CURDIR))
ARM9_DIR = src/arm9
ARM11_DIR = src/arm11
ARM9_ELF = $(ARM9_DIR)/arm9.elf
ARM11_ELF = $(ARM11_DIR)/arm11.elf


FONT=6X10
.PHONY: clean all firm arm9 arm11

all: $(TARGET).firm

arm9:
	@echo Build $@
	@echo font $(FONT)
	@$(MAKE) -C $(ARM9_DIR) FONT=$(FONT)
arm11:
	@echo Build $@
	@$(MAKE) -C $(ARM11_DIR)

$(TARGET).firm: arm9 arm11
	@echo Build $@
	@firmtool build $@ -D $(ARM9_ELF) $(ARM11_ELF) -C NDMA XDMA -i
	
clean:
	@echo clean $(TARGET)...
	@$(foreach dir,$(ARM9_DIR) $(ARM11_DIR),\
		$(MAKE) -C $(dir) clean;)
	@rm -f $(TARGET).firm