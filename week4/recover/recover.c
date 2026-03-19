#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ---------------------------------------------------------------------------
    // TODO 1: Validate command-line arguments
    // ---------------------------------------------------------------------------
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // ---------------------------------------------------------------------------
    // TODO 2: Open the memory card file for reading (binary mode)
    // ---------------------------------------------------------------------------
    FILE *card = fopen(argv[1], "rb");
    if (card == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // ---------------------------------------------------------------------------
    // TODO 3: Declare your variables
    // ---------------------------------------------------------------------------
    BYTE buffer[512];      // A 512-byte buffer to store one block of data
    int count = 0;         // A counter for JPEGs found
    FILE *img = NULL;      // A FILE* for current output file
    char filename[8];      // A filename string "###.jpg\0" (7 chars + null terminator)

    // ---------------------------------------------------------------------------
    // TODO 4: Main loop — read one 512-byte block at a time
    // ---------------------------------------------------------------------------
    while (fread(buffer, 1, 512, card) == 512)
    {
        // STEP A: Check if this block starts a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // STEP B: If we already have an open file, close it
            if (img != NULL)
            {
                fclose(img);
            }

            // STEP C: Open a new output file
            sprintf(filename, "%03i.jpg", count);
            img = fopen(filename, "wb");
            if (img == NULL)
            {
                fclose(card);
                printf("Could not create output file %s.\n", filename);
                return 1;
            }
            count++;
        }

        // STEP D: If we currently have an open output file, write the block
        // (This skips garbage data at the beginning of the card before the first JPEG)
        if (img != NULL)
        {
            fwrite(buffer, 1, 512, img);
        }
    }

    // ---------------------------------------------------------------------------
    // TODO 5: Close any remaining open file and close the card
    // ---------------------------------------------------------------------------
    if (img != NULL)
    {
        fclose(img);
    }
    
    fclose(card);

    return 0;
}
