# AS := nasm -felf64

LD := ld
LDFLAGES :=
CC := gcc
CFLAGES := -Wall -Wstrict-prototypes -O2 -c -fno-pic -fno-stack-protector -nostdinc -nostdlib -fno-builtin -Wmain
OBJ := main.o

ISO := iso

export BUILD_DIR := $(abspath build)

all: $(ISO)

$(OBJ): %.o: %.c
	@mkdir -p $(BUILD_DIR)
	@$(CC) -c $(CFLAGS) $< -o $(BUILD_DIR)/$@

kernel.bin: $(OBJ)
	@$(MAKE) -C boot
	@cd $(BUILD_DIR); $(LD) -o $@ boot.o $(OBJ)


$(ISO): kernel.bin grub.cfg
	@mkdir -p isofile/boot/grub
	@cp $(BUILD_DIR)/kernel.bin isofile/boot/kernel.bin
	@cp grub.cfg isofile/boot/grub
	@grub-mkrescue -o iso isofile 2> /dev/null
	@rm -rf isofile

run: $(ISO)
	@qemu-system-x86_64 -cdrom iso

	
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(ISO)

