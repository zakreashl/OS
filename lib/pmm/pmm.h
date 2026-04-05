#ifndef PMM_H
#define PMM_H

#include "../stdlib/stdlib.h"

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
} MemMapEntry;

void     pmm_init(MemMapEntry *map, uint16_t count);
uint32_t pmm_alloc_frame(void);
void     pmm_free_frame(uint32_t addr);
void     pmm_mark_used(uint32_t start, uint32_t end);

#endif