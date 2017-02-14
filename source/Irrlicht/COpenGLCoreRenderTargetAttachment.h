//
//  COpenGLCoreRenderTargetAttachment.h
//  Irrlicht
//
//  Created by Ajmal Kunnummal on 2/10/17.
//
//

#ifndef COpenGLCoreRenderTargetAttachment_h
#define COpenGLCoreRenderTargetAttachment_h

#include "IrrCompileConfig.h"

#if defined(_IRR_COMPILE_WITH_OPENGL_) || defined(_IRR_COMPILE_WITH_OGLES1_) || defined(_IRR_COMPILE_WITH_OGLES2_)

#include "IRenderTargetAttachment.h"

namespace irr
{
    namespace video
    {
        template <class TOpenGLDriver>
        class COpenGLCoreRenderTargetAttachment : public IRenderTargetAttachment
        {
        public:
            COpenGLCoreRenderTargetAttachment(const core::dimension2d<u32>& size, ECOLOR_FORMAT format, TOpenGLDriver* driver) : Driver(driver)
            {
                DriverType = Driver->getDriverType();
                Size = size;
                ColorFormat = ECF_UNKNOWN == format ? ECF_A8R8G8B8 : format;
                
                GLenum PixelFormat;
                GLenum PixelType;
                void (*converter)(const void*, s32, void*);
                
                Driver->getColorFormatParameters(ColorFormat, InternalFormat, PixelFormat, PixelType, &converter);
            }
            
            virtual void attachToCurrentFBOAsDepth()
            {
                os::Printer::log("UNIMPLEMENTED", ELL_ERROR);
            }
            
            virtual void attachToCurrentFBOAsStencil()
            {
                os::Printer::log("UNIMPLEMENTED", ELL_ERROR);
            }
            
            virtual void detachFromCurrentFBOAsDepth()
            {
                os::Printer::log("UNIMPLEMENTED", ELL_ERROR);
            }
            
            virtual void detachFromCurrentFBOAsStencil()
            {
                os::Printer::log("UNIMPLEMENTED", ELL_ERROR);
            }
            
        protected:
            TOpenGLDriver* Driver;
            GLint InternalFormat;
            
        };
        
    }
}

#endif

#endif /* COpenGLCoreRenderTargetAttachment_h */
