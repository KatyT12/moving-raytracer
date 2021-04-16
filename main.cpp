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
    olc::vi2d lastMouse;


    bool OnUserCreate() override
	{
        int dpi = 72;
        width = ScreenWidth();
        height = ScreenHeight();
        double aspectRatio = (double)width / (double) height;

        lastMouse = GetMousePos();

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
        Light* scene_light = new Light(lightPosition,whiteLight);
        //Scene objects
        Sphere* scene_sphere = new Sphere(world.getO(),1,cool_green);//Sphere is at origin 
        Plane* scene_plane = new Plane(world.getY(),-1,maroon);//Directly beneath sphere
        world.add(*scene_light);
        world.add(*scene_sphere);
        world.add(*scene_plane);


        renderer.setAll(&world,width,height);
 
		// Called once at the start, so create things here
        buffer = renderer.renderImage();

		std::cout << ScreenWidth() << "\n";
		return true;
	}

    void reDraw()
    {        
        buffer = renderer.renderImage();
    }

	bool OnUserUpdate(float fElapsedTime) override
	{
        Clear(olc::WHITE);
        world.update();
        reDraw();
		// called once per frame
        for(int x = 0; x < ScreenWidth();x++)
        {
            for(int y = 0; y < ScreenHeight();y++)
            {
                Draw(olc::vi2d(x,y),buffer[(ScreenHeight() - (y+1)) * ScreenWidth() + x]);
            }
        }

	delete buffer;
        checkKeys();
		return true;
	}

    Vector getPointingDirection(){
        olc::vi2d mousePos;
        mousePos.x = GetMouseX();
        mousePos.y = ScreenHeight() - GetMouseY();
        double xamnt;
        double yamnt;
        renderer.getCentrePixels(xamnt,yamnt,mousePos.x,mousePos.y);
        Vector cameraRayDirection = scene_cam.getCameraDirection().vectorAdd(scene_cam.getCameraRight().scalarMult(xamnt - 0.5).vectorAdd(scene_cam.getCameraDown().scalarMult(yamnt - 0.5))).getNormalized();
        return cameraRayDirection.getNormalized();
    }

    void checkKeys()
    {
        if(GetKey(olc::D).bHeld)
        {
            scene_cam.moveRight(0.2);
        }
        if(GetKey(olc::A).bHeld)
        {
            scene_cam.moveLeft(0.2);
        }
        
        
        if(GetKey(olc::W).bHeld)
        {
            scene_cam.moveForward(0.2);
        }
        if(GetKey(olc::S).bHeld)
        {
            scene_cam.moveBack(0.2);
        }

        if(GetKey(olc::SPACE).bHeld)
        {
            scene_cam.moveUp(0.2);
        }
        if(GetKey(olc::RETURN).bHeld)
        {
            scene_cam.moveDown(0.2);
        }
        
        if(GetKey(olc::ESCAPE).bPressed)
        {
            quick_exit(0);
        }
        
        if(GetKey(olc::Q).bPressed)
        {
            Sphere* s = new Sphere(scene_cam.getCameraPosition(),0.5,Color(1,0,0,1));

            s->setVel(getPointingDirection().scalarMult(7));
            world.add(*s);
        }


        if(GetMouse(0).bPressed)
        {

            olc::vi2d mousePos;
            mousePos.x = GetMouseX();
            mousePos.y = ScreenHeight() - GetMouseY();

            Vector cameraRayDirection = getPointingDirection();
            Ray ray(scene_cam.getCameraPosition(),cameraRayDirection);
            scene_cam.lookAt(scene_cam.getCameraPosition(),scene_cam.getCameraPosition().vectorAdd(cameraRayDirection),world.getY());

        }
    }
};



int main()
{
    std::cout << "Raytracing\n";

    Game demo;
	if (demo.Construct(200,200, 4, 4))
		demo.Start();

 
    
    return 0;
}