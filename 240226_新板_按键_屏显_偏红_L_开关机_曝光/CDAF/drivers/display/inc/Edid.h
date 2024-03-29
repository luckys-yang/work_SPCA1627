/******************************************************************************\

          (c) Copyright Explore Semiconductor, Inc. Limited 2005
                           ALL RIGHTS RESERVED 

--------------------------------------------------------------------------------

  File        :  Edid.h 

  Description :  Head file of Edid IO Interface 

\******************************************************************************/

#ifndef EDID_H
#define EDID_H

#define EDID_BLOCK_SIZE  128

//#define max(a,b) (((a)>(b))? (a):(b))


//
// Structure Definitions
//

extern unsigned char EDID_GetHDMICap(unsigned char *pTarget);
extern unsigned char EDID_GetPCMFreqCap(unsigned char *pTarget);
extern unsigned char EDID_GetPCMChannelCap(unsigned char *pTarget);
extern unsigned char EDID_GetDataBlockAddr(unsigned char *pTarget, unsigned char Tag);
extern unsigned char EDID_Parse_VideoSTD(unsigned char *pTarget);
#endif // EDID_H


