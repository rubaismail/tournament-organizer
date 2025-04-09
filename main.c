/* COP 3502C Assignment 4
* This program is written by: Ruba Ismail
* UCF ID: 5186181
* Date: 3/09/2025
* Class: COP 3205C, Professor Tanvir
* Purpose: sort stadium coordinates using merge sort and insert sort
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int x;
int y;

typedef struct coordinates{
    int x;
    int y;
}coord;


// creates an array of coordinates structs that stores the coordinate points of stadium locations
coord* ReadData(int numStadiums)
{
    coord *stadiums = malloc(sizeof(coord) * numStadiums);

    for(int i = 0; i < numStadiums; i++)
    {
        scanf("%d", &stadiums[i].x);
        scanf("%d", &stadiums[i].y);
    }

    return stadiums;
}

// finds the point with closer distance to the coordinates
int compareTo(coord* point1, coord* point2)
{
    // returns 0 if the two coordinate points are identical
    if(point1->x == point2->x) {
        if(point1->y == point2->y) {
            return 0;
        }
    }

    // calculates distance from point 1
    int diff1x = pow(point1->x - x, 2);
    int diff1y = pow(point1->y - y, 2);
    int distance1 = diff1x + diff1y;

    // calculates distance from point 2
    int diff2x = pow(point2->x - x, 2);
    int diff2y = pow(point2->y - y, 2);
    int distance2 = diff2x + diff2y;

    // returns negative if point1 is closer, positive if point2 is closer
    if(distance1 < distance2)
        return -1;

    else if(distance1 > distance2)
        return 1;

    // the points are equal distance from the point but are different coordinates
    else {
        if (point1->x < point2->x)
            return -1;
        else if(point1->x > point2->x)
            return 1;
        else if (point1->y < point2->y)
            return -1;
        else
            return 1;
    }
}


// merges the sorted sub-arrays from mergeSort
void merge(coord *stadiums, int l, int mid, int r) {
    int sizeL = mid - l + 1;
    int sizeR = r - mid;

    coord *L = malloc(sizeof(coord) * sizeL);
    coord *R = malloc(sizeof(coord) * sizeR);

    for (int i = 0; i < sizeL; i++)
        L[i] = stadiums[l + i];

    for (int j = 0; j < sizeR; j++)
        R[j] = stadiums[(mid + 1) + j];

    int i = 0, j = 0, k = l;

    while (i < sizeL && j < sizeR)
    {
        if (compareTo(&L[i], &R[j]) <= 0) {
            stadiums[k++] = L[i++];
        }
        else {
            stadiums[k++] = R[j++];
        }
    }

    while (i < sizeL) {
        stadiums[k++] = L[i++];
    }

    while (j < sizeR) {
        stadiums[k++] = R[j++];
    }

    free(L);
    free(R);
}

// insertion sort for sub-arrays smaller than the threshold
void insertionSort(coord *stadiums, int l, int r)
{
    int i, j;
    for(i = (l + 1); i <= r; i++)
    {
        coord hand = stadiums[i];
        for(j = i-1; j>=l; j--)
        {
            if(compareTo(&stadiums[j], &hand) >= 0)
                stadiums[j+1] = stadiums[j];

            else
                break;
        }
        stadiums[j+1] = hand;
    }
}

// uses mergeSort if array is bigger than the threshold, on sub-arrays smaller it uses insertion sort
void mergeSort(coord *stadiums, int l, int r, int threshold)
{
    if(l < r)
    {
        if ((r - l + 1) <= threshold)
            insertionSort(stadiums, l, r);

        else {
            int mid = (l+r)/2;

            mergeSort(stadiums, l, mid, threshold);

            mergeSort(stadiums, mid+1, r, threshold);

            merge(stadiums, l, mid, r);
        }
    }
}

// calls mergeSort on the entire array of stadiums
void wrapperSortStadiums(coord *stadiums, int numStadiums, int threshold) {
    mergeSort(stadiums, 0, numStadiums-1, threshold);
}

// searches for a specific stadium coordinates in the sorted stadium list
int binarySearch(coord *stadiums, coord query, int numStadiums)
{
    int left = 0, right = numStadiums-1, mid;

    while(left <= right){

        mid = (left + right) / 2;

        if(compareTo(&stadiums[mid], &query) == 0)
            return mid;

        else if(compareTo(&stadiums[mid], &query) < 0)
            left = mid + 1;

        else
            right = mid - 1;

    }
    return -1;
}


int main(void)
{

    scanf("%d", &x);
    scanf("%d", &y);

    int numStadiums;
    scanf("%d", &numStadiums);

    int numQueries;
    scanf("%d", &numQueries);

    int threshold;
    scanf("%d", &threshold);

    coord *stadiums = ReadData(numStadiums);

    wrapperSortStadiums(stadiums, numStadiums, threshold);

    // prints the sorted stadium coordinates form the sorted list
    for (int i = 0; i < numStadiums; i++) {
        printf("%d %d\n", stadiums[i].x, stadiums[i].y);
    }

    // processing query points
    for (int i = 0; i < numQueries; i++)
        {
        coord query;
        scanf("%d %d", &query.x, &query.y);

        int index = binarySearch(stadiums, query, numStadiums);
        if (index == -1)
            printf("%d %d: no stadium found at this location\n", query.x, query.y);
        else
            printf("%d %d: stadium found at rank %d in the sorted order\n", query.x, query.y, index + 1);
    }

    free(stadiums);

    return 0;
}
