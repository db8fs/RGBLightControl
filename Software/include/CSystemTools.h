//////////////////////////////////////////////////////////////////////////////
//! @file 	CSystemTools.h
//! @date	Created on: 08.04.2011
//! @author	falk
//////////////////////////////////////////////////////////////////////////////

#ifndef CSYSTEMTOOLS_H_
#define CSYSTEMTOOLS_H_

//////////////////////////////////////////////////////////////////////////////

#include <string>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Provides functions for doing some operating system stuff.
  //////////////////////////////////////////////////////////////////////////

  class CSystemTools
  {
  public:
    static 	std::string		GetCommandOutput(std::string oCmd);
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CSYSTEMTOOLS_H_ */
