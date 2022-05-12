
INCLUDE := $(abspath include)

export LD := ld
export LDFLAGES :=
export CC := gcc
export CPPFLAGS := -I$(INCLUDE)
export CFLAGES := -Wall -Wstrict-prototypes -O1 -fno-pic -nostdinc -nostdlib -fno-builtin -fno-stack-protector -c

OBJS := main.o

ISO := iso

export BUILD_DIR := $(abspath build)

all: $(ISO)

$(OBJS): %.o: %.c
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGES) $< -o $(BUILD_DIR)/$@

kernel.bin: $(OBJS)
	@$(MAKE) -C boot
	@$(MAKE) -C arch
	@$(MAKE) -C dev
	@$(MAKE) -C kernel
	@$(MAKE) -C multiboot
	@$(MAKE) -C mm
	@cd $(BUILD_DIR); $(LD) $(LDFLAGES) -o $@ *.o


$(ISO): kernel.bin grub.cfg
	@mkdir -p isofile/boot/grub
	@cp $(BUILD_DIR)/kernel.bin isofile/boot/kernel.bin
	@cp grub.cfg isofile/boot/grub
	@grub-mkrescue -o iso isofile 2> /dev/null
	@rm -rf isofile

run: $(ISO)
	@qemu-system-x86_64 -cdrom iso -m 8192 -serial stdio -device isa-debug-exit,iobase=0xf4,iosize=0x04 -M hpet=on 
test: $(ISO)
	@qemu-system-x86_64 -cdrom iso -m 4096 -serial stdio -device isa-debug-exit,iobase=0xf4,iosize=0x04 -display none

	
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(ISO)

