///////////////////////////////////////////////////////////////////////////////
//! @file 	CAccelerometerMode.cpp
//! @date	Created on: 26.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#include "CAccelerometerMode.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CAccelerometerMode::CAccelerometerMode(IAccelerometer* pAccelerometer)
    : m_pAccelerometer(pAccelerometer)
  {
    this->m_pCallback 		= NULL;
    this->m_pCallbackObject = NULL;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CAccelerometerMode::~CAccelerometerMode()
  {
  }


  //////////////////////////////////////////////////////////////////////////
  //! Sets a constant color for the RGBLightControl
  //////////////////////////////////////////////////////////////////////////

  bool CAccelerometerMode::SetColors(const t_Configuration & oConfig)
  {
    m_oConfiguration = oConfig;

    if(!m_pCallback) return false;

    m_pCallback(m_pCallbackObject, m_oConfiguration);
  }


  //////////////////////////////////////////////////////////////////////////
  //! Returns the current set color.
  //////////////////////////////////////////////////////////////////////////

  t_Configuration CAccelerometerMode::GetColors() const
  {
    return m_oConfiguration;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Set a callback to execute for applying the calculated color set
  //////////////////////////////////////////////////////////////////////////

  bool CAccelerometerMode::SetCallback(void* pObject,
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



  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
