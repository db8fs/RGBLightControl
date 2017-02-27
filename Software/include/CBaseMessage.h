//////////////////////////////////////////////////////////////////////////////
//! @file 	CBaseMessage.h
//! @date	Created on: 01.04.2011
//! @author	Falk Schilling
//////////////////////////////////////////////////////////////////////////////

#ifndef CRGBLIGHTCONTROLBASEMESSAGE_H_
#define CRGBLIGHTCONTROLBASEMESSAGE_H_

//////////////////////////////////////////////////////////////////////////////

#include <iterator>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! This is the base class for messages that may be sent to the DB8FS
  //!	RGBLightControl system. There is some fancy iterator support that
  //! makes it really comfortable to iterate through the message bytes.
  //////////////////////////////////////////////////////////////////////////

  class CBaseMessage
  {
  public:
    struct iterator : std::iterator< std::forward_iterator_tag, char >
    {
      explicit iterator(char* pPtr) : m_pPtr(pPtr)
      {}

      char & operator*() { return *m_pPtr; }
      char* operator->() { return m_pPtr; }

      iterator & operator++() { ++m_pPtr; return *this;}
      iterator 	operator++(int) { iterator tmp = *this; ++*this; return tmp; }
      bool operator!=(iterator rhs) { return m_pPtr != rhs.m_pPtr; }
      bool operator==(iterator rhs) { return m_pPtr == rhs.m_pPtr; }

    private:
      char*	m_pPtr;
    };

    struct const_iterator : std::iterator< std::input_iterator_tag, char >
    {
      explicit const_iterator(const char* pPtr) : m_pPtr(pPtr)
      {}

      const char & operator*() const { return *m_pPtr; }
      const char*  operator->() const { return m_pPtr; }

      const const_iterator & operator++() { ++m_pPtr; return *this;}
      const const_iterator   operator++(int) { const_iterator tmp = *this; ++*this; return tmp; }
      bool operator!=(const_iterator rhs) { return m_pPtr != rhs.m_pPtr; }
      bool operator==(const_iterator rhs) { return m_pPtr == rhs.m_pPtr; }


    private:
      const char* 	m_pPtr;
    };


  public:
    CBaseMessage();
    virtual ~CBaseMessage() = 0;

    virtual CBaseMessage::iterator begin();
    virtual CBaseMessage::iterator end();

    virtual CBaseMessage::const_iterator begin() const;
    virtual CBaseMessage::const_iterator end() 	 const;

    static const int MESSAGE_SIZE = 16;

  protected:
    char	m_acMessage[MESSAGE_SIZE];
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CRGBLIGHTCONTROLBASEMESSAGE_H_ */
