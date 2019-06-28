#include <iostream>
#include "common.h"
#include "Parameter.h"
#include "Gridtype.h"
#include "Cordtype.h"

Parameter::Parameter()
{
    //ctor
}

Parameter::~Parameter()
{
    //dtor
}

void Parameter::init_dimensions() // Call this once length and height are set.
    {
        area = length * height;
        return;
    }

void Parameter::init_start() // Call this once start coordinates are set.
    {
        iStart = (yStart*length) + xStart;
        return;
    }
