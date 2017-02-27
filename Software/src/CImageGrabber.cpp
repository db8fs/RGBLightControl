//////////////////////////////////////////////////////////////////////////////
//! @file 	CImageGrabber.cpp
//! @date	Created on: 06.04.2011
//! @author	Falk Schilling
//////////////////////////////////////////////////////////////////////////////

#include <boost/system/config.hpp>
#include <boost/spirit.hpp>

#include "CSystemTools.h"
#include "CImageGrabber.h"
#include "CDreamboxGrabberDM8000.h"

/*
  #include "CDreamboxGrabberDM7020.h"
  #include "CDreamboxGrabberPallas.h"
  #include "CDreamboxGrabberXilleon.h"
  #include "CDreamboxGrabberVulcan.h"
*/

//////////////////////////////////////////////////////////////////////////////

using namespace boost::spirit;

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CImageGrabber::CImageGrabber()
  {
    switch(GetDreamboxType())
      {
      case BRCM7400 :
	m_pDreamboxGrabber = new CDreamboxGrabberDM8000();
	break;

	/*
	  case BRCM7405 :
	  m_pDreamboxGrabber = new CDreamboxGrabberDM7020();
	  break;

	  case PALLAS:
	  m_pDreamboxGrabber = new CDreamboxGrabberPallas();
	  break;

	  case XILLEON:
	  m_pDreamboxGrabber = new CDreamboxGrabberXilleon();
	  break;

	  case VULCAN:
	  m_pDreamboxGrabber = new CDreamboxGrabberVulcan();
	  break;
	*/

      default:
	throw "Unknown Dreambox";
      }
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CImageGrabber::~CImageGrabber()
  {
    if(m_pDreamboxGrabber)
      {
	delete m_pDreamboxGrabber;
	m_pDreamboxGrabber = NULL;
      }
  }


  //////////////////////////////////////////////////////////////////////////
  //! Try to find out which Dreambox we have. Bases on code from the
  //! AiO Dreambox Screengrabber.
  //////////////////////////////////////////////////////////////////////////

  t_Dreambox CImageGrabber::GetDreamboxType()
  {
    t_Dreambox eDreamboxType = UNKNOWN;

#ifdef BOOST_POSIX_API
    std::string strOutput = CSystemTools::GetCommandOutput("cat /proc/fb");

    boost::spirit::parse( strOutput.c_str(),
			  (
			   (int_p >> as_lower_d["bcmfb"] ^ (int_p >> as_lower_d["bcm7401"]))[assign_a(eDreamboxType, BRCM7401)] |
			   (int_p >> as_lower_d["vulcan"])[assign_a(eDreamboxType, VULCAN)] |
			   (int_p >> as_lower_d["pallas"])[assign_a(eDreamboxType, PALLAS)] |
			   (int_p >> as_lower_d["xilleon"])[assign_a(eDreamboxType, XILLEON)]
			   ),space_p);


    // distinguish broadcom dreamboxes
    if(eDreamboxType == BRCM7401)
      {
	strOutput = CSystemTools::GetCommandOutput("cat /proc/stb/info/model");

	boost::spirit::parse( strOutput.c_str(), (
						  (as_lower_d["dm8000"])[assign_a(eDreamboxType, BRCM7400)] 	|
						  (as_lower_d["dm7020hd"])[assign_a(eDreamboxType, BRCM7405)] |
						  (as_lower_d["dm500hd"])[assign_a(eDreamboxType, BRCM7405)] 	|
						  (as_lower_d["dm800se"])[assign_a(eDreamboxType, BRCM7405)]
						  ),space_p);
      }

    std::cout << "Found Chipset: ";

    switch(eDreamboxType)
      {
      case BRCM7400 :
	std::cout << "DM8000" << std::endl;
	break;
      case BRCM7405 :
	std::cout << "DM800SE, DM500HD, DM7020HD" << std::endl;
	break;
      case PALLAS:
	std::cout << "PALLAS" << std::endl;
	break;
      case XILLEON:
	std::cout << "XILLEON" << std::endl;
	break;
      case VULCAN:
	std::cout << "VULCAN" << std::endl;
	break;
      default:
	std::cout << "UNKNOWN" << std::endl;
      }

#endif

    return eDreamboxType;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This method acquires an image and returns it to the caller.
  //////////////////////////////////////////////////////////////////////////

  const CImage & CImageGrabber::GetImage()
  {
    return m_pDreamboxGrabber->GetImage();
  }




  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

