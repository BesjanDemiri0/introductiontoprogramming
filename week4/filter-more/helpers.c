#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------
// TODO 1: GRAYSCALE
// ---------------------------------------------------------------------------
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average of R, G, B
            float average = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            int rounded = round(average);

            // Set all channels to the average
            image[i][j].rgbtRed = rounded;
            image[i][j].rgbtGreen = rounded;
            image[i][j].rgbtBlue = rounded;
        }
    }
}

// ---------------------------------------------------------------------------
// TODO 2: REFLECT
// ---------------------------------------------------------------------------
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixel at j with pixel at (width - 1 - j)
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// ---------------------------------------------------------------------------
// TODO 3: BLUR
// ---------------------------------------------------------------------------
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            // Check 3x3 neighborhood
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // If neighbor is within bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sumR += copy[ni][nj].rgbtRed;
                        sumG += copy[ni][nj].rgbtGreen;
                        sumB += copy[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            // Assign average to original image
            image[i][j].rgbtRed = round(sumR / count);
            image[i][j].rgbtGreen = round(sumG / count);
            image[i][j].rgbtBlue = round(sumB / count);
        }
    }
}

// ---------------------------------------------------------------------------
// TODO 4: EDGES (Sobel Operator)
// ---------------------------------------------------------------------------
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Sobel kernels
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    // Create copy to read from
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Use long to prevent overflow during squaring
            long gx_r = 0, gx_g = 0, gx_b = 0;
            long gy_r = 0, gy_g = 0, gy_b = 0;

            // Iterate over 3x3 neighborhood
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if within bounds (out of bounds pixels are treated as 0/black)
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        int weightX = Gx[di + 1][dj + 1];
                        int weightY = Gy[di + 1][dj + 1];

                        gx_r += weightX * copy[ni][nj].rgbtRed;
                        gx_g += weightX * copy[ni][nj].rgbtGreen;
                        gx_b += weightX * copy[ni][nj].rgbtBlue;

                        gy_r += weightY * copy[ni][nj].rgbtRed;
                        gy_g += weightY * copy[ni][nj].rgbtGreen;
                        gy_b += weightY * copy[ni][nj].rgbtBlue;
                    }
                }
            }

            // Combine Gx and Gy using sqrt(Gx^2 + Gy^2)
            int finalR = round(sqrt(gx_r * gx_r + gy_r * gy_r));
            int finalG = round(sqrt(gx_g * gx_g + gy_g * gy_g));
            int finalB = round(sqrt(gx_b * gx_b + gy_b * gy_b));

            // Cap at 255 and assign to image
            image[i][j].rgbtRed = (finalR > 255) ? 255 : finalR;
            image[i][j].rgbtGreen = (finalG > 255) ? 255 : finalG;
            image[i][j].rgbtBlue = (finalB > 255) ? 255 : finalB;
        }
    }
}
