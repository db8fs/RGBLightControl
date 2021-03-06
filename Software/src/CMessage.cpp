///////////////////////////////////////////////////////////////////////////////
//! @file 	CMessage.cpp
//! @date	Created on: 01.04.2011
//! @author	Falk Schilling
///////////////////////////////////////////////////////////////////////////////

#include "CMessage.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CMessage::CMessage() : CBaseMessage()
  {
    std::fill(begin(), end(), '\0');
  }


  //////////////////////////////////////////////////////////////////////////
  //! Copy construction
  //////////////////////////////////////////////////////////////////////////

  CMessage::CMessage(const CMessage & rhs)
  {
    std::copy(rhs.begin(), rhs.end(), this->begin());
  }


  //////////////////////////////////////////////////////////////////////////
  //! Type conversion
  //////////////////////////////////////////////////////////////////////////

  CMessage::CMessage(const CConfiguration & rhs)
  {
    LEDSelectByte_t uLEDSelectByte;

    uLEDSelectByte.cByte = 0;

    short nCounter = 0;
    char* pMessagePointer = &m_acMessage[3];

    for(CConfiguration::const_iterator it=rhs.begin(); it!=rhs.end(); it++)
      {
	if(it->m_bEnabled)
	  {
	    uLEDSelectByte.sByte.cLowNibble |= (1 << nCounter);
	  }

	if(it->m_bHSIMode)
	  {
	    uLEDSelectByte.sByte.cHighNibble |= (1 << nCounter);
	  }

	*pMessagePointer++ = it->m_cValue1;
	*pMessagePointer++ = it->m_cValue2;
	*pMessagePointer++ = it->m_cValue3;

	nCounter++;
      }

    m_acMessage[0] = MAGIC_BYTE;
    m_acMessage[1] = uLEDSelectByte.cByte;
    m_acMessage[2] = rhs.GetDisplayMode()();

    m_acMessage[MESSAGE_SIZE-1] = this->GetChecksum();
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CMessage::~CMessage()
  {
  }


  //////////////////////////////////////////////////////////////////////////
  //! This is the overwritten assignment operator for messages.
  //////////////////////////////////////////////////////////////////////////

  CMessage & CMessage::operator=(const CMessage & rhs)
  {
    if(this != &rhs)
      {
	std::copy(rhs.begin(), rhs.end(), this->begin());
      }

    return *this;
  }


  //////////////////////////////////////////////////////////////////////////
  //! Calculates the message checksum for transmission.
  //////////////////////////////////////////////////////////////////////////

  char CMessage::GetChecksum() const
  {
    short nSum = 0x0000;

    for(int i=1; i<MESSAGE_SIZE-1; i++)
      {
	nSum += short(m_acMessage[i]);
      }

    return char(0xFF & nSum);
  }


  //////////////////////////////////////////////////////////////////////////
  //! This is the overloaded operator for outputting the hex values of
  //! a message in an output stream.
  //////////////////////////////////////////////////////////////////////////

  std::ostream &operator<<(std::ostream & oStream, const CMessage & oMessage)
  {
    for(CMessage::const_iterator it = oMessage.begin(); it != oMessage.end(); it++)
      {
	oStream << std::hex << (0x00FF & (short)*it) << " ";
      }

    return oStream;
  }

  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
