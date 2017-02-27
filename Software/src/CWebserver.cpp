///////////////////////////////////////////////////////////////////////////////
//! @file 	CWebserver.cpp
//! @date	Created on: 27.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#include "CWebserver.h"

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//! Construction
//////////////////////////////////////////////////////////////////////////

CWebserver::CWebserver()
  : m_pServer(NULL),
    m_pThread(NULL),
    m_bInitialized(false),
    m_bStarted(false)
{
}


//////////////////////////////////////////////////////////////////////////
//! Destruction
//////////////////////////////////////////////////////////////////////////

CWebserver::~CWebserver()
{
  if(m_bStarted)
    {
      (void) Stop();
    }

  if(m_bInitialized)
    {
      (void) Cleanup();
    }

}


//////////////////////////////////////////////////////////////////////////
//! Initializes the HTTP server.
//////////////////////////////////////////////////////////////////////////

bool CWebserver::RegisterIncludeCode(const char* idname,
				     fnIncludeCodeCallback fun )
{
  if(m_bInitialized)
    {
      m_pServer->RegisterIncludeCode(idname, fun);
    }

  return m_bInitialized;
}


//////////////////////////////////////////////////////////////////////////
//! Initializes the HTTP server.
//////////////////////////////////////////////////////////////////////////

bool CWebserver::RegisterActionCode(const char* idname,
				    fnActionCodeCallback fun )
{
  if(m_bInitialized)
    {
      m_pServer->RegisterActionCode(idname, fun);
    }

  return m_bInitialized;
}


//////////////////////////////////////////////////////////////////////////
//! Initializes the HTTP server.
//////////////////////////////////////////////////////////////////////////

bool CWebserver::Init(	const char* strAddress,
			const char* strPort,
			const char* strDocRoot)
{
  bool bRet = false;

  while(1)
    {
      if(m_bStarted)
	{
	  break;
	}

      if(m_bInitialized)
	{
	  break;
	}

      if(!strAddress || !strPort || !strDocRoot)
	{
	  break;
	}

      m_pServer = new http::server::cWebem(strAddress, strPort, strDocRoot);

      if(!m_pServer)
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
//! Cleans up allocated stuff.
//////////////////////////////////////////////////////////////////////////

bool CWebserver::Cleanup()
{
  bool bRet = false;

  while(1)
    {
      if(m_bStarted)
	{
	  break;
	}

      if(!m_bInitialized)
	{
	  break;
	}

      if(!m_pServer)
	{
	  break;
	}

      delete m_pServer;
      m_pServer = NULL;

      m_bInitialized = false;
      bRet = true;
      break;
    }

  return bRet;
}


//////////////////////////////////////////////////////////////////////////
//! Runs the http server.
//////////////////////////////////////////////////////////////////////////

bool CWebserver::Start()
{
  bool bRet = false;

  while(1)
    {
      if(!m_bInitialized)
	{
	  break;
	}

      if(m_bStarted)
	{
	  break;
	}

      m_pThread = new boost::thread(boost::bind(
						&http::server::cWebem::Run, m_pServer));

      if(!m_pThread)
	{
	  break;
	}

      m_bStarted = true;
      bRet = true;
      break;
    }

  return bRet;
}


//////////////////////////////////////////////////////////////////////////
//! Stops the http server.
//////////////////////////////////////////////////////////////////////////

bool CWebserver::Stop()
{
  bool bRet = false;

  while(1)
    {
      if(!m_bInitialized)
	{
	  break;
	}

      if(!m_bStarted)
	{
	  break;
	}

      if(!m_pThread)
	{
	  break;
	}

      m_pThread->join();

      delete m_pThread;
      m_pThread = NULL;

      m_bStarted = false;
      bRet = true;
      break;
    }

  return bRet;
}

//////////////////////////////////////////////////////////////////////////////
