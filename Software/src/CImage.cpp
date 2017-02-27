//////////////////////////////////////////////////////////////////////////////
//! @file 	CImage.cpp
//! @date	Created on: 06.04.2011
//! @author	falk
//////////////////////////////////////////////////////////////////////////////

#include "CImage.h"
#include <cstdio>
#include <cstring>

//////////////////////////////////////////////////////////////////////////////

using namespace boost::filesystem;

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CImage::CImage(	unsigned short iWidth,
			unsigned short iHeight,
			unsigned short iChannels)
    : m_iWidth(iWidth),
      m_iHeight(iHeight),
      m_iChannels(iChannels),
      m_oRegionOfInterest(0,0,iWidth-1, iHeight-1)
  {
    unsigned int uiBufferSize = m_iWidth * m_iHeight * m_iChannels;

    m_pBuffer = new unsigned char[uiBufferSize * sizeof(unsigned char)];

    if(!m_pBuffer)
      {
	throw "CImage error when constructing\n";
      }

    memset(m_pBuffer, 0, uiBufferSize * sizeof(unsigned char));
  }


  //////////////////////////////////////////////////////////////////////////
  //! Copy construction
  //////////////////////////////////////////////////////////////////////////

  CImage::CImage(const CImage & rhs)
    : m_iWidth(rhs.m_iWidth),
      m_iHeight(rhs.m_iHeight),
      m_iChannels(rhs.m_iChannels),
      m_oRegionOfInterest(rhs.m_oRegionOfInterest)
  {
    unsigned int uiBufferSize = m_iWidth * m_iHeight * m_iChannels;

    m_pBuffer = new unsigned char[uiBufferSize * sizeof(unsigned char)];

    if(!m_pBuffer)
      {
	throw "CImage error when copy constructing!\n";
      }

    memcpy(m_pBuffer, rhs.m_pBuffer, uiBufferSize * sizeof(unsigned char));
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CImage::~CImage()
  {
    if(m_pBuffer)
      {
	delete[] m_pBuffer;
	m_pBuffer = 0;
      }
  }


  //////////////////////////////////////////////////////////////////////////
  //! Sets the region of interest for this image. All values outside this
  //! region will have undefined behaviour. Returns true if the passed ROI
  //! is valid, else false. In the latter case we will not change the ROI.
  //!
  //! The ROI's purpose is to reduce the data set that has to be
  //! transferred from the video devices and that has to be converted from
  //! YUV to RGB.
  //////////////////////////////////////////////////////////////////////////

  bool CImage::SetROI(const CRect & oROI)
  {
    bool bRet = false;

    while(1)
      {
	if(oROI.x >= m_iWidth)
	  {
	    break;
	  }

	if(oROI.y >= m_iHeight)
	  {
	    break;
	  }

	if(oROI.y + oROI.height >= m_iHeight)
	  {
	    break;
	  }

	if(oROI.x + oROI.width >= m_iWidth)
	  {
	    break;
	  }

	m_oRegionOfInterest = oROI;

	bRet = true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This method delivers the RGB pixel triple at position (x,y) in sValue.
  //! Returns true if pixel was successfully read, else false.
  //! Note that the pixels in this image are in BGR format.
  //////////////////////////////////////////////////////////////////////////

  bool CImage::GetPixel(	unsigned short x,
				unsigned short y,
				RGBA & sValue) const
  {
    bool bRet = false;

    while(1)
      {
	if(x < m_oRegionOfInterest.x)
	  {
	    break;
	  }

	if(x > m_oRegionOfInterest.x + m_oRegionOfInterest.width)
	  {
	    break;
	  }

	if(y < m_oRegionOfInterest.y)
	  {
	    break;
	  }

	if(y > m_oRegionOfInterest.y + m_oRegionOfInterest.height)
	  {
	    break;
	  }

	const unsigned char* 	pPosition = m_pBuffer + y*m_iWidth*m_iChannels + x*m_iChannels;
	memcpy(sValue.uPixel.cPixel, pPosition, 3*sizeof(unsigned char));

	bRet = true;
	break;
      }

    return bRet;
  }


  //////////////////////////////////////////////////////////////////////////
  //! This method saves this image in an bitmap file.
  //////////////////////////////////////////////////////////////////////////

  bool CImage::SaveBitmap(const boost::filesystem::path & oFile) const
  {
    bool bRet = false;

    while(1)
      {
	try
	  {
	    if(	boost::filesystem::exists(oFile) &&
		boost::filesystem::is_directory(oFile) )
	      {
		break;
	      }
	  }
	catch(...)
	  {
	    break;
	  }

	// TODO: convert the following stuff of Dreambox AiO Grab into clean C++ with fstreams and boost::filesystem
	// and find a nicer way for constructing the bitmap header without these nasty macros.

	FILE *fd2 = fopen(oFile.string().c_str(), "wr");

	// write bmp
	unsigned char hdr[14 + 40];
	int i = 0;

#define PUT32(x) hdr[i++] = ((x)&0xFF); hdr[i++] = (((x)>>8)&0xFF); hdr[i++] = (((x)>>16)&0xFF); hdr[i++] = (((x)>>24)&0xFF);
#define PUT16(x) hdr[i++] = ((x)&0xFF); hdr[i++] = (((x)>>8)&0xFF);
#define PUT8(x) hdr[i++] = ((x)&0xFF);

	PUT8('B'); PUT8('M');
	PUT32((((m_iWidth * m_iHeight) * m_iChannels + 3) &~ 3) + 14 + 40);
	PUT16(0); PUT16(0); PUT32(14 + 40);
	PUT32(40); PUT32(m_iWidth); PUT32(m_iHeight);
	PUT16(1);
	PUT16(m_iChannels*8); // bits
	PUT32(0); PUT32(0); PUT32(0); PUT32(0); PUT32(0); PUT32(0);

#undef PUT32
#undef PUT16
#undef PUT8
	fwrite(hdr, 1, i, fd2);

	signed int y;

	for (y=m_iHeight-1; y>=0 ; --y) {
	  fwrite(m_pBuffer+(y*m_iWidth*m_iChannels),m_iWidth*m_iChannels,1,fd2);
	}

	fclose(fd2);

	bRet = true;
	break;
      }

    return bRet;
  }

  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
