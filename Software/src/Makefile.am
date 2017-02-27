bin_PROGRAMS=RGBLightControl
RGBLightControl_SOURCES=\
		CBaseConfiguration.cpp\
		CBaseMessage.cpp\
		CConfiguration.cpp\
		CDisplayMode.cpp\
		CDreamboxGrabberDM8000.cpp\
		CImage.cpp\
		CImageGrabber.cpp\
		CImageReader.cpp\
		CLEDConfig.cpp\
		CMessage.cpp\
		CWebserver.cpp\
		CDevice.cpp\
		RGBLightControl.cpp\
		CSerialPort.cpp\
		CSystemTools.cpp\
		IDreamboxGrabber.cpp\
		CAmbientLightMode.cpp\
		CSequenceLightMode.cpp\
		CConstantLightMode.cpp\
		CAccelerometerMode.cpp\
		CThermometerMode.cpp\
		CI2CMaster.cpp\
		webem/server/connection.cpp\
		webem/server/connection_manager.cpp\
		webem/server/mime_types.cpp\
		webem/server/reply.cpp\
		webem/server/request_handler.cpp\
		webem/server/request_parser.cpp\
		webem/server/server.cpp\
		webem/cWebem.cpp\
		main.cpp

AM_CPPFLAGS = -DDATADIR=\"$(pkgdatadir)\"	
RGBLightControl_LDFLAGS = -lboost_thread-mt -lboost_system-mt -lboost_filesystem-mt -lboost_program_options
INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src/webem/ -I$(top_srcdir)/src/webem/server/

