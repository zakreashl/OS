#include "pmm.h"

#define FRAME_SIZE 4096
#define BITMAP_SIZE 32768

static uint32_t bitmap[BITMAP_SIZE];

static void set_frame(uint32_t addr, int used) {
    uint32_t frame = addr / FRAME_SIZE;
    if (used) {
        bitmap[frame / 32] |=  (1 << (frame % 32));
    } else {
        bitmap[frame / 32] &= ~(1 << (frame % 32));
    }
}

void pmm_init(MemMapEntry *map, uint16_t count) {
    // Set all bits to 1 (all frames are in use)
    for(int i = 0; i < BITMAP_SIZE; i++) {
        bitmap[i] = 0xFFFFFFFF;
    }

    // Then set all unused bits
    for (int i = 0; i < count; i++) {
        if (map[i].type != 1) continue;  // skip reserved regions

        uint32_t start = (uint32_t)map[i].base;
        uint32_t end   = start + (uint32_t)map[i].length;

        for (uint32_t addr = start; addr < end; addr += FRAME_SIZE) {
            set_frame(addr, 0);  // mark free
        }
    }
}

uint32_t pmm_alloc_frame(void) {
    for (int i = 0; i < 32768; i++) {
        if (bitmap[i] == 0xFFFFFFFF) continue;  // all 32 frames used, skip

        // find the free bit
        for (int bit = 0; bit < 32; bit++) {
            if (!(bitmap[i] & (1 << bit))) {
                bitmap[i] |= (1 << bit);         // mark used
                return (i * 32 + bit) * FRAME_SIZE;  // return physical address
            }
        }
    }
    return 0;  // out of memory
}

void pmm_free_frame(uint32_t addr) {
    set_frame(addr, 0);
}

void pmm_mark_used(uint32_t start, uint32_t end) {
    for(uint32_t addr = start; addr < end; addr += FRAME_SIZE) {
        set_frame(addr, 1);
    }
}