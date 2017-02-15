//
//  SpriteNode.cpp
//  CustomSceneNode
//
//  Created by Ajmal Kunnummal on 1/3/17.
//
//

#include "SpriteNode.hpp"
#include <OpenGLES/ES2/gl.h>

using namespace irr;
using namespace scene;
using namespace video;

SpriteNode::SpriteNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, f32 size, irr::core::vector2df centerOffset)
    : scene::ISceneNode(parent, mgr)
{
    Material.MaterialType = EMT_SOLID;
    
    Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    Material.Wireframe = false;
    Material.Lighting = false;
    
    Material.UseMipMaps = false;
     
    Material.TextureLayer[0].BilinearFilter = true;
    Material.TextureLayer[0].AnisotropicFilter = false;
    Material.TextureLayer[0].TextureWrapU = ETC_CLAMP_TO_EDGE;
    Material.TextureLayer[0].TextureWrapV = ETC_CLAMP_TO_EDGE;
    Material.TextureLayer[0].TextureWrapW = ETC_CLAMP_TO_EDGE;
    
    // get height via width and ratio and replace the 0.5s below
    
    Vertices[0].Pos = core::vector3df(0.5,  0.5,  0.0); // top right
    Vertices[1].Pos = core::vector3df(0.5,  -0.5,  0.0); // bottom right
    Vertices[2].Pos = core::vector3df(-0.5,  0.5,  0.0); // top left
    Vertices[3].Pos = core::vector3df(-0.5,  -0.5,  0.0); // bottom left
    
    // 1 unit = width of the screen/player
    // give this class both a width and a ratio to get size
    for(auto& vertex: Vertices) {
        // each point shifted up and to the right of itself to create another square with its bottom-left corner at the first square's center
        auto textureOffset = vertex.Pos + irr::core::vector3df(0.5,  0.5,  0);
        auto offset = irr::core::vector3df(centerOffset.X, centerOffset.Y, 0);
        vertex.Pos = vertex.Pos * size + offset;
        vertex.Color = video::SColor(255, 255, 255, 255);
        vertex.TCoords = core::vector2df(textureOffset.X,  textureOffset.Y);
    }
    
    // Bounding box of a scene node.
    Box.reset(Vertices[0].Pos);
    for (s32 i=1; i<4; ++i)
        Box.addInternalPoint(Vertices[i].Pos);
}

/*
 Before it is drawn, the irr::scene::ISceneNode::OnRegisterSceneNode()
 method of every scene node in the scene is called by the scene manager.
 If the scene node wishes to draw itself, it may register itself in the
 scene manager to be drawn. This is necessary to tell the scene manager
 when it should call irr::scene::ISceneNode::render(). For
 example, normal scene nodes render their content one after another,
 while stencil buffer shadows would like to be drawn after all other
 scene nodes. And camera or light scene nodes need to be rendered before
 all other scene nodes (if at all). So here we simply register the
 scene node to render normally. If we would like to let it be rendered
 like cameras or light, we would have to call
 SceneManager->registerNodeForRendering(this, SNRT_LIGHT_AND_CAMERA);
 After this, we call the actual
 irr::scene::ISceneNode::OnRegisterSceneNode() method of the base class,
 which simply lets also all the child scene nodes of this node register
 themselves.
 */
void SpriteNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);
    
    ISceneNode::OnRegisterSceneNode();
}

void SpriteNode::render()
{
    /* Indices into the 'Vertices' array. A triangle needs 3 vertices
     so you have to pass the 3 corresponding indices for each triangle to
     tell which of the vertices should be used for it.	*/
    u16 indices[] = { 3,2,0, 3,0,1 };
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    
    driver->setMaterial(Material);
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    
   
    
    driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 2);
    
    
}

/*
 And finally we create three small additional methods.
 irr::scene::ISceneNode::getBoundingBox() returns the bounding box of
 this scene node, irr::scene::ISceneNode::getMaterialCount() returns the
 amount of materials in this scene node (our tetraeder only has one
 material), and irr::scene::ISceneNode::getMaterial() returns the
 material at an index. Because we have only one material here, we can
 return the only one material, assuming that no one ever calls
 getMaterial() with an index greater than 0.
 */
const core::aabbox3d<f32>& SpriteNode::getBoundingBox() const
{
    return Box;
}

u32 SpriteNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& SpriteNode::getMaterial(u32 i)
{
    return Material;
}
