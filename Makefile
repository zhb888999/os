
INCLUDE := $(abspath include)

export LD := ld
export LDFLAGES :=
export CC := gcc
export CPPFLAGS := -I$(INCLUDE)
export CFLAGES := -Wall -Wstrict-prototypes -O1 -fno-pic -nostdinc -nostdlib -fno-builtin -fno-stack-protector -c

OBJS := main.o

ISO := iso

DISK := block.qcow2

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

$(DISK):
	@qemu-img create -f qcow2 $@ 500M

$(ISO): kernel.bin grub.cfg
	@mkdir -p isofile/boot/grub
	@cp $(BUILD_DIR)/kernel.bin isofile/boot/kernel.bin
	@cp grub.cfg isofile/boot/grub
	@grub-mkrescue -o $@ isofile 2> /dev/null
	@rm -rf isofile

QEMU_FLAGES := -enable-kvm -cdrom $(ISO) -m 8G \
			-serial stdio -device isa-debug-exit,iobase=0xf4,iosize=0x04 \
			-machine hpet=on -smp cpus=4 \
			-hda $(DISK)

run: $(ISO) $(DISK)
	@qemu-system-x86_64 $(QEMU_FLAGES)
test: $(ISO) $(DISK)
	@qemu-system-x86_64 $(QEMU_FLAGES) -display none

	
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(ISO)
	@rm -rf $(DISK)

