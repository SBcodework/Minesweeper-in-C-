#ifndef GRIDTYPE_H
#define GRIDTYPE_H
#include <vector>
#include "Cordtype.h"
#include "Parameter.h"

class Gridtype /// "get state" functions to be implemented
{
    public:
        Parameter* pParams = nullptr;

        Gridtype(Parameter& in_param);
        virtual ~Gridtype();

        void initSet_grid(char* in_allocatedArray); // Sets .raw pointer to argument.
                                         /// Argument must be dynamically allocated (new keyword, etc).

        char get_state(Cordtype in_cords); // Returns the state at the coordinate.
        char get_state(int in_x, int in_y); // See above. Converts arguments to a temporary Cordtype object.
                                           // Then calls above function.
        char get_state(int in_index); // See above.

        void set_grid(char* chptr);

        void set_minelist(int* nptr);

        void disp();
        void dispEnd( Cordtype& lastPoint );
        void rawDisp();

        void free();

        int action( Cordtype& point );
        int action( int index, char mode );

        int singleClick( int index, int recursive = 0 );
        int flag( int index );
        int middleClick( int index );

        int checkWin();  /// 0, no win. 1, win.

        int flagNumber = 0 ;
        int hasWon     = 0 ;
        char* raw                  = nullptr ; // The grid itself.
        int*  mineList             = nullptr ;
        std::vector<bool> flags   ;
        std::vector<bool> visible ;
        //std::vector<bool> mineVector ; depreciated
};

#endif // GRIDTYPE_H
