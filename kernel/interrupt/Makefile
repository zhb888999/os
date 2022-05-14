AS := as

BUILD_DIR ?= ../../build
OBJS_AS := entry.o
OBJS_CC := exceptions.o irq.o

all: $(OBJS_AS) $(OBJS_CC)

$(OBJS_AS): %.o: %.s
	@mkdir -p $(BUILD_DIR)
	@$(AS) $< -o $(BUILD_DIR)/$@

$(OBJS_CC): %.o: %.c
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGES) -c $< -o $(BUILD_DIR)/$@

clean:
	@cd $(BUILD_DIR); rm -rf $(OBJS)