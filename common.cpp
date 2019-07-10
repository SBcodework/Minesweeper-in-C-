/**
BSD 2-Clause License

Copyright (c) 2019, SBcodework
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#include <iostream>
#include <algorithm>
#include <vector>
#include <bitset>
#include <random>
#include <time.h>
#include <cmath>
#include "common.h"
#include "consoleCommon.h"
#include "Parameter.h"
#include "Gridtype.h"
#include "Cordtype.h"


/** Change-log:
 To-do:

For now, move functions that strongly require or are used for the iosteam/console IO into "consoleCommon".
"common" should contain functions that work with the GUI back end model.
Segment long functions into helper functions.
Replace console error reporting with something else.
Replace the unique RNG number generator.

 What was done:

 Split part of gengrid(...) into dedicated helper functions for code quality, receptive to feedback.
 Removed redundant comments and added insightful ones.
 Depreciated useless include directives.
 Moved the functions that belong to the console application to "consoleCommon.h".

**/

// Helper functions unique to each function, implementation at bottom of file
inline int getSafeSpotsInStartSquare_gengrid(int startSquare[9], char* grid);
inline void constructNumberlessGrid_gengrid(int* mineList, int mines, char* grid, int gridArea);
inline void insertMineNumbers_gengrid(int* mineList, int mineCount, int* grid, int gridW, int gridH);

void gengrid(Gridtype& out_grid)
{
    srand(time(0));

    Parameter* params = out_grid.pParams;
    int w = params->length;
    int h = params->height;
    int area = h*w;
    int xstart = params->xStart;
    int ystart = params->yStart;
    int istart = params->iStart;
    int mines = params->mines;

    char* rawGrid = new char[area];
    int   startSquare[9] {0};         // The area around the start location. There should be no mines here.
    int*  mineList = new int[mines];  // Indexed positions of mines.

    int clearCounter = getSafeSpotsInStartSquare_gengrid(startSquare, xstart, ystart, rawGrid, w, h);  // Get the area of the startSquare and designate safe spots on the grid.
    // clearCounter is currently unused, but it may be useful when mineCandidates of the function below needs to have an accurate count/size of how many mines are in the start
    // square (instead of a fixed size of 9 for mineCandidates).

    constructNumberlessGrid_gengrid(startSquare, mineList, mines, rawGrid, area); // Construct a grid without mine numbers, but with mines.

    insertMineNumbers_gengrid(mineList, mines, rawGrid, w, h);

    out_grid.set_grid(rawGrid);
    out_grid.set_minelist(mineList);
    out_grid.action( istart, 's' );

    return;
}

void getSquare(int centerX, int centerY, int w, int h, int out_square[9])
{
    int xpart = 0;
    int ypart = 0;
    int startX = centerX - 1;
    int startY = centerY - 1;

    for (int iy = 0, counter = 0; iy < 3 ; iy++)
    {
        for (int ix = 0; ix < 3; ix++, counter++)
        {
            xpart = startX + ix;
            ypart = startY + iy;
            out_square[counter] = toIndexCheck(xpart, ypart, w, h);
        }
    }
    return;
}

int toIndexCheck(int x, int y, int w, int h)
{
    if (outOfBounds(x, y, w, h))
    {
        return -1;
    }
    return ((y*w) + x );
}

int outOfBounds(int x, int y, int w, int h)
{
    if (x >= 0 && y >= 0 && x < w && y <h)
    {
        return 0;
    }
    return 1;
}

// Numlimit is exclusive. Output must be of size "size".
int uniqueRand(int size, int start, int numlimit, int* output)
{
    /// init srand
    std::vector<bool> bvector(numlimit, 0);

    int currentStart = 0;
    int currentEnd = 0; // Inclusive
    int currentMiddle = 0; // Exclusive; the inclusive start of the second sequence.
    int currentSize = 0;

    for (int counter = 0; counter < size; counter++)
    {
        currentStart = start;
        currentEnd = numlimit;
        currentMiddle = (numlimit / 2); // Seperator at right. -1 used to be here
        currentSize = numlimit - start; /// changed from numlimit

        for (int state = 0, choice = 0 ; ; )
        {
            if (currentStart == (currentEnd-1))
            {
                if (bvector[currentStart] == 1)
                {
                    std::cout << "Duplicate error!\n";
                }
                output[counter] = currentStart; // We have found our number
                bvector[currentStart] = true;
                break;
            }

            state = allCheck(currentStart, currentMiddle, currentEnd, bvector);

            switch (state)
            {
                case 0: // Choose a random element. 0 = left, 1 = right.
                choice = (rand() % 2);
                break;

                case 1: // Choose left
                choice = 0;
                break;

                case 2: // Choose right
                choice = 1;
                break;

                case 3:
                std::cout << "Array full! Size: " << currentSize << " Start: " << currentStart << " Middle: " << currentMiddle << " End: " << currentEnd << "\n";
                for ( int n=0; n<size; n++)
                {
                    std::cout << output[n];
                }
                std::cout << "\n";
                return 1;

            }

            switch (choice)
            {
                case 0:
                currentEnd = currentMiddle;
                currentSize = currentEnd - currentStart; // removed +1
                currentMiddle = currentStart + (currentSize / 2); // removed -1, added currentstart
                break;

                case 1:
                currentStart = currentMiddle; //removed +1
                currentSize = currentEnd - currentStart; //removed +1
                currentMiddle = currentStart + (currentSize / 2); //removed -1
                break;
            }
        }
    }
    return 0;
}
// OLD documentation:
// Include start and end. Middle refers to the seperatation RIGHT of the index. It is added one locally, same with end.
// Return 0 if neither left or right is all 1's. 1 if only the left one has a zero in it, 2 if it's right, and 3 for both.
// New documentation: Include start, in_middle includes the start of the second sequence, in_end is exclusive.
int allCheck(int start, int middle, int end, std::vector<bool>& in_vector)
{
    int allLeft = 1; // 1 meaning all ones are in the bit array.
    int allRight = 1;
    ///int middle = in_middle + 1; // LEFT of index; testing
    //int end = in_end + 1; // Exclusive
    //int middle = (end - start) / 2;
    for (int currentStart = start, currentEnd = middle, *flag = &allLeft, t = 0;
         t < 2;
         t++, currentStart = middle, currentEnd = end, flag = &allRight) // Two times, go through left and right

    {
        for (int i = currentStart , size = currentEnd - currentStart; i < currentEnd; i++ ) // Commented out portion was unused
        {
            if (in_vector[i] == false) // Remember to init the vector with zeroes, else seg fault here
            {
                *flag = 0;  // There's a zero in the bit array.
                break;
            }
        }
    }

    switch (allLeft + allRight)
    {
        case 0: // Neither are ones
            return 0;

        case 1:
            if (allLeft) // Right only has no ones
                {
                    return 2;
                }
                return 1; // Left has no ones

        case 2:
            std::cout << "Sub-array full! Dump: \n";
            for ( int n=0; n<(end-start); n++)
            {
                std::cout << in_vector[n];
            }
            std::cout << "\n";
            return 3; // Both have ones
    }
    std::cout << "Error in allCheck(...)!\n";  // Shouldn't reach here, added to avoid compiler warning
    return 0;
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Dedicated Helper Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~///

inline int getSafeSpotsInStartSquare_gengrid(int startSquare[9], int xStart, int yStart, char* grid, int gridW, int gridH)
{
    int area = gridW * gridH;
    std::fill_n(grid, area, '0');
    getSquare(xStart, yStart, gridW, gridH, startSquare);

    int clearCounter = 0;
    for (int i = 0, state = 0 ; i<9 ; i++) // Go through startSquare
    {
        state = startSquare[i];
        if (state != -1 && state < area) // -1 means an invalid spot
        {
            grid[state] = 'c';
            clearCounter++;  // Count the number of valid mine-less spots
        }
    }

    return clearCounter;
}

inline void constructNumberlessGrid_gengrid(int* startSquare, int* mineList, int mines, char* grid, int gridArea)
{
    std::fill_n(mineList, mines, 0);
    int mineCandidates[mines + 9] {0};  // The extra 9 mines are in case we select a mine that is in the restricted startSquare area.

    uniqueRand(mines + 9, 0, gridArea, mineCandidates);  // mineCandidates now has a list of unique mines.

    for (int i = 0, mineLCounter = 0, *minePos = 0; ((i < mines+9) && (mineLCounter < mines)) ; i++)  // MineLCounter is to determine when enough mines are filled in and
    {                                                                                                    // to iterate through the array.

        if ( (grid[mineCandidates[i]]) != 'c' )
        {
            minePos  = &(mineList[mineLCounter]);

            *minePos = mineCandidates[i];
            grid[*minePos] = 'X';
            mineLCounter++;
            continue;
        }
    }

    for (int i = 0, state = 0 ; i<9 ; i++) // Go through startSquare, set back to 0's, we don't need 'c's anymore
    {
        state = startSquare[i];
        if (state != -1 && state < gridArea) // -1 means an invalid spot
        {
            grid[state] = '0';
        }
    }

}

inline void insertMineNumbers_gengrid(int* mineList, int mineCount, char* grid, int gridW, int gridH)
{
    for (int i = 0, square[9] {0}, currentMine; i < mineCount; i++)  // Count numbers and put them in
    {
        currentMine = mineList[i];
        getSquare(currentMine % gridW, currentMine / gridW, gridW, gridH, square); ///Watch in case of bug
        for (int n = 0, state = 0; n < 9; n++)
        {
            state = square[n];
            if ( state != -1 && grid[state] != 'X' && grid[state] >= '0' && grid[state] < '8' )
            {
                grid[state]++; // '0' goes to '1' and so on
            }
        }
    }

    return;
}
