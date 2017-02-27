///////////////////////////////////////////////////////////////////////////////
//! @file 	CAmbientLightMode.h
//! @date	Created on: 26.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#ifndef CAMBIENTLIGHTMODE_H_
#define CAMBIENTLIGHTMODE_H_

//////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
#include "CImageReader.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! This class models the mode for Amb*L*ght-like behaviour using the
  //! current tv image.
  //////////////////////////////////////////////////////////////////////////

  class CAmbientLightMode : public IAmbientLightMode
  {
  public:
    CAmbientLightMode();
    virtual ~CAmbientLightMode();

    // ----------------------------------------------------------------- //

    virtual t_DeviceMode GetType() const { return LIGHTMODE_AMBIENTLIGHT; }

    //! This callback will be executed when all computing is completed
    virtual bool SetCallback(void* pObject,
			     fnChannelColorsAvailableCallback pCallback);

    // ----------------------------------------------------------------- //

    //! Initializes the ambient light mode
    virtual bool Init();

    //! Cleans up the ambient light mode
    virtual bool Cleanup();

    //! Starts video grabbing and color computation
    virtual bool Start();

    //! Stops video grabbing and color computation.
    virtual bool Stop();

    //! Returns true if the light mode is initialized
    virtual bool isInitialized() const { return m_bInitialized; }

    //! Returns true if the video decoding is running
    virtual bool isStarted() const { return m_bStarted; }

    // ----------------------------------------------------------------- //
  protected:
    //! This callback is executed by the CImageReader, when a new frame arrived
    static void PixelsAvailableCallback(	void* pTHIS,
						const RGBA & sPixelLED1,
						const RGBA & sPixelLED2,
						const RGBA & sPixelLED3,
						const RGBA & sPixelLED4);

  private:
    bool	m_bInitialized;
    bool 	m_bStarted;

    CImageReader	m_oImageReader;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CAMBIENTLIGHTMODE_H_ */

//////////////////////////////////////////////////////////////////////////////
