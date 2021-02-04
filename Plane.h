#pragma once

#include "Vector.h"
#include "Color.h"
#include "Object.h"


class Plane : public Object{
    Vector normal;
    double distanceFromOrigin;
    Color color;

    public:
        Plane();
        Plane(Vector,double,Color);
        void setAll(Vector normal,double distance, Color col){
            this->normal = normal;
            distanceFromOrigin = distance;
            color = col;
        }

        Vector getPlaneNormal(){return normal;}
        double getPlaneDistance(){return distanceFromOrigin;}
        Color getColor(){return color;}

        Vector getNormalAt(Vector point){
            return normal;
        }

        double findIntersection(Ray ray){
            Vector ray_direction = ray.getRayDirection();
            double a = ray_direction.getDotProductWith(normal);

            if(a == 0)
            {
                //If ray is paralel to the plane
                return -1; //No intersection
            }
            else
            {  
                //You sure this is right
                double b = normal.getDotProductWith(ray.getRayOrigin().vectorAdd(normal.scalarMult(distanceFromOrigin).getNegative()));
                
                return -1 * b/a;
            }

        }


};


Plane::Plane()
: normal(Vector(1,0,0)),distanceFromOrigin(0.0), color(Color(1,0,0,1))
{
}
Plane::Plane(Vector n,double d, Color c)
:normal(n),distanceFromOrigin(d),color(c)
{
}
