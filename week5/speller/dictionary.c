// CS50x Week 5 — Speller: dictionary.c
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>   // for strcasecmp

#include "dictionary.h"

// ---------------------------------------------------------------------------
// HASH TABLE CONFIGURATION
// ---------------------------------------------------------------------------
// N set to a prime number around 10k to reduce collisions for the large dictionary
#define N 10007

// ---------------------------------------------------------------------------
// NODE — one entry in the hash table's linked list
// ---------------------------------------------------------------------------
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// ---------------------------------------------------------------------------
// GLOBALS
// ---------------------------------------------------------------------------
node *table[N];
unsigned int word_count = 0;

// ---------------------------------------------------------------------------
// TODO 1: hash()
// ---------------------------------------------------------------------------
unsigned int hash(const char *word)
{
    // A polynomial rolling hash or djb2-style function for better distribution
    unsigned long hash_val = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // Use tolower to ensure case-insensitivity
        hash_val = (hash_val * 31) + tolower(word[i]);
    }
    return hash_val % N;
}

// ---------------------------------------------------------------------------
// TODO 2: load()
// ---------------------------------------------------------------------------
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    // Read words one by one until EOF
    while (fscanf(file, "%s", word) != EOF)
    {
        // Malloc a new node for this word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }

        // Copy word into node
        strcpy(n->word, word);

        // Hash the word to find its bucket
        unsigned int index = hash(word);

        // Prepend node to bucket's linked list
        n->next = table[index];
        table[index] = n;

        // Increment global word counter
        word_count++;
    }

    // Close the file
    fclose(file);
    return true;
}

// ---------------------------------------------------------------------------
// TODO 3: check()
// ---------------------------------------------------------------------------
bool check(const char *word)
{
    // Hash the word to find the correct bucket
    unsigned int index = hash(word);

    // Walk the linked list in that bucket
    node *cursor = table[index];

    while (cursor != NULL)
    {
        // Use strcasecmp for case-insensitive comparison
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// ---------------------------------------------------------------------------
// TODO 4: size()
// ---------------------------------------------------------------------------
unsigned int size(void)
{
    // Return the global counter incremented during load()
    return word_count;
}

// ---------------------------------------------------------------------------
// TODO 5: unload()
// ---------------------------------------------------------------------------
bool unload(void)
{
    // Loop over all N buckets
    for (int i = 0; i < N; i++)
    {
        // Cursor to track current node
        node *cursor = table[i];

        while (cursor != NULL)
        {
            // Save pointer to next node before freeing current
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}
