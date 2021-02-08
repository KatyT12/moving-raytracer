#pragma once

#include "Ray.h"
#include "Vector.h"
#include "Color.h"

enum objectType{
    PLANE,
    SPHERE
};

class Object{

    public:
        objectType type;

        Object();

        virtual Color getColor(){return Color(0.0,0.0,0.0,0.0);}
        virtual double findIntersection(Ray ray){ //Virtual because it is necessary for dynamic casting
            return 0;
        }

        virtual Vector getNormalAt(Vector point){return Vector(0,1,0);}
   

};

Object::Object(){}

