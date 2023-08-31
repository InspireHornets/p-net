#include "utils.h"

bool are_arrays_equal (
   const uint8_t arr1[],
   int size1,
   const uint8_t arr2[],
   int size2)
{
   if (size1 != size2)
   {
      return false;
   }

   for (int i = 0; i < size1; i++)
   {
      if (arr1[i] != arr2[i])
      {
         return false;
      }
   }

   return true;
}

uint32_t combine_bytes_to_uint32 (const uint8_t * bytes)
{
   uint32_t result = 0;

   result |= (uint32_t)bytes[0] << 24;
   result |= (uint32_t)bytes[1] << 16;
   result |= (uint32_t)bytes[2] << 8;
   result |= (uint32_t)bytes[3];

   return result;
}
