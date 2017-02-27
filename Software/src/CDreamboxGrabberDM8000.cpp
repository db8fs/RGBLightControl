//////////////////////////////////////////////////////////////////////////////
//! @file 	CDreamboxGrabberDM8000.cpp
//! @date	Created on: 06.04.2011
//! @author	Falk Schilling, based on Seddi's <seddi@ihad.tv>
//! 		AiO Dreambox Screengrabber v0.83a code (which is licensed under
//!			GPLv2).
//////////////////////////////////////////////////////////////////////////////

#include "CDreamboxGrabberDM8000.h"
#include "config.h"

#ifdef HAVE_DREAMBOX
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/types.h>
#include <linux/videodev.h>
#include <linux/fb.h>

#define MEMORY_DEVICE	"/dev/mem"
#define VIDEO_DEVICE	"/dev/video"
#define SPARE_RAM 		252*1024*1024
#define DMA_BLOCKSIZE 	0x3FF000
#endif

#include <iostream>

//////////////////////////////////////////////////////////////////////////////

namespace RGBLightControl {

  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  //! Construction
  //////////////////////////////////////////////////////////////////////////

  CDreamboxGrabberDM8000::CDreamboxGrabberDM8000()
    : IDreamboxGrabber(),
      m_oImage(VIDEO_WIDTH, VIDEO_HEIGHT, 3)
  {
  }


  //////////////////////////////////////////////////////////////////////////
  //! Destruction
  //////////////////////////////////////////////////////////////////////////

  CDreamboxGrabberDM8000::~CDreamboxGrabberDM8000()
  {
    this->~IDreamboxGrabber();
  }


  //////////////////////////////////////////////////////////////////////////
  //! This method fetches an image from the decoder and does some
  //! conversion stuff to create a RGB image. It is way too much stuff in
  //! this function - a better approach would be splitting it up into more
  //! member functions and use member variables instead.
  //!
  //! Note that the ROI will influence the grabbing behaviour. If you set
  //! set the ROI to (0,0)-(0,0) you will grab the complete picture.
  //////////////////////////////////////////////////////////////////////////

  const CImage & CDreamboxGrabberDM8000::GetImage()
  {
#ifdef HAVE_DREAMBOX

    bool bRes = false;

    while(1)
      {
	void* 	pMemory 	= NULL;
	int		iMemoryFD 	= 0;

	if((iMemoryFD = open(MEMORY_DEVICE, O_RDWR | O_SYNC)) < 0)
	  {
	    std::cerr << "Main memory: Can't open /dev/mem!" << std::endl;
	    break;
	  }

	//std::cout << "Grabbing video image started" << std::endl;

	unsigned char *pLuma 				= NULL;
	unsigned char *pChroma 				= NULL;
	unsigned char *pTemporaryMemory		= NULL;

	int stride 	= 0;
	int res		= 0;
	int t		= 0;


	// now do the real operations

	while(1)
	  {
	    pMemory = (unsigned char*) mmap(0, 100, PROT_READ, MAP_SHARED, iMemoryFD, 0x10100000);

	    if(!pMemory)
	      {
		std:: cerr << "Main memory: <memmapping failed!>" << std::endl;
		break;
	      }

	    unsigned char data[100];

	    int adr,adr2,ofs,ofs2,offset/*,vert_start,vert_end*/;
	    int xtmp,xsub,ytmp,t2,dat1;

	    memcpy(data,pMemory,100);

	    //vert_start=data[0x1B]<<8|data[0x1A];
	    //vert_end=data[0x19]<<8|data[0x18];
	    stride = data[0x15]<<8 | data[0x14];
	    ofs	 = (data[0x28]<<8  | data[0x27]) >> 4;
	    ofs2 = (data[0x2c]<<8  | data[0x2b]) >> 4;
	    adr	= (data[0x1f]<<24  | data[0x1e]<<16 | data[0x1d]<<8 | data[0x1c]) & 0xFFFFFF00;
	    adr2 = (data[0x23]<<24 | data[0x22]<<16 | data[0x21]<<8 | data[0x20]) & 0xFFFFFF00;
	    offset = adr2-adr;

	    munmap(pMemory, 100);

	    // read the vertical resolution

	    boost::spirit::parse( 	CSystemTools::GetCommandOutput("cat /proc/stb/vmpeg/0/yres").c_str(),
					hex_p[assign_a(res)],
					space_p);

	    pLuma 	= new (unsigned char[stride*ofs]);
	    pChroma	= new (unsigned char[stride*(ofs2+64)]);

	    // use DMA on DM8000 for acquiring luma and chroma planes

	    pTemporaryMemory = (unsigned char*) mmap(0, DMA_BLOCKSIZE + 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, iMemoryFD, SPARE_RAM);

	    if(!pTemporaryMemory)
	      {
		std:: cerr << "Main memory: <memmapping failed!>" << std::endl;
		break;
	      }

	    volatile unsigned long *pMemoryDMA = NULL;

	    pMemoryDMA = (volatile unsigned long*) mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, iMemoryFD, 0x10c02000);

	    if(!pMemoryDMA)
	      {
		std:: cerr << "Main memory: <memmapping failed!>" << std::endl;
		break;
	      }

	    int i = 0;
	    int	iTempLength = DMA_BLOCKSIZE;
	    int iTempSize = offset + stride*(ofs2+64);

	    //std::cout << "Starting DMA..." << std::endl;

	    for(i=0; i< iTempSize; i += DMA_BLOCKSIZE)
	      {
		unsigned long *pDescriptor = ((unsigned long*) pTemporaryMemory);

		if(i + DMA_BLOCKSIZE > iTempSize)
		  {
		    iTempLength = iTempSize - i;
		  }

		pDescriptor[0]	= /* READ 	*/	adr + i;
		pDescriptor[1]	= /* WRITE 	*/	SPARE_RAM + 0x1000;
		pDescriptor[2]	= /* LENGTH */	0x40000000 | iTempLength;
		pDescriptor[3]	= 0;
		pDescriptor[4]	= 0;
		pDescriptor[5]	= 0;
		pDescriptor[6] 	= 0;
		pDescriptor[7] 	= 0;

		pMemoryDMA[1]	= /* FIRST_DESCRIPTOR */ 	SPARE_RAM;
		pMemoryDMA[3]	= /* DMA WAKE CTRL */		3;

		// start transfer
		pMemoryDMA[2]	= 	1;

		// wait for DMA being finished
		while(pMemoryDMA[5] == 1)
		  {
		    usleep(2);
		  }

		// reset DMA
		pMemoryDMA[2] 	= 	0;
	      }

	    munmap((void*)pMemoryDMA, 0x1000);
	    pTemporaryMemory += 0x1000;

	    t 	 = 0;
	    t2 	 = 0;
	    dat1 = 0;
	    
	    int iStrideChromaLuma = 0x40;
	    int sw = 1;
	    
	    xsub = iStrideChromaLuma;

	    //std::cout << "Decoding luma and chroma planes..." << std::endl;

	    // decode luma and chroma plane

	    for(xtmp = 0; xtmp < stride; xtmp += iStrideChromaLuma)
	      {
		if ((stride-xtmp) <= iStrideChromaLuma)
		  {
		    xsub = stride - xtmp;
		  }

		dat1 	= xtmp;
		sw 		= 1;

		for(ytmp = 0; ytmp < ofs; ++ytmp)
		  {
		    memcpy(pLuma + dat1, pTemporaryMemory + t, xsub);

		    t += iStrideChromaLuma;

		    // switch commands for performance reasons

		    switch(ofs2 - ytmp)
		      {
		      case 0:
			sw = 0;
			break;
		      }

		    switch(sw)
		      {
		      case 1:
			memcpy(pChroma + dat1, pTemporaryMemory + offset + t2, xsub);
			t2 += iStrideChromaLuma;
			break;
		      }

		    dat1 += stride;
		  }
	      }

	    munmap(pTemporaryMemory, DMA_BLOCKSIZE + 0x1000);

	    //std::cout << "Swapping Chroma and Luma" << std::endl;

	    for(t=0; t<stride*ofs; t+=4)
	      {
		SWAP(pLuma[t], 		pLuma[t+3]);
		SWAP(pLuma[t+1], 	pLuma[t+2]);

		if( t < stride*(ofs>>1) )
		  {
		    SWAP(pChroma[t], 	pChroma[t+3]);
		    SWAP(pChroma[t+1], 	pChroma[t+2]);
		  }
	      }

	    //std::cout << "Applying YUV2RGB conversion" << std::endl;

	    int Y, U, V, y, x, out1, pos, RU, GU, GV, BV, rgbstride;
	    unsigned char* pVideo = m_oImage.GetRawData();

	    Y=U=V=0;
	    out1=pos=t=0;
	    rgbstride = stride*3;

	    // YUV2RGB conversion

	    for(y = res; y!=0; y-=2)
	      {
		for(x = stride; x != 0; x-=2)
		  {
		    U = pChroma[t++];
		    V = pChroma[t++];

		    // use lookuptables

		    RU = yuv2rgbtable_ru[U];
		    GU = yuv2rgbtable_gu[U];
		    GV = yuv2rgbtable_gv[V];
		    BV = yuv2rgbtable_bv[V];

		    Y = yuv2rgbtable_y[ pLuma[pos] ];

		    pVideo[out1] 	= CLAMP((Y + RU) >> 16);
		    pVideo[out1+1] 	= CLAMP((Y - GV - GU) >> 16);
		    pVideo[out1+2]	= CLAMP((Y + BV) >> 16);

		    Y = yuv2rgbtable_y[ pLuma[stride + pos] ];

		    pVideo[out1+rgbstride]   = CLAMP((Y + RU) >> 16);
		    pVideo[out1+rgbstride+1] = CLAMP((Y - GV - GU) >> 16);
		    pVideo[out1+rgbstride+2] = CLAMP((Y + BV) >> 16);

		    out1 += 3;
		    ++pos;

		    Y = yuv2rgbtable_y[ pLuma[pos] ];

		    pVideo[out1] 	= CLAMP((Y + RU) >> 16);
		    pVideo[out1+1] 	= CLAMP((Y - GV - GU) >> 16);
		    pVideo[out1+2]	= CLAMP((Y + BV) >> 16);

		    Y = yuv2rgbtable_y[ pLuma[stride + pos] ];

		    pVideo[out1+rgbstride]   = CLAMP((Y + RU) >> 16);
		    pVideo[out1+rgbstride+1] = CLAMP((Y - GV - GU) >> 16);
		    pVideo[out1+rgbstride+2] = CLAMP((Y + BV) >> 16);

		    out1 += 3;
		    ++pos;
		  }

		out1 += rgbstride;
		pos  += stride;
	      }

	    //std::cout << "Video image successfully grabbed!" << std::endl;
	    break;
	  }

	close(iMemoryFD);

	if(pLuma)
	  {
	    delete[] pLuma;
	    pLuma = NULL;
	  }

	if(pChroma)
	  {
	    delete[] pChroma;
	    pChroma = NULL;
	  }

	bRes = true;
	break;
      }

#endif

    return m_oImage;
  }


  //////////////////////////////////////////////////////////////////////////////

} // namespace RGBLightControl

//////////////////////////////////////////////////////////////////////////////
