//////////////////////////////////////////////////////////////////////////////
//! @file 	CImageGrabber.h
//! @date	Created on: 06.04.2011
//! @author	Falk Schilling
//////////////////////////////////////////////////////////////////////////////

#ifndef CIMAGEGRABBER_H_
#define CIMAGEGRABBER_H_

//////////////////////////////////////////////////////////////////////////////

#include "CImage.h"
#include "IDreamboxGrabber.h"
#include <string>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  typedef enum {UNKNOWN,PALLAS,VULCAN,XILLEON,BRCM7401,BRCM7400,BRCM7405} t_Dreambox;

  //////////////////////////////////////////////////////////////////////////
  //! This funny little class reads out an image from a video source.
  //!
  //! Note: the YUV2RGB conversions and device detection in the
  //! implementations of the IDreamboxGrabber interfaces are based
  //! on the project:
  //!
  //! AiO Dreambox Screengrabber v0.83a
  //! written 2006 - 2009 by Seddi
  //! Contact: seddi@ihad.tv / http://www.ihad.tv
  //! License: GPL-v2
  //////////////////////////////////////////////////////////////////////////

  class CImageGrabber
  {
  public:
    CImageGrabber();
    virtual ~CImageGrabber();

    virtual const CImage & GetImage();

  private:
    t_Dreambox 		GetDreamboxType();

  private:
    IDreamboxGrabber*		m_pDreamboxGrabber;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CIMAGEGRABBER_H_ */
