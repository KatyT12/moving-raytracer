#pragma once

#include "World.h"
#include "olcPixelGameEngine.h"


class Renderer
{
    public: 
        World world;
        int width;
        int height;
        double aspectRatio;
        double accuracy;

        Color getColorAt(Vector intersectionPos,Vector intersectionDir,std::vector<Object*> worldObjects,int indexOfWinningObjects,std::vector<Source*>worldLights,double accuracy,double ambientLight);
        int winningObjectIndex(std::vector<double> intersections);
    

   


    void getCentrePixels(double &xamnt,double &yamnt,int x, int y)
    {
          if(width > height)
          {
              //The image is wider than it is tall
              xamnt = ((x + 0.5)/width) * aspectRatio - (((width - height)/ (double) height)/2);
              yamnt = ((height - y) + 0.5)/height;
          }
          else if (height >  width)
          {
              //The image is taller than it is wide
              xamnt = (x + 0.5)/width;
              yamnt = (((height - y) + 0.5)/height)/aspectRatio - (((height - width)/ (double) width)/2);
          }
          else
          {
              
              //The image is square
              xamnt = (x + 0.5)/width;
              yamnt = ((height-y) + 0.5)/height;
          }
    }




    Renderer(World& world, const int width, const int height)
    :width(width), height(height), world(world), accuracy(0.000001)
    {
        aspectRatio = (double)width/(double)height;        
    }
    Renderer(){}
    void setAll(World& world, const int width, const int height)
    {
        this->width = width;
        this->height = height;
        this->world = world;
        this-> accuracy = 0.000001;
        aspectRatio = (double)width/(double)height;
    }

    
   
    olc::Pixel* render()
    {
        olc::Pixel* pixels = new olc::Pixel[width * height];
        double xamnt, yamnt;

        for(int x =0; x < width; x++)
        {
            for(int y = 0; y < height;y++)
            {

                
                int thisone = y*width + x;


                double xamnt;
                double yamnt;

                getCentrePixels(xamnt,yamnt,x,y);             
            
                Camera scene_cam = *world.cam;

                Vector cameraRayOrigin = scene_cam.getCameraPosition();
                /*Direction of camera rays throught the view plane*/
                Vector cameraRayDirection = scene_cam.getCameraDirection().vectorAdd(scene_cam.getCameraRight().scalarMult(xamnt - 0.5).vectorAdd(scene_cam.getCameraDown().scalarMult(yamnt - 0.5))).getNormalized();


                Ray cameraRay(cameraRayOrigin,cameraRayDirection);
                std::vector<double> intersections = world.findIntersections(cameraRay);

                int indexOfWinningObjects = winningObjectIndex(intersections);


                if(indexOfWinningObjects == -1) //No intersection
                {
                    Color bg_col = world.getBackgroundColor();
                    pixels[thisone] = olc::PixelF(bg_col.getColorRed(),bg_col.getColorGreen(),bg_col.getColorBlue());
                }
                else{
                    double this_intersection = intersections[indexOfWinningObjects];
                    if(this_intersection > accuracy){ //If intersecion is greater than accuracy
                        Vector intersectionPos  = cameraRayOrigin.vectorAdd(cameraRayDirection.scalarMult(this_intersection)); 
                        Vector intersectionDir = cameraRayDirection;

                        Color intersectionCol = getColorAt(intersectionPos,intersectionDir,world.worldObjects,indexOfWinningObjects,world.worldLights,accuracy,world.getAmbientLight());
                        pixels[thisone] = olc::PixelF(intersectionCol.getColorRed(),intersectionCol.getColorGreen(),intersectionCol.getColorBlue());
                    } 
                    
                    
                    
                    

                }
            }
        }

        return pixels;
    }



};




 Color Renderer::getColorAt(Vector intersectionPos,Vector intersectionDir,std::vector<Object*> worldObjects,int indexOfWinningObjects,std::vector<Source*>worldLights,double accuracy,double ambientLight)
    {
    Vector intersectionNormal = worldObjects[indexOfWinningObjects]->getNormalAt(intersectionPos);

    Color winningObjCol = worldObjects[indexOfWinningObjects]->getColor();

        
    if(winningObjCol.getColorSpecial() == 2)
    {
        winningObjCol.tiling(intersectionPos);
    }
    
    Color final_color = winningObjCol.scale(ambientLight); //If in shadow then it will just have the ambient


    //For every light get the color it gives the winning object
    for(int l_index = 0; l_index < worldLights.size();l_index++)
    {
        
        Vector lightdir = worldLights[l_index]->getLightPosition().vectorAdd(intersectionPos.getNegative()).getNormalized(); //Vector from intersection to light
        float dotProduct = intersectionNormal.getDotProductWith(lightdir);

        //90 degrees or less different from the normal
        if(dotProduct > 0) 
        { 
            bool inShadow = false; //Not in shadow by default
            float distanceToLight = worldLights[l_index]->getLightPosition().vectorAdd(intersectionPos.getNegative()).getMagnitude();

            Ray shadowRay(intersectionPos,(worldLights[l_index]->getLightPosition().vectorAdd(intersectionPos.getNegative())).getNormalized()); 

            std::vector<double> shadowIntersections;
            /*If there is an intersection between the first intersection and the light source the the first intersection is in shadow*/
            for(int obj_index = 0; obj_index < worldObjects.size() && inShadow == false; obj_index++)
            {
                if(obj_index != indexOfWinningObjects)
                {
                    shadowIntersections.push_back(worldObjects[obj_index]->findIntersection(shadowRay)); 
                }
            }
            for(int s_i_index = 0; s_i_index < shadowIntersections.size();s_i_index++)
            {
                if(shadowIntersections[s_i_index] > accuracy)
                {
                    if(shadowIntersections[s_i_index] <= distanceToLight)
                    {
                        inShadow = true;
                    }
                }
            }
   
            if(inShadow == false)
            {
                final_color = final_color.addColor(winningObjCol.multiplyColor(worldLights[l_index]->getColor().scale(dotProduct))); //diffuse color

                if(winningObjCol.getColorSpecial() > 0 && winningObjCol.getColorSpecial() <=1)
                {

                    /* A poor attemp at using the blinn phong model, no matter what power I gave it the specular reflection would just look like a white dot
                    Vector halfwayVector = (intersectionDir.getNegative().vectorAdd((worldLights[l_index]->getLightPosition().vectorAdd(intersectionPos.getNegative())).getNormalized())).getNormalized();
                    float dot = intersectionNormal.getDotProductWith(halfwayVector);
                    if(dot > 0)
                    {
                        final_color = final_color.addColor(winningObjCol.multiplyColor(worldLights[l_index]->getColor().scale(pow(dot,100))));
                    }
                    */

                   //Get reflection of view direction from normal
                   Vector reflection = intersectionDir.getRelectionWith(intersectionNormal);

                   //specular
                   double spec = reflection.getDotProductWith(lightdir);
                   if(spec >0)
                   {
                       spec = pow(spec,10);
                       final_color = final_color.addColor(worldLights[l_index]->getColor().scale(spec * winningObjCol.getColorSpecial()));
                   }
                }

                final_color.Clamp();
            }            
        }
    
    }
    return final_color;

}



int Renderer::winningObjectIndex(std::vector<double> intersections)
{
    //Return the index of the winning intersection
    int iMinVal;

    //prevent unecessary calculations
    
    if(intersections.size() == 0)
    {   
        return  -1; //No intersections
    }
    
    else if(intersections.size() == 1)
    {
        if(intersections[0] >  0) //If Greater than zero it is in the right range
        {
            return 0;            
        }
        else{
            return -1;
        }

    }

    //More than one intersection
    else{
        double max = 0;
        //Find max value
        for(int i =0; i < intersections.size(); i++)
        {
             if(max < intersections[i]){
                 max = intersections[i];
             }
        }
        
        if(max > 0) 
        {
            for (int i = 0; i < intersections.size(); i++)
            {
                if(intersections[i] > 0 && intersections[i] <= max) // Want the smallest value (above 0) for the winning intersection
                {   
                    max = intersections[i];
                    iMinVal = i;
                }
            }
            return iMinVal;
        }
        else {
        return -1;}

    }

}