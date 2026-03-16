// credit.c
// CS50x - Week 1, Problem Set 1
//
// Exercise: Credit Card Validator
// --------------------------------
// Validate a credit card number using Luhn's Algorithm,
// then identify whether it is AMEX, MASTERCARD, VISA, or INVALID.
//
// How to compile:  make credit
// How to run:      ./credit
// How to check:    check50 cs50/problems/2024/x/credit

#include <cs50.h>
#include <stdio.h>

// ---------------------------------------------------------------------------
// Luhn's Algorithm (overview)
// ---------------------------------------------------------------------------
// 1. Starting from the SECOND-TO-LAST digit, multiply every other digit by 2.
// 2. If any product >= 10, add its digits together (e.g. 14 → 1+4 = 5).
// 3. Sum all those results → call it sum_doubled.
// 4. Sum all the OTHER digits (the ones you didn't double) → call it sum_rest.
// 5. If (sum_doubled + sum_rest) % 10 == 0, the number is VALID.
// ---------------------------------------------------------------------------
// Card type detection:
//   AMEX:       15 digits, starts with 34 or 37
//   MASTERCARD: 16 digits, starts with 51–55
//   VISA:       13 or 16 digits, starts with 4
// ---------------------------------------------------------------------------

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // STEP 0: Prompt user for a credit card number
    long number = get_long("Number: ");

    // -----------------------------------------------------------------------
    // STEP 1: Count the number of digits
    // -----------------------------------------------------------------------
    int length = 0;
    long temp_n = number;

    while (temp_n > 0)
    {
        temp_n = temp_n / 10;
        length++;
    }

    // Check if length is even valid before proceeding
    if (length != 13 && length != 15 && length != 16)
    {
        printf("INVALID\n");
        return 0;
    }

    // -----------------------------------------------------------------------
    // STEP 2: Apply Luhn's Algorithm
    // -----------------------------------------------------------------------
    int sum_doubled = 0;
    int sum_rest = 0;
    long n = number;

    for (int i = 0; i < length; i++)
    {
        // Get the last digit of the current number
        int digit = n % 10;

        if (i % 2 == 1) // If position is ODD (starting from 0 as the last digit)
        {
            int product = digit * 2;
            // If product is 12, we add 1 + 2. 
            // product % 10 gets the 2, product / 10 gets the 1.
            sum_doubled += (product % 10) + (product / 10);
        }
        else // If position is EVEN
        {
            sum_rest += digit;
        }

        // Remove the last digit from the number
        n = n / 10;
    }

    // -----------------------------------------------------------------------
    // STEP 3: Check validity
    // -----------------------------------------------------------------------
    if ((sum_doubled + sum_rest) % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // -----------------------------------------------------------------------
    // STEP 4: Identify card type
    // -----------------------------------------------------------------------
    
    // Extract the first two digits
    long first_two = number;
    while (first_two >= 100)
    {
        first_two /= 10;
    }

    // Identify brand
    // AMEX: 15 digits, starts with 34 or 37
    if (length == 15 && (first_two == 34 || first_two == 37))
    {
        printf("AMEX\n");
    }
    // MASTERCARD: 16 digits, starts with 51–55
    else if (length == 16 && (first_two >= 51 && first_two <= 55))
    {
        printf("MASTERCARD\n");
    }
    // VISA: 13 or 16 digits, starts with 4
    else if ((length == 13 || length == 16) && (first_two / 10 == 4))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}
