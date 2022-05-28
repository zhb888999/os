#ifndef MM_PMANAGER_H
#define MM_PMANAGER_H
#include <int.h>

#define MANAGER_FREE_MAX_SIZE 10000

typedef struct {
    uint64_t head;
    uint64_t length;
} FreePages; 

typedef struct {
    uint64_t frees, maxfress, lostsize, losts;
    FreePages free[MANAGER_FREE_MAX_SIZE];
} PageManager;

int pfree(uint64_t head, uint64_t length);
uint16_t palloc(uint64_t length);
void pinfo(void);

#endif //MM_MANAGER_H
