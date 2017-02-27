///////////////////////////////////////////////////////////////////////////////
//! @file 	CI2CMaster.cpp
//! @date	Created on: 29.04.2013
//! @author	Falk Schilling
//! @license    GPLv3+
///////////////////////////////////////////////////////////////////////////////

#include "CI2CMaster.h"
#include "CI2CSlave.h"
#include "CI2CDevices.h"

#include <config.h>
#include <iostream>
#include <cstdlib>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#ifdef HAVE_LINUX
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#endif

#ifndef OK
#define OK 0
#endif

#define I2C_TYPE_ACCELEROMETER II2CSlave::I2C_TYPE_ACCELEROMETER
#define I2C_TYPE_THERMOMETER   II2CSlave::I2C_TYPE_THERMOMETER
#define I2C_TYPE_UNKNOWN       II2CSlave::I2C_TYPE_UNKNOWN

 ///////////////////////////////////////////////////////////////////////////////

 namespace RGBLightControl 
 {

   /////////////////////////////////////////////////////////////////////////////

   CI2CMaster::CI2CMaster(const char* deviceName)
   {
     m_I2CDevice = open(deviceName, O_RDWR);

     if (m_I2CDevice == -1)
       {
	 std::cerr << "Error opening device '" << deviceName << "'" << std::endl;
	 exit(-1);
       }

     std::cout << "I2C device '"<< deviceName << "'successfully opened!" << std::endl;
   }


   /////////////////////////////////////////////////////////////////////////////


   CI2CMaster::~CI2CMaster()
   {
     prvClean();

     close(m_I2CDevice);
     m_I2CDevice = 0;

     std::cout << "I2C device successfully closed!" << std::endl;
   }


   /////////////////////////////////////////////////////////////////////////////
   //! Enumerates all I²C slaves

   bool CI2CMaster::Scan( II2CSlave** dev, int* num, unsigned long filter)
   {
     int errcode = OK;
     struct i2c_smbus_ioctl_data data = {0};		 
     
     prvClean();
     
     if(filter != 0UL && dev != NULL && num != NULL)
       {
	 // probe all possible slaves for response and try to figure out device types

	 std::cout << "Scanning I2C bus... " << std::endl;

#ifdef HAVE_LINUX
       
	 for(int i=3; i<128; ++i)
	   {
	     errcode = ioctl( m_I2CDevice, I2C_SLAVE, i);
	     
	     if(errcode != OK)
	       {
		 // slave address could not be selected
		 continue;
	       }
	     
	     data.size = I2C_SMBUS_QUICK;
	     data.read_write = I2C_SMBUS_WRITE;
	     
	     errcode = ioctl( m_I2CDevice, I2C_SMBUS, &data);
	     
	     if(errcode != OK)
	       {
		 // no response to request therefore no device access possible
		 continue;
	       }

	     unsigned int type = prvFilterSlave( i, filter );
	     
	     if(type != 0)
	       {
		 std::cout << "Device found at \t0x" << std::hex << i
			   << "  Type: ";
		 
		 switch(type)
		   {
		   case I2C_TYPE_ACCELEROMETER:
		     std::cout << "Accelerometer" ;
		     break;
		   case I2C_TYPE_THERMOMETER:
		     std::cout << "Thermometer";
		     break;
		   default:
		     std::cout << "Unknown";
		     break;
		   }
		 
		 std::cout << std::endl;
		 
		 CI2CSlave* pSlave = new CI2CSlave(this, i, type);
		 
		 m_Slaves.push_back( pSlave );
	       }
	   }
#endif	 
	 
	 *num = m_Slaves.size();
	 *dev = (II2CSlave*) new II2CSlave*[*num];
	 
	 int i=0;
	 
	 for(std::vector<CI2CSlave*>::iterator it = m_Slaves.begin(); it != m_Slaves.end(); ++it)
	   {
	     *(dev + i) = static_cast<II2CSlave*> (*it);
	   }

       }
    
     return m_Slaves.size() > 0;
   }
   
   
   ////////////////////////////////////////////////////////////////////////////
   //! Applies some extra queries for determining the device type of the given
   //! slave.
   
   unsigned long CI2CMaster::prvFilterSlave( int slaveid, unsigned long filter )
   {
     unsigned long result = 0UL;

     if( filter & I2C_TYPE_UNKNOWN)
       {
	 result = I2C_TYPE_UNKNOWN;
       }     
     
     if( filter & I2C_TYPE_ACCELEROMETER)
       {
	 if( prvCheckAccelerometer(slaveid) )
	   {
	     result = I2C_TYPE_ACCELEROMETER;
	   }
       }
     
     if( filter & I2C_TYPE_THERMOMETER)
       {
	 if( prvCheckThermometer(slaveid) )
	   {
	     result = I2C_TYPE_THERMOMETER;
	   }
       }
     
     return result;
   }
   
   
   ////////////////////////////////////////////////////////////////////////////
   //! Checks if the I²C device accelerometer
   
   bool CI2CMaster::prvCheckAccelerometer(int slaveid)
   {
     return false;
   }   
   
   
   ////////////////////////////////////////////////////////////////////////////
   //! Checks if the slave is a I²C thermometer
   
   bool CI2CMaster::prvCheckThermometer(int slaveid)
   {
     

     return false;
   }

   
   ////////////////////////////////////////////////////////////////////////////
   //! Cleans up all allocated ressources
   
   void CI2CMaster::prvClean()
   {     
     for( std::list<IThermometer*>::iterator it = m_Thermometers.begin();
	  it != m_Thermometers.end(); ++it)
       {
	 if(*it)
	   delete static_cast<CI2CThermometer*>(*it);
       }

     for( std::list<IAccelerometer*>::iterator it = m_Accelerometers.begin();
	  it != m_Accelerometers.end(); ++it)
       {
	 if(*it)
	   delete static_cast<CI2CAccelerometer*>(*it);
       }

     for( std::vector<CI2CSlave*>::iterator it = m_Slaves.begin(); 
	  it != m_Slaves.end(); ++it )
       {
	 if(*it)
	   delete *it;
       }

   }
   
   
  /////////////////////////////////////////////////////////////////////////////
  //! Raw I²C access for reading data into a buffer
  
  bool CI2CMaster::ReadData( II2CSlave* slave, unsigned char* data, int len)
  {
    if(slave && data && len > 0)
      {

#ifdef HAVE_LINUX
	int errcode = ioctl(m_I2CDevice, I2C_SLAVE, slave->GetSlaveId());

	if(errcode != OK)
	  {
	    std::cerr << "II2CMaster::ReadData() - ioctl failed!" << std::endl;
	    return false;
	  }

        write(m_I2CDevice, data,len);
	return true;
#endif
	
      }

    return false;
  }
  
  
  /////////////////////////////////////////////////////////////////////////////
  //! Raw I²C access for sending data from a buffer
  
  bool CI2CMaster::SendData( II2CSlave* slave, unsigned char* data, int len)
  {
    if(slave && data && len > 0)
      {

#ifdef HAVE_LINUX
	int errcode = ioctl(m_I2CDevice, I2C_SLAVE, slave->GetSlaveId());

	if(errcode != OK)
	  {
	    std::cerr << "II2CMaster::WriteData() - ioctl failed!" << std::endl;
	    return false;
	  }

        read(m_I2CDevice, data,len);
	return true;
#endif
      }
    
    return false;
  }
  
  
  /////////////////////////////////////////////////////////////////////////////
  //! Creates an I²C-Accelerometer
  
  IAccelerometer*  CI2CMaster::GetAccelerometer( II2CSlave* slv )
  { 
    CI2CSlave* slave = static_cast<CI2CSlave*>(slv);
    
    if( slave )
      {	
	if( slave->GetType() & ( I2C_TYPE_ACCELEROMETER | I2C_TYPE_UNKNOWN ) )
	  {
	    IAccelerometer* pAccelerometer =  
	      static_cast<IAccelerometer*>(new CI2CAccelerometer(*slave));
	    
	    // keep track on the allocated accelerometers for cleanup
	    m_Accelerometers.push_back(pAccelerometer);

	    return pAccelerometer;
	  }
      }
    
    return NULL;
  }

  
  /////////////////////////////////////////////////////////////////////////////
  //! Creates an I²C-Thermometer
  
  IThermometer* CI2CMaster::GetThermometer( II2CSlave* slv)
  {
    CI2CSlave* slave = static_cast<CI2CSlave*>(slv);

    if( slave )
      {
	if( slave->GetType() & ( I2C_TYPE_THERMOMETER | I2C_TYPE_UNKNOWN ) )
	  {
	    IThermometer* pThermometer = 
	      static_cast<IThermometer*>( new CI2CThermometer(*slave) );
	    
	    // keep track on the allocated thermometers for cleanup
	    m_Thermometers.push_back(pThermometer);
	    
	    return pThermometer;
	  }
      }

    return NULL;
  }
  

  /////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

///////////////////////////////////////////////////////////////////////////////

