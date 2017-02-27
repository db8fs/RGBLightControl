//////////////////////////////////////////////////////////////////////////////
//! @file 	CImageReader.cpp
//! @date	Created on: 05.04.2011
//! @author	Falk Schilling
//////////////////////////////////////////////////////////////////////////////

#include "CImageReader.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CImageReader::CImageReader()
    : m_bInitialized(false),
      m_bStarted(false),
      m_bWorkerThreadKillEvent(false),
      m_uiWaitIntervallInUSec(200000),
      m_pImageGrabber(NULL),
      m_pWorkerThread(NULL),
      m_pPixelsAvailableCallback(NULL),
      m_pPixelsAvailableCallbackObject(NULL)
  {
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CImageReader::~CImageReader()
  {
    if(isStarted())
      {
	this->Stop();
      }

    if(isInitialized())
      {
	this->Cleanup();
      }
  }


  //////////////////////////////////////////////////////////////////////////
  //! Returns whether the initialization was successfully done.
  //////////////////////////////////////////////////////////////////////////

  bool CImageReader::isInitialized() const
  {
    return m_bInitialized;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Returns whether we are acquiring images from the grabber.
  //////////////////////////////////////////////////////////////////////////

  bool CImageReader::isStarted() const
  {
    return m_bStarted;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Initializes the image reader.
  //////////////////////////////////////////////////////////////////////////

  bool CImageReader::Init()
  {
    bool bRet = false;

    while(1)
      {
	if(isStarted())
	  {
	    break;
	  }

	if(isInitialized())
	  {
	    break;
	  }

	m_pImageGrabber = new CImageGrabber();

	if(!m_pImageGrabber)
	  {
	    break;
	  }

	m_bInitialized 	= true;
	bRet 			= true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Cleans up reserved stuff.
  //////////////////////////////////////////////////////////////////////////

  bool CImageReader::Cleanup()
  {
    bool bRet = false;

    while(1)
      {
	if(isStarted())
	  {
	    break;
	  }

	if(!isInitialized())
	  {
	    break;
	  }

	if(m_pWorkerThread)
	  {
	    delete m_pWorkerThread;
	    m_pWorkerThread = NULL;
	  }

	if(m_pImageGrabber)
	  {
	    delete m_pImageGrabber;
	    m_pImageGrabber = NULL;
	  }


	m_bInitialized	= false;
	bRet 			= true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Starts grabbing images
  //////////////////////////////////////////////////////////////////////////

  bool CImageReader::Start()
  {
    bool bRet = false;

    while(1)
      {
	if(isStarted())
	  {
	    break;
	  }

	if(!isInitialized())
	  {
	    break;
	  }

	m_bWorkerThreadKillEvent = false;
	m_pWorkerThread = new boost::thread(boost::bind(CImageReader::WorkerThreadProc, this));

	if(!m_pWorkerThread)
	  {
	    break;
	  }

	m_bStarted 	= true;
	bRet 		= true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Stops grabbing images.
  //////////////////////////////////////////////////////////////////////////

  bool CImageReader::Stop()
  {
    bool bRet = false;

    while(1)
      {
	if(!isStarted())
	  {
	    break;
	  }

	if(!isInitialized())
	  {
	    break;
	  }

	if(m_pWorkerThread)
	  {
	    m_bWorkerThreadKillEvent = true;
	    m_pWorkerThread->join();
	  }

	m_bStarted 	= false;
	bRet		= true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This member function tells the CImageReader object which function to
  //! execute when some pixels values are grabbed and available.
  //////////////////////////////////////////////////////////////////////////

  bool CImageReader::SetPixelValuesAvailableCallback(	void* pObject,
							PPIXELVALUESAVAILABLECALLBACK pCallback	)
  {
    bool bRet = true;

    if(pCallback)
      {
	m_pPixelsAvailableCallback 			= pCallback;
	m_pPixelsAvailableCallbackObject 	= pObject;
      }
    else
      {
	m_pPixelsAvailableCallback 			= NULL;
	m_pPixelsAvailableCallbackObject 	= NULL;
	bRet = false;
      }

    return bRet;
  }

  //////////////////////////////////////////////////////////////////////////
  //! This thread grabs images.
  //////////////////////////////////////////////////////////////////////////

  void CImageReader::WorkerThreadProc(void* pVoid)
  {
    CImageReader* pTHIS = (CImageReader*) pVoid;

    while(1)
      {
	if(!pTHIS)
	  {
	    break;
	  }

	if(!pTHIS->m_pImageGrabber)
	  {
	    break;
	  }

	const CImage & oImage = pTHIS->m_pImageGrabber->GetImage();

	RGBA sLED1, sLED2, sLED3, sLED4;

	unsigned short x 		= 1920/5;
	unsigned short x_step 	= 1920/5;
	unsigned short y 		= 1080/2;


	if(!oImage.GetPixel(x, y, sLED1))
	  {
	    std::cerr << "Error when reading pixel for LED1" << std::endl;
	  }

	if(!oImage.GetPixel(x + x_step, y, sLED2))
	  {
	    std::cerr << "Error when reading pixel for LED2" << std::endl;
	  }

	if(!oImage.GetPixel(x + 2*x_step, y, sLED3))
	  {
	    std::cerr << "Error when reading pixel for LED3" << std::endl;
	  }

	if(!oImage.GetPixel(x + 3*x_step, y, sLED4))
	  {
	    std::cerr << "Error when reading pixel for LED4" << std::endl;
	  }

	if(pTHIS->m_pPixelsAvailableCallback)
	  {
	    pTHIS->m_pPixelsAvailableCallback(	pTHIS->m_pPixelsAvailableCallbackObject,
						sLED1, sLED2, sLED3, sLED4);
	  }

	/*			if(oImage.SaveBitmap("/media/hdd/bitmap.bmp"))
				{
				std::cout << "Bitmap successfully saved!" << std::endl;
				}
	*/



	if(pTHIS->m_bWorkerThreadKillEvent)
	  {
	    break;
	  }

	usleep(pTHIS->m_uiWaitIntervallInUSec);
      }
  }


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
