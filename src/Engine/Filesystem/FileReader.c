#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <Engine/Filesystem/FileReader.h>

// tldr:
// texture data in opengl is just 3 numbers describing RGB values for a specific pixel
// The goal is these functions to return a unsigned char array of this data from a file

unsigned char* readBMP(char* filepath, uint32_t* bmpWidth, uint32_t* bmpHeight) {
    FILE* filePtr;

    filePtr = fopen(filepath, "rb");

    char bmpHeader[14];

    fgets(bmpHeader, 14, filePtr);

    if (bmpHeader[0] != 'B' || bmpHeader[1] != 'M') {
        printf("%s is not a BMP\n", filepath);
        fclose(filePtr);
        return;
    }
    
    uint32_t fileSize;
    memcpy(&fileSize, &(bmpHeader[2]), 4);

    uint32_t dataOffset;
    memcpy(&dataOffset, &(bmpHeader[10]), 4);

    fseek(filePtr, 14, SEEK_SET);

    uint32_t sizeDIB;
    fread(&sizeDIB, 1, 4, filePtr);

    fseek(filePtr, 14, SEEK_SET);

    if (sizeDIB != 40) {
        fclose(filePtr);
        printf("Cannot read DIB header\n");
        return;
    }

    char* headerDIB = malloc(sizeDIB);
    fread(headerDIB, 1, sizeDIB, filePtr);

    uint16_t colorDepth; // BITS per pixel
    uint32_t compressionMethod; // Will likely be 0 (no compression)
    uint32_t bmpRawDataSize;

    memcpy(bmpWidth, &(headerDIB[4]), 4);
    memcpy(bmpHeight, &(headerDIB[8]), 4);
    memcpy(&colorDepth, &(headerDIB[14]), 2);
    memcpy(&compressionMethod, &(headerDIB[16]), 4);
    memcpy(&bmpRawDataSize, &(headerDIB[20]), 4);
    
    unsigned char* pixelData = malloc(bmpRawDataSize);
    fseek(filePtr, dataOffset, SEEK_SET);
    fread(pixelData, 1, bmpRawDataSize, filePtr);

    uint32_t bytesPerRow = ceil( (colorDepth * (*bmpWidth) ) / 32 ) * 4;

    fclose(filePtr);

    return pixelData;
}
inline void freeFileData(unsigned char *data) {
    if (data != NULL) {
        free(data);
    }
}