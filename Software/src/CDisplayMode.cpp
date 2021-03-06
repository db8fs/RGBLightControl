//////////////////////////////////////////////////////////////////////////////
//! @file 	CDisplayMode.cpp
//! @date	Created on: 05.04.2011
//! @author	falk
//////////////////////////////////////////////////////////////////////////////

#include "CDisplayMode.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CDisplayMode::CDisplayMode(t_DisplayMode eMode)
  {
    m_eMode = eMode;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CDisplayMode::~CDisplayMode()
  {
  }


  //////////////////////////////////////////////////////////////////////////
  //! Functor
  //////////////////////////////////////////////////////////////////////////

  char CDisplayMode::operator()() const
  {
    return static_cast<char>(m_eMode);
  }

  //////////////////////////////////////////////////////////////////////////
  //! Functor
  //////////////////////////////////////////////////////////////////////////

  CDisplayMode & CDisplayMode::operator()(t_DisplayMode eMode)
  {
    m_eMode = eMode;
    return *this;
  }


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
