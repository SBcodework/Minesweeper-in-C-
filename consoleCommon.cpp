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

#include "common.h"
#include "consoleCommon.h"
#include "Parameter.h"
#include "Gridtype.h"
#include "Cordtype.h"
#include <iostream>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <bitset>
#include <random>
#include <time.h>
#include <cmath>

int extractDigitInt( int digits, std::string input )
{
    int result = 0;
    int length = input.length();
    if ( length == 0 || digits < 0 )
    {
        return -1;
    }
    /// Find the furthest index of the string representing the smallest digit (under 10) as string length permits.
    int digitsFirstGuess = digits; //(length > digits) ? length : digits;
    int digitsSecondGuess = 0;  /// Below: find the first digit.
    for ( int i = 0; (i < digitsFirstGuess) && (isdigit(input[i])) && (i < length); i++ )
    {
        digitsSecondGuess++;
    }
    if ( digitsSecondGuess == 0 )
    {
        return -1;
    }

    for ( int i = (digitsSecondGuess - 1), magnitude = 0, inputDigit = 0, power = 0; i >= 0; i--, power++ )  /// Start backwards from the furthest index
    {
        inputDigit = (int)(input[i] - '0');
        magnitude  = (int)(std::pow(10.0, (float)power));
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
    int length   = in_line.length();

    if ( (length < 3) || (length > 5) )
    {
        return 2;  // Too long or too short; for legacy reasons 2 is the error code for user errors
    }

    int x = extractDigitInt( 2, in_line );
    int slicedStart = (x < 10) ? 2 : 3;

    if ( (x == -1) || (slicedStart > length) )
    {
        return 2;  // Invalid X or string is too short for a separator to fit
    }

    char separator = in_line[slicedStart - 1];

    if ( ( (separator != sep) && sep != '\0' ) || ( (sep == '\0') && isdigit(separator) ) )
    {
        return 2;  // An invalid separator is found when it is defined in the arguments, or if the separator is a digit when searching for one.
    }

    std::string slicedString = in_line.substr( slicedStart, length );
    int y = extractDigitInt( 2, slicedString );

    if ( y == -1 )
    {
        return 2;  // Invalid y
    }

    if ( out_sep != nullptr )
    {
        *out_sep = separator;
    }

    out_x = x;
    out_y = y;

    return 0;
}

void askSelectionXYTLoop( Cordtype& out_cord )
{
    // Gather input (h_extract_cords)
    // Output trimmed input to out_cord

    // Find the seperator, then run h_extractcords
    std::string line = "\0" ;
    int x = 0, y = 0 ;
    char sep = '1';


    for (int error = 1, loopError = 1; loopError ;)
    {
        std::cout << "\nSelect a point and an action. 's' to uncover, 'm' to middle click, and 'f' to flag. Like '13f7'. \n";
        std::cin >> line;

        error = h_extractCoords( line, x, y, &sep ) ;
        if ( error || (x < 0) || (y < 0) || (x > (out_cord.pParams->length)) || (y > (out_cord.pParams->height) ) )
        {
            std::cout << "Invalid input! Try again.\n";
            continue;
        }

        switch (sep)
        {
        case 's':

        case 'm':

        case 'f':
            loopError = 0;
            break;
        default:
            std::cout << "AskXYT Error, sep is invalid! Sep: " << sep << ". Try again.\n";
        }
    }

    out_cord.setter(x, y, sep) ;

    return ;
}
