///////////////////////////////////////////////////////////////////////////////
//! @file 	CI2CSlave.h
//! @date	Created on: 29.04.2013
//! @author	Falk Schilling
//! @license    GPLv3+
///////////////////////////////////////////////////////////////////////////////

#ifndef CI2CSLAVE_H_
#define CI2CSLAVE_H_

#include "RGBLightControl.h"

///////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl 
{

  /////////////////////////////////////////////////////////////////////////////

  struct CI2CSlave : public II2CSlave
  {
    //! Returns the type of this I²C slave device
    virtual unsigned int GetType() const { return m_SlaveType; }

    //! Returns the id of this I²C slave device
    virtual unsigned char GetSlaveId() const { return m_SlaveId; }

    //! Returns the I²C master object to whom this I²C slave is assigned to.
    virtual const II2CMaster* GetMaster() { return m_pMaster; }

    friend struct CI2CMaster;
    
  protected:
    II2CMaster* m_pMaster;
    unsigned char m_SlaveId;
    unsigned int m_SlaveType;
    
  private:
    CI2CSlave(II2CMaster* pMaster, unsigned char id, unsigned int type)
      : m_pMaster(pMaster), m_SlaveId(id), m_SlaveType(type) {}

  };  


  /////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CI2CMASTER_H_ */
