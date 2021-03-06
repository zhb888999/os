#include <mm/kmanager.h>
#include <dev/serial.h>
#include <dev/vga.h>


BumpAllocator bumps[BUMP_ALLOCATER_NR];

static uint64_t bump_alloc(BumpAllocator *bump, uint64_t size, uint64_t align);
static uint64_t noraml_malloc(uint64_t size, uint64_t align);
static uint64_t large_malloc(uint64_t size);
static void bump_free(BumpAllocator *bump, uint64_t addr);

uint64_t kmalloc(uint64_t size, uint64_t align) {
    return size <= BUMP_ALLOCATER_SIZE ? noraml_malloc(size, align) : large_malloc(size);
};

uint64_t noraml_malloc(uint64_t size, uint64_t align) {
    for(int i = 0; i < BUMP_ALLOCATER_NR; i++) {
        uint64_t addr = bump_alloc(bumps + i, size, align);
        if(addr) 
            return addr;
    }
    return 0;
}

uint64_t large_malloc(uint64_t size) {
    int start = 0, count = 0;
    uint64_t alloc_size = 0;
    for(int i=0; i < BUMP_ALLOCATER_NR; i++) {
        if(alloc_size >= size) 
            break;
        if((bumps[i].allocations == 0) && ((BUMP_ALLOCATER_SIZE - 1) & bumps[i].start) == 0) {
            if(alloc_size == 0) start = i;
            alloc_size += BUMP_ALLOCATER_SIZE;
            count++;
        } else {
            alloc_size = 0;
            count = 0;
        }
    }
    if(alloc_size < size) return 0;
    for(int i=0; i<count; i++)
        bumps[start + i].continuous = count;
    return start << BUMP_ALLOCATER_BITS;
}

void kfree(uint64_t address) {
    uint64_t bump_index = BUMP_INDEX(address);
    if(!bumps[bump_index].continuous) {
        bump_free(bumps + bump_index, address);
        return;
    }
    uint64_t continuous = bumps[bump_index].continuous;
    for(uint64_t i = 0; i < continuous; i++) {
        bumps[bump_index + i].continuous = 0;
    }
} 


inline static uint64_t bump_alloc(BumpAllocator *bump, uint64_t size, uint64_t align) {
    if(bump->continuous) 
        return 0;
    uint64_t alloc_start = align ? ALIGN_UP(bump->next, align) : bump->next;
    uint64_t alloc_end = alloc_start + size;
    if(alloc_end > bump->end) 
        return 0;
    bump->next = alloc_end;
    bump->allocations++;
    return alloc_start;
}

inline static void bump_free(BumpAllocator *bump, uint64_t addr) {
    bump->allocations--;
    if(bump->allocations == 0) bump->next = bump->start;
}

void bumps_info(void) {
    for(int i=0; i < BUMP_ALLOCATER_NR; i++) {
        printsf(">> bump%d\n", i);
        printsf("     bump.start       = 0x%X\n", bumps[i].start);
        printsf("     bump.end         = 0x%X\n", bumps[i].end);
        printsf("     bump.next        = 0x%X\n", bumps[i].next);
        printsf("     bump.allocations = 0x%X\n", bumps[i].allocations);
        printsf("     bump.continuous  = 0x%X\n", bumps[i].continuous);
    }
}

void bumps_info2(int start, int end) {
    for(int i=start; i < end; i++) {
        printsf(">> bump%d\n", i);
        printsf("     bump.start       = 0x%X\n", bumps[i].start);
        printsf("     bump.end         = 0x%X\n", bumps[i].end);
        printsf("     bump.next        = 0x%X\n", bumps[i].next);
        printsf("     bump.allocations = 0x%X\n", bumps[i].allocations);
        printsf("     bump.continuous  = 0x%X\n", bumps[i].continuous);
    }
}