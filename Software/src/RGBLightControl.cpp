///////////////////////////////////////////////////////////////////////////////
//! @file 	RGBLightControl.cpp
//! @date	Created on: 28.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
#include "CDevice.h"
#include "CWebserver.h"
#include "CConstantLightMode.h"
#include "CSequenceLightMode.h"
#include "CAmbientLightMode.h"
#include "CAccelerometerMode.h"
#include "CThermometerMode.h"
#include "CI2CMaster.h"

///////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

#if(0)

      //! Get the interface for using ambient light mode.
    static IAmbientLightMode* 	GetAmbientLightMode();

    //! Creates an light mode object for displaying constant colors
    static IConstantLightMode*	CreateConstantLightMode();

    //! Creates an light mode object for displaying sequences
    static ISequenceLightMode* 	CreateSequenceLightMode();

    //! Creates an light mode object for accelerometer control
    static IAccelerometerMode* CreateAccelerometerMode(IAccelerometer* pAccelerometer);

    //! Creates an light mode object for thermometer control
    static IThermometerMode* CreateThermometerMode(IThermometerMode* pThermometer);


    //! Frees a device mode object
    static void FreeDeviceMode(IDeviceMode* pMode);

  //! Creates an light mode object for displaying constant colors
  IConstantLightMode*	IRGBLightControl::CreateConstantLightMode()
  {
    return new CConstantLightMode();
  }


  //! Creates an light mode object for displaying sequences
  ISequenceLightMode* IRGBLightControl::CreateSequenceLightMode()
  {
    return new CSequenceLightMode();
  }

  //! Creates an light mode object for accelerometer control
  IAccelerometerMode* IRGBLightControl::CreateAccelerometerMode(IAccelerometer* pAccelerometer)
  {
    return new CAccelerometerMode(pAccelerometer);
  }
  
  //! Creates an light mode object for thermometer control
  IThermometerMode* IRGBLightControl::CreateThermometerMode(IThermometer* pThermometer)
  {
    return new CThermometerMode(pThermometer);
  }
  

  //! Get the interface for using ambient light mode.
  IAmbientLightMode* 	IRGBLightControl::GetAmbientLightMode()
  {
    static CAmbientLightMode oAmbilight;
    return &oAmbilight;
  }


#endif

  ///////////////////////////////////////////////////////////////////////////////

  //! creates a new device object
  IDevice* 	IRGBLightControl::CreateDevice()
  {
    return new CDevice();
  }


  //! creates a new webserver object
  IWebserver*	IRGBLightControl::CreateWebserver()
  {
    return new CWebserver();
  }


  //! frees an existing device object
  void IRGBLightControl::FreeDevice(IDevice* pDevice)
  {
    if(pDevice)
      {
	delete static_cast<CDevice*>(pDevice);
      }
  }


  //! frees an existing webserver object
  void IRGBLightControl::FreeWebserver(IWebserver* pServer)
  {
    if(pServer)
      {
	delete static_cast<CWebserver*>(pServer);
      }
  }
  

  //! Creates an I²C Master
  II2CMaster* IRGBLightControl::CreateI2CMaster(const char* devicename)
  {
    return new CI2CMaster(devicename);
  }
    
  
  //! Frees an allocated I²C Master
  void IRGBLightControl::FreeI2CMaster(II2CMaster* pMaster)
  {
    if(pMaster)
      {
	delete static_cast<CI2CMaster*>(pMaster);
      }
  }
  
  
  ///////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
