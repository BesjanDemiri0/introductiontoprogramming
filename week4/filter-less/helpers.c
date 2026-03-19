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
            // Calculate the average of Red, Green, and Blue
            // Use 3.0 to ensure floating point division before rounding
            float average = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            int rounded_avg = round(average);

            // Set all channels to the same average value
            image[i][j].rgbtRed = rounded_avg;
            image[i][j].rgbtGreen = rounded_avg;
            image[i][j].rgbtBlue = rounded_avg;
        }
    }
}

// ---------------------------------------------------------------------------
// TODO 2: SEPIA
// ---------------------------------------------------------------------------
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Store original values
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            // Compute sepia values using the formula
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            // Cap values at 255
            image[i][j].rgbtRed = (sepiaRed > 255) ? 255 : sepiaRed;
            image[i][j].rgbtGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            image[i][j].rgbtBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;
        }
    }
}

// ---------------------------------------------------------------------------
// TODO 3: REFLECT
// ---------------------------------------------------------------------------
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // Only loop until the middle of the row
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
// TODO 4: BLUR
// ---------------------------------------------------------------------------
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of the image to read from
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
            float totalRed = 0, totalGreen = 0, totalBlue = 0;
            int counter = 0;

            // Iterate through the 3x3 neighbor grid
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if neighbor is within image bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        totalRed += copy[ni][nj].rgbtRed;
                        totalGreen += copy[ni][nj].rgbtGreen;
                        totalBlue += copy[ni][nj].rgbtBlue;
                        counter++;
                    }
                }
            }

            // Calculate average and assign to original image
            image[i][j].rgbtRed = round(totalRed / counter);
            image[i][j].rgbtGreen = round(totalGreen / counter);
            image[i][j].rgbtBlue = round(totalBlue / counter);
        }
    }
}
