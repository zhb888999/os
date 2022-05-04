AS := as

BUILD_DIR ?= ../build
OBJS := boot.o

$(OBJS): %.o: %.s
	@mkdir -p $(BUILD_DIR)
	@$(AS) $< -o $(BUILD_DIR)/$@

clean:
	@cd $(BUILD_DIR); rm -rf $(OBJS)