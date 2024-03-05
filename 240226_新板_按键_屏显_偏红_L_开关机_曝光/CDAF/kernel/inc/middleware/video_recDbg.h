/**************************************************************************
 *         Copyright(c) 2007 by Sunplus mMedia  Technology Co., Ltd.       *
 *                                                                         *
 *  This software is copyrighted by and is the property of Sunplus mMedia  *
 *  Technology Co., Ltd. All rights are reserved by Sunplus mMedia Techno- *
 *  logy Co., Ltd. This software may only be used in accordance with the   *
 *  corresponding license agreement. Any unauthorized use, duplication,    *
 *  distribution, or disclosure of this software is expressly forbidden.   *
 *                                                                         *
 *  This Copyright notice "M U S T" not be removed or modified without     *
 *  prior written consent of Sunplus mMedia Technology  Co., Ltd.          *
 *                                                                         *
 *  Sunplus mMedia Technology Co., Ltd. reserves the right to modify this  *
 *  software without notice.                                               *
 *                                                                         *
 *  Sunplus mMedia Technology Co., Ltd.                                    *
 *  19-1, Innovation First Road, Science-Based Industrial Park,            *
 *  Hsin-Chu, Taiwan, R.O.C.                                               *
 *                                                                         *
 **************************************************************************/
#ifndef __VIDEO_RECDBG_H__
#define __VIDEO_RECDBG_H__
#include "general.h"
#include "dbg_def.h"
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define VIDEO_REC_TIMER 0
#if VIDEO_REC_TIMER
void vidDbgTimerPos(UINT8 frameStart);
void vidDbgTimerPrintf();
void vidDbgTimerReset();
extern UINT16 vidTimeSkip[];

#else
#define vidDbgTimerPos(frameStart)
#define vidDbgTimerPrintf()
#define vidDbgTimerReset()
#define vidTimeSkip ((UINT8*)0x3800)
#endif 

//#define vidPrintfAbort printf
//#define vidPrintfMem printf
/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/


/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/


/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/
#ifndef vidPrintfAbort
#define vidPrintfAbort /_printf
#endif

#ifndef vidPrintfMem
#define vidPrintfMem /_printf
#endif

#endif

