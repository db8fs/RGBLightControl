///////////////////////////////////////////////////////////////////////////////
//! @file 	CAccelerometerMode.h
//! @date	Created on: 26.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#ifndef CACCELEROMETERMODE_H_
#define CACCELEROMETERMODE_H_

//////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
#include "CConfiguration.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  class CAccelerometerMode : public IAccelerometerMode
  {
  public:
    CAccelerometerMode(IAccelerometer* pAccelerometer);
    virtual ~CAccelerometerMode();

    // ---------------------------------------------------------------- //

    virtual t_DeviceMode GetType() const { return LIGHTMODE_ACCELEROMETER; }

    virtual bool SetCallback(void* pObject,
			     fnChannelColorsAvailableCallback pCallback);

    // ---------------------------------------------------------------- //

    virtual bool SetColors(const t_Configuration & oConfig);
    virtual t_Configuration  GetColors() const;

    // ---------------------------------------------------------------- //

  protected:
    IAccelerometer*  m_pAccelerometer;
    t_Configuration  m_oConfiguration;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CCONSTANTLIGHTMODE_H_ */

//////////////////////////////////////////////////////////////////////////////
