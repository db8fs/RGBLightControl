///////////////////////////////////////////////////////////////////////////////
//! @file 	CI2CMaster.h
//! @date	Created on: 29.04.2013
//! @author	Falk Schilling
//! @license    GPLv3+
///////////////////////////////////////////////////////////////////////////////

#ifndef CI2CMASTER_H_
#define CI2CMASTER_H_

#include <list>
#include <vector>
#include "RGBLightControl.h"

///////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl 
{

  /////////////////////////////////////////////////////////////////////////////
  
  class CI2CSlave;

  class CI2CMaster : public II2CMaster
    {
    public:
      CI2CMaster(const char* devicename);
      virtual ~CI2CMaster();      
    
      //! Enumerates all I²C slaves
      virtual bool Scan( II2CSlave** dev, int* num, unsigned long filter = -1);

      //! Raw I²C access for reading data into a buffer
      virtual bool ReadData( II2CSlave* slave, unsigned char* data, int len);
      
      //! Raw I²C access for sending data from a buffer
      virtual bool SendData( II2CSlave* slave, unsigned char* data, int len);
      
      //! Creates an I²C-Accelerometer
      virtual IAccelerometer*  GetAccelerometer( II2CSlave* slave );
      
      //! Creates an I²C-Thermometer
      virtual IThermometer* GetThermometer( II2CSlave* slave );
      
    private:
      unsigned long prvFilterSlave( int slaveid, unsigned long filter );
      
      bool prvCheckAccelerometer(int slaveid);
      bool prvCheckThermometer(int slaveid);
      void prvClean();
      
      int m_I2CDevice;      
      
      std::vector<CI2CSlave*>    m_Slaves;
      std::list<IThermometer*>   m_Thermometers;
      std::list<IAccelerometer*> m_Accelerometers;
    };
  


  /////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CI2CMASTER_H_ */
