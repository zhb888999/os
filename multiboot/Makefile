BUILD_DIR ?= ../../../build
OBJS := multiboot2.o

$(OBJS): %.o: %.c
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGES) -c $< -o $(BUILD_DIR)/$@

clean:
	@cd $(BUILD_DIR); rm -rf $(OBJS)