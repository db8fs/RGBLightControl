///////////////////////////////////////////////////////////////////////////////
//! @file 	CI2CDevices.h
//! @date	Created on: 29.04.2013
//! @author	Falk Schilling
//! @license    GPLv3+
///////////////////////////////////////////////////////////////////////////////

#ifndef CI2CDEVICES_H_
#define CI2CDEVICES_H_

#include "RGBLightControl.h"
#include "CI2CSlave.h"

///////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl 
{

  /////////////////////////////////////////////////////////////////////////////

  struct CI2CAccelerometer : public CI2CSlave, public IAccelerometer
  {
  CI2CAccelerometer(const CI2CSlave & slave) : CI2CSlave(slave) {}
    virtual ~CI2CAccelerometer() {}
    
    virtual bool GetAcceleration(double & X, double & Y, double & Z) const { return false; }
    virtual bool GetTiltStatus(unsigned int & status) const { return false; }
    
  };  


  struct CI2CThermometer : public CI2CSlave, public IThermometer
  {
  CI2CThermometer(const CI2CSlave & slave) : CI2CSlave(slave) {}
    virtual ~CI2CThermometer() {}

    virtual double GetTemperature() const { return 0.0; }
  };

  /////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CI2CDEVICES_H_ */
