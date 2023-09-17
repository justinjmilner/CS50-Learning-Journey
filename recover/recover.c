#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int BLOCK_SIZE = 512;
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check only one file was input
    if (argc != 2)
    {
        printf("Input one file name\n");
        return 1;
    }

    // open memory card
    FILE *memoryCard = fopen(argv[1], "r");

    // check for correct file input name
    if (memoryCard == NULL)
    {
        printf("Could not open file");
        return 2;
    }

    int counter = 0;

    // create buffer memory for current FAT file bytes
    BYTE buffer[BLOCK_SIZE];

    // create empty data location for jpeg file name
    char *fileName = malloc(8 * sizeof(char));

    // create a pointer that will be used to write data to
    FILE *jpegData = NULL;

    // read 512 bytes into buffer and repeat until end of memorycard
    while (fread(buffer, 1, BLOCK_SIZE, memoryCard) == BLOCK_SIZE)
    {
        // if start of jpeg is found, open a new file to write to
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if first jpeg
            if (counter == 0)
            {
                sprintf(fileName, "%03i.jpg", counter);
                counter++;
                jpegData = fopen(fileName, "w");
            }

            // if not first jpeg
            else
            {
                fclose(jpegData);
                sprintf(fileName, "%03i.jpg", counter);
                counter++;
                jpegData = fopen(fileName, "w");
            }
        }

        // write to current file
        if (jpegData != NULL)
        {
            fwrite(buffer, BLOCK_SIZE, 1, jpegData);
        }
    }

    // close remaining files
    free(fileName);
    fclose(jpegData);
    fclose(memoryCard);

    return 0;
}