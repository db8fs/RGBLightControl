///////////////////////////////////////////////////////////////////////////////
//! @file 	CWebserver.h
//! @date	Created on: 27.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#ifndef CWEBSERVER_H_
#define CWEBSERVER_H_

//////////////////////////////////////////////////////////////////////////////

#include "webem/cWebem.h"
#include <boost/thread.hpp>

//////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
using namespace RGBLightControl;

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//! This class is a wrapper for a simple HTTP server based on the
//! WebEm-server written by J. Bremner and C.Kohlhoff (BSD + Boost license)
//////////////////////////////////////////////////////////////////////////

class CWebserver : public IWebserver
{
 public:
  CWebserver();
  virtual ~CWebserver();

  virtual bool RegisterIncludeCode(const char* idname,
				   fnIncludeCodeCallback fun );

  virtual bool RegisterActionCode(const char* idname,
				  fnActionCodeCallback fun );


  virtual bool Init(	const char* strAddress,
			const char* strPort,
			const char* strDocRoot);
  virtual bool Cleanup();

  virtual bool Start();
  virtual bool Stop();

  virtual bool isInitialized() const 	{ return m_bInitialized; }
  virtual bool isStarted() const 		{ return m_bStarted; }

 protected:
  bool 	m_bInitialized;
  bool 	m_bStarted;

  boost::thread*			m_pThread;
  http::server::cWebem*	m_pServer;
};


//////////////////////////////////////////////////////////////////////////////

#endif /* CWEBSERVER_H_ */

//////////////////////////////////////////////////////////////////////////////
