

#include <stdio.h>
#include "busSystem.h"



void printRomInfo(ROM *rom) {
    printf("ROM-Informationen\n");

    // Family Code
    printf("Family Code: %u\n", rom->familyCode);

    // Seriennummer umgedreht
    printf("Serial Number: ");
    for (int i = 5; i >= 0; i--) {
        printf("%u", rom->serialNumber[i]);
        if (i > 0) printf("-");
    }
    printf("\n");

    // CRC
    printf("CRC Checksum: %u\n", rom->crc);
}
