/**************************************************************************
 *
 *        Copyright (c) 2008-2008 by Sunplus mMedia Inc., Ltd.
 *
 *  This software is copyrighted by and is the property of Sunplus
 *  mMedia Inc., Ltd. All rights are reserved by Sunplus mMedia
 *  Inc., Ltd. This software may only be used in accordance with the
 *  corresponding license agreement. Any unauthorized use, duplication,
 *  distribution, or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice MUST not be removed or modified without prior
 *  written consent of Sunplus mMedia Inc., Ltd.
 *
 *  Sunplus mMedia Inc., Ltd. reserves the right to modify this
 *  software without notice.
 *
 *  Sunplus mMedia Inc., Ltd.
 *  19-1, Innovation First Road, Science-Based Industrial Park,
 *  Hsin-Chu, Taiwan, R.O.C.
 *
 **************************************************************************/
/**
 * @file		disp_para.c
 * @brief		set display parameters.
 * @author		Ted Chiu, Matt Wang
 * @since		2008-04-29
 * @date		2008-10-03
 */

/**************************************************************************
 *                            H E A D E R   F I L E S
 **************************************************************************/
#include "general.h"
#include "disp_api.h"

void
dispPnlDimGet(
	UINT16 *dispH,
	UINT16 *dispW
)
{
	dispCapabDesc_t dispCap;


	dispCapabilityGet(dispDevActGet(), &dispCap);

	*dispW = dispCap.width;
	*dispH = dispCap.height;
}
//xian ++ 20090915
void dispPnlAspectRatioGet(UINT8 *ratX, UINT8 *ratY)
{
    dispCapabDesc_t dispCap;

    dispCapabilityGet(dispDevActGet(), &dispCap);
    *ratX = dispCap.aspectRatioX;
    *ratY = dispCap.aspectRatioY;
}
//xian --
