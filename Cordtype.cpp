#include <iostream>
#include "common.h"
#include "Parameter.h"
#include "Gridtype.h"
#include "Cordtype.h"
#include <typeinfo>

Cordtype::Cordtype(Parameter& Params)
{
    setter(Params);
}

Cordtype::~Cordtype()
{

}

Parameter* Cordtype::pParams = nullptr; /// Remember: "static" can not be here.

void Cordtype::setter(int in_x, int in_y, int in_mode)
{
    x = in_x;
    y = in_y;
    index = (in_y*(pParams->length)) + in_x;
    if (in_mode == -1)
    {
        return;
    }
    mode = in_mode;
    return;
}

int Cordtype::setter(int in_index, int in_mode)
{
    index = in_index;
    if (pParams == nullptr)
    {
        std::cout << "Error: Cordtype::setter(int in_index)'s pParams is null!\n";
        return 1;
    }
    x = in_index % (pParams->length);
    y = in_index % (pParams->height);
    if (in_mode == -1)
    {
        return 0;
    }
    mode = in_mode;
    return 0;
}

int Cordtype::setter(Parameter& Params)
{
    if (typeid(Params) != typeid(Parameter) )
    {
        std::cout << "Error: Cordtype::setter(Parameter& Params)'s input is invalid!\n";
        return 1;
    }
    pParams = &Params;
    return 0;
}

int Cordtype::get_x()
{
    return x;
}

int Cordtype::get_y()
{
    return y;
}

int Cordtype::get_index()
{
    return index;
}

int Cordtype::get_mode()
{
    return mode;
}

void Cordtype::free_static()
{
    if (pParams == nullptr)
    {
        return;
    }
    delete pParams;
    pParams = nullptr;
    return;
}
