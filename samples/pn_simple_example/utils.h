#ifndef PROFINET_UTILS_H
#define PROFINET_UTILS_H

#include <stdint.h>
#include <stdbool.h>

bool are_arrays_equal (
   const uint8_t arr1[],
   int size1,
   const uint8_t arr2[],
   int size2);

uint32_t combine_bytes_to_uint32 (const uint8_t * bytes);

#endif /* PROFINET_UTILS_H */