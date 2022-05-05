#ifndef MM_MANAGER_H
#define MM_MANAGER_H
#include <int.h>

#define MANAGER_FREE_MAX_SIZE 10000

struct __MMFreePageInfo {
    uint64_t head;
    uint64_t length;
}; 
typedef struct __MMFreePageInfo MMFreePageInfo;

struct __MMFreePageManager {
    uint64_t frees, maxfress, lostsize, losts;
    MMFreePageInfo free[MANAGER_FREE_MAX_SIZE];
};
typedef struct __MMFreePageManager MMFreePageManager;

void init_mm_page_manager(MMFreePageManager *);
int mm_free_pages(MMFreePageManager *, uint64_t head, uint64_t length);
uint16_t mm_alloc_pages(MMFreePageManager *, uint64_t length);

#endif //MM_MANAGER_H
