// tideman.c
// CS50x — Week 3, Problem Set 3
//
// Exercise: Tideman (Ranked Pairs) ★★★ Advanced
// ───────────────────────────────────────────────

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// ── Limits ────────────────────────────────────────────────────────────────────
#define MAX 9

// ── Data ──────────────────────────────────────────────────────────────────────

// preferences[i][j] = number of voters who prefer candidate i over candidate j
int preferences[MAX][MAX];

// locked[i][j] = true means there is a locked edge FROM i TO j in the graph
bool locked[MAX][MAX];

// A "pair" records which candidate won and which lost a head-to-head matchup
typedef struct
{
    int winner;
    int loser;
} pair;

// Candidate names and the pairs array
string candidates[MAX];
pair   pairs[MAX * (MAX - 1) / 2]; // maximum possible pairs

int pair_count;
int candidate_count;

// ── Function prototypes — do NOT change these signatures ──────────────────────
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool creates_cycle(int start, int end);

// ── main() — provided, do not modify ─────────────────────────────────────────
int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Initialise arrays to zero / false
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j]      = false;
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Collect rankings and build preference matrix
    for (int i = 0; i < voter_count; i++)
    {
        int ranks[candidate_count]; // ranks[j] = index of j-th preference

        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);
            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// TODO 1: vote(rank, name, ranks[])
// ─────────────────────────────────────────────────────────────────────────────
bool vote(int rank, string name, int ranks[])
{
    // TODO: loop i from 0 to candidate_count
    for (int i = 0; i < candidate_count; i++)
    {
        // if strcmp(candidates[i], name) == 0:
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// TODO 2: record_preferences(ranks[])
// ─────────────────────────────────────────────────────────────────────────────
void record_preferences(int ranks[])
{
    // TODO: nested loop
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// TODO 3: add_pairs()
// ─────────────────────────────────────────────────────────────────────────────
void add_pairs(void)
{
    // TODO: for i from 0 to candidate_count - 1:
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser  = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser  = i;
                pair_count++;
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// TODO 4: sort_pairs()
// ─────────────────────────────────────────────────────────────────────────────
void sort_pairs(void)
{
    // TODO: sort pairs[] by preferences[winner][loser] descending
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            int strength1 = preferences[pairs[j].winner][pairs[j].loser];
            int strength2 = preferences[pairs[j + 1].winner][pairs[j + 1].loser];

            if (strength1 < strength2)
            {
                pair tmp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = tmp;
            }
        }
    }
}

// ── Cycle-detection helper ────────────────────────────────────────────────────
bool creates_cycle(int start, int end)
{
    // Base case: path leads back to start — cycle!
    if (start == end)
    {
        return true;
    }

    // TODO: for k from 0 to candidate_count - 1:
    for (int k = 0; k < candidate_count; k++)
    {
        if (locked[end][k])
        {
            if (creates_cycle(start, k))
            {
                return true;
            }
        }
    }

    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// TODO 5: lock_pairs()
// ─────────────────────────────────────────────────────────────────────────────
void lock_pairs(void)
{
    // TODO: for i from 0 to pair_count - 1:
    for (int i = 0; i < pair_count; i++)
    {
        if (!creates_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// TODO 6: print_winner()
// ─────────────────────────────────────────────────────────────────────────────
void print_winner(void)
{
    // TODO: for i from 0 to candidate_count - 1:
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_source = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                is_source = false;
                break;
            }
        }
        if (is_source)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
