#ifndef CORDTYPE_H
#define CORDTYPE_H
#include "Cordtype.h"

class Cordtype
{
    public:
        Cordtype(Parameter& Params);
        virtual ~Cordtype();

        static Parameter* pParams;

        void setter(int in_x, int in_y, int in_mode = -1);
        int setter(int in_index, int in_mode = -1);
        int setter(Parameter& Params);

        int get_x();
        int get_y();
        int get_index();
        int get_mode();

        void free_static();

    protected:
        int x=0;
        int y=0;
        int index=0;
        int mode=-1;

};

#endif // CORDTYPE_H
