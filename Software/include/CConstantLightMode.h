///////////////////////////////////////////////////////////////////////////////
//! @file 	CConstantLightMode.h
//! @date	Created on: 26.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#ifndef CCONSTANTLIGHTMODE_H_
#define CCONSTANTLIGHTMODE_H_

//////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
#include "CConfiguration.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  class CConstantLightMode : public IConstantLightMode
  {
  public:
    CConstantLightMode();
    virtual ~CConstantLightMode();

    // ---------------------------------------------------------------- //

    virtual t_DeviceMode GetType() const { return LIGHTMODE_CONSTANTCOLOR; }

    virtual bool SetCallback(void* pObject,
			     fnChannelColorsAvailableCallback pCallback);

    // ---------------------------------------------------------------- //

    virtual bool SetColors(const t_Configuration & oConfig);
    virtual t_Configuration  GetColors() const;

    // ---------------------------------------------------------------- //

  protected:
    t_Configuration 	m_oConfiguration;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CCONSTANTLIGHTMODE_H_ */

//////////////////////////////////////////////////////////////////////////////
