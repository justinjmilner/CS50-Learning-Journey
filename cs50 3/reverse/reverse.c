#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // check for correct input arguments
    if (argc != 3)
    {
        printf("Proper usage err\n");
        return 1;
    }
    int result = strcmp(argv[1], "input.wav");
    if (result != 0)
    {
        printf("Proper input err\n");
        return 1;
    }
    result = strcmp(argv[2], "output.wav");
    if (result != 0)
    {
        printf("Proper output err\n");
        return 1;
    }

    // open input.wav file for reading
    FILE *input_wav = fopen(argv[1], "r");
    if (input_wav == NULL)
    {
        printf("Input file err\n");
        return 1;
    }

    // read the header bytes from the input_wav to a buffer
    WAVHEADER bufferHeader;
    fread(&bufferHeader, HEADER_SIZE, 1, input_wav);
    printf("%li\n", ftell(input_wav));

    // Use check_format to ensure WAV format
    if (check_format(bufferHeader) != 0)
    {
        printf("File type err\n");
        return 2;
    }

    // open the output.wav file for writing
    FILE *output_wav = fopen(argv[2], "w");
    if (output_wav == NULL)
    {
        printf("Output file err\n");
        return 1;
    }

    // Write header to file
    fwrite(&bufferHeader, HEADER_SIZE, 1, output_wav);

    // Use get_block_size to calculate size of block
    int blockSize = get_block_size(bufferHeader);
    printf("%i\n", blockSize);

    // read and write from the end of input into output
    BYTE bufferBytes[blockSize];
    fseek(input_wav, -blockSize, SEEK_END);
    fread(bufferBytes, 1, blockSize, input_wav);
    printf("%li\n", ftell(input_wav));
    fwrite(bufferBytes, 1, blockSize, output_wav);
    while (ftell(input_wav) - blockSize > HEADER_SIZE)
    {
        fseek(input_wav, -(2 * blockSize), SEEK_CUR);
        fread(bufferBytes, 1, blockSize, input_wav);
        printf("%li\n", ftell(input_wav));
        fwrite(bufferBytes, 1, blockSize, output_wav);
    }

    // close opened files
    fclose(input_wav);
    fclose(output_wav);
    return 0;
}

int check_format(WAVHEADER header)
{
    // check if bytes 8-11 contain the chars W A V E
    if (header.format[0] == 0x57 && header.format[1] == 0x41 && header.format[2] == 0x56 && header.format[3] == 0x45)
    {
        return 0;
    }
    return 1;
}
int get_block_size(WAVHEADER header)
{
    int blockSize = header.numChannels * (header.bitsPerSample / 8);
    return blockSize;
}