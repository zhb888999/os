#include <int.h>
#include <mm/pmanager.h>
#include <dev/vga.h>

static PageManager pmanager = {0};

uint16_t palloc(uint64_t length) {
    uint64_t head;
    for(uint64_t i = 0; i < pmanager.frees; i++) {
        if(pmanager.free[i].length >= length) {
            head = pmanager.free[i].head;
            pmanager.free[i].head += length;
            pmanager.free[i].length -= length;
            if(pmanager.free[i].length == 0) {
                pmanager.frees--;
                for(; i < pmanager.frees; i++) {
                    pmanager.free[i] = pmanager.free[i + 1];
                }
            }
            return head;
        }
    }
    return 0;
}

/* success return 0 else -1 */
int pfree(uint64_t head, uint64_t length) {
    if(length < 1) return 0;
    uint64_t i;
    for(i = 0; i < pmanager.frees; i++) {
        if(pmanager.free[i].head > head) break;
    }
    if(i > 0) {
        if(pmanager.free[i - 1].head + pmanager.free[i - 1].length == head) {
            pmanager.free[i - 1].length += length;
            if(i < pmanager.frees) {
                if(head + length == pmanager.free[i].head) {
                    pmanager.free[i - 1].length += pmanager.free[i].length;
                    pmanager.frees--;
                    for(; i < pmanager.frees; i++) {
                        pmanager.free[i] = pmanager.free[i+1];
                    }
                }
            }
            return 0;
        }
    }
    if(i < pmanager.frees) {
        if(head + length == pmanager.free[i].head) {
            pmanager.free[i].head = head;
            pmanager.free[i].length += length;
            return 0;
        }
    }
    if (pmanager.frees < MANAGER_FREE_MAX_SIZE) {
        for(uint64_t j = pmanager.frees; j > i; j--) {
            pmanager.free[j] = pmanager.free[j - 1];
        }
        pmanager.frees++;
        if(pmanager.maxfress < pmanager.frees) {
            pmanager.maxfress = pmanager.frees;
        }
        pmanager.free[i].head = head;
        pmanager.free[i].length = length;
        return 0;
    }
    pmanager.losts++;
    pmanager.lostsize += length;
    return -1;
}

void pinfo(void) {
    uint64_t sum = 0;
    printf("fress:%D maxfress:%D losts:%D lostsize:%D\n", pmanager.frees, pmanager.maxfress, pmanager.losts, pmanager.lostsize);
    for(uint64_t i = 0; i < pmanager.frees; i++) {
        printf("  [%D] %D-%D %D\n", i, 
                   pmanager.free[i].head, 
                   pmanager.free[i].head + pmanager.free[i].length, 
                   pmanager.free[i].length);
        sum += pmanager.free[i].length;
    }
    printf("sum:%D\n", sum);
}
