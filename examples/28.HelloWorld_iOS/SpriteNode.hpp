//
//  SpriteNode.hpp
//  CustomSceneNode
//
//  Created by Ajmal Kunnummal on 1/3/17.
//
//

#ifndef SpriteNode_hpp
#define SpriteNode_hpp

#include <stdio.h>
#include <irrlicht.h>
/*
class MaskShaderCallBack : public IShaderConstantSetCallBack
{
public:
    MyShaderCallBack(GLuint texture, GLuint mask) : FirstUpdate(true), texture(0), mask(1) { }
    
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) {
        IVideoDriver* driver = services->getVideoDriver();
        
        if (FirstUpdate)
        {
            textureID = services->getVertexShaderConstantID("texture");
            maskID = services->getVertexShaderConstantID("mask");
            WVPMatrixID = services->getVertexShaderConstantID("uWVPMatrix");
            
            FirstUpdate = false;
        }
        
        services->setPixelShaderConstant(textureID, &texture, 1);
        services->setPixelShaderConstant(maskID, &mask, 1);
        
        const core::matrix4 W = driver->getTransform(ETS_WORLD);
        const core::matrix4 V = driver->getTransform(ETS_VIEW);
        const core::matrix4 P = driver->getTransform(ETS_PROJECTION);
        
        core::matrix4 Matrix = P * V * W;
        services->setPixelShaderConstant(WVPMatrixID, Matrix.pointer(), 16);
    }
    
private:
    bool FirstUpdate;
    
    s32 textureID;
    s32 maskID;
    s32 WVPMatrixID;
    
    s32 texture;
    s32 mask;
};
*/
class SpriteNode : public irr::scene::ISceneNode
{
    irr::core::aabbox3d<irr::f32> Box;
    irr::video::S3DVertex Vertices[4];
    irr::video::SMaterial Material;
public:
    SpriteNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::f32 size, irr::core::vector2df centerOffset = irr::core::vector2df(0,0));
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
    virtual irr::u32 getMaterialCount() const;
    virtual irr::video::SMaterial& getMaterial(irr::u32 i);
};

#endif /* SpriteNode_hpp */
