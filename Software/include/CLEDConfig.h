//////////////////////////////////////////////////////////////////////////////
//! @file 	CLEDConfig.h
//! @date	Created on: 05.04.2011
//! @author	falk
//////////////////////////////////////////////////////////////////////////////

#ifndef CLEDCONFIG_H_
#define CLEDCONFIG_H_

///////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  ///////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  //! Objects of this class represent the configuration for one single
  //! LED module.
  ///////////////////////////////////////////////////////////////////////////

  struct CLEDConfig : public t_LEDConfig
  {
    CLEDConfig();
    CLEDConfig(const t_LEDConfig & rhs);
    CLEDConfig(short nVal1, short nVal2, short nVal3, bool bHSI = false);
    CLEDConfig(const CLEDConfig & rhs);

    friend std::ostream &operator<<(std::ostream & oStream, const CLEDConfig & cfg);
    CLEDConfig & operator=(const CLEDConfig & rhs);

    CLEDConfig & operator()();
    CLEDConfig & operator()(short nVal1, short nVal2, short nVal3, bool bHSI = false);
  };


  ///////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

///////////////////////////////////////////////////////////////////////////////

#endif /* CLEDCONFIG_H_ */
