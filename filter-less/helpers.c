#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int j = 0; j < height; j++) // parse through each pixel
    {
        for (int k = 0; k < width; k++)
        {
            float median = ((image[j][k].rgbtRed + image[j][k].rgbtGreen + image[j][k].rgbtBlue) /
                            3.0); // find median value of all colors added together
            median = round(median);
            image[j][k].rgbtBlue = image[j][k].rgbtGreen = image[j][k].rgbtRed = median; // divide evenly to create different shades of grey
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int j = 0; j < height; j++) // parse through all pixels
    {
        for (int k = 0; k < width; k++)
        {
            int newRed = round(.393 * image[j][k].rgbtRed + .769 * image[j][k].rgbtGreen + .189 *
                               image[j][k].rgbtBlue); // multiply color by the sepia formula
            int newGreen = round(.349 * image[j][k].rgbtRed + .686 * image[j][k].rgbtGreen + .168 * image[j][k].rgbtBlue);
            int newBlue = round(.272 * image[j][k].rgbtRed + .534 * image[j][k].rgbtGreen + .131 * image[j][k].rgbtBlue);

            image[j][k].rgbtRed = (newRed > 255) ? 255 : newRed; // set new color in place of the old
            image[j][k].rgbtGreen = (newGreen > 255) ? 255 : newGreen;
            image[j][k].rgbtBlue = (newBlue > 255) ? 255 : newBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];
    for (int j = 0; j < height; j++) // parse through half the pixels
    {
        for (int k = 0; k < width / 2; k++)
        {
            buffer[j][k] = image[j][k]; // swap with the mirroring side
            image[j][k] = image[j][width - (k + 1)];
            image[j][width - (k + 1)] = buffer[j][k];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];

    for (int i = 0; i < height; i++) // parse through the pixel array
    {
        for (int j = 0; j < width; j++)
        {
            float red = 0;
            float blue = 0;
            float green = 0;
            float counter = 0;
            for (int k = -1; k < 2; k++) // parse through itself and each of the surrounding pixels
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k < 0 || i + k > height - 1)
                    {
                        break; // increase for loop for k
                    }
                    if (j + l < 0 || j + l > width - 1)
                    {
                        continue; // increase nested for look for l
                    }
                    red += image[i + k][j + l].rgbtRed; // read the color grade from the array
                    blue += image[i + k][j + l].rgbtBlue;
                    green += image[i + k][j + l].rgbtGreen;
                    counter++;
                }
            }
            buffer[i][j].rgbtRed = round(red / counter); // store the average color grade of itself and the surrounding pixels
            buffer[i][j].rgbtBlue = round(blue / counter);
            buffer[i][j].rgbtGreen = round(green / counter);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = buffer[i][j].rgbtBlue;
            image[i][j].rgbtGreen = buffer[i][j].rgbtGreen;
            image[i][j].rgbtRed = buffer[i][j].rgbtRed;
        }
    }
    return;
}
