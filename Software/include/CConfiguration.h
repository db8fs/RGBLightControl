//////////////////////////////////////////////////////////////////////////////
//! @file 	CConfiguration.h
//! @date	Created on: 04.04.2011
//! @author	Falk Schilling
//////////////////////////////////////////////////////////////////////////////

#ifndef CCONFIGURATION_H_
#define CCONFIGURATION_H_

//////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
#include "CBaseConfiguration.h"
#include "CDisplayMode.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! This class encapsulates the configuration for all LED modules in one
  //! object.
  //////////////////////////////////////////////////////////////////////////

  class CConfiguration : public CBaseConfiguration
  {
  public:
    CConfiguration();
    CConfiguration(const CConfiguration & rhs);
    CConfiguration(t_Configuration oConfig);

    explicit CConfiguration( CLEDConfig oLED1, CLEDConfig oLED2,
			     CLEDConfig oLED3, CLEDConfig oLED4);
    virtual ~CConfiguration();

    virtual const CDisplayMode & GetDisplayMode() const 	{ return m_oDisplayMode; }
    virtual void SetDisplayMode(const CDisplayMode & rhs) 	{ m_oDisplayMode = rhs;	 }

    using CBaseConfiguration::begin;
    using CBaseConfiguration::end;

    friend std::ostream &operator<<(std::ostream & oStream, const CConfiguration & cfg);
    CConfiguration & operator=(const CConfiguration & rhs);

    CConfiguration & operator()(CLEDConfig oLED1, CLEDConfig oLED2,
				CLEDConfig oLED3, CLEDConfig oLED4);

  protected:
    CDisplayMode	m_oDisplayMode;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl


//////////////////////////////////////////////////////////////////////////////

#endif /* CCONFIGURATION_H_ */
