/*
********************************************************************************

NOTE - One of the two copyright statements below may be chosen
       that applies for the software.

********************************************************************************

This software module was originally developed by

Heiko Schwarz    (Fraunhofer HHI),
Tobias Hinz      (Fraunhofer HHI),
Karsten Suehring (Fraunhofer HHI)

in the course of development of the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video
Coding) for reference purposes and its performance may not have been optimized.
This software module is an implementation of one or more tools as specified by
the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding).

Those intending to use this software module in products are advised that its
use may infringe existing patents. ISO/IEC have no liability for use of this
software module or modifications thereof.

Assurance that the originally developed software module can be used
(1) in the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding) once the
ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding) has been adopted; and
(2) to develop the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding): 

To the extent that Fraunhofer HHI owns patent rights that would be required to
make, use, or sell the originally developed software module or portions thereof
included in the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding) in a
conforming product, Fraunhofer HHI will assure the ISO/IEC that it is willing
to negotiate licenses under reasonable and non-discriminatory terms and
conditions with applicants throughout the world.

Fraunhofer HHI retains full right to modify and use the code for its own
purpose, assign or donate the code to a third party and to inhibit third
parties from using the code for products that do not conform to MPEG-related
ITU Recommendations and/or ISO/IEC International Standards. 

This copyright notice must be included in all copies or derivative works.
Copyright (c) ISO/IEC 2005. 

********************************************************************************

COPYRIGHT AND WARRANTY INFORMATION

Copyright 2005, International Telecommunications Union, Geneva

The Fraunhofer HHI hereby donate this source code to the ITU, with the following
understanding:
    1. Fraunhofer HHI retain the right to do whatever they wish with the
       contributed source code, without limit.
    2. Fraunhofer HHI retain full patent rights (if any exist) in the technical
       content of techniques and algorithms herein.
    3. The ITU shall make this code available to anyone, free of license or
       royalty fees.

DISCLAIMER OF WARRANTY

These software programs are available to the user without any license fee or
royalty on an "as is" basis. The ITU disclaims any and all warranties, whether
express, implied, or statutory, including any implied warranties of
merchantability or of fitness for a particular purpose. In no event shall the
contributor or the ITU be liable for any incidental, punitive, or consequential
damages of any kind whatsoever arising from the use of these programs.

This disclaimer of warranty extends to the user of these programs and user's
customers, employees, agents, transferees, successors, and assigns.

The ITU does not represent or warrant that the programs furnished hereunder are
free of infringement of any third-party patents. Commercial implementations of
ITU-T Recommendations, including shareware, may be subject to royalty fees to
patent holders. Information regarding the ITU-T patent policy is available from 
the ITU Web site at http://www.itu.int.

THIS IS NOT A GRANT OF PATENT RIGHTS - SEE THE ITU-T PATENT POLICY.

********************************************************************************
*/



#include "H264AVCDecoderLibTest.h"
#include "H264AVCDecoderTest.h"


H264AVCDecoderTest::H264AVCDecoderTest() :
  m_pcH264AVCDecoder( NULL ),
  m_pcH264AVCDecoderSuffix( NULL ), //JVT-S036 lsj
//TMM_EC  m_pcReadBitstream( NULL ),
//TMM_EC  m_pcWriteYuv( NULL ),
  m_pcParameter( NULL ),
  m_cActivePicBufferList( ),
  m_cUnusedPicBufferList( )
{
}


H264AVCDecoderTest::~H264AVCDecoderTest()
{
}


ErrVal H264AVCDecoderTest::create( H264AVCDecoderTest*& rpcH264AVCDecoderTest )
{
  rpcH264AVCDecoderTest = new H264AVCDecoderTest;
  ROT( NULL == rpcH264AVCDecoderTest );
  return Err::m_nOK;
}


ErrVal H264AVCDecoderTest::init( DecoderParameter *pcDecoderParameter, WriteYuvToFile *pcWriterYuv, ReadBitstreamFile *pcReadBitstreamFile ) //TMM_EC
{
  ROT( NULL == pcDecoderParameter );

  m_pcParameter = pcDecoderParameter;
  m_pcParameter->nResult = -1;
/*TMM_EC
  RNOKS( WriteYuvToFile::create( m_pcWriteYuv, m_pcParameter->cYuvFile ) );

  ReadBitstreamFile *pcReadBitstreamFile;
  RNOKS( ReadBitstreamFile::create( pcReadBitstreamFile ) ); 
  RNOKS( pcReadBitstreamFile->init( m_pcParameter->cBitstreamFile ) );  
//*///
	m_pcWriteYuv	=	pcWriterYuv;
  m_pcReadBitstream = (ReadBitstreamIf*)pcReadBitstreamFile;

  RNOK( h264::CreaterH264AVCDecoder::create( m_pcH264AVCDecoder ) );
	m_pcH264AVCDecoder->setec( m_pcParameter->uiErrorConceal);

	RNOK( h264::CreaterH264AVCDecoder::create( m_pcH264AVCDecoderSuffix ) );  //JVT-S036 
  return Err::m_nOK;
}

ErrVal	H264AVCDecoderTest::setec( UInt uiErrorConceal)
{
	return	m_pcH264AVCDecoder->setec( uiErrorConceal);
}

ErrVal H264AVCDecoderTest::destroy()
{

  if( NULL != m_pcH264AVCDecoder )       
  {
    RNOK( m_pcH264AVCDecoder->destroy() );       
  }

  if( NULL != m_pcH264AVCDecoderSuffix )       
  {//JVT-S036 
    RNOK( m_pcH264AVCDecoderSuffix->destroy() );       
  }
/*TMM_EC
  if( NULL != m_pcWriteYuv )              
  {
    RNOK( m_pcWriteYuv->destroy() );  
  }

  if( NULL != m_pcReadBitstream )     
  {
    RNOK( m_pcReadBitstream->uninit() );  
    RNOK( m_pcReadBitstream->destroy() );  
  }
*/
  AOF( m_cActivePicBufferList.empty() );
  
  //===== delete picture buffer =====
  PicBufferList::iterator iter;
  for( iter = m_cUnusedPicBufferList.begin(); iter != m_cUnusedPicBufferList.end(); iter++ )
  {
    delete (*iter)->getBuffer();
    delete (*iter);
  }
  for( iter = m_cActivePicBufferList.begin(); iter != m_cActivePicBufferList.end(); iter++ )
  {
    delete (*iter)->getBuffer();
    delete (*iter);
  }

  delete this;
  return Err::m_nOK;
}


ErrVal H264AVCDecoderTest::xGetNewPicBuffer ( PicBuffer*& rpcPicBuffer, UInt uiSize )
{
  if( m_cUnusedPicBufferList.empty() )
  {
    rpcPicBuffer = new PicBuffer( new UChar[ uiSize ] );
  }
  else
  {
    rpcPicBuffer = m_cUnusedPicBufferList.popFront();
  }

  m_cActivePicBufferList.push_back( rpcPicBuffer );
  return Err::m_nOK;
}


ErrVal H264AVCDecoderTest::xRemovePicBuffer( PicBufferList& rcPicBufferUnusedList )
{
  while( ! rcPicBufferUnusedList.empty() )
  {
    PicBuffer* pcBuffer = rcPicBufferUnusedList.popFront();

    if( NULL != pcBuffer )
    {
      PicBufferList::iterator  begin = m_cActivePicBufferList.begin();
      PicBufferList::iterator  end   = m_cActivePicBufferList.end  ();
      PicBufferList::iterator  iter  = std::find( begin, end, pcBuffer );
    
      AOT( pcBuffer->isUsed() )
      m_cUnusedPicBufferList.push_back( pcBuffer );
	  if (iter!=end)
	      m_cActivePicBufferList.erase    (  iter );
    }
  }
  return Err::m_nOK;
}


ErrVal H264AVCDecoderTest::go()
{
  PicBuffer*    pcPicBuffer = NULL;
  PicBufferList cPicBufferOutputList; 
  PicBufferList cPicBufferUnusedList;
  PicBufferList cPicBufferReleaseList;

  UInt      uiMbX           = 0;
  UInt      uiMbY           = 0;
  UInt      uiNalUnitType   = 0;
  UInt      uiSize          = 0;
  UInt      uiLumOffset     = 0;
  UInt      uiCbOffset      = 0;
  UInt      uiCrOffset      = 0;
  UInt      uiFrame;
  
  Bool      bEOS            = false;
  Bool      bYuvDimSet      = false;


  // HS: packet trace
  UInt   uiMaxPocDiff = m_pcParameter->uiMaxPocDiff;
  UInt   uiLastPoc    = MSYS_UINT_MAX;
  UChar* pcLastFrame  = 0;
  UInt   uiPreNalUnitType = 0;

  cPicBufferOutputList.clear();
  cPicBufferUnusedList.clear();


  RNOK( m_pcH264AVCDecoder->init(true, m_pcParameter) ); 


  Bool bToDecode = false; //JVT-P031
  for( uiFrame = 0; ( uiFrame <= MSYS_UINT_MAX && ! bEOS); )
  {
    BinData* pcBinData;
    BinDataAccessor cBinDataAccessor;

    Int  iPos;
//    Bool bFinishChecking;

    RNOK( m_pcReadBitstream->getPosition(iPos) );

    //JVT-P031
    Bool bFragmented = false;
    Bool bDiscardable = false;
    Bool bStart = false;
    Bool bFirst = true;
    UInt uiTotalLength = 0;
#define MAX_FRAGMENTS 10 // hard-coded
    BinData* pcBinDataTmp[MAX_FRAGMENTS];
    BinDataAccessor cBinDataAccessorTmp[MAX_FRAGMENTS];
    UInt uiFragNb, auiStartPos[MAX_FRAGMENTS], auiEndPos[MAX_FRAGMENTS];
	Bool bConcatenated = false; //FRAG_FIX_3
    uiFragNb = 0;
    bEOS = false;
    pcBinData = 0;

    while(!bStart && !bEOS)
    {
      if(bFirst)
      {
          RNOK( m_pcReadBitstream->setPosition(iPos) );
          bFirst = false;
      }

      RNOK( m_pcReadBitstream->extractPacket( pcBinDataTmp[uiFragNb], bEOS ) );

//TMM_EC {{
			if( !bEOS && ((pcBinDataTmp[uiFragNb]->data())[0] & 0x1f )== 0x0b)
			{
				printf("end of stream\n");
				bEOS=true;
				uiNalUnitType= uiPreNalUnitType;
        RNOK( m_pcReadBitstream->releasePacket( pcBinDataTmp[uiFragNb] ) );
        pcBinDataTmp[uiFragNb] = new BinData;
				uiTotalLength	=	0;
        pcBinDataTmp[uiFragNb]->set( new UChar[uiTotalLength], uiTotalLength );
			}
//TMM_EC }}

      pcBinDataTmp[uiFragNb]->setMemAccessor( cBinDataAccessorTmp[uiFragNb] );
      // open the NAL Unit, determine the type and if it's a slice get the frame size

      RNOK( m_pcH264AVCDecoder->initPacket( &cBinDataAccessorTmp[uiFragNb], 
                                            uiNalUnitType, uiMbX, uiMbY, uiSize,  true, 
		  false, //FRAG_FIX_3
		  bStart, auiStartPos[uiFragNb], auiEndPos[uiFragNb], bFragmented, bDiscardable ) );

      uiTotalLength += auiEndPos[uiFragNb] - auiStartPos[uiFragNb];

      if(!bStart)
      {
        ROT( bEOS) ; //jerome.vieron@thomson.net
        uiFragNb++;
      }
      else
      {
        if(pcBinDataTmp[0]->size() != 0)
        {
          pcBinData = new BinData;
          pcBinData->set( new UChar[uiTotalLength], uiTotalLength );
          // append fragments
          UInt uiOffset = 0;
          for(UInt uiFrag = 0; uiFrag<uiFragNb+1; uiFrag++)
          {
              memcpy(pcBinData->data()+uiOffset, pcBinDataTmp[uiFrag]->data() + auiStartPos[uiFrag], auiEndPos[uiFrag]-auiStartPos[uiFrag]);
              uiOffset += auiEndPos[uiFrag]-auiStartPos[uiFrag];
              RNOK( m_pcReadBitstream->releasePacket( pcBinDataTmp[uiFrag] ) );
              pcBinDataTmp[uiFrag] = NULL;
              if(uiNalUnitType != 6) //JVT-T054
              m_pcH264AVCDecoder->decreaseNumOfNALInAU();
			  //FRAG_FIX_3
			  if(uiFrag > 0) 
				  bConcatenated = true; //~FRAG_FIX_3
          }
          
          pcBinData->setMemAccessor( cBinDataAccessor );
          bToDecode = false;
          if((uiTotalLength != 0) && (!bDiscardable || bFragmented))
          {
              //FRAG_FIX
			if( (uiNalUnitType == 20) || (uiNalUnitType == 21) || (uiNalUnitType == 1) || (uiNalUnitType == 5) )
            {
                uiPreNalUnitType=uiNalUnitType;
                RNOK( m_pcH264AVCDecoder->initPacket( &cBinDataAccessor, uiNalUnitType, uiMbX, uiMbY, uiSize, 
					//uiNonRequiredPic, //NonRequired JVT-Q066
                    false, bConcatenated, //FRAG_FIX_3
					bStart, auiStartPos[uiFragNb+1], auiEndPos[uiFragNb+1], 
                    bFragmented, bDiscardable) );
            }

        else if( uiNalUnitType == 14 )
          {
                uiPreNalUnitType=uiNalUnitType;
                RNOK( m_pcH264AVCDecoder->initPacket( &cBinDataAccessor, uiNalUnitType, uiMbX, uiMbY, uiSize, 
					//uiNonRequiredPic, //NonRequired JVT-Q066
                    false, bConcatenated, //FRAG_FIX_3
					bStart, auiStartPos[uiFragNb+1], auiEndPos[uiFragNb+1], 
                    bFragmented, bDiscardable) );
              }
              else
                  m_pcH264AVCDecoder->initPacket( &cBinDataAccessor );
              bToDecode = true;

              if( uiNalUnitType == 14 )
                bToDecode = false;
          }
        }
      }
    }

    //~JVT-P031

//NonRequired JVT-Q066{
	if(m_pcH264AVCDecoder->isNonRequiredPic())
		continue;
//NonRequired JVT-Q066}


// JVT-Q054 Red. Picture {
  RNOK( m_pcH264AVCDecoder->checkRedundantPic() );
  if ( m_pcH264AVCDecoder->isRedundantPic() )
    continue;
// JVT-Q054 Red. Picture }



  if(bToDecode)//JVT-P031
  {
    // get new picture buffer if required if coded Slice || coded IDR slice
    pcPicBuffer = NULL;
    
    if( uiNalUnitType == 1 || uiNalUnitType == 5 || uiNalUnitType == 20 || uiNalUnitType == 21 )
    {
      RNOK( xGetNewPicBuffer( pcPicBuffer, uiSize ) );

      if( ! bYuvDimSet )
      {
        UInt uiLumSize  = ((uiMbX<<3)+  YUV_X_MARGIN) * ((uiMbY<<3)    + YUV_Y_MARGIN ) * 4;
        uiLumOffset     = ((uiMbX<<4)+2*YUV_X_MARGIN) * YUV_Y_MARGIN   + YUV_X_MARGIN;  
        uiCbOffset      = ((uiMbX<<3)+  YUV_X_MARGIN) * YUV_Y_MARGIN/2 + YUV_X_MARGIN/2 + uiLumSize; 
        uiCrOffset      = ((uiMbX<<3)+  YUV_X_MARGIN) * YUV_Y_MARGIN/2 + YUV_X_MARGIN/2 + 5*uiLumSize/4;
        bYuvDimSet = true;

        // HS: decoder robustness
        pcLastFrame = new UChar [uiSize];
        ROF( pcLastFrame );
      }
    }
    

    // decode the NAL unit
    RNOK( m_pcH264AVCDecoder->process( pcPicBuffer, cPicBufferOutputList, cPicBufferUnusedList, cPicBufferReleaseList ) );

	// ROI DECODE ICU/ETRI
	m_pcH264AVCDecoder->RoiDecodeInit();

    // picture output
    while( ! cPicBufferOutputList.empty() )
    {

//JVT-V054    
      if(!m_pcWriteYuv->getFileInitDone() )
      {
        UInt *vcOrder = m_pcH264AVCDecoder->getViewCodingOrder();
       	m_pcWriteYuv->xInitMVC(m_pcParameter->cYuvFile, vcOrder, m_pcParameter->getNumOfViews()); // JVT-AB024 modified remove active view info SEI  			
      }

        PicBuffer* pcPicBufferTmp = cPicBufferOutputList.front();
      cPicBufferOutputList.pop_front();
        if( pcPicBufferTmp != NULL )
      {
        // HS: decoder robustness
          while( uiLastPoc + uiMaxPocDiff < (UInt)pcPicBufferTmp->getCts() )
        {
          RNOK( m_pcWriteYuv->writeFrame( pcLastFrame + uiLumOffset, 
                                          pcLastFrame + uiCbOffset, 
                                          pcLastFrame + uiCrOffset,
                                           uiMbY << 4,
                                           uiMbX << 4,
                                          (uiMbX << 4)+ YUV_X_MARGIN*2 ) );
          printf("REPEAT FRAME\n");
          uiFrame   ++;
          uiLastPoc += uiMaxPocDiff;
        }

		  
          if(m_pcParameter->getNumOfViews() > 0)
          {
			  UInt view_cnt;
			  for (view_cnt=0; view_cnt < m_pcParameter->getNumOfViews(); view_cnt++)
				if ((UInt)pcPicBufferTmp->getViewId() == m_pcH264AVCDecoder->getViewCodingOrder()[view_cnt])
					break;

			  RNOK( m_pcWriteYuv->writeFrame( *pcPicBufferTmp + uiLumOffset, 
                                              *pcPicBufferTmp + uiCbOffset, 
                                              *pcPicBufferTmp + uiCrOffset,
                                              uiMbY << 4,
                                              uiMbX << 4,
                                              (uiMbX << 4)+ YUV_X_MARGIN*2,
                                              //(UInt)pcPicBufferTmp->getViewId(),
											   view_cnt) ); 
          }
          else
        RNOK( m_pcWriteYuv->writeFrame( *pcPicBufferTmp + uiLumOffset, 
                                        *pcPicBufferTmp + uiCbOffset, 
                                        *pcPicBufferTmp + uiCrOffset,
                                         uiMbY << 4,
                                         uiMbX << 4,
                                        (uiMbX << 4)+ YUV_X_MARGIN*2 ) );
        uiFrame++;
      
    
        // HS: decoder robustness
        uiLastPoc = (UInt)pcPicBufferTmp->getCts();
        ::memcpy( pcLastFrame, *pcPicBufferTmp+0, uiSize*sizeof(UChar) );
      }
    }
   } 
    RNOK( xRemovePicBuffer( cPicBufferReleaseList ) );
    RNOK( xRemovePicBuffer( cPicBufferUnusedList ) );
    if( pcBinData )
    {
      RNOK( m_pcReadBitstream->releasePacket( pcBinData ) );
      pcBinData = 0;
    }
  }
  printf("\n %d frames decoded\n", uiFrame );

  delete [] pcLastFrame; // HS: decoder robustness
  
  RNOK( m_pcH264AVCDecoder->uninit( true ) );
  
  m_pcParameter->nFrames  = uiFrame;
  m_pcParameter->nResult  = 0;

  return Err::m_nOK;
}





