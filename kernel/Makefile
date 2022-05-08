BUILD_DIR ?= ../build
OBJS := format.o init.o

all: $(OBJS)
	@$(MAKE) -C interrupt

$(OBJS): %.o: %.c
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGES) -c $< -o $(BUILD_DIR)/$@

clean:
	@cd $(BUILD_DIR); rm -rf $(OBJS)