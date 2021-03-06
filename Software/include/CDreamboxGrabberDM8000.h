//////////////////////////////////////////////////////////////////////////////
//! @file 	CDreamboxGrabberDM8000.h
//! @date	Created on: 06.04.2011
//! @author	falk
//////////////////////////////////////////////////////////////////////////////

#ifndef CDREAMBOXGRABBERDM8000_H_
#define CDREAMBOXGRABBERDM8000_H_

//////////////////////////////////////////////////////////////////////////////

#include "IDreamboxGrabber.h"
#include "CSystemTools.h"
#include "CColorspaceConverter.h"
#include <boost/spirit.hpp>

//////////////////////////////////////////////////////////////////////////////

using namespace boost::spirit;

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

#define VIDEO_WIDTH 	1920
#define	VIDEO_HEIGHT	1080


  //////////////////////////////////////////////////////////////////////////
  //! This class implements the interface IDreamboxGrabber for the
  //! DM8000.
  //////////////////////////////////////////////////////////////////////////

  class CDreamboxGrabberDM8000 : public IDreamboxGrabber
  {
  public:
    CDreamboxGrabberDM8000();
    virtual ~CDreamboxGrabberDM8000();

    virtual const CImage & GetImage();

  protected:
    RGBLightControl::CImage 	m_oImage;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CDREAMBOXGRABBERDM8000_H_ */
