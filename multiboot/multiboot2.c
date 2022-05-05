#include <int.h>
#include <dev/vga.h>
#include <dev/serial.h>
#include <multiboot/multiboot2.h>

void parse_multiboot2(uint32_t addr) {
    struct multiboot_tag *tag;
    uint32_t size = *(uint32_t *) addr;
  printf ("Announced mbi size 0x%x\n", size);
  for (tag = (struct multiboot_tag *) (addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7)))
    {
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
        case MULTIBOOT_TAG_TYPE_MMAP:
          {
            multiboot_memory_map_t *mmap;

            printf ("mmap\n");
      
            for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
                 (multiboot_uint8_t *) mmap 
                   < (multiboot_uint8_t *) tag + tag->size;
                 mmap = (multiboot_memory_map_t *) 
                   ((unsigned long) mmap
                    + ((struct multiboot_tag_mmap *) tag)->entry_size))
              printf (" base_addr = 0x%x%x,"
                      " length = 0x%x%x, type = 0x%x\n",
                      (unsigned) (mmap->addr >> 32),
                      (unsigned) (mmap->addr & 0xffffffff),
                      (unsigned) (mmap->len >> 32),
                      (unsigned) (mmap->len & 0xffffffff),
                      (unsigned) mmap->type);
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
          printsf("   min:%X max:%X\n", min, max);
        }
          break;
        default:
          break;
        }
    }
}

