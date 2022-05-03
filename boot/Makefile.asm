AS := nasm -felf64

BUILD_DIR ?= ../build
OBJS := boot.o

$(OBJS): %.o: %.asm
	@mkdir -p $(BUILD_DIR)
	$(AS) $< -o $(BUILD_DIR)/$@

clean:
	@cd $(BUILD_DIR); rm -rf $(OBJS)