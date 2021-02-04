#pragma once
#include "math.h"

class Color{
    double red,green,blue,special;

    public:
        Color();
        Color(double,double,double,double);

        double getColorRed(){return red;}
        double getColorGreen(){return green;}
        double getColorBlue(){return blue;}
        double getColorSpecial(){return special;}

        void setColorRed(double redVal){ red = redVal;}
        void setColorGreen(double greenVal){green = greenVal;}
        void setColorBlue(double blueVal){blue = blueVal;}
        void setColorSpecial(double specialVal){special = specialVal;}

        double getBrightness(){
            return (red + green + blue)/3;
        }

        Color scale(double val)
        {
            return Color (red*val, green*val,blue*val,special);
        }


        Color addColor(Color color)
        {
            return Color(red+color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(),special);
        }
        Color multiplyColor(Color color)
        {
            return Color(red*color.getColorRed(),green * color.getColorGreen(), blue * color.getColorBlue(),special);
        }

        Color colorAverage(Color color)
        {
            return Color((red*color.getColorRed())/2,(green + color.getColorGreen()/2), (blue * color.getColorBlue()/2),special);
        }

        void Clamp(double min = 0.0, double max = 1.0){
            if(red < min) red = min;
            if(green < min) green = min;
            if(blue < min) blue = min;

            if(red > max) red = max;
            if(green > max) green = max;
            if(blue > max) blue = max;
        }



        void tiling(Vector position)
        {
            int square = floor(position.getVectorX()) + floor(position.getVectorZ());
            if(square % 2 ==0)
            {
                setColorRed(0);
                setColorGreen(0);
                setColorBlue(0);
            }
            else{
                setColorRed(1);
                setColorGreen(1);
                setColorBlue(1);
            }
        }

};


Color::Color(double r, double g, double b, double a)
:red(r), green(g), blue(b), special(a)
{
} 


Color::Color()
:red(0),green(0),blue(0),special(1)
{
}





