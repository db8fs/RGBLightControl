///////////////////////////////////////////////////////////////////////////////
//! @file 	CSequenceLightMode.h
//! @date	Created on: 26.12.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#ifndef CSEQUENCELIGHTMODE_H_
#define CSEQUENCELIGHTMODE_H_

//////////////////////////////////////////////////////////////////////////////

#include "RGBLightControl.h"
#include <list>
#include <boost/thread.hpp>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  struct t_Sequence
  {
    t_Configuration		sColors;
    unsigned long 		uiHoldTime;
  };

  //////////////////////////////////////////////////////////////////////////
  //! This class represents a player for realizing time-dependent colors
  //! changes with the RGBLightControl PCB.
  //////////////////////////////////////////////////////////////////////////

  class CSequenceLightMode : public ISequenceLightMode
  {
  public:
    CSequenceLightMode();
    virtual ~CSequenceLightMode();

    virtual t_DeviceMode GetType() const { return LIGHTMODE_SEQUENCE; }

    //! Set a callback to execute for applying the calculated color set
    virtual bool SetCallback(void* pObject,
			     fnChannelColorsAvailableCallback pCallback);

    //! Clears all sequences
    virtual void Clear();

    //! Add a new light sequence
    virtual bool Add(const t_Configuration & sConfiguration, unsigned long uiMilliseconds);

    //! Plays the set of sequences
    virtual bool Play();

    //! Stops playing the sequences
    virtual bool Stop();

    //! Enables or disables the repetition
    virtual void Repeat(bool bRepeat);

    //! Enables or disables shuffling of the set of sequences
    virtual void Shuffle(bool bShuffle);

    //! Returns whether repeat mode is enabled or not (default: yes)
    virtual bool isRepeating() const { return m_bRepeat; }

    //! Returns whether shuffle mode is enabled or not (default: nope)
    virtual bool isShuffling() const { return m_bShuffle; }

    static void WorkerThreadProc(void* pVoid);

  protected:
    bool					m_bWorkerThreadKillEvent;
    bool 					m_bRepeat;
    bool 					m_bShuffle;
    std::list<t_Sequence*>	m_vSequences;
    boost::thread*			m_pWorkerThread;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CSEQUENCELIGHTMODE_H_ */

//////////////////////////////////////////////////////////////////////////////
