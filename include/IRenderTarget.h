// Copyright (C) 2015 Patryk Nadrowski
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_RENDER_TARGET_H_INCLUDED__
#define __I_RENDER_TARGET_H_INCLUDED__

#include "IReferenceCounted.h"
#include "EDriverTypes.h"
#include "irrArray.h"

#ifdef _IRR_COMPILE_WITH_OGLES2_

#if defined(_IRR_COMPILE_WITH_IOS_DEVICE_)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(_IRR_COMPILE_WITH_ANDROID_DEVICE_)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/eglplatform.h>
#else
#include <GLES2/gl2.h>
#include <EGL/eglplatform.h>
typedef char GLchar;
#if defined(_IRR_OGLES2_USE_EXTPOINTER_)
#include "gles2-ext.h"
#endif
#endif
#endif


namespace irr
{
namespace video
{
	class ITexture;

	//! Interface of a Render Target.
	class IRenderTarget : public virtual IReferenceCounted
	{
	public:

		//! constructor
		IRenderTarget() : DepthStencil(0), DepthStencilType(0), DriverType(EDT_NULL)
		{
		}

		//! Returns an array of previously set textures.
		const core::array<ITexture*>& getColourTextureArray() const
		{
			return Texture;
		}

		//! Returns a of previously set depth / depth-stencil texture.
		ITexture* getDepthStencilTexture() const
		{
			return DepthStencil;
		}
        
        bool ColourAtIndexIsTexture(int i)
        {
            return StorageType[i] == 0;
        }
        
        

		//! Set multiple textures.
		/** Set multiple textures for the render target.
		\param texture Array of texture objects. These textures are used for a color outputs.
		\param depthStencil Depth or packed depth-stencil texture. This texture is used as depth
		or depth-stencil buffer. */
		virtual void setTexture(const core::array<ITexture*>& texture, ITexture* depthStencil) = 0;
        
        //! Create multiple buffers.
        /** Set multiple colour storage buffers for the render target.
         \param buffers: whether to generate a buffer for this colour unit
         \param depthStencil whether to generate Depth or packed depth-stencil buffer */
        virtual void createBuffers(const core::array<bool>& buffers, bool depth)
        {}
	      
		//! Set one texture.
		void setTexture(ITexture* texture, ITexture* depthStencil)
		{
			core::array<ITexture*> textureArray(1);
			textureArray.push_back(texture);

			setTexture(textureArray, depthStencil);
		}
        
        void setColourIsTexture(int i, bool value)
        {
            StorageType[i] = value ? 0 : 1;
        }
        
        void setDepthIsTexture(bool value)
        {
            DepthStencilType = value ? 0 : 1;
        }

		//! Get driver type of render target.
		E_DRIVER_TYPE getDriverType() const
		{
			return DriverType;
		}
		
	protected:
	
        // in order to maintain compatibility with other code which uses RenderTarget to create shadow stencil maps, we have to provide access to DepthStencil texture even though it doesn't work on iOS.
        
        core::array<int> StorageType;       // 0 = Texture, 1 = Buffer
        
		//! Color Storage Textures assigned to render target.
		core::array<ITexture*> Texture;
        
        //! Colour Storage Buffers assigned to render target.
        core::array<GLuint> Buffer;
		
        int DepthStencilType; // as above
        
		//! Depth or packed depth-stencil storage texture assigned to render target.
		ITexture* DepthStencil;
        
        //! Depth or packed depth-stencil storage buffer assigned to render target.
        GLuint DepthStencilBuffer;

		//! Driver type of render target.
		E_DRIVER_TYPE DriverType;
	};

}
}

#endif
