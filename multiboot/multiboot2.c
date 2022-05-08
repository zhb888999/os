#include <int.h>
#include <dev/vga.h>
#include <dev/serial.h>
#include <multiboot/multiboot2.h>
#include <mm/page.h>
#include <kernel/init.h>


int parse_multiboot2(uint32_t addr, MemorySegmentPage *free, MemorySegment *kernel) {
    int segment_count = 0;
    uint64_t mm_kernel_min, mm_kernel_max;
  
    struct multiboot_tag *tag;
    uint32_t size = addr;
    printf ("Announced mbi size 0x%x\n", size);
    for (tag = (struct multiboot_tag *) (addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))) {
        printf ("Tag 0x%x, Size 0x%x\n", tag->type, tag->size);
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                printf ("Command line = %s\n",
                        ((struct multiboot_tag_string *) tag)->string);
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                printf ("Boot loader name = %s\n",
                        ((struct multiboot_tag_string *) tag)->string);
                break;
            case MULTIBOOT_TAG_TYPE_MODULE:
                printf ("Module at 0x%x-0x%x. Command line %s\n",
                        ((struct multiboot_tag_module *) tag)->mod_start,
                        ((struct multiboot_tag_module *) tag)->mod_end,
                        ((struct multiboot_tag_module *) tag)->cmdline);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                printf ("mem_lower = %DKB, mem_upper = %DKB\n",
                        ((struct multiboot_tag_basic_meminfo *) tag)->mem_lower,
                        ((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
                break;
            case MULTIBOOT_TAG_TYPE_MMAP: {
                multiboot_memory_map_t *mmap;
                printf ("mmap\n");
                for (mmap = ((struct multiboot_tag_mmap *) tag)->entries; (multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size; mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag)->entry_size)) {
                    if(mmap->type != 1) continue;
                    (free + segment_count)->start = mmap->addr % 4096 == 0 ? page_id(mmap->addr) : page_id(mmap->addr) + 1;
                    (free + segment_count)->end = page_id(mmap->addr + mmap->len);
                    segment_count += 1;
                    if (segment_count == MAX_FREE_SEGMENT_COUNT) return segment_count;
                    printf (" Mem: 0x%X-0x%X, length = 0x%X, type = 0x%x page_id:%D-%D\n",
                            mmap->addr,
                            mmap->addr + mmap->len,
                            mmap->len,
                            mmap->type,
                            page_id(mmap->addr),
                            page_id(mmap->addr + mmap->len));
                    }
                }
                break;
            case MULTIBOOT_TAG_TYPE_ELF_SECTIONS: {
                uint64_t min = 0xffffffffffff;
                uint64_t max = 0;
                Elf64Shdr *sections = (Elf64Shdr *)(((struct multiboot_tag_elf_sections *) tag)->sections);
                Elf64Shdr *shstr = sections + ((struct multiboot_tag_elf_sections *) tag)->shndx;
                for(int i=1; i < ((struct multiboot_tag_elf_sections *) tag)->num; i++) {
                    if(sections[i].sh_flags != 0x30 && sections[i].sh_flags != 0) {
                        if (sections[i].sh_addr < min) min = sections[i].sh_addr; 
                        if (sections[i].sh_addr + sections[i].sh_size > max) max = sections[i].sh_addr + sections[i].sh_size;
                    }
                    char *section_name = (char *)(shstr->sh_addr + sections[i].sh_name);
                    printsf("    section: %s %d %x %X %X\n", section_name, i, sections[i].sh_flags, sections[i].sh_addr, sections[i].sh_size);
                }
                printf("   min:%X max:%X page: %D-%D\n", min, max, page_id(min), page_id(max));
                mm_kernel_min = page_id(min);
                mm_kernel_max = page_id(max);
                }
                break;
            default:
                break;
        }
    }
    return segment_count;
    // init mm_page
    for(int i = 0; i < segment_count; i++) {
        if(segments[i].start < 3) segments[i].start = 3;
        if(segments[i].end - segments[i].start < 1) continue;

        int cross = 0;
        if(segments[i].start <= mm_kernel_min && segments[i].end >= mm_kernel_min) {
            mm_free_pages(man, segments[i].start, mm_kernel_min - segments[i].start);
            cross = 1;
        }
        if(segments[i].start <= mm_kernel_max && segments[i].end >= mm_kernel_max) {
            mm_free_pages(man, mm_kernel_max + 1, segments[i].end - mm_kernel_max - 1);
            cross = 1;
        }
        if (cross) continue;
            
        mm_free_pages(man, segments[i].start, segments[i].end - segments[i].start);
    }
}

