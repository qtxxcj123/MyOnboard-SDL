#ifndef _crc_h
#define _crc_h
#include <stdint.h>
typedef uint64_t crc;
crc getCRC(uint8_t bytes[], int lenght);
#endif
