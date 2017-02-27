///////////////////////////////////////////////////////////////////////////////
//! @file 	CThermometerMode.h
//! @date	Created on: 26.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#ifndef THERMOMETERMODE_H_
#define THERMOMETERMODE_H_

//////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
#include "CConfiguration.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  class CThermometerMode : public IThermometerMode
  {
  public:
    CThermometerMode(IThermometer* pThermometer);
    virtual ~CThermometerMode();

    // ---------------------------------------------------------------- //

    virtual t_DeviceMode GetType() const { return LIGHTMODE_THERMOMETER; }

    virtual bool SetCallback(void* pObject,
			     fnChannelColorsAvailableCallback pCallback);

    // ---------------------------------------------------------------- //

    virtual bool SetColors(const t_Configuration & oConfig);
    virtual t_Configuration  GetColors() const;

    // ---------------------------------------------------------------- //

  protected:
    IThermometer*   m_pThermometer;
    t_Configuration m_oConfiguration;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CCONSTANTLIGHTMODE_H_ */

//////////////////////////////////////////////////////////////////////////////
