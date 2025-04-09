/* 
* This program is written by Ruba Ismail
* Date: 2/20/2025
* Purpose: Find the number of possible permutations in a seating plan
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10
#define MAX_LEN 20

char names[SIZE][MAX_LEN];
int hasSnacks[SIZE];        // 0 if they dont have, 1 if they have snacks
int allowed[SIZE][SIZE];    // matrix that stores 0 if the 2 name indexes aren't allowed to sit next to each other, 1 otherwise


// loops through the names array and finds the index of a name
int nameIndex(char *name, int n)
{
    for (int i = 0; i < n; i++) {
        if (strcmp(names[i], name) == 0)
            return i;
    }
    return -1;
}

// checks if each person in the permutation has a neighbor with snacks or has snacks themselves. returns 1 if it is valid.
int checkSnacks(int *perm, int n)
{
    for (int i = 0; i < n; i++)
    {
        int index = perm[i];

        if (hasSnacks[index] == 1)
            continue;


        int leftOk = 0, rightOk = 0;
        // checks if left neighbor has snacks, if this isnt the first person in the list
        if (i > 0) {
            if (hasSnacks[perm[i - 1]] == 1)
                leftOk = 1;

        }
        // checks right neighbor has snacks, if this isnt the last person in the list
        if (i < n - 1) {
            if (hasSnacks[perm[i + 1]] == 1)
                rightOk = 1;

        }

        // no neighbor has snacks, not a valid permutation
        if (leftOk == 0 && rightOk == 0) {
            return 0;
        }
    }

    return 1;
}


// used array permutations to generate seating arrangements
void printPerms(int *perm, int *used, int k, int n, int* validPermCount) {

    if (k == n)
    {
        // Check if the person to the left or right has snacks
        if (checkSnacks(perm, n) == 1)
            (*validPermCount)++;
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (used[i] == 0)
        {

            // checks if placing person i in position k is not allowed based on the conflicts, and skips it
            if (k > 0)
            {
                int prevIndex = perm[k - 1];
                if (allowed[prevIndex][i] == 0)
                    // conflict, skip
                    continue;
            }

            perm[k] = i;
            used[i] = 1;
            printPerms(perm, used, k + 1, n, validPermCount);
            used[i] = 0;
        }
    }
}


int main(void)
{
    int n, p;
    scanf("%d %d", &n, &p);

    // reads names and who has snacks
    for (int i = 0; i < n; i++) {
        scanf("%s %d", names[i], &hasSnacks[i]);
    }

    // initializes allowed[][] matrix to 1 (everyone can sit next to everyone)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            allowed[i][j] = 1;
        }
    }

    // reads the p conflict pairs and marks them as 0 in the matrix
    for (int k = 0; k < p; k++)
    {
        char nameA[MAX_LEN], nameB[MAX_LEN];
        scanf("%s %s", nameA, nameB);
        int indexA = nameIndex(nameA, n);
        int indexB = nameIndex(nameB, n);
        allowed[indexA][indexB] = 0;
        allowed[indexB][indexA] = 0;
    }

    int perm[SIZE];
    int used[SIZE];
    for (int i = 0; i < n; i++) {
        used[i] = 0;
    }
    int validPermCount = 0;
    printPerms(perm, used, 0, n, &validPermCount);

    printf("%d\n", validPermCount);

    return 0;
}
