#ifndef CORDTYPE_H
#define CORDTYPE_H
#include "Cordtype.h"

class Cordtype
{
    public:
        Cordtype(Parameter& Params);
        virtual ~Cordtype();

        static Parameter* pParams;

        void setter(int in_x, int in_y, char in_mode = '\0');
        int setter(int in_index, int in_mode = '\0');
        int setter(Parameter& Params);

        int get_x();
        int get_y();
        int get_index();
        char get_mode();

        void free_static();

    protected:
        int x = 0;
        int y = 0;
        int index = 0;
        char mode = '\0';

};

#endif // CORDTYPE_H
