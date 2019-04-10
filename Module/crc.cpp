#include "crc.h"

/**
* º∆À„CRC16–£—È¬Î
*
* @param bytes
* @return
*/
crc getCRC(uint8_t bytes[], int lenght)
{
	crc CRC = 0x0000ffff;
	int POLYNOMIALS = 0x0000a001;

	int i, j;
	for (i = 0; i < lenght; i++) {
		CRC ^= ((int)bytes[i] & 0x000000ff);
		for (j = 0; j < 8; j++) {
			if ((CRC & 0x00000001) != 0) {
				CRC >>= 1;
				CRC ^= POLYNOMIALS;
			}
			else {
				CRC >>= 1;
			}
		}
	}
	return CRC;
}

