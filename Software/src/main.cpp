///////////////////////////////////////////////////////////////////////////////
//! @file 			main.cpp
//! @date			Created on: 01.04.2011
//! @author			Falk Schilling
//! @description  This is a tiny application providing a web interface and
//!		  the possibility of remote controlling a RGBLightControl
//!		  device via HTTP-Get and AJAX requests. Oh yes - a little
//!		  Amb*Light clone for your Dreambox is also included... :)
///////////////////////////////////////////////////////////////////////////////

#include <config.h>
#include <iostream>
#include <cstdlib>
#include "CLEDConfig.h"
#include "RGBLightControl.h"
#include <boost/system/config.hpp>
#include <boost/program_options.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/spirit.hpp>

//////////////////////////////////////////////////////////////////////////////

using namespace RGBLightControl;
using namespace boost::spirit;

//////////////////////////////////////////////////////////////////////////////

// some globals just for the moment
static IWebserver* 		pWebserver 	= NULL;
static IDevice*  		pDevice 	= NULL;

//////////////////////////////////////////////////////////////////////////////

struct CProgramConfig
{
  CProgramConfig() 
    : strHttpAddress("127.0.0.1"), 
      strHttpPort("9123"), 
      strHttpDocRoot(DATADIR), 
      strCmd(""),
      strMode(""),
      strDevice("/dev/ttyUSB0"), 
      uiBaudrate(57600)
  {
  }
  
  std::string strHttpAddress;
  std::string strHttpPort;
  std::string strHttpDocRoot;
  std::string strCmd;
  std::string strMode;
  std::string strDevice;
  unsigned int uiBaudrate;
};

std::ostream &operator<<(std::ostream & oStream, const CProgramConfig & conf)
{
  oStream << "RGBLightControl Configuration " << std::endl 
          << "------------------------------" << std::endl;
  oStream << "HttpAddress: " << conf.strHttpAddress << std::endl;
  oStream << "HttpPort: " << conf.strHttpPort << std::endl;
  oStream << "HttpDocRoot: " << conf.strHttpDocRoot << std::endl;
  oStream << "Device: " << conf.strDevice << std::endl;
  oStream << "Baudrate: " << conf.uiBaudrate << std::endl;

  return oStream;
}

/////////////////////////////////////////////////////////////////////////////

static CLEDConfig ParseColorTriple(const std::string & strColorTriple, bool bHSI=false)
{
  uint_parser<unsigned, 16, 1, 2> hexcolorcomponent_p;
  
  int r = 0;
  int g = 0;
  int b = 0;

  boost::spirit::parse( strColorTriple.c_str(),
			(
			 hexcolorcomponent_p[ assign_a(r) ] >>
			 hexcolorcomponent_p[ assign_a(g) ] >>
			 hexcolorcomponent_p[ assign_a(b) ]
			 ),space_p);

  r = std::max(std::min(r/2, 127), 0);
  g = std::max(std::min(g/2, 127), 0);
  b = std::max(std::min(b/2, 127), 0);

  return CLEDConfig(r,g,b, bHSI);
}

static std::string GetWebserverArgument(const char* idname, IWebserverActionArguments* pWebServerArguments)
{
  std::string strValue = "";

  while(1)
    {
      if(!pWebServerArguments || !idname)
	{
	  break;
	}

      if(!pWebServerArguments->hasArgument(idname))
	{
	  std::cerr << "Argument " << idname << "not found!" << std::endl;
	  break;
	}

      long lSize = pWebServerArguments->GetValueLength(idname);

      if(lSize == 0)
	{
	  std::cerr << "Argument " << idname << "has empty value!" << std::endl;
	  break;
	}

      char* pBuffer = new char[lSize];

      if(!pWebServerArguments->GetValue(idname, pBuffer, lSize))
	{
	  std::cerr << "Argument '" << idname << "' NOT successfully read!" << std::endl;
	}

      strValue = pBuffer;

      delete[] pBuffer;
      break;
    }

  return strValue;
}

char* OnSetColor(IWebserverActionArguments* pWebServerArguments)
{
  if(pWebServerArguments)
    {
      std::string strColor = GetWebserverArgument("color", pWebServerArguments);

      t_LEDConfig sColor = ParseColorTriple(strColor, false);

      std::cout	<< "OnSetColor" << std::endl;
      std::cout 	<< (int) sColor.m_cValue1 << ","
			<< (int) sColor.m_cValue2 << ","
			<< (int) sColor.m_cValue3 << std::endl;

      if(pDevice && pDevice->isStarted())
	{
	  t_Configuration sConfiguration;

	  sConfiguration.sLED1 = sColor;
	  sConfiguration.sLED2 = sColor;
	  sConfiguration.sLED3 = sColor;
	  sConfiguration.sLED4 = sColor;
	  
	  //if(pDeviceModeConstant)
	  // pDeviceModeConstant->SetColors( sConfiguration );
	}
    }

  return (char*) "/index.html";
}


char* OnPlay(IWebserverActionArguments* pWebServerArguments)
{	
  std::cout << "main: OnPlay() " << std::endl;

  if(pWebServerArguments)
    {
      std::string strColor = GetWebserverArgument("cmd", pWebServerArguments);

      bool bPlay = false;

      boost::spirit::parse( strColor.c_str(),
			    (as_lower_d["play"])[assign_a(bPlay, true)], space_p);

      // if(!bPlay)
      // 	{
      // 	  if(pDeviceModeSequence)
      // 	    pDeviceModeSequence->Play();
      // 	}
      // else
      // 	{
      // 	  if(pDeviceModeSequence)
      // 	    pDeviceModeSequence->Stop();
      // 	}

    }
  return (char*) "/index.html";
}


char* OnDelete(IWebserverActionArguments* pWebServerArguments)
{
  std::cout << "OnDelete" << std::endl;

  // if(pDeviceModeSequence)
  //   {
  //     pDeviceModeSequence->Stop();
  //     pDeviceModeSequence->Clear();
  //   }

  return (char*) "/index.html";
}

/////////////////////////////////////////////////////////////////////////////

namespace po = boost::program_options;

bool CheckProgramOptions(CProgramConfig & config, int argc, char* argv[])
{
  po::options_description cmdline_options("Usage");
  po::options_description generic("Generic");
  po::options_description device("Device");
  po::options_description webinterface("Web Interface");
  po::options_description cmdlineinterface("Commands");

  generic.add_options()
    ("help,h", "this description")
    ("config", "prints the current configuration")
    ("version,v", "about this software")
    ;

  device.add_options()
    ("device,d", po::value< std::string >()->default_value( "/dev/ttyUSB0" ), "path to the RGBLightControl device")
    ("baudrate,b", po::value<unsigned int>()->default_value( 57600U ), "sets baudrate for selected device")
    ;

  webinterface.add_options()
    ("ip,i", po::value< std::string >()->default_value( "127.0.0.1" ), "Address of the http server" )
    ("port,p", po::value< std::string >()->default_value( "9123" ), "Tcp port of the http server" )
    ("docroot,r", po::value< std::string >()->default_value( DATADIR ), "Document root of the http server" )
    ;
  
  cmdlineinterface.add_options()
    ("mode,m", po::value< std::string >(), 
     "Software control mode\n\n"
     "Possible Values:\n"
     "  direct: \tDirectly pass messages to the RGBLightControl device and quit\n"
     "  sequence: \tPlays sequences\n"
     "  accelerometer: \tUses inputs from an I²C Acclerometer\n"
     "  thermometer: \tUses inputs from an I²C Thermometer\n"
     //     "  webcam: \tUses inputs from a webcam\n"
     //     "  web: \tConfiguration through Web interface\n" // not sure yet
#ifdef HAVE_DREAMBOX
     "  ambientlight: \tGrabs the colors from your Dreambox's live video.\n"
#endif
     )
    ("cmd,c", po::value< std::string >(), 
     "Control Command\n\n" 
     "Mode 'direct':\n"
     "  rgb[n](r,g,b): \n"
     "                 \t n - Channel to set (1,2,3,4; 0 - all)\n"
     "                 \t r - Red Value    (-128 <= r <= 127)\n"
     "                 \t g - Green Value  (-128 <= g <= 127)\n"
     "                 \t b - Blue Value   (-128 <= b <= 127)\n"
     "  hsi[n](h,s,i): \n"
     "                 \t n - Channel to set (1,2,3,4; 0 - all)\n"
     "                 \t h - Hue (0 <= h <= 180)\n"
     "                 \t s - Saturation - ignored\n"
     "                 \t i - Intensity  (-128 <= i <= 127)\n\n"
     "  Example (all channels orange):\n"
     "   $ ./RGBLightControl -m direct -c 'rgb[0](127,16,0)'\n"
     )
    ;
  
  cmdline_options.add(generic).add(device).add(webinterface).add(cmdlineinterface);

  try
    { 
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
      po::notify(vm);    

      // device
      if (vm.count("baudrate")) 
	{
	  config.uiBaudrate = vm["baudrate"].as<unsigned int>();
	}
  
      if(vm.count("device"))
	{
	  config.strDevice = vm["device"].as< std::string >();
	}

      // webserver 
      if(vm.count("ip"))
	{
	  config.strHttpAddress = vm["ip"].as< std::string > ();
	}
  
      if(vm.count("port"))
	{
	  config.strHttpPort = vm["port"].as< std::string > ();
	}
  
      if(vm.count("docroot"))
	{
	  config.strHttpDocRoot = vm["docroot"].as< std::string >();
	}


      // direct command mode
      if(vm.count("cmd"))
	{
	  config.strCmd = vm["cmd"].as< std::string > ();
	}


      if(vm.count("mode"))
	{
	  config.strMode = vm["mode"].as< std::string > ();
	}
      

      // generic
      if (vm.count("help")) 
	{
	  std::cout << cmdline_options << "\n";
	  return false;
	}

      if(vm.count("version"))
	{
	  std::cout << PACKAGE_STRING << " <" << PACKAGE_URL << "> " << std::endl;
	  std::cout << "(c) 2011-2013 by Falk Schilling" << std::endl;
	  std::cout << "License GPLv3+: GNU GPL Version 3 or higher <http://gnu.org/licenses/gpl.html>" << std::endl;
	  return false;
	}

      if(vm.count("config"))
	{ 
	  std::cout << config;
	}
    }
  catch(...)
    {
      std::cout << cmdline_options << std::endl;
      return false;
    }
  
  return true;
}

//////////////////////////////////////////////////////////////////////////////

bool TestI2C(CProgramConfig & options)
{
    using namespace RGBLightControl;
    II2CMaster* pMaster = IRGBLightControl::CreateI2CMaster("/dev/i2c-1");
   
    if(pMaster)
      {
	II2CSlave* pSlaves = NULL;
	int numSlaves = 0;
	IAccelerometer* pAccelerometer = NULL;
	  	
	if(pMaster->Scan(&pSlaves, &numSlaves, II2CSlave::I2C_TYPE_ALL))
	  {
	    for(int i=0; i<numSlaves; ++i)
	      {
		bool found_smth = false;
		
		// found_smth = ...  // filter out the correct one, e.g. match by slaveid
		
		if(found_smth)
		  {
		    pAccelerometer = pMaster->GetAccelerometer( &pSlaves[i] );
		  }
	      }

	    // delete the array pointing to each I²C slaves
	    delete[] pSlaves;
	  }
	
	if(pAccelerometer)
	  {
	    double X=0., Y=0., Z=0.;
	    
	    if(pAccelerometer->GetAcceleration(X, Y, Z))
	      {
		std::cout << "X: " << X << " Y: " << Y << " Z: " << Z << std::endl;
	      }
	  }

	// frees the master and all of its slaves, including accelerometers
	IRGBLightControl::FreeI2CMaster(pMaster);  
	return true;
      }
  
  return false;
}


//////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  CProgramConfig options;

  while(1)
    {
      if(!CheckProgramOptions(options, argc, argv))
	{
	  break;
	}

#if(1)
      if(!TestI2C(options))
	{
	  std::cout << "Error when testing i2c bus!" << std::endl;
	  break;
	}
      
      break;
    }
#else

      pWebserver	  = IRGBLightControl::CreateWebserver();
      pDevice 		  = IRGBLightControl::CreateDevice();
      
      if(!pDevice || !pWebserver )
	{
	  break;
	}
      
      if(!pDevice->Init(options.strDevice.c_str(), options.uiBaudrate))
	{
	  std::cerr 	<< "Could not initialize RGBLightControl device! " << std::endl
			<< "Do you have the permissions for accessing the device?"
			<< std::endl;
	  break;
	}
      
      //      pDevice->SetDeviceMode(pDeviceModeConstant);
      
      if(!pWebserver->Init(options.strHttpAddress.c_str(), options.strHttpPort.c_str(), options.strHttpDocRoot.c_str() ))
	{
	  std::cerr << "Error when initializing webserver!\n";
	  break;
	}
      
      std::cout << "WebInterface: http://" 
		<< options.strHttpAddress << ":" 
		<< options.strHttpPort 
		<< "/index.html" 
		<< std::endl;
      
      pWebserver->RegisterActionCode("setcolor", &OnSetColor);
      pWebserver->RegisterActionCode("play", &OnPlay);
      pWebserver->RegisterActionCode("delete", &OnDelete);
      
      if(!pWebserver->Start())
	{
	  std::cerr << "Error when starting webserver!\n";
	}

      if(!pDevice->Start())
	{
	  std::cerr << "Error when starting the RGBLightControl!" << std::endl;
	  break;
	}

      std::cout << "Running..." << std::endl;
      while(pDevice->isStarted())
	{
	  char cInput;
	  std::cin.get(cInput);

	  if(cInput == 3)
	    {
	      break;
	    }
	}

      std::cout << "Performing shutdown" << std::endl;

      if(!pDevice->Stop())
	{
	  std::cerr << "Error when stopping the RGBLightControl!" << std::endl;
	  break;
	}

      if(!pDevice->Cleanup())
	{
	  std::cerr << "Error when cleaning up RGBLightControl object" << std::endl;
	}

      break;
    }
  
  IRGBLightControl::FreeDevice(pDevice);
  IRGBLightControl::FreeWebserver(pWebserver);
#endif
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////


#if(0) 

/////////////////////////////////////////////////////////////////////////////
// EXAMPLE USAGE OF RGBLIGHTCONTROL API
/////////////////////////////////////////////////////////////////////////////


// -- snip ---------------------------------------------------------------
// I²C example: intended usage of I²C API

  {
    using namespace RGBLightControl;
    II2CMaster* pMaster = CreateI2CMaster('/dev/i2c-1');
   
    if(pMaster)
      {
	II2CSlave* pSlaves = NULL;
	int numSlaves = 0;
	IAccelerometer* pAccelerometer = NULL;
	
	if(pMaster->Scan(pSlaves, &numSlaves, II2CSlave::I2C_TYPE_ACCELEROMETER))
	  {
	    for(int i=0; i<numSlaves; ++i)
	      {
		bool found_smth = false;

		// found_smth = ...  // filter out the correct one, e.g. match by slaveid

		if(found_smth)
		  {
		    pAccelerometer = pMaster->GetAccelerometer( pSlaves[i] );
		  }
	      }

	    // delete the array pointing to each I²C slaves
	    delete[] pSlaves;
	  }
	
	if(pAccelerometer)
	  {
	    double X=0., Y=0., Z=0.;
	    
	    if(pAccelerometer->GetAcceleration(X, Y, Z))
	      {
		std::cout << "X: " << X << " Y: " << Y << " Z: " << Z << std::endl;
	      }
	  }

	// frees the master and all of its slaves, including accelerometers
	FreeI2CMaster(pMaster);  
      }
  }

// -- snap --------------------------------------------------------------------



#endif

