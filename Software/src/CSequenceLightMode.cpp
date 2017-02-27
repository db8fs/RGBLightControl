///////////////////////////////////////////////////////////////////////////////
//! @file 	CSequenceLightMode.cpp
//! @date	Created on: 26.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#include "CSequenceLightMode.h"
#include "RGBLightControl.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CSequenceLightMode::CSequenceLightMode()
    : 	m_bWorkerThreadKillEvent(false),
	m_bRepeat(true),
	m_bShuffle(false),
	m_pWorkerThread(NULL)
  {
    m_pCallback        	= NULL;
    m_pCallbackObject 	= NULL;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CSequenceLightMode::~CSequenceLightMode()
  {
    Clear();
  }


  //////////////////////////////////////////////////////////////////////////
  //! This method clears the current set of light sequences.
  //////////////////////////////////////////////////////////////////////////

  void CSequenceLightMode::Clear()
  {
    std::cout << "CSequenceLightMode:Clear() - Clearing all sequences" << std::endl;

    for(std::list<t_Sequence*>::iterator it = m_vSequences.begin();
	it != m_vSequences.end(); ++it)
      {
	t_Sequence* pSequence = *it;

	if(!pSequence)
	  {
	    continue;
	  }

	delete pSequence;
      }

    m_vSequences.clear();
  }


  //////////////////////////////////////////////////////////////////////////
  //! This method adds a new light sequence to the current set.
  //////////////////////////////////////////////////////////////////////////

  bool CSequenceLightMode::Add(const t_Configuration & sConfiguration, unsigned long uiMilliseconds)
  {
    bool bRet = false;

    if(m_vSequences.size() < 1000000)
      {
	std::cout << "CSequenceLightMode::Add() - Adding Sequence " << std::endl;
	t_Sequence* pSequence = new t_Sequence;

	pSequence->sColors 		= sConfiguration;
	pSequence->uiHoldTime 	= uiMilliseconds;

	m_vSequences.push_back(pSequence);
      }
    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This method starts playing the current set of light sequences.
  //////////////////////////////////////////////////////////////////////////

  bool CSequenceLightMode::Play()
  {
    std::cout << "CSequenceLightMode::Play() - Playing Sequence" << std::endl;
    if(!m_pWorkerThread)
      {
	m_bWorkerThreadKillEvent = false;
	m_pWorkerThread = new boost::thread(boost::bind(CSequenceLightMode::WorkerThreadProc, this));
      }

    return true;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Enables or disables the repetition
  //////////////////////////////////////////////////////////////////////////

  bool CSequenceLightMode::Stop()
  {
    if(m_pWorkerThread)
      {
	m_bWorkerThreadKillEvent = true;
	m_pWorkerThread->join();

	delete m_pWorkerThread;
	m_pWorkerThread = NULL;
      }

    std::cout << "Stopped" << std::endl;
    return true;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Enables or disables the repetition
  //////////////////////////////////////////////////////////////////////////

  void CSequenceLightMode::Repeat(bool bRepeat)
  {
    m_bRepeat = bRepeat;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Enables or disables shuffling of the set of sequences
  //////////////////////////////////////////////////////////////////////////

  void CSequenceLightMode::Shuffle(bool bShuffle)
  {
    m_bShuffle = bShuffle;
  }



  //////////////////////////////////////////////////////////////////////////
  //! Set a callback to execute for applying the calculated color set
  //////////////////////////////////////////////////////////////////////////

  bool CSequenceLightMode::SetCallback(void* pObject,
				       fnChannelColorsAvailableCallback pCallback)
  {
    bool bRet = true;

    if(pCallback)
      {
	this->m_pCallback 		= pCallback;
	this->m_pCallbackObject = pObject;
      }
    else
      {
	this->m_pCallback 		= NULL;
	this->m_pCallbackObject = NULL;
	bRet = false;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! WorkerThread for playing the sequences
  //////////////////////////////////////////////////////////////////////////

  void CSequenceLightMode::WorkerThreadProc(void* pVoid)
  {
    CSequenceLightMode* pTHIS = (CSequenceLightMode*) pVoid;

    std::cout << "CSequenceLightMode::WorkerThreadProc" << std::endl;

    while(1)
      {
	if(!pTHIS)
	  {
	    break;
	  }

	if(pTHIS->m_bWorkerThreadKillEvent)
	  {
	    break;
	  }

	for(std::list<t_Sequence*>::iterator it = pTHIS->m_vSequences.begin();
	    it != pTHIS->m_vSequences.end(); ++it)
	  {
	    t_Sequence* pSequence = *it;

	    if(!pSequence) continue;

	    if(pTHIS->m_bWorkerThreadKillEvent) break;

	    std::cout << "Playing sequence" << std::endl;

	    if(pTHIS->m_pCallback)
	      pTHIS->m_pCallback(pTHIS->m_pCallbackObject, pSequence->sColors);

	    for(unsigned int mSecCounter = 0; mSecCounter < pSequence->uiHoldTime; ++mSecCounter)
	      {
		usleep(1000);
	      }
	  }

	if(!pTHIS->m_bRepeat)
	  {
	    break;
	  }
      }

    std::cout << "Leaving thread" << std::endl;
  }


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
