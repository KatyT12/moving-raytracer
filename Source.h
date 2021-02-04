#pragma once

#include "Vector.h"
#include "Color.h"



class Source{
    public:

    Source();
    virtual Vector getLightPosition(){
        return Vector(0,0,0);
    }

    virtual Color getColor(){
        return Color(1,1,1,0);
    } 
};


Source::Source(){
    
}