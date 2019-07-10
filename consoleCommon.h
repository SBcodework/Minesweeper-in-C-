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

#ifndef CONSOLECOMMON_H_INCLUDED
#define CONSOLECOMMON_H_INCLUDED

#include "Parameter.h"
#include "Cordtype.h"
#include "Gridtype.h"
#include <string>

int extractDigitInt( int digits, std::string input );

void askXYMinesLoop(Parameter& out_param);  // Gets a valid inquiry into the length, width, and mines. Outputs to Parameter object.

void dispEmpty(Parameter& in_param);   // Displays an empty grid.

int askStartXYLoop(Parameter& out_param);  // Gets a valid inquiry into the start coordinates. Outputs to Parameter object.

int h_extractCoords(std::string in_line, int& out_x, int& out_y, char* out_sep = nullptr, char sep = '\0' );     // Helper function to extract coords from a string like "31#5", where '#' is the separator.
                                                                          // Returns 1 if error, 0 if not.
                                                                          /// If sep is null, the function scans for a separator. Outsep, when not a nullptr,
                                                                          /// outputs the non-digit separator that is found.
void askSelectionXYTLoop(Cordtype& out_cord);    // Gets a valid inquiry into the X cord, Y cord, and the type of action.
                                           // Outputs to Cordtype, including the type of action. The grid is used for verification.


#endif // CONSOLECOMMON_H_INCLUDED

