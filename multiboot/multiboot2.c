#include <int.h>
#include <multiboot/multiboot2.h>
#include <arch/x86_64.h>
#include <kernel/init.h>

#include <dev/serial.h>

int parse_multiboot2(uint32_t addr, MemorySegmentPage *free, MemorySegment *kernel) {
    int segment_count = 0;
    struct multiboot_tag *tag = (struct multiboot_tag *) (uint64_t) (addr + 8);
    for (; tag->type != MULTIBOOT_TAG_TYPE_END; 
         tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))) {
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_MMAP: {
                multiboot_memory_map_t *mmap = ((struct multiboot_tag_mmap *) tag)->entries;
                for (; (multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
                     mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag)->entry_size)) {
                    // printsf(">> type:%d %X-%X size:%X\n",mmap->type, mmap->addr, mmap->addr+mmap->len, mmap->len);
                    if(mmap->type != 1) 
                        continue;
                    (free + segment_count)->start = mmap->addr % 4096 == 0 ? page_id(mmap->addr) : page_id(mmap->addr) + 1;
                    (free + segment_count)->end = page_id(mmap->addr + mmap->len);
                    segment_count ++;
                    if (segment_count == MAX_FREE_SEGMENT_COUNT) 
                        return segment_count;
                }
                }
                break;
            case MULTIBOOT_TAG_TYPE_ELF_SECTIONS: {
                uint64_t min = 0xffffffffffff;
                uint64_t max = 0;
                Elf64Shdr *sections = (Elf64Shdr *)(((struct multiboot_tag_elf_sections *) tag)->sections);
                for(int i=1; i < ((struct multiboot_tag_elf_sections *) tag)->num; i++) {
                    if(sections[i].sh_flags != 0x30 && sections[i].sh_flags != 0) {
                        if (sections[i].sh_addr < min) min = sections[i].sh_addr; 
                        if (sections[i].sh_addr + sections[i].sh_size > max) max = sections[i].sh_addr + sections[i].sh_size;
                    }
                }
                kernel->start = min;
                kernel->end = max;
                }
                break;
            default:
                break;
            }
    }
    return segment_count;
}