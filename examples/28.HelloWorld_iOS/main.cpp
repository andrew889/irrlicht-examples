#include <irrlicht.h>
#include "exampleHelper.h"
#include "SpriteNode.hpp"
#include <iostream>
#include <OpenGLES/ES2/gl.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

// It's important for iOS projects to use 'irrlicht_main' instead of standard 'main' function.

void irrlicht_main()
{
	IrrlichtDevice *device = createDevice(EDT_OGLES2, dimension2d<u32>(0, 0), 32, false, true, false, 0);

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    gui::IGUIEnvironment* env = device->getGUIEnvironment();
    
    const io::path mediaPath = getExampleMediaPath();
    
    // load and display animated fairy mesh
    
    scene::IAnimatedMeshSceneNode* fairy = smgr->addAnimatedMeshSceneNode(smgr->getMesh(mediaPath + "sydney.md2"));
    
    if (fairy)
    {
        fairy->setMaterialTexture(0, driver->getTexture(mediaPath + "sydney.bmp")); // set diffuse texture
        fairy->setPosition(core::vector3df(0, -30, -100));
        fairy->setMD2Animation ( scene::EMAT_STAND );
    }
    
    // set ambient light
    smgr->setAmbientLight(video::SColor(0,60,60,60));
    
    // create test cube
    scene::ISceneNode* test = smgr->addCubeSceneNode(20);
    
    // let the cube rotate and set some light settings
    scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.3f, 0.3f,0));
    
    test->setPosition(core::vector3df(0, 40, -100));
    test->setMaterialFlag(video::EMF_LIGHTING, false); // disable dynamic lighting
    test->addAnimator(anim);
    anim->drop();
    
    // create render target
    video::IRenderTarget* renderTarget = 0;
    video::ITexture* renderTargetTex = 0;
    scene::ICameraSceneNode* fixedCam = 0;
    
    renderTargetTex = driver->addRenderTargetTexture(core::dimension2d<u32>(512, 512), "RTT1", video::ECF_R8G8B8);
    video::ITexture* renderTargetDepth = driver->addRenderTargetTexture(core::dimension2d<u32>(512, 512), "DepthStencil", video::ECF_D16);
    
    renderTarget = driver->addRenderTarget();
    renderTarget->setTexture(0, renderTargetDepth);  // DOESN'T WORK WITH DEPTH TEXTURE!!
    
    test->setMaterialTexture(0, renderTargetTex); // set material of cube to render target
    
    auto string = glGetString(GL_EXTENSIONS);
    
//    cout << string << endl;
    
    // add fixed camera
    fixedCam = smgr->addCameraSceneNode(0, core::vector3df(0,0,-180));
    
    // add fps camera
    scene::ICameraSceneNode* fpsCamera = smgr->addCameraSceneNodeFPS();
    fpsCamera->setPosition(core::vector3df(-50,50,-350));
    
    
    auto sprite = SpriteNode(smgr->getRootSceneNode(), smgr, 75);
    sprite.getMaterial(0).setTexture(0, driver->getTexture(mediaPath + "axe.jpg"));
    auto rotanim = smgr->createRotationAnimator(vector3df { 0, 0, 0.5 });
    sprite.addAnimator(rotanim);
    
    while(device->run())
        if (device->isWindowActive())
        {
            driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH, video::SColor(0));
            
            // draw scene into render target
            
            // set render target texture
            driver->setRenderTargetEx(renderTarget, video::ECBF_COLOR | video::ECBF_DEPTH, video::SColor(0,0,0,255));
        
            // make cube invisible and set fixed camera as active camera
            test->setVisible(false);
            smgr->setActiveCamera(fixedCam);
        
            // draw whole scene into render buffer
            smgr->drawAll();
            
            // set back old render target
            // The buffer might have been distorted, so clear it
            driver->setRenderTargetEx(0, 0, video::SColor(0));
            
            // make the cube visible and set the user controlled camera as active one
            test->setVisible(true);
//            smgr->setActiveCamera(fpsCamera);
            
            // draw scene normally
            smgr->drawAll();
            env->drawAll();
            
            driver->endScene();
        }
    
    device->drop(); // drop device
}
