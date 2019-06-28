#ifndef PARAMETER_H
#define PARAMETER_H

/// Remember to call the "init_...." functions as needed.

class Parameter
{
    public:
        Parameter();
        virtual ~Parameter();

        int length=0;
        int height=0;
        int mines=0;
        int xStart=0;
        int yStart=0;

        int area=0;
        int iStart=0;

        void init_dimensions(); /// Call this once length and height are set.
        void init_start(); /// Call this once start coordinates are set.

    protected:

};

#endif // PARAMETER_H
