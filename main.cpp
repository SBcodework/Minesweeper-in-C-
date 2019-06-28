#include <iostream>
#include <typeinfo>
#include "common.h"
#include "Parameter.h"
#include "Gridtype.h"
#include "Cordtype.h"

#include <string>


/// Test and check askXYminesloop() and it's helper functions
/// Edit to below: working version first
/// To-do: structally refactor project for clarity and simplictiy. Settle the recursive back-and-forth loop of 0's with the
/// selection of a single point.

/// Progress: working game acheived. To-do: input-saftey and changing the mineCanditates buffer to be
/// based on the runtime area of the start square rather than a constant 9 on gengrid() on "common.cpp".
/// Next, refactor.

//init()  /// Non-input init
//g_init() /// Wait-on-player init
//g_loop() ///


int main ( )
{
    int     endloop  = 1 ;
    for ( ; endloop == 1 ; )
    {
        Parameter        params   ;  // .X, .Y, .mines.
        askXYMinesLoop ( params ) ;  // returns to params
        dispEmpty      ( params ) ;
        askStartXYLoop ( params ) ;  // Output is params.
                                     // Above is clean, below to be implemented; remove unneeded params from gengrid(...)
        Gridtype grid  ( params ) ;  //.raw, .flags, .shown. Grid also keeps a reference to params.
        gengrid        ( grid   ) ;  // Output is grid argument
        int dbcounter    = 0      ;  ///debug

        for ( int i = 0 ; i < params.area ; i++ )
        {
                                                       //std::cout << grid.raw[i] << ",";
            if ( grid.raw[i] == 'X' )
            {
                 dbcounter++ ;
            }
        }

        std::cout << '\n' ;
        std::cout << "dbcounter, mines: " << dbcounter << " " << params.mines << '\n' ;

        Cordtype selection ( params ) ;
        for ( int endGame = 0 ; endGame == 0 ; )
        {
            grid.disp ( ) ;
            askSelectionXYTLoop    ( selection ) ; // Function also gives type modifier to selection. Output is selection.
            endGame = grid.action  ( selection ) ;
            if ( endGame )
            {
                grid.dispEnd ( selection ) ; // Disp here inside function
                break ;
            }
        }
        grid.free ( ) ;
        selection.free_static( ) ;

        std::cout << "Continue? 1 if so, 0 otherwise. ";
        std::cin >> endloop;
    }
    return 0 ;  /// Free memory!
}




