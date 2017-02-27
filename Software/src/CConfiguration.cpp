///////////////////////////////////////////////////////////////////////////////
//! @file 	CConfiguration.cpp
//! @date	Created on: 04.04.2011
//! @author	Falk Schilling
///////////////////////////////////////////////////////////////////////////////

#include "CConfiguration.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CConfiguration::CConfiguration() : CBaseConfiguration()
  {
    std::fill(begin(), end(), CLEDConfig() );
  }


  //////////////////////////////////////////////////////////////////////////
  //! Construction with initialization
  //////////////////////////////////////////////////////////////////////////

  CConfiguration::CConfiguration(	CLEDConfig oLED1, CLEDConfig oLED2,
					CLEDConfig oLED3, CLEDConfig oLED4)
  {
    m_aoConfig[0] = oLED1;
    m_aoConfig[1] = oLED2;
    m_aoConfig[2] = oLED3;
    m_aoConfig[3] = oLED4;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Type conversion
  //////////////////////////////////////////////////////////////////////////

  CConfiguration::CConfiguration(t_Configuration oConfig)
  {
    m_aoConfig[0] = CLEDConfig(oConfig.sLED1);
    m_aoConfig[1] = CLEDConfig(oConfig.sLED2);
    m_aoConfig[2] = CLEDConfig(oConfig.sLED3);
    m_aoConfig[3] = CLEDConfig(oConfig.sLED4);
  }


  //////////////////////////////////////////////////////////////////////////
  //! Copy Construction
  //////////////////////////////////////////////////////////////////////////

  CConfiguration::CConfiguration(const CConfiguration & rhs)
  {
    std::copy(rhs.begin(), rhs.end(), this->begin());
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CConfiguration::~CConfiguration()
  {
  }


  //////////////////////////////////////////////////////////////////////////
  //! Use this functor to enable the LED and quickly set its value to a
  //! RGB or HSI triple.
  //////////////////////////////////////////////////////////////////////////

  CConfiguration & CConfiguration::operator()(CLEDConfig oLED1, CLEDConfig oLED2,
					      CLEDConfig oLED3, CLEDConfig oLED4)
  {
    m_aoConfig[0] = oLED1;
    m_aoConfig[1] = oLED2;
    m_aoConfig[2] = oLED3;
    m_aoConfig[3] = oLED4;

    return *this;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This is the overwritten assignment operator for configurations.
  //////////////////////////////////////////////////////////////////////////

  CConfiguration & CConfiguration::operator=(const CConfiguration & rhs)
  {
    if(this != &rhs)
      {
	std::copy(rhs.begin(), rhs.end(), this->begin());
      }

    return *this;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This is the overloaded operator for outputting the triples of
  //! a configuration in an output stream.
  //////////////////////////////////////////////////////////////////////////

  std::ostream &operator<<(std::ostream & oStream, const CConfiguration & oCfg)
  {
    short nCounter = 0;

    for(CConfiguration::const_iterator it = oCfg.begin(); it != oCfg.end(); it++)
      {
	oStream << "LED"<< ++nCounter << ": "<< (*it) << std::endl;
      }

    return oStream;
  }


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
