//////////////////////////////////////////////////////////////////////////////
//! @file 	CBaseConfiguration.h
//! @date	Created on: 05.04.2011
//! @author	Falk Schilling
//////////////////////////////////////////////////////////////////////////////

#ifndef CBASECONFIGURATION_H_
#define CBASECONFIGURATION_H_

//////////////////////////////////////////////////////////////////////////////

#include <iterator>
#include "CLEDConfig.h"

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! This is the base class for configuring all LED modules.
  //////////////////////////////////////////////////////////////////////////

  class CBaseConfiguration
  {
  public:
    struct iterator : std::iterator< std::forward_iterator_tag, CLEDConfig >
      {
	explicit iterator(CLEDConfig* pPtr) : m_pPtr(pPtr)
	{}

	CLEDConfig &	operator*() 	{ return *m_pPtr; 	}
	CLEDConfig*		operator->() 	{ return m_pPtr; 	}
	iterator & 	operator++() { ++m_pPtr; return *this;	}
	iterator 	operator++(int) { iterator tmp = *this; ++*this; return tmp; }
	bool		operator!=(iterator rhs) { return m_pPtr != rhs.m_pPtr; }
	bool		operator==(iterator rhs) { return m_pPtr == rhs.m_pPtr; }

      private:
	CLEDConfig*	m_pPtr;
      };


    struct const_iterator : std::iterator< std::input_iterator_tag, CLEDConfig >
      {
	explicit const_iterator(const CLEDConfig* pPtr) : m_pPtr(pPtr)
	{}

	const CLEDConfig &		operator*() 	const { return *m_pPtr; }
	const CLEDConfig*		operator->() 	const { return m_pPtr; 	}

	const const_iterator & 	operator++() { ++m_pPtr; return *this;}
	const const_iterator 	operator++(int) { const_iterator tmp = *this; ++*this; return tmp; }
	bool					operator!=(const_iterator rhs) { return m_pPtr != rhs.m_pPtr; }
	bool					operator==(const_iterator rhs) { return m_pPtr == rhs.m_pPtr; }

      private:
	const CLEDConfig*	m_pPtr;
      };


  public:
    CBaseConfiguration();
    virtual ~CBaseConfiguration() = 0;

    virtual CBaseConfiguration::iterator begin();
    virtual CBaseConfiguration::iterator end();

    virtual CBaseConfiguration::const_iterator begin() 	const;
    virtual CBaseConfiguration::const_iterator end() 	const;

    static const int NUMBER_OF_LEDS = 4;

  protected:
    CLEDConfig		m_aoConfig[NUMBER_OF_LEDS];
  };



  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CBASECONFIGURATION_H_ */
