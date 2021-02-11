#pragma once
#include "math.h"
#define GRAVITY -9.8f

class Vector{
    double x,y,z;

    public:
        Vector();
        Vector(double,double,double);

        double getVectorX(){return x;}
        double getVectorY(){return y;}
        double getVectorZ(){return z;}

        void setVectorX(float a){x=a;}
        void setVectorY(float a){y=a;}
        void setVectorZ(float a){z=a;}

        double getMagnitude(){
            return sqrt((x*x) + (y*y) + (z*z));
        }

        Vector getSelf()
        {
            return Vector(x,y,z);
        }

        Vector getNormalized()
        {
            double magnitude = sqrt((x*x) + (y*y) + (z*z));
            return Vector(x/magnitude,y/magnitude,z/magnitude);
        }

        Vector getNegative()
        {
            return Vector(-x,-y,-z);
        }

        double getDotProductWith(Vector v)
        {
            return x*v.getVectorX() + y*v.getVectorY() + z*v.getVectorZ();
        }

        Vector getCrossProductWith(Vector v)
        {
            return Vector(y*v.getVectorZ() - z*v.getVectorY(), z*v.getVectorX() - x*v.getVectorZ(),x*v.getVectorY() - y*v.getVectorX());
        }

        Vector vectorAdd(Vector v)
        {
            return Vector(x+v.getVectorX(), y + v.getVectorY(), z + v.getVectorZ());
        }

        Vector scalarMult(double s)
        {
            return Vector(x * s, y * s, z * s);
        }

        //This function expects a normal that is normalised 
        Vector getRelectionWith(Vector normal)
        {
            double dotProduct = normal.getDotProductWith(getNegative()); 
            Vector projectedOntoNormal = normal.scalarMult(dotProduct);  //Projection of view direction on normal
            Vector halfwayVector = projectedOntoNormal.vectorAdd(getSelf());  //adding the intersection dir is the same as minusing the negative dir (going from the intersectionpos) 
            Vector doubleHalfwayVector = halfwayVector.scalarMult(2); //Need to double the vector because otherwise minusing it would just get the projected N
            Vector unNormalisedReflection = doubleHalfwayVector.vectorAdd(getSelf().getNegative()); 
            Vector reflection = unNormalisedReflection.getNormalized();
            return reflection;
        }
};


Vector::Vector(double i, double j, double k)
{
    x=i;
    y=j;
    z=k;
}


Vector::Vector()
{
    x=0;
    y=0;
    z=0;
}







