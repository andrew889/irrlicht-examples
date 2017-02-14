//
//  OpenGLCoreRenderBuffer.h
//  Irrlicht
//
//  Created by Ajmal Kunnummal on 2/10/17.
//
//

#ifndef COpenGLCoreRenderBuffer_h
#define COpenGLCoreRenderBuffer_h

#if defined(_IRR_COMPILE_WITH_OPENGL_) || defined(_IRR_COMPILE_WITH_OGLES1_) || defined(_IRR_COMPILE_WITH_OGLES2_)

#include "COpenGLCoreRenderTargetAttachment.h"

namespace irr
{
    namespace video
    {
        template <typename TOpenGLDriver>
        class COpenGLCoreRenderBuffer : public COpenGLCoreRenderTargetAttachment<TOpenGLDriver>
        {
        public:
            COpenGLCoreRenderBuffer(const core::dimension2d<u32>& size, ECOLOR_FORMAT format, TOpenGLDriver* driver)
            : COpenGLCoreRenderTargetAttachment<TOpenGLDriver>(size, format, driver)
            {
                this->Driver->irrGlGenRenderbuffers(1, &BufferName);
                this->Driver->irrGlBindRenderbuffer(GL_RENDERBUFFER, BufferName);
                this->Driver->irrGlRenderbufferStorage(GL_RENDERBUFFER, this->InternalFormat, size.Width, size.Height);
                
            }
            
            virtual bool canRenderDepth() _IRR_OVERRIDE_
            {
                return this->BufferName && IImage::isDepthFormat(this->getColorFormat());
            }
            
            virtual void attachToCurrentFBOAsDepth() _IRR_OVERRIDE_
            {
                this->Driver->irrGlFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, BufferName);
            }
            
            virtual void attachToCurrentFBOAsStencil() _IRR_OVERRIDE_
            {
                this->Driver->irrGlFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, BufferName);
            }
            
            virtual void detachFromCurrentFBOAsDepth() _IRR_OVERRIDE_
            {
                this->Driver->irrGlFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
            }
            
            virtual void detachFromCurrentFBOAsStencil() _IRR_OVERRIDE_
            {
                this->Driver->irrGlFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
            }
            
        protected:
            GLuint BufferName = 0;
        };
        
    }
}

#endif

#endif /* COpenGLCoreRenderBuffer_h */
