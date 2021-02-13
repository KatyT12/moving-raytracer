#pragma once

#include "Vector.h"
#include "Color.h"
#include "Object.h"
#include "Ray.h"
#include "Source.h"
#include <vector>
#include "Camera.h"
#include "Sphere.h"

#define CUT_DISTANCE 150

class World{
    private:
        Vector O;
        Vector X;
        Vector Y;
        Vector Z;

        Color backgroundColor;
        double ambientLight;

        clock_t time1, time2;
    public:
        std::vector<Object*>worldObjects;
        std::vector<Source*>worldLights;
        std::vector<Plane*> planes;
        std::vector<Sphere*> spheres;

        Camera *cam;

        World()
        :O(Vector(0,0,0)), X(Vector(1,0,0)), Y(Vector(0,1,0)), Z(Vector(0,0,1)), backgroundColor(Color(0,0,0,0))
        {
            time1 = clock();
        }

        World(Vector origin, Vector Xaxis, Vector Yaxis, Vector Zaxis, Color color)
        :O(origin), X(Xaxis), Y(Yaxis), Z(Zaxis), backgroundColor(color)
        {}

        std::vector<double> findIntersections(Ray cameraRay)
        {
            std::vector<double> intersections;
            for(int i = 0; i < worldObjects.size(); i++)
            {
                intersections.push_back(worldObjects[i]->findIntersection(cameraRay));
                if(worldObjects[i]->type == SPHERE)
                {
                    if(cameraRay.getRayDirection().getDotProductWith(((Sphere*)worldObjects[i])->getSphereCentre().vectorAdd(cam->getCameraPosition().getNegative()).getNormalized()) < 0)
                    {
                        continue;
                    }
                }
            }
            return intersections;
        }

        Vector getO(){
            return O;
        }
        Vector getX(){
            return X;
        }

        Vector getY(){
            return Y;
        }
           
        Vector getZ(){
            return Z;
        }

        Color getBackgroundColor(){
            return backgroundColor;
        }

        double getAmbientLight()
        {
            return ambientLight;
        }

        void setAmbientLight(double newAmbient)
        {
            ambientLight = newAmbient;
        }

        void emptyVectors(){
           worldObjects = {};
           worldLights = {};
        } 

        template <typename T>
        void add(T& obj)
        {

            if(std::is_same<T,Light>())
            {
                worldLights.push_back(dynamic_cast<Light*>(&obj));
            }
            else{
                worldObjects.push_back(dynamic_cast<Object*>(&obj));
                if(worldObjects[worldObjects.size() - 1]->type == PLANE){
                    planes.push_back((Plane*)&obj);
                }
                else if(worldObjects[worldObjects.size() - 1]->type == SPHERE)
                {
                    spheres.push_back((Sphere*)&obj);
                }

            }
        }

        void update()
        {
            time2 = clock();
            for(int i = 0; i<worldObjects.size(); i++)
            {
                Object* obj = worldObjects[i];
                if(obj->type == objectType::SPHERE)
                {
                    Sphere* s = (Sphere*)obj;
                    s->moveByVel(0.2);
                    float distance = s->getSphereCentre().vectorAdd(cam->getCameraPosition().getNegative()).getMagnitude();

                    int sp;
                    int j = 0;
                    for(Sphere* sphere: spheres)
                    {
                        if(s != sphere)
                        {
                            s->checkCollisionWithSphere(sphere);
                        }
                        else{
                            sp = j;
                        }
                        j++;

                      
                    }
                    for(Plane* p : planes)
                    {
                        s->checkCollidWithPlane(p);
                    }
                   
                    s->updateVelocity((((float)time2 - (float)time1)/CLOCKS_PER_SEC) * 0.4);

                    if(distance > CUT_DISTANCE)
                    {
                        delete[] spheres[sp];
                        worldObjects.erase(worldObjects.begin() + i);                       
                        spheres.erase(spheres.begin() + sp);
                    }

                    
                }
            }
            time1 = time2;
            
        }

        ~World(){
            for(Source* l : worldLights)
            {
                delete l;
            }
            for(Object* obj : worldObjects)
            {
                delete obj;
            }

        }

};