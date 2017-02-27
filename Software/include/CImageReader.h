//////////////////////////////////////////////////////////////////////////////
//! @file 	CImageReader.h
//! @date	Created on: 05.04.2011
//! @author	Falk Schilling
//////////////////////////////////////////////////////////////////////////////

#ifndef CIMAGEREADER_H_
#define CIMAGEREADER_H_

//////////////////////////////////////////////////////////////////////////////

#include "CImageGrabber.h"
#include <boost/thread.hpp>
#include <string>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////


  typedef void (*PPIXELVALUESAVAILABLECALLBACK)(	void* pObject,
							const RGBA & sPixelLED1,
							const RGBA & sPixelLED2,
							const RGBA & sPixelLED3,
							const RGBA & sPixelLED4);


  //////////////////////////////////////////////////////////////////////////
  //! This class grabs video images, takes some pixels for each LED module
  //! and notifies the CRGBLightControl object about the read configuration.
  //////////////////////////////////////////////////////////////////////////

  class CImageReader
  {
  public:
    CImageReader();
    virtual ~CImageReader();

    virtual bool isInitialized() const;
    virtual bool isStarted() 	 const;

    virtual bool Init();
    virtual bool Cleanup();

    virtual bool Start();
    virtual bool Stop();

    virtual void SetIntervall(unsigned int uiWaitIntervallInUSec) { m_uiWaitIntervallInUSec = uiWaitIntervallInUSec; }
    virtual bool SetPixelValuesAvailableCallback(	void* pObject,
							PPIXELVALUESAVAILABLECALLBACK pCallback	);

    static void WorkerThreadProc(void* pVoid);

  protected:
    bool			m_bInitialized;
    bool			m_bStarted;
    bool			m_bWorkerThreadKillEvent;
    unsigned int	m_uiWaitIntervallInUSec;
    CImageGrabber*	m_pImageGrabber;
    boost::thread*	m_pWorkerThread;

    PPIXELVALUESAVAILABLECALLBACK	m_pPixelsAvailableCallback;
    void*							m_pPixelsAvailableCallbackObject;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CIMAGEREADER_H_ */
