//////////////////////////////////////////////////////////////////////////////
//! @file 	CRGBLightControl.cpp
//! @date	Created on: 31.03.2011
//! @author	Falk Schilling
//////////////////////////////////////////////////////////////////////////////

#include "CDevice.h"
#include <string>
#include <iomanip>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl{

  //////////////////////////////////////////////////////////////////////////////

  static CLEDConfig RGB(127,0,0);
  static CLEDConfig HSI(170,0,127,true);

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CDevice::CDevice() :
    m_bInitialized(false),
    m_bRunning(false),
    m_pSerialPort(NULL),
    m_pDeviceMode(NULL),
    m_pIOServiceSerialPortThread(NULL)
  {
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CDevice::~CDevice()
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
  //! This function initializes this CRGBLightControl object.
  //////////////////////////////////////////////////////////////////////////

  bool CDevice::Init(const char* strDevice,
		     unsigned int uiBaudRate)
  {
    bool bRet = false;

    while (1)
      {
	if(isStarted())
	  {
	    break;
	  }

	if(isInitialized())
	  {
	    break;
	  }

	try
	  {
	    m_pSerialPort = new CSerialPort(m_oIOServiceSerialPort, uiBaudRate,
					    serial_port_base::flow_control::none, strDevice);

	  } catch (...)
	  {
	    std::cerr << "Error when initializing RGBLightControl object!"
		      << std::endl;
	    break;
	  }

	if (!m_pSerialPort)
	  {
	    break;
	  }

	m_bInitialized = true;
	bRet = true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This function cleans up dynamically allocated memory and stuff.
  //////////////////////////////////////////////////////////////////////////

  bool CDevice::Cleanup()
  {
    bool bRet = false;

    while (1)
      {
	if (isStarted())
	  {
	    break;
	  }

	if (!isInitialized())
	  {
	    break;
	  }

	if (m_pSerialPort)
	  {
	    delete m_pSerialPort;
	    m_pSerialPort = NULL;
	  }

	if(m_pIOServiceSerialPortThread)
	  {
	    delete m_pIOServiceSerialPortThread;
	    m_pIOServiceSerialPortThread = NULL;
	  }

	m_bInitialized = false;
	bRet = true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This operation sets an operation mode for this device.
  //////////////////////////////////////////////////////////////////////////

  void CDevice::SetDeviceMode(IDeviceMode* pMode)
  {
    if(pMode)
      {
	if(m_pDeviceMode)
	  m_pDeviceMode->SetCallback(NULL, NULL);

	m_pDeviceMode = pMode;
	m_pDeviceMode->SetCallback(this, &CDevice::LEDConfigurationAvailableCallback );
      }
  }


  //////////////////////////////////////////////////////////////////////////
  //! Returns a pointer to the current device mode.
  //////////////////////////////////////////////////////////////////////////

  IDeviceMode* CDevice::GetDeviceMode()
  {
    return m_pDeviceMode;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This operation starts the communication with the RGBLightControl.
  //! Before you may send or receive messages, you have to successfully
  //! execute this method to start the IOService that does all the
  //! communication stuff. Returns true on success.
  //////////////////////////////////////////////////////////////////////////

  bool CDevice::Start()
  {
    bool bRet = false;

    while (1)
      {
	if(isStarted())
	  {
	    break;
	  }

	if (!isInitialized())
	  {
	    break;
	  }

	m_pSerialPort->SetReadCompletionCallback((void*) this,
						 (PREADCOMPLETECALLBACK) &CDevice::SerialPortReadCompleteHandler);

	m_pSerialPort->SetWriteCompletionCallback((void*) this,
						  (PWRITECOMPLETECALLBACK) &CDevice::SerialPortWriteCompleteHandler);

	m_pIOServiceSerialPortThread = new boost::thread(boost::bind(
								     &boost::asio::io_service::run, &m_oIOServiceSerialPort));

	if (!m_pIOServiceSerialPortThread)
	  {
	    break;
	  }

	m_bRunning = true;
	bRet = true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This operation stops the communication with the RGBLightControl.
  //////////////////////////////////////////////////////////////////////////

  bool CDevice::Stop()
  {
    bool bRet = false;

    while (1)
      {
	if (!isStarted())
	  {
	    break;
	  }

	if (!isInitialized())
	  {
	    break;
	  }

	if(m_pIOServiceSerialPortThread && m_pSerialPort)
	  {
	    m_pSerialPort->Close();
	    m_pIOServiceSerialPortThread->join();
	  }

	m_bRunning = false;
	bRet = true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This function sends the argument object via the serial port to the
  //! RGBLightControl system.
  //////////////////////////////////////////////////////////////////////////

  bool CDevice::SendControlMessage( const RGBLightControl::CMessage & oMessage)
  {
    bool bRet = false;

    while (1)
      {
	if (!isInitialized())
	  {
	    break;
	  }

	if (!isStarted())
	  {
	    break;
	  }

	// send some bytes via UART

	for (CMessage::const_iterator it = oMessage.begin(); it != oMessage.end(); it++)
	  {
	    m_pSerialPort->Write(*it);
	  }

	bRet = true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Returns true if the object was initialized.
  //////////////////////////////////////////////////////////////////////////

  bool CDevice::isInitialized() const
  {
    return m_bInitialized;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Returns true if the IOServices are still running and everything's
  //! good.
  //////////////////////////////////////////////////////////////////////////

  bool CDevice::isStarted() const
  {
    bool bRet = false;

    while (1)
      {
	if (!m_bRunning)
	  {
	    break;
	  }

	if (!m_pSerialPort)
	  {
	    break;
	  }

	if (!m_pSerialPort->IsActive())
	  {
	    break;
	  }

	bRet = true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This callback function will be executed by the CSerialPort object
  //! when a read operation has finished.
  //////////////////////////////////////////////////////////////////////////

  void CDevice::SerialPortReadCompleteHandler(void* pObject,
					      const char* pReadMsg, size_t nBytesTransferred)
  {
    CDevice* pTHIS = NULL;

    while (1)
      {
	if (!pObject)
	  {
	    break;
	  }

	pTHIS = (CDevice*) pObject;

	if (!pTHIS->m_bInitialized)
	  {
	    break;
	  }

	if (!pTHIS->m_bRunning)
	  {
	    break;
	  }

	std::string strReceivedMessage;

	char* pszString = new char[nBytesTransferred + 1];

	memcpy(pszString, pReadMsg, nBytesTransferred);
	pszString[nBytesTransferred] = '\0';

	strReceivedMessage = std::string(pszString);

	if (pszString)
	  {
	    delete[] pszString;
	    pszString = NULL;
	  }

	/*std::cout << "RGBLightControl says: " << strReceivedMessage
	  << std::endl;
	*/
	break;
      }
  }

  //////////////////////////////////////////////////////////////////////////
  //! This callback function will be executed by the CSerialPort object
  //! when a write operation has finished.
  //////////////////////////////////////////////////////////////////////////

  void CDevice::SerialPortWriteCompleteHandler(void* pObject)
  {
    CDevice* pTHIS = NULL;

    while (1)
      {
	if (!pObject)
	  {
	    break;
	  }

	pTHIS = (CDevice*) pObject;

	if (!pTHIS->m_bInitialized)
	  {
	    break;
	  }

	if (!pTHIS->m_bRunning)
	  {
	    break;
	  }

	break;
      }
  }


  //////////////////////////////////////////////////////////////////////////
  //! This callback function will be executed when the CModeManager
  //! computed a new LED configuration according to the used mode.
  //////////////////////////////////////////////////////////////////////////

  void CDevice::LEDConfigurationAvailableCallback
  (void* pObject, const t_Configuration & oConfiguration )
  {
    CDevice* pTHIS = NULL;

    while (1)
      {
	if (!pObject)
	  {
	    break;
	  }

	pTHIS = (CDevice*) pObject;

	if (!pTHIS->m_bInitialized)
	  {
	    break;
	  }

	if (!pTHIS->m_bRunning)
	  {
	    break;
	  }

	// send the configuration via UART
	pTHIS->SendControlMessage(CMessage(oConfiguration));

	break;
      }

  }


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
