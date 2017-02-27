///////////////////////////////////////////////////////////////////////////////
//! @file 	IRGBLightControl.h
//! @date	Created on: 27.12.2011, last change: 28.04.2013
//! @author	Falk Schilling
//! @license    GPLv3+
///////////////////////////////////////////////////////////////////////////////

#ifndef IRGBLIGHTCONTROL_H_
#define IRGBLIGHTCONTROL_H_

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  struct t_LEDConfig
  {
    bool	m_bEnabled;
    bool 	m_bHSIMode;
    char	m_cValue1; 	// red	 | hue
    char	m_cValue2; 	// green | saturation
    char	m_cValue3; 	// blue  | intensity
  };

  struct t_Configuration
  {
    t_LEDConfig 	sLED1;
    t_LEDConfig 	sLED2;
    t_LEDConfig 	sLED3;
    t_LEDConfig 	sLED4;
  };

  typedef void (*fnChannelColorsAvailableCallback)
    (void* pObject, const t_Configuration & sConfiguration);

  
  //////////////////////////////////////////////////////////////////////////
  //! Interface for accessing Thermometer data
  //////////////////////////////////////////////////////////////////////////
  
  struct IThermometer
  {
    virtual double GetTemperature() const = 0;
  };


  //////////////////////////////////////////////////////////////////////////
  //! Interface for accessing Accelerometer sensor data
  //////////////////////////////////////////////////////////////////////////
  
  struct IAccelerometer
  {
    virtual bool GetAcceleration(double & X, double & Y, double & Z) const = 0;
    virtual bool GetTiltStatus(unsigned int & status) const = 0;
  };
  
  
  //////////////////////////////////////////////////////////////////////////
  //! Interface for I²C Slaves
  //////////////////////////////////////////////////////////////////////////

  struct II2CMaster;

  struct II2CSlave
  {
    typedef enum 
      {
	I2C_TYPE_ALL           = -1,
	I2C_TYPE_UNKNOWN       = (1 << 0),
	I2C_TYPE_ACCELEROMETER = (1 << 1),
	I2C_TYPE_THERMOMETER   = (1 << 2), 
	I2C_TYPE_COUNT = 3
      } t_I2CSlaveTypes;
    
    //! Returns the type of this I²C slave device
    virtual unsigned int GetType() const = 0;
    
    //! Returns the id of this I²C slave device
    virtual unsigned char GetSlaveId() const = 0;
    
    //! Returns the I²C master object to whom this I²C slave is assigned to.
    virtual const II2CMaster* GetMaster() = 0;
  };
  

  //////////////////////////////////////////////////////////////////////////
  //! Interface of the I²C Master
  //////////////////////////////////////////////////////////////////////////
  
  struct II2CMaster
  {    
    //! Enumerates all I²C slaves
    virtual bool Scan( II2CSlave** dev, int* num, unsigned long filter = -1) = 0;
    
    //! Raw I²C access for reading data into a buffer
    virtual bool ReadData( II2CSlave* slave, unsigned char* data, int len) = 0;
    
    //! Raw I²C access for sending data from a buffer
    virtual bool SendData( II2CSlave* slave, unsigned char* data, int len) = 0;
    
    //! Creates an I²C-Accelerometer
    virtual IAccelerometer*  GetAccelerometer( II2CSlave* slave ) = 0;
    
    //! Returns an I²C-Thermometer
    virtual IThermometer* GetThermometer( II2CSlave* slave) = 0;
    
  };
    


  //////////////////////////////////////////////////////////////////////////
  //! Enumeration for the different playback modes.
  //////////////////////////////////////////////////////////////////////////

  typedef enum
  {
    LIGHTMODE_UNDEFINED = -1,
    LIGHTMODE_AMBIENTLIGHT = 0,
    LIGHTMODE_SEQUENCE,
    LIGHTMODE_CONSTANTCOLOR,
    LIGHTMODE_ACCELEROMETER,
    LIGHTMODE_THERMOMETER,
    LIGHTMODE_COUNT
  } t_DeviceMode;


  //////////////////////////////////////////////////////////////////////////
  //! This is the basic interface for mode plugins
  //////////////////////////////////////////////////////////////////////////

  class IDeviceMode
  {
  public:
    //! Returns the type of the current used light mode
    virtual t_DeviceMode GetType() const = 0;

    //! Sets a callback function to execute, when a new configuration is available
    virtual bool SetCallback
      (void* pObject,	fnChannelColorsAvailableCallback pCallback) = 0;

  protected:
    fnChannelColorsAvailableCallback m_pCallback;
    void* m_pCallbackObject;
  };


  //////////////////////////////////////////////////////////////////////////
  //! Interface class for setting a constant color configuration.
  //////////////////////////////////////////////////////////////////////////

  class IConstantLightMode : public IDeviceMode
  {
  public:
    //! Sets the colors of the RGBLightControl to the given values
    virtual bool SetColors(const t_Configuration & oConfig) = 0;

    //! Returns the current RGBLightControl LED configuration
    virtual t_Configuration GetColors() const = 0;
  };


  //////////////////////////////////////////////////////////////////////////
  //! Interface class for playing light sequences
  //////////////////////////////////////////////////////////////////////////

  class ISequenceLightMode : public IDeviceMode
  {
  public:
    //! Clears all sequences
    virtual void Clear() = 0;

    //! Add a new light sequence
    virtual bool Add(const t_Configuration & sConfiguration, unsigned long uiMilliseconds) = 0;

    //! Plays the set of sequences
    virtual bool Play() = 0;

    //! Stops playing the sequences
    virtual bool Stop() = 0;

    //! Enables or disables the repetition
    virtual void Repeat(bool bRepeat) = 0;

    //! Enables or disables shuffling of the set of sequences
    virtual void Shuffle(bool bShuffle)  = 0;

    //! Returns whether repeat mode is enabled or not (default: yes)
    virtual bool isRepeating() const = 0;

    //! Returns whether shuffle mode is enabled or not (default: nope)
    virtual bool isShuffling() const = 0;
  };


  //////////////////////////////////////////////////////////////////////////
  //! Interface for accessing the video grabbing mode
  //////////////////////////////////////////////////////////////////////////

  class IAmbientLightMode : public IDeviceMode
  {
  public:
    //! Initializes the ambient light mode
    virtual bool Init() = 0;

    //! Cleans up the ambient light mode
    virtual bool Cleanup() = 0;

    //! Starts video grabbing and color computation
    virtual bool Start() = 0;

    //! Stops video grabbing and color computation.
    virtual bool Stop() = 0;

    //! Returns true if the light mode is initialized
    virtual bool isInitialized() const = 0;

    //! Returns true if the video decoding is running
    virtual bool isStarted() const = 0;
  };


  //////////////////////////////////////////////////////////////////////////
  //! Accelerometer color control mode :)
  //////////////////////////////////////////////////////////////////////////

  class IAccelerometerMode : public IDeviceMode
  {
  public:
    //! Sets the colors of the RGBLightControl to the given values
    virtual bool SetColors(const t_Configuration & oConfig) = 0;

    //! Returns the current RGBLightControl LED configuration
    virtual t_Configuration GetColors() const = 0;
  };
  

  //////////////////////////////////////////////////////////////////////////
  //! Thermometer color control mode
  //////////////////////////////////////////////////////////////////////////

  class IThermometerMode : public IDeviceMode
  {
  public:
    //! Sets the colors of the RGBLightControl to the given values
    virtual bool SetColors(const t_Configuration & oConfig) = 0;

    //! Returns the current RGBLightControl LED configuration
    virtual t_Configuration GetColors() const = 0;
  };
  
  
  //////////////////////////////////////////////////////////////////////////
  //! Interface for accessing an physical RGBLightControl device
  //////////////////////////////////////////////////////////////////////////

  class IDevice
  {
  public:
    virtual bool isInitialized() 	const = 0;
    virtual bool isStarted() 		const = 0;

    virtual bool Init(const char* pszDevice, unsigned int uiBaudRate) = 0;
    virtual bool Cleanup() = 0;

    virtual void SetDeviceMode(IDeviceMode* pMode) = 0;
    virtual IDeviceMode* GetDeviceMode() = 0;

    virtual bool Start() = 0;
    virtual bool Stop() = 0;
  };


  //////////////////////////////////////////////////////////////////////////
  //! A tiny webserver interface for providing control via web
  //////////////////////////////////////////////////////////////////////////

    struct IWebserverActionArguments
    {
      //! checks if there exists an argument with the keyword given by name
      virtual bool hasArgument(const char* pszName) = 0;

      //! returns the necessary length for a string buffer for storing the value
      virtual long GetValueLength(const char* pszName) = 0;

      //! fetches the value belonging to the argument identified by name
      virtual bool GetValue(const char* pszName, char* pszValue, long lSize) = 0;
    };
  
    typedef char* (*fnIncludeCodeCallback)(	void );
    typedef char* (*fnActionCodeCallback) (IWebserverActionArguments*);
  
  struct IWebserver
  {
    
    //! write text defined by the callback when <!--#webem idname --> is seen
    virtual bool RegisterIncludeCode(const char* idname,
				     fnIncludeCodeCallback fun ) = 0;

    //! execute callback function when http://server:port/idname.webem?key=value is called
    virtual bool RegisterActionCode(const char* idname,
				    fnActionCodeCallback fun ) = 0;

    //! initializes the webserver
    virtual bool Init(	const char* strAddress,
			const char* strPort,
			const char* strDocRoot) = 0;

    //! cleans up everything
    virtual bool Cleanup() = 0;

    //! starts the webserver to listen
    virtual bool Start() = 0;

    //! stops the webserver
    virtual bool Stop() = 0;

    //! returns true if the webserver is initialized.
    virtual bool isInitialized() const = 0;

    //! returns true if the webserver is started
    virtual bool isStarted() const = 0;

  };


  //////////////////////////////////////////////////////////////////////////
  //! Facade to use from the user interface
  //////////////////////////////////////////////////////////////////////////

  class IRGBLightControl
  {
  public:
    //! Creates an RGBLightControl device object
    static IDevice* CreateDevice();

    //! Frees an allocated device object
    static void FreeDevice(IDevice* pDevice);

    //! Creates an webserver object
    static IWebserver* CreateWebserver();

    //! Frees an allocated webserver object
    static void FreeWebserver(IWebserver* pServer);

    //! Creates an I²C Master
    static II2CMaster* CreateI2CMaster(const char* devicename);
    
    //! Frees an allocated I²C Master
    static void FreeI2CMaster(II2CMaster* pMaster);

  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* IRGBLIGHTCONTROL_H_ */

//////////////////////////////////////////////////////////////////////////////
