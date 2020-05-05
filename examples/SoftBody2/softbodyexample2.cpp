// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include "softbodyexample.h"
#include <irrlicht.h>
#include <irrBullet.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace std;

/*
    Note: The flag example in this demo is only as good as its wind generator, and
    in this demo it is a very simple wind generator in one direction; however, it
    still achieves decent results.
*/

CSoftbodyExample::CSoftbodyExample()
{
}



bool CSoftbodyExample::OnEvent(const SEvent& event)
{
	if (!device)
		return false;

    switch(event.EventType)
    {
        case EET_MOUSE_INPUT_EVENT:
        {
            if(event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)
            {
				auto body = shootCube(vector3df(10, 10, 10), 2);

				auto deleteAffector = new ICollisionObjectAffectorDelete(4000);
                body->addAffector(deleteAffector);
                return true;
            }

            else
            if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN)
            {
				auto body = shootSphere(vector3df(0.2, 0.2, 0.2), 0.2);

				auto deleteAffector = new ICollisionObjectAffectorDelete(4000);
                body->addAffector(deleteAffector);
                return true;
            }
        }
        break;

        case EET_KEY_INPUT_EVENT:
        {
            if(event.KeyInput.Key == KEY_KEY_P && event.KeyInput.PressedDown == false)
            {
                world->pauseSimulation(!world->simulationPaused());
                return true;
            }
        }
        break;
        default:
            break;
    }
    return false;
}

void CSoftbodyExample::runExample()
{
    debugDraw = true;
    drawProperties = true;
    drawWireFrame = false;

    int choice = 5;
    int rows = 2;
    int columns = 1;
    softVersusSoft = 1;

    cin >> softVersusSoft;

    device.reset(createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
            false, false, false, this));


    device->setWindowCaption(L"irrBullet Softbody Example - Josiah Hartzell");

    device->getFileSystem()->addFileArchive("../../media/");


	auto light = device->getSceneManager()->addLightSceneNode(0, vector3df(20, 40, -50), SColorf(1.0f, 1.0f, 1.0f), 4000.0f);
    light->setLightType(ELT_DIRECTIONAL);
    light->setRotation(vector3df(0,200,30));

    device->getSceneManager()->setAmbientLight(SColor(100,100,100,100));

    camera = device->getSceneManager()->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(50,15,4));
	camera->setTarget(vector3df(0,0,0));


    // Create irrBullet World 
    world.reset(createIrrBulletWorld(device, true, debugDraw));

	world->setDebugMode(irrPhysicsDebugMode::EPDM_DrawAabb |
		irrPhysicsDebugMode::EPDM_DrawContactPoints);

    world->setGravity(vector3df(0,-10,0));

    world->getSoftBodyWorldInfo().air_density = 0.05;

    createGround();


    for(u32 i = 0; i < rows; i++) {
            createSoftbodyType(choice, vector3df(56*(i), 200+i, 56*(i)));
    }

    // Set our delta time and time stamp
    u32 TimeStamp = device->getTimer()->getTime();
    u32 DeltaTime = 0;
    u32 lastWindUpdate = 0;
    f32 xWind = 0.0f;
    f32 zWind = 0.0f;

    while(device->run())
    {
        device->getVideoDriver()->beginScene(true, true, SColor(255,100,101,140));

        DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

		// Step the simulation with our delta time
        world->stepSimulation(DeltaTime*0.001f, 120);

        //world->debugDrawWorld(debugDraw);
        // This call will draw the technical properties of the physics simulation
        // to the GUI environment.
        world->debugDrawProperties(drawProperties);


        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();

        device->getVideoDriver()->endScene();
    }
}

// 1 = sphere, 2 = newspaper(aero), 3 = cloth, 4 = feather(aero), 5 = flag(aero)
ISoftBody* CSoftbodyExample::createSoftbodyType(u32 type, const vector3df& position)
{
    IMeshSceneNode* softbodyNode;
	
	
	//NEW
	device->getSceneManager()->getMesh("ExampleCar.dae");
    scene::IMeshCache* cache = device->getSceneManager()->getMeshCache();
    for (int i = 0; i < cache->getMeshCount (); i++) {
       //device->getSceneManager()->addAnimatedMeshSceneNode ( cache->getMeshByIndex ( i ) );
	   device->getSceneManager()->addMeshSceneNode ( cache->getMeshByIndex ( i ) );
    }
	
	
	// OLD
	device->getSceneManager()->getMeshCache()->removeMesh(device->getSceneManager()->getMeshCache()->getMeshByName("ExampleCar.dae"));
	auto mesh = device->getSceneManager()->getMesh("ExampleCar.dae");
	softbodyNode = device->getSceneManager()->addMeshSceneNode(mesh);
	// OLD
	
	//softbodyNode->setMaterialTexture(0, device->getVideoDriver()->getTexture("detailmap3.jpg"));
	softbodyNode->setAutomaticCulling(EAC_OFF);
	softbodyNode->setScale(vector3df(100,100,100));
	softbodyNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);

    softbodyNode->setPosition(position);
    softbodyNode->setDebugDataVisible(EDS_BBOX);

	auto softbody = world->addSoftBody(softbodyNode);

    softbody->setName("SOFTBODY1");
    softbody->setActivationState(EActivationState::EAS_DISABLE_DEACTIVATION);

    if(softVersusSoft) {
        softbody->generateClusters(60.0f);
        softbody->addCollisionFlag(ESoftBodyCollisionFlag::ESBCM_CLUSTER_SOFT_VERSUS_SOFT);
    }

	softbody->getConfiguration().poseMatchingCoefficient = 0.0;
	softbody->updateConfiguration();
	softbody->generateBendingConstraints(2);
	softbody->randomizeConstraints();
 

    return softbody;

}


CSoftbodyExample::~CSoftbodyExample()
{
    
}
