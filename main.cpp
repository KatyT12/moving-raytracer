#include "Vector.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"
#include "World.h"
#include "Source.h"
#include "Renderer.h"
#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Game";
	}

public:
	olc::Pixel* buffer;
    Camera scene_cam;
    int width;
    int height;
    World world;
    Renderer renderer;

    Sphere scene_sphere;
    Plane scene_plane;
    Light scene_light;

    bool OnUserCreate() override
	{
        int dpi = 72;
        width = ScreenWidth();
        height = ScreenHeight();
        double aspectRatio = (double)width / (double) height;

        world.setAmbientLight(0.2);
        Vector CameraPosition(3,1.5,-4);

        scene_cam.lookAt(CameraPosition,world.getO(),world.getY());
        world.cam = &scene_cam;

        Color whiteLight(1.0,1.0,1.0,1.0);
        Color cool_green(0.5,1.0,0.5,0.3);
        Color gray(0.5,0.5,0.5,0.0);
        Color maroon(0.5,0.8,0.8,2);

        Vector lightPosition(-7,10,-10);

        //Vector lightPosition(-4,7,10);
        scene_light.setAll(lightPosition,whiteLight);
        //Scene objects
        scene_sphere.setAll(world.getO(),1,cool_green);//Sphere is at origin 
        scene_plane.setAll(world.getY(),-1,maroon);//Directly beneath sphere

        world.add(scene_light);
        world.add(scene_sphere);
        world.add(scene_plane);


        renderer.setAll(world,width,height);
 
		// Called once at the start, so create things here
        buffer = renderer.render();

		std::cout << ScreenWidth() << "\n";
		return true;
	}

    void reDraw()
    {        
        buffer = renderer.render();
    }

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
        for(int x = 0; x < ScreenWidth();x++)
        {
            for(int y = 0; y < ScreenHeight();y++)
            {
                Draw(olc::vi2d(x,y),buffer[(ScreenHeight() - (y+1)) * ScreenWidth() + x]);
            }
        }

        checkKeys();
		return true;
	}


    void checkKeys()
    {
        if(GetKey(olc::D).bPressed)
        {
            scene_cam.moveRight(0.2);
            reDraw();
        }
        if(GetKey(olc::A).bPressed)
        {
            scene_cam.moveLeft(0.2);
            reDraw();
        }
        
        
        if(GetKey(olc::W).bPressed)
        {
            scene_cam.moveForward(0.2);
            reDraw();
        }
        if(GetKey(olc::S).bPressed)
        {
            scene_cam.moveBack(0.2);
            reDraw();
        }

        if(GetKey(olc::SPACE).bPressed)
        {
            scene_cam.moveUp(0.2);
            reDraw();
        }
        if(GetKey(olc::RETURN).bPressed)
        {
            scene_cam.moveDown(0.2);
            reDraw();
        }

        if(GetMouse(0).bPressed)
        {

            olc::vi2d mousePos;
            mousePos.x = GetMouseX();
            mousePos.y = ScreenHeight() - GetMouseY();
            
            double xamnt;
            double yamnt;
            renderer.getCentrePixels(xamnt,yamnt,mousePos.x,mousePos.y);
            Vector cameraRayDirection = scene_cam.getCameraDirection().vectorAdd(scene_cam.getCameraRight().scalarMult(xamnt - 0.5).vectorAdd(scene_cam.getCameraDown().scalarMult(yamnt - 0.5))).getNormalized();
            Ray ray(scene_cam.getCameraPosition(),cameraRayDirection);
            std::vector<double> intersections = world.findIntersections(ray);
            int index = renderer.winningObjectIndex(intersections);
            if(index < 0)
            {

                return; 
            }
            else{
                scene_cam.lookAt(scene_cam.getCameraPosition(),scene_cam.getCameraPosition().vectorAdd(cameraRayDirection.scalarMult(intersections[index])),world.getY());
            }

            reDraw();

        }
    }
};



int main()
{
    std::cout << "Raytracing\n";
    
    Game demo;
	if (demo.Construct(640, 480, 4, 4))
		demo.Start();
    
    
    return 0;
}