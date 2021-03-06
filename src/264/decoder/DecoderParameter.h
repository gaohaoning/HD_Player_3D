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



#if !defined(AFX_DECODERPARAMETER_H__79149AEA_06A8_49CE_AB0A_7FC9ED7C05B5__INCLUDED_)
#define AFX_DECODERPARAMETER_H__79149AEA_06A8_49CE_AB0A_7FC9ED7C05B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// If max and min are defined by the preprocessor, STL stuff breaks so
// undefine them.
#undef max
#undef min
#include <vector>
#include "YUVFileParams.h"
// now redefine them
#define max(x,y) (((x) < (y)) ? (y) : (x))
#define min(x,y) (((x) < (y)) ? (x) : (y))
using namespace std;


class DecoderParameter  
{
public:
	DecoderParameter();
	virtual ~DecoderParameter();

  ErrVal init(int argc, char** argv);

  std::string  cBitstreamFile;
  std::string  cYuvFile;
  Int          nResult; 
  UInt         nFrames;
  UInt         uiMaxPocDiff; // HS: decoder robustness
  UInt				 uiErrorConceal;

  UInt         uiNumOfViews;
  UInt getNumOfViews() { return uiNumOfViews;}


  Bool    equals( const Char* str1, const Char* str2, UInt nLetter ) { return 0 == ::strncmp( str1, str2, nLetter); }
  std::vector <YUVFileParams> m_MultiviewReferenceFileParams;
  Int			vFramePeriod;

protected:

	
  ErrVal xPrintUsage(char** argv);
};



#endif // !defined(AFX_DECODERPARAMETER_H__79149AEA_06A8_49CE_AB0A_7FC9ED7C05B5__INCLUDED_)
