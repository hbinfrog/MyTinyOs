#ifndef mmu_guard
#define mmu_guard

#include <stdint.h>
#include "memory/dram.h"

uint64_t va2pa(uint64_t vaddr);

#endif