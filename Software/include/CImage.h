///////////////////////////////////////////////////////////////////////////////
//! @file 	CImage.h
//! @date	Created on: 06.04.2011
//! @author	falk
///////////////////////////////////////////////////////////////////////////////

#ifndef CIMAGE_H_
#define CIMAGE_H_

//////////////////////////////////////////////////////////////////////////////

#include <boost/filesystem.hpp>
#include <algorithm>
#include <cmath>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //!
  //////////////////////////////////////////////////////////////////////////

  union URGBA
  {
    unsigned int 	uiValue;
    unsigned char 	cPixel[4];
  };


  //////////////////////////////////////////////////////////////////////////
  //! Objects of this structure encapsulate an RGBA color value.
  //////////////////////////////////////////////////////////////////////////

  struct RGBA
  {
    //////////////////////////////////////////////////////////////////////
    //! Construction
    //////////////////////////////////////////////////////////////////////

  RGBA() : 	red		(uPixel.cPixel[2]),
      green	(uPixel.cPixel[1]),
      blue	(uPixel.cPixel[0]),
      alpha	(uPixel.cPixel[3])
    {}


    //////////////////////////////////////////////////////////////////////
    //! Destruction
    //////////////////////////////////////////////////////////////////////

    const RGBA & operator()(unsigned char r,
			    unsigned char g,
			    unsigned char b,
			    unsigned char a='\255')
    {
      red 	= r;
      green 	= g;
      blue	= b;
      alpha 	= a;
    }


    //////////////////////////////////////////////////////////////////////
    //! RGB2HSI transformation
    //////////////////////////////////////////////////////////////////////

    void GetHSI(	unsigned char & ucHue,
			unsigned char & ucSaturation,
			unsigned char & ucValue) const
    {
      double dMax = std::max(std::max(red, green), blue);
      double dMin = std::min(std::min(red, green), blue);

      double dHue 		= 0.0;
      double dSaturation 	= 0.0;

      // calculating hue (0° < hue < 360°)

      while(1)
	{
	  if(dMax == dMin)
	    {
	      dHue = 0.0;
	      break;
	    }

	  if(dMax == red)
	    {
	      dHue = 60.0 * (0.0 + (double(green) - double(blue))/(dMax-dMin));
	      break;
	    }

	  if(dMax == green)
	    {
	      dHue = 60.0 * (2.0 + (double(blue) - double(red))/(dMax-dMin));
	      break;
	    }

	  if(dMax == blue)
	    {
	      dHue = 60.0 * (4.0 + (double(red) - double(green))/(dMax-dMin));
	      break;
	    }

	  break;
	}

      // calculating saturation

      while(1)
	{
	  if(dHue < 0.0)
	    {
	      dHue += 360.0;
	      continue;
	    }

	  if(dHue > 360.0)
	    {
	      dHue -= 360.0;
	      continue;
	    }

	  if(dMax == 0.0)
	    {
	      dSaturation = 0.0;
	      break;
	    }

	  dSaturation = (dMax - dMin) / dMax;
	  break;
	}

      ucHue      	= (unsigned char)(int(dHue) / 2) % 180;
      ucSaturation 	= (unsigned char) (dSaturation * 255.0);
      ucValue		= (unsigned char) dMax;
    }

    //////////////////////////////////////////////////////////////////////

    unsigned char &		alpha;
    unsigned char & 	red;
    unsigned char & 	green;
    unsigned char & 	blue;

    URGBA 	uPixel;
  };


  //////////////////////////////////////////////////////////////////////////
  //! A nice structure for defining the region of interest for images.
  //////////////////////////////////////////////////////////////////////////

  struct CRect
  {
  CRect(	unsigned short pos_x,
		unsigned short pos_y,
		unsigned short w,
		unsigned short h)
  : x(pos_x), y(pos_y), width(w), height(h) {}


    const CRect & operator()(	unsigned short pos_x,
				unsigned short pos_y,
				unsigned short w,
				unsigned short h)
    {
      x = pos_x;
      y = pos_y;
      width = w;
      height = h;

      return *this;
    }


    unsigned short	x;
    unsigned short	y;
    unsigned short	width;
    unsigned short 	height;
  };


  //////////////////////////////////////////////////////////////////////////
  //! This class represents a grabbed image.
  //////////////////////////////////////////////////////////////////////////

  class CImage
  {
  public:
    CImage(	unsigned short iWidth,
		unsigned short iHeight,
		unsigned short iChannels);

    CImage(const CImage & rhs);

    virtual ~CImage();

    virtual unsigned short GetWidth() 		const { return m_iWidth; 	}
    virtual unsigned short GetHeight() 		const { return m_iHeight; 	}
    virtual unsigned short GetChannels() 	const { return m_iChannels; }

    virtual const CRect & GetROI() 			const { return m_oRegionOfInterest; }
    virtual bool SetROI(const CRect & oROI);

    virtual unsigned char* GetRawData() { return m_pBuffer; }

    virtual bool GetPixel(	unsigned short x,
				unsigned short y,
				RGBA & sValue) const;

    virtual bool SaveBitmap(const boost::filesystem::path & oFile) const;


  protected:
    unsigned short	m_iWidth;
    unsigned short	m_iHeight;
    unsigned short	m_iChannels;

    CRect 			m_oRegionOfInterest;

    unsigned char* 	m_pBuffer;
  };


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////

#endif /* CIMAGE_H_ */
