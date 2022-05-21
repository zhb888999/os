BUILD_DIR ?= ../build
OBJS := local_apic.o io_apic.o

$(OBJS): %.o: %.c
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGES) -c $< -o $(BUILD_DIR)/$@

clean:
	@cd $(BUILD_DIR); rm -rf $(OBJS)