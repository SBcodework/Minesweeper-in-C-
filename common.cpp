#include <iostream>
#include "common.h"
#include "Parameter.h"
#include "Gridtype.h"
#include "Cordtype.h"
#include <string>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <bitset>
#include <random>
#include <time.h>
#include <cmath>

int extractTwoDigitInt( std::string input )
{
    int result = 0;
    int length = input.length();
    if ( length == 0 )
    {
        return -1;
    }
    char firstDigit = input[0];
    if ( !isdigit(firstDigit) )
    {
        return -1;
    }
    result += ((int)(firstDigit-'0')) * 10;
    if ( length > 1 && isdigit(input[1]) )
    {
        result += ((int)((input[1])-'0'));
    }

    if ( result < 0 || result > 99 )
    {
        return -1;
    }
    return result;
}

int extractDigitInt( int digits, std::string input )
{
    int result = 0;
    int length = input.length();
    if ( length == 0 || digits < 0 || digits > length )
    {
        return -1;
    }
    /// Find the furthest index of the string representing the smallest digit (under 10) as string length permits.
    int digitsFirstGuess = digits; //(length > digits) ? length : digits;
    int digitsSecondGuess = 0;  /// Below: find the first digit.
    for ( int i = 0; i < digitsFirstGuess || isdigit(input[i]); i++ )
    {
        digitsSecondGuess++;
    }
    if ( digitsSecondGuess == 0 )
    {
        return -1;
    }

    //float digit = digitsSecondGuess;
    for ( int i = (digitsSecondGuess - 1), magnitude = 0, inputDigit = 0; i >= 0; i--/*, digit--*/ )  /// Start backwards from the furthest index
    {
        inputDigit = (int)(input[i] - '0');
        magnitude  = (int)(std::pow(10.0, (float)i));
        result += (inputDigit * magnitude);
    }

    return result;
}

void askXYMinesLoop(Parameter& out_param)
{
    int x, y, mines, area; // Output containers
    std::string strx;
    std::string stry;
    std::string strMines;
    for (;;)
    {
        std::cout << "Grid Length? ";
        std::cin >> strx;
        std::cout << "Grid Height? ";
        std::cin >> stry;
        std::cout << "Number of mines? ";
        std::cin >> strMines;
        //x = extractTwoDigitInt(strx);
        //y = extractTwoDigitInt(stry);
        x = extractDigitInt(2, strx);
        y = extractDigitInt(2, stry);
        mines = extractDigitInt(4, strMines);  //4 digits

        if ( x < 1 || y < 1 || mines < 1 || x > 99 || y > 99 || mines > 9801 ||
            ( x < 4 && y < 4 ) || ( mines > ((x*y)-9) ) )
        {
            std::cout << "Invalid input! Try again.\n";
            continue;
        }

        out_param.length = x;
        out_param.height = y;
        out_param.mines = mines;
        out_param.init_dimensions();
        return;
        break;
    }
}

void dispEmpty(Parameter& in_param)
{
    int length = in_param.length;
    int height = in_param.height;
    std::cout << "     ";  // 5 spaces
    for (int i=0; i<length; i++)
    {
        std::cout << "{" << (i/10) << (i%10) << "}";
    }
    std::cout << "\n";
    for (int i=0; i<height; i++)
    {
        std::cout << "{" << (i/10) << (i%10) << "} ";
        for (int n=0; n<length; n++)
        {
            std::cout << "[  ]";
        }
        std::cout << "\n";
    }
    return;
}

int askStartXYLoop(Parameter& out_param)
{
    int error = 0, length = 0;
    std::string line;  // "cin.get()" Is not used, as it skips the first character of input.

    for (int skip = 0; ;skip = 0)
    {
        line = "";  // Reset the line each loop
        std::cout << "Enter X cord, followed by a comma, and Y cord. Like '13,8'. \n";
        std::cin >> line;
        length = line.length();
        //std::getline(std::cin, line); Old method, above worked better
        if ( (length < 3) || ((line[1] != ',') && (line[2] != ',')) || !isdigit(line[0]) ||
            (!isdigit(line[1]) && (line[1] != ',')) || (!isdigit(line[2]) && (line[2] != ',')) ||
            ((length > 3) && (line[2] == ',') && !isdigit(line[3]) ) )
        {
            std::cout << "Invalid! Try again. Line: " << line << "\n";
            continue;
        }
        error = h_extractCoords(line, out_param.xStart, out_param.yStart, nullptr, ',');
        if ((out_param.xStart >= out_param.length) || (out_param.yStart >= out_param.height))
        {
            std::cout << "Coordinates out of range! Try again. Line: " << line << "\n";
            continue;
        }

        switch (error)
        {
        case 0:
            break;
        case 1:
            std::cout << "Error in askStartXYLoop! ";
            std::cout << "x, y: " << out_param.xStart << " " << out_param.yStart << "\n";
            return 1;
        case 2:
            std::cout << "Invalid! Try again. Line: " << line << "\n";
            skip = 1;
        }
        if (skip)
        {
            continue;
        }
     out_param.init_start();
     return 0;
    }
}

int h_extractCoords(std::string in_line, int& out_x, int& out_y, char* out_sep, char sep)
{
    int x = 0, y = 0, length = 0; // Output types
    char chr = 'a'; // If we run this through the function returns 2 (user error)
    length = in_line.length() > 4 ? 5 : in_line.length();

    for(int i = 0, start = 1, * current = &x; i<length; i++) //Traverse the line.
    {
        chr = in_line[i];
        if ( (chr == ' ' && chr != sep && sep != NULL) || chr == '\n' || chr == '\0') // At end of line:
        {
            if (!start) // If the user's input for y is one digit:
            {
                y /= 10; // One digit input is multiplied by ten, so undo it.
            }
            break;
        }
        if ( (sep == NULL && !((int(chr) > 47) && (int(chr) < 58))) || chr == sep) // If a comma is found:
        {
            if (sep == NULL)
            {
                sep = chr;
                if ( out_sep != nullptr )
                {
                    *out_sep = sep;
                }
            }
            if (!start) // If x is one digit:
            {
                x /= 10;
                start = 1; // Reset "start" so "y" can use it.
            }
            current = &y; // Variable swap
            continue;
        }
        if (! ( (int(chr) > 47) && (int(chr) < 58 ) ) ) // Checking invalid input, using asci values for numbers
        {
            return 2; // User error
        }
        if (start) // If on the start of a coordinate:
        {
            *current += (chr-'0') * 10; // Multiply it by ten. REMEMBER: it's a pointer!
            start = 0; // If 0, we only have one digit.
            continue;
        }
        *current += chr-'0'; // We should have two digits. Add the final digit.
        start = 1;
    }
    if ( ( typeid(int) == typeid(x) ) && ( typeid(int) == typeid(y) ) &&  (x>=0) && (x<100) && (y>=0) && (y<100) )
        {
            out_x = x;
            out_y = y;
            return 0; // No error
        }
    std::cout << "Error in h_extractCoords!\n";
    std::cout << "in_line: " << in_line << "\n";
    std::cout << "sep: " << sep << "\n";
    std::cout << "x, y: " << x << " " << y << "\n";
    std::cout << "Coordcheck: " << (typeid(int) == typeid(x)) << (typeid(int) == typeid(y)) << (x>=0) << (x<100) << (y>=0) << (y<100) << "\n";
    return 1; // Error
}

/// Above is clean


void gengrid(Gridtype& out_grid)
{   // Parameters are declared below
    srand(time(0));  // Init randomizer

    Parameter* params = out_grid.pParams;
    int w = params->length;
    int h = params->height;
    int area = h*w;
    int xstart = params->xStart;
    int ystart = params->yStart;
    int istart = params->iStart; //Unneeded at the moment
    int mines = params->mines;

    char* rawGrid = new char[area];
    std::fill_n(rawGrid, area, '0');  // Fill with zeros

    int startSquare[9] {0}; // Indexes of the start square
    getSquare(xstart, ystart, w, h, startSquare);

    int clearCounter = 0;
    for (int i = 0, state = 0 ; i<9 ; i++) // Go through startSquare
    {
        state = startSquare[i]; /*
        if (state > -1 && state < area)
        {
            continue;
        }*/
        if (state != -1 && state < area) // -1 means an invalid spot
        {
            rawGrid[state] = 'c';
            clearCounter++;  // Count the number of valid mine-less spots
        }
    }

    int* mineList = new int[mines];
    std::fill_n(mineList, mines, 0);
    int mineLCounter = 0;

    int mineCandidates[mines + 9] {0};
    uniqueRand(mines+9, 0, area, mineCandidates); // mineCandidates now has a list of unique mines.

    for (int i = 0; ((i < mines+9) && (mineLCounter < mines)) ; i++)
    {
        if ((rawGrid[mineCandidates[i]] != 'c') && mineLCounter < (mines))
        {
            mineList[mineLCounter] = mineCandidates[i];
            rawGrid[mineList[mineLCounter]] = 'X';
            mineLCounter++; /// This needs to be here, not in the above for() declaration.
        }
    }

    for (int i = 0, state = 0 ; i<9 ; i++) // Go through startSquare, set back to 0's, we don't need 'c's anymore
    {
        state = startSquare[i];
        if (state != -1 && state < area) // -1 means an invalid spot
        {
            rawGrid[state] = '0';
        }
    }

    for (int i = 0, square[9] {0}, currentMine; i < mines; i++)
    {
        currentMine = mineList[i];
        getSquare(currentMine % 10, currentMine / 10, w, h, square); ///Watch in case of bug
        for (int n = 0, state = 0; n < 9; n++)
        {
            state = square[n];
            if ( state != -1 && rawGrid[state] != 'X' && rawGrid[state] >= '0' && rawGrid[state] < '8' )
            {
                rawGrid[state]++; // '0' goes to '1' and so on
            }
        }
    }

    out_grid.set_grid(rawGrid);  // Return
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
    /// The compiler may flag a warning here on reaching end of non-void function
}

void askSelectionXYTLoop( Cordtype& out_cord )
{
    // Gather input (h_extract_cords)
    // Output trimmed input to out_cord

    // Find the seperator, then run h_extractcords
    std::string line = "\0" ;
    int x = 0, y = 0 ;
    char sep = '\0'  ;

    //for (int length = 0, sepPos = 0, endPos = 0;;)
    //{
    for (int error = 1;;)
    {
        std::cout << "\nSelect a point and an action. 's' to uncover, 'm' to middle click, and 'f' to flag. Like '13f7'. \n";
        std::cin >> line;
        //length = line.length();

        //sepPos = (!isdigit(line[1])) ? ( !isdigit(line[2]) ? 0 : 2 ) : 1;
        //endPos = (sepPos == 1) ? 2 : 3;
        //break;
    //}
        error = h_extractCoords( line, x, y, &sep ) ;
        if ( error )
        {
            std::cout << "Invalid input! Try again.\n";
            continue;
        }
        break;
    }
    switch (sep)
    {
    case 's':
        break     ;
    case 'm':
        break     ;
    case 'f':
        break     ;
    default:
        std::cout << "AskXYT Error, sep is invalid! Sep: " << sep << '\n';
        break;
    }
    out_cord.setter(x, y, sep) ;
    return ;
}








