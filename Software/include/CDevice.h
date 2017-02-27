//////////////////////////////////////////////////////////////////////////////
//! @file 	CRGBLightControl.h
//! @date	Created on: 01.04.2011
//! @author	Falk Schilling
//////////////////////////////////////////////////////////////////////////////

#ifndef CRGBLIGHTCONTROL_H_
#define CRGBLIGHTCONTROL_H_

//////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
#include "CSerialPort.h"
#include "CMessage.h"
#include <boost/thread.hpp>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {
  
  //////////////////////////////////////////////////////////////////////////////
  
  //////////////////////////////////////////////////////////////////////////
  //! Objects of thisclass realizes the communication with the DB8FS
  //! RGBLightControl system.
  //////////////////////////////////////////////////////////////////////////

  class CDevice : public IDevice
  {
  public:
    CDevice();
    virtual ~CDevice();

    virtual bool isInitialized() 	const;
    virtual bool isStarted() 		const;

    virtual bool Init(	const char* strDevice,
			unsigned int uiBaudRate);
    virtual bool Cleanup();

    virtual void SetDeviceMode(IDeviceMode* pMode);
    virtual IDeviceMode* GetDeviceMode();

    virtual bool Start();
    virtual bool Stop();

    virtual bool SendControlMessage(const RGBLightControl::CMessage & oMsg);

  protected:
    static void SerialPortReadCompleteHandler( 	void* pTHIS,
						const char* pReadMsg,
						size_t nBytesTransferred);

    static void SerialPortWriteCompleteHandler( void* pTHIS );

    static void LEDConfigurationAvailableCallback( void* pTHIS,
						   const t_Configuration & oConfiguration );

  private:
    bool 	    m_bInitialized;
    bool	    m_bRunning;
    io_service	    m_oIOServiceSerialPort;
    io_service	    m_oIOServiceNetworkServer;
    CSerialPort*    m_pSerialPort;
    IDeviceMode*    m_pDeviceMode;
    boost::thread*  m_pIOServiceSerialPortThread;
  };
  
  
  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CRGBLIGHTCONTROL_H_ */
