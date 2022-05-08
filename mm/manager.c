#include <int.h>
#include <mm/manager.h>
#include <dev/vga.h>

void init_mm_page_manager(MMFreePageManager *man) {
    man->frees = 0;
    man->maxfress = 0;
    man->losts = 0;
    man->lostsize = 0;
}

uint16_t mm_alloc_pages(MMFreePageManager *man, uint64_t length) {
    uint64_t head;
    for(uint64_t i = 0; i < man->frees; i++) {
        if(man->free[i].length >= length) {
            head = man->free[i].head;
            man->free[i].head += length;
            man->free[i].length -= length;
            if(man->free[i].length == 0) {
                man->frees--;
                for(; i < man->frees; i++) {
                    man->free[i] = man->free[i + 1];
                }
            }
            return head;
        }
    }
    return 0;
}

/* success return 0 else -1 */
int mm_free_pages(MMFreePageManager *man, uint64_t head, uint64_t length) {
    if(length < 1) return 0;
    uint64_t i;
    for(i = 0; i < man->frees; i++) {
        if(man->free[i].head > head) break;
    }
    if(i > 0) {
        if(man->free[i - 1].head + man->free[i - 1].length == head) {
            man->free[i - 1].length += length;
            if(i < man->frees) {
                if(head + length == man->free[i].head) {
                    man->free[i - 1].length += man->free[i].length;
                    man->frees--;
                    for(; i < man->frees; i++) {
                        man->free[i] = man->free[i+1];
                    }
                }
            }
            return 0;
        }
    }
    if(i < man->frees) {
        if(head + length == man->free[i].head) {
            man->free[i].head = head;
            man->free[i].length += length;
            return 0;
        }
    }
    if (man->frees < MANAGER_FREE_MAX_SIZE) {
        for(uint64_t j = man->frees; j > i; j--) {
            man->free[j] = man->free[j - 1];
        }
        man->frees++;
        if(man->maxfress < man->frees) {
            man->maxfress = man->frees;
        }
        man->free[i].head = head;
        man->free[i].length = length;
        return 0;
    }
    man->losts++;
    man->lostsize += length;
    return -1;
}

void mm_info(MMFreePageManager *man) {
    uint64_t sum = 0;
    printf("fress:%D maxfress:%D losts:%D lostsize:%D\n", man->frees, man->maxfress, man->losts, man->lostsize);
    for(uint64_t i = 0; i < man->frees; i++) {
        printf("  [%D] %D-%D %D\n", i, 
                   man->free[i].head, 
                   man->free[i].head + man->free[i].length, 
                   man->free[i].length);
        sum += man->free[i].length;
    }
    printf("sum:%D\n", sum);
}
