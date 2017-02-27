///////////////////////////////////////////////////////////////////////////////
//! @file 	CAmbientLightMode.cpp
//! @date	Created on: 26.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#include "CAmbientLightMode.h"
#include "RGBLightControl.h"
#include "CLEDConfig.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CAmbientLightMode::CAmbientLightMode()
    : m_bInitialized(false),
      m_bStarted(false),
      m_oImageReader()
  {
    this->m_pCallback 		= NULL;
    this->m_pCallbackObject = NULL;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CAmbientLightMode::~CAmbientLightMode()
  {
    if(m_bStarted)
      {
	(void) Stop();
      }

    if(m_bInitialized)
      {
	(void) Cleanup();
      }
  }


  //////////////////////////////////////////////////////////////////////////
  //! Initializes the ambient light mode
  //////////////////////////////////////////////////////////////////////////

  bool CAmbientLightMode::Init()
  {
    bool bRet = false;

    while(1)
      {
	if(m_bInitialized)
	  {
	    break;
	  }

	if(!m_oImageReader.Init())
	  {
	    std::cerr << "Could not initialize ImageReader!" << std::endl;
	    break;
	  }

	m_oImageReader.SetIntervall(1000000/10);

	m_oImageReader.SetPixelValuesAvailableCallback((void*) this,
						       (PPIXELVALUESAVAILABLECALLBACK) & CAmbientLightMode::PixelsAvailableCallback);


	m_bInitialized = true;
	bRet = true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  bool CAmbientLightMode::Cleanup()
  {
    bool bRet = false;

    while(1)
      {
	if(!m_bInitialized)
	  {
	    break;
	  }

	if(!m_oImageReader.Cleanup())
	  {
	    break;
	  }

	m_bInitialized = false;
	bRet = true;
	break;
      }

    return bRet;
  }



  //////////////////////////////////////////////////////////////////////////
  //! This method starts the delivering of Configurations for all LED
  //! channels.
  //////////////////////////////////////////////////////////////////////////

  bool CAmbientLightMode::Start()
  {
    bool bRet = false;

    while(1)
      {
	if(!m_bInitialized)
	  {
	    break;
	  }

	if(m_bStarted)
	  {
	    break;
	  }

	if(!m_oImageReader.Start())
	  {
	    break;
	  }

	m_bStarted = true;
	bRet = true;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This method stops the delivering of Configurations for all LED
  //! channels
  //////////////////////////////////////////////////////////////////////////

  bool CAmbientLightMode::Stop()
  {
    bool bRet = false;

    while(1)
      {
	if(!m_bStarted)
	  {
	    break;
	  }

	if(!m_oImageReader.Stop())
	  {
	    break;
	  }


	m_bStarted = false;
	bRet = true;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Set a callback to execute for applying the calculated color set
  //////////////////////////////////////////////////////////////////////////

  bool CAmbientLightMode::SetCallback(void* pObject,
				      fnChannelColorsAvailableCallback pCallback)
  {
    bool bRet = true;

    if(pCallback)
      {
	this->m_pCallback 		= pCallback;
	this->m_pCallbackObject = pObject;
      }
    else
      {
	this->m_pCallback 		= NULL;
	this->m_pCallbackObject = NULL;
	bRet = false;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This callback function will be executed by the CImageReader object
  //! when the grabbing of pixels is complete.
  //////////////////////////////////////////////////////////////////////////

  void CAmbientLightMode::PixelsAvailableCallback(void* pObject,
						  const RGBA & sPixelLED1,
						  const RGBA & sPixelLED2,
						  const RGBA & sPixelLED3,
						  const RGBA & sPixelLED4)
  {
    CAmbientLightMode* pTHIS = (CAmbientLightMode*)pObject;

    while(1)
      {
	if(!pTHIS)
	  {
	    break;
	  }

	unsigned char ucHue[4];
	unsigned char ucSaturation[4];
	unsigned char ucValue[4];

	sPixelLED1.GetHSI(ucHue[0], ucSaturation[0], ucValue[0]);
	sPixelLED2.GetHSI(ucHue[1], ucSaturation[1], ucValue[1]);
	sPixelLED3.GetHSI(ucHue[2], ucSaturation[2], ucValue[2]);
	sPixelLED4.GetHSI(ucHue[3], ucSaturation[3], ucValue[3]);

	short iHueAverage 			= 0;
	short iHueCounter			= 0;
	short iSaturationCounter 	= 0;
	short iAverageValue			= 0;

	for(int i=0; i<4; ++i)
	  {
	    // let the unsaturated colors be shown as white

	    if(ucSaturation[i] < 80)
	      {
		iSaturationCounter++;
	      }

	    // average the hues for determining the resulting color

	    iHueAverage += ucHue[i];
	    iHueCounter++;

	    iAverageValue += ucValue[i];
	  }

	t_Configuration sConf;

	if(iSaturationCounter >= 2 || iHueCounter == 0)
	  {
	    CLEDConfig 	oWhite(127, 127, 127, false);

	    sConf.sLED1 = (t_LEDConfig) oWhite;
	    sConf.sLED2 = (t_LEDConfig) oWhite;
	    sConf.sLED3 = (t_LEDConfig) oWhite;
	    sConf.sLED4 = (t_LEDConfig) oWhite;
	  }
	else
	  {
	    iHueAverage 	/=  iHueCounter;
	    iAverageValue 	/=	4;

	    CLEDConfig 	oColor(iHueAverage, 127, 127, true);

	    sConf.sLED1 = (t_LEDConfig) oColor;
	    sConf.sLED2 = (t_LEDConfig) oColor;
	    sConf.sLED3 = (t_LEDConfig) oColor;
	    sConf.sLED4 = (t_LEDConfig) oColor;
	  }

	if(pTHIS->m_pCallback)
	  {
	    pTHIS->m_pCallback( pTHIS->m_pCallbackObject, sConf);
	  }

	break;
      }
  }


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
