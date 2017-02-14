//
//  IRenderTargetAttachment.h
//  Irrlicht
//
//  Created by Ajmal Kunnummal on 2/10/17.
//
//

#ifndef IRenderTargetAttachment_h
#define IRenderTargetAttachment_h

#include "IReferenceCounted.h"

namespace irr
{
    namespace video
    {
        
        //! Interface of a Render Target Attachment.
        class IRenderTargetAttachment : public virtual IReferenceCounted
        {
        public:
            IRenderTargetAttachment() : ColorFormat(ECF_UNKNOWN) {}
            //! Get driver type of attachment.
            /** This is the driver, which created the attachment. This method is used
             internally by the video devices, to check, if they may use an attachment
             because attachments may be incompatible between different devices.
             \return Driver type of attachment. */
            E_DRIVER_TYPE getDriverType() const { return DriverType; };
            
            //! Get dimension (=size) of the attachment.
            /** \return The size of the attachment. */
            const core::dimension2d<u32>& getSize() const { return Size; };
            
            //! Get the color format of texture.
            /** \return The color format of texture. */
            ECOLOR_FORMAT getColorFormat() const { return ColorFormat; };
            
            //! Returns true if the impolementation can render depth
            virtual bool canRenderDepth() { return false; }
            
        protected:
        	E_DRIVER_TYPE DriverType;
            core::dimension2d<u32> Size;
            ECOLOR_FORMAT ColorFormat;
        };
        
    }
}

#endif /* IRenderTargetAttachment_h */
