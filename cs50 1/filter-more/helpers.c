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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE buffer[height][width];

    for (int i = 0; i < height; i++) // parse through the pixel array
    {
        for (int j = 0; j < width; j++)
        {
            int xRed = 0;
            int xBlue = 0;
            int xGreen = 0;
            int yRed = 0;
            int yBlue = 0;
            int yGreen = 0;

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
                    xRed += image[i + k][j + l].rgbtRed * gx[k + 1][l + 1]; // read the color grade from the array
                    xBlue += image[i + k][j + l].rgbtBlue * gx[k + 1][l + 1];
                    xGreen += image[i + k][j + l].rgbtGreen * gx[k + 1][l + 1];
                    yRed += image[i + k][j + l].rgbtRed * gy[k + 1][l + 1];
                    yBlue += image[i + k][j + l].rgbtBlue * gy[k + 1][l + 1];
                    yGreen += image[i + k][j + l].rgbtGreen * gy[k + 1][l + 1];
                }
            } // store the calculated color grade of itself and the surrounding pixels
            buffer[i][j].rgbtRed = ((round(sqrt(xRed * xRed + yRed * yRed))) > 255) ? 255
                                   : round(sqrt(xRed * xRed + yRed * yRed));
            buffer[i][j].rgbtBlue = ((round(sqrt(xBlue * xBlue + yBlue * yBlue))) > 255) ? 255
                                    : round(sqrt(xBlue * xBlue + yBlue * yBlue));
            buffer[i][j].rgbtGreen = ((round(sqrt(xGreen * xGreen + yGreen * yGreen))) > 255) ? 255
                                     : round(sqrt(xGreen * xGreen + yGreen * yGreen));
        }
    }
    for (int i = 0; i < height; i++) // write updated values into original array;
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
