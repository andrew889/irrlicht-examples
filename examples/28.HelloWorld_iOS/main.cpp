#include <irrlicht.h>
#include "exampleHelper.h"
#include "SpriteNode.hpp"
#include <iostream>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

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
    scene::ICameraSceneNode* fixedCam = 0;
    
    auto size = core::dimension2d<u32>(512, 512);
    auto renderTargetTex = driver->addRenderTargetTexture(core::dimension2d<u32>(512, 512), "RTT1", video::ECF_R8G8B8);
    auto renderTargetDepth = driver->addRenderTargetBuffer(size, ECF_D24S8);
    
    auto renderTarget = driver->addRenderTarget();
    
    // set image storage to be a usable texture
    // You need to do this first, so any renderbuffers know what size they need to be
    // (alternatively, use setDimensions to pre-empt it)
    renderTarget->setAttachment(renderTargetTex, renderTargetDepth);
    
    // finally, set the material of the cube to be the image storage of the rendertarget
    test->setMaterialTexture(0, renderTargetTex);
    
    // add fixed camera
    fixedCam = smgr->addCameraSceneNode(0, core::vector3df(0,0,-180));
    
    // add fps camera
    scene::ICameraSceneNode* fpsCamera = smgr->addCameraSceneNodeFPS();
    fpsCamera->setPosition(core::vector3df(-50,50,-350));
    
    
    auto sprite = SpriteNode(smgr->getRootSceneNode(), smgr, 75);
    sprite.getMaterial(0).setTexture(0, driver->getTexture(mediaPath + "axe.jpg"));
    auto rotanim = smgr->createRotationAnimator(vector3df { 0, 0, 0.5 });
    sprite.addAnimator(rotanim);
    sprite.remove();
    
    while(device->run())
        if (device->isWindowActive())
        {
            driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH | video::ECBF_STENCIL, video::SColor(0),1.0f,0);
            
            // make cube invisible and set fixed camera as active camera
            test->setVisible(false);
            smgr->setActiveCamera(fixedCam);
            
            
            // draw scene into render target
            
            // set render target texture
            driver->setRenderTargetEx(renderTarget, video::ECBF_COLOR | video::ECBF_DEPTH | video::ECBF_STENCIL, video::SColor(0,0,0,255), 1.0f, 0);
            
            glEnable(GL_STENCIL_TEST);
            glStencilMask(0xFF);
            
            glClearStencil(0);
            glClear(GL_STENCIL_BUFFER_BIT);
            
            glStencilFunc(GL_ALWAYS, 0xFF, 1);
            glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);   // always write whatever
            
            
            
            // set render target texture
            driver->setRenderTargetEx(renderTarget, video::ECBF_COLOR | video::ECBF_DEPTH | video::ECBF_STENCIL, video::SColor(0,0,0,255), 1.0f, 0);
            
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            
            // render sprite panel into depth and stencil buffers
            sprite.render();
            
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

            
            // reset stencil functions
            glStencilFunc(GL_EQUAL, 0xFF, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);   // turn off write
            
            // draw whole scene into render buffer
            smgr->drawAll();
            
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glStencilMask(0x00);
            glDisable(GL_STENCIL_TEST);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            
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
