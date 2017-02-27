//////////////////////////////////////////////////////////////////////////////
//! @file 	CLEDConfig.cpp
//! @date	Created on: 05.04.2011
//! @author	falk
//////////////////////////////////////////////////////////////////////////////

#include "CLEDConfig.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CLEDConfig::CLEDConfig()
  {
    m_cValue1	= '\0';
    m_cValue2	= '\0';
    m_cValue3	= '\0';
    m_bEnabled	= false;
    m_bHSIMode	= false;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Construction with arguments. Note that the passed integers will be
  //! converted into characters. 0 is minimum, 127 is maximum (except the
  //! hue component with maximum 180).
  //////////////////////////////////////////////////////////////////////////

  CLEDConfig::CLEDConfig(short nVal1, short nVal2, short nVal3, bool bHSI)
  {
    m_cValue1 	= 	static_cast<char>((unsigned short)0xFF & nVal1);
    m_cValue2 	= 	static_cast<char>((unsigned short)0xFF & nVal2);
    m_cValue3 	= 	static_cast<char>((unsigned short)0xFF & nVal3);
    m_bHSIMode 	= 	bHSI;
    m_bEnabled 	= 	true;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Upcast
  //////////////////////////////////////////////////////////////////////////

  CLEDConfig::CLEDConfig(const t_LEDConfig & rhs)
  {
    m_cValue1 	= rhs.m_cValue1;
    m_cValue2 	= rhs.m_cValue2;
    m_cValue3 	= rhs.m_cValue3;
    m_bHSIMode 	= rhs.m_bHSIMode;
    m_bEnabled 	= rhs.m_bEnabled;
  }

  //////////////////////////////////////////////////////////////////////////
  //! Copy Construction
  //////////////////////////////////////////////////////////////////////////

  CLEDConfig::CLEDConfig(const CLEDConfig & rhs)
  {
    m_cValue1 	= rhs.m_cValue1;
    m_cValue2 	= rhs.m_cValue2;
    m_cValue3 	= rhs.m_cValue3;
    m_bHSIMode 	= rhs.m_bHSIMode;
    m_bEnabled 	= rhs.m_bEnabled;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Use this functor to disable the LED.
  //////////////////////////////////////////////////////////////////////////

  CLEDConfig & CLEDConfig::operator()()
  {
    m_bEnabled = false;

    return *this;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Use this functor to enable the LED and quickly set its value to a
  //! RGB or HSI triple.
  //////////////////////////////////////////////////////////////////////////

  CLEDConfig & CLEDConfig::operator()(short nVal1, short nVal2, short nVal3, bool bHSI)
  {
    m_cValue1 	= 	static_cast<char>((unsigned short)0xFF & nVal1);
    m_cValue2 	= 	static_cast<char>((unsigned short)0xFF & nVal2);
    m_cValue3 	= 	static_cast<char>((unsigned short)0xFF & nVal3);
    m_bEnabled 	= 	true;
    m_bHSIMode	= 	bHSI;

    return *this;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This is the overwritten assignment operator for configurations.
  //////////////////////////////////////////////////////////////////////////

  CLEDConfig& CLEDConfig::operator=(const CLEDConfig & rhs)
  {
    if(this != &rhs)
      {
	m_cValue1 	= rhs.m_cValue1;
	m_cValue2 	= rhs.m_cValue2;
	m_cValue3 	= rhs.m_cValue3;
	m_bHSIMode 	= rhs.m_bHSIMode;
	m_bEnabled 	= rhs.m_bEnabled;
      }

    return *this;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This is the overloaded operator for outputting into an output stream.
  //////////////////////////////////////////////////////////////////////////

  std::ostream &operator<<(std::ostream & oStream, const CLEDConfig & oCfg)
  {
    while(1)
      {
	if(!oCfg.m_bEnabled)
	  {
	    oStream << "not changed";
	    break;
	  }

	if(oCfg.m_bHSIMode)
	  {
	    oStream << "HSI(";
	  }
	else
	  {
	    oStream << "RGB(";
	  }

	oStream << (unsigned short) (0xFF & oCfg.m_cValue1) << ","
		<< (unsigned short) (0xFF & oCfg.m_cValue2) << ","
		<< (unsigned short) (0xFF & oCfg.m_cValue3) << ")";

	break;
      }

    return oStream;
  }


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
