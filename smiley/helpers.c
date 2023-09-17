#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Select the data column
    for (int j = 0; j < height; j++)
    {
        // Select the data row
        for (int i = 0; i < width; i++)
        {
            // Parse through the memory array as each space contains the color data for one pixel
            if (image[j][i].rgbtRed == 0x00 && image[j][i].rgbtBlue == 0x00 && image[j][i].rgbtGreen == 0x00)
            {
                // Starting with 0x00 (black), turn on any color to change the smiley color
                image[j][i].rgbtRed = 0x79;
                image[j][i].rgbtBlue = 0xdf;
                image[j][i].rgbtGreen = 0xff;
            }
        }
    }
}
