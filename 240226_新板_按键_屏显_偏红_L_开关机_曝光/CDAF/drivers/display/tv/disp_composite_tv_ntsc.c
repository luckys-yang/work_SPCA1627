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
 * @file		disp_composite_tv_ntsc.c
 * @brief		display composite tv driver with ntsc timing.
 * @author		Matt Wang
 * @since		2008-06-03
 * @date		2008-08-26
 */

/*
 * History
 *
 * 1000, 20080828 Matt Wang
 *    a. Created
 */

/**************************************************************************
 *                            H E A D E R   F I L E S
 **************************************************************************/
#include "general.h"
#include "disp_api.h"

#include "host_func_opt.h"

#if FUNC_HOST_TV
/**************************************************************************
 *                              C O N S T A N T S
 **************************************************************************/
 /* function type difinition */
#define __STATIC		//static

/* render dimension. */
#define DEV_TYPE  DISP_DEVICE_TYPE_COMPOSITE_NTSC
#define WIDTH     720
#define HEIGHT    480
#define HSYNCW    3//5
#define VSYNCW    64//5
#define HPIXEL    858
#define VLINE     525
#define PNLH0     117//121//106
#define PNLV0     33//29
#define PNLH1     (PNLH0 + WIDTH - 1)
#define PNLV1     (PNLV0 + HEIGHT - 1)

/* preview size */
#define PV_SIZE_X 320
#define PV_SIZE_Y 240

/* image region */
#define PVX0      0
#define PVY0      0
#define PVX1      (PV_SIZE_X - 1)
#define PVY1      (PV_SIZE_Y - 1)
#define PVH0      PNLH0
#define PVV0      PNLV0
#define PVH1      PNLH1
#define PVV1      PNLV1

/* osd region */
#define OSDX0     0 
#define OSDY0     0 
#define OSDX1     (PV_SIZE_X - 1) 
#define OSDY1     (PV_SIZE_Y - 1) 
#define OSDH0     PNLH0+36
#define OSDV0     PNLV0+8// +12
#define OSDH1     PNLH1 - 52 
#define OSDV1     PNLV1+8// -  14

/* clock config. */
#define CLK_SRC    DISP_CLK_SRC_XTAL27M
#define CLK_DIVNX  1
#define CLK_DIV1X  2

/**************************************************************************
 *                                  M A C R O S
 **************************************************************************/
/* function type definition. */
#define __STATIC  //static

/* operating function. */
#define dispOpDevNameGet     dispNtscOpDevNameGet
#define dispOpCapabilityGet  dispNtscOpCapabilityGet
#define dispOpPowerOn        dispNtscOpPowerOn
#define dispOpPowerOff       dispNtscOpPowerOff

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*******************************************************************************/
/* operating function. */
__STATIC UINT8 *dispOpDevNameGet(void);
__STATIC void dispOpCapabilityGet(dispCapabDesc_t *pcapabDesc);
__STATIC void dispOpPowerOn(void);
__STATIC void dispOpPowerOff(void);

/*******************************************************************************
*                            G L O B A L   D A T A
*******************************************************************************/
/* device name. */
static code UINT8 dispDevName[] = "TV - COMPOSITE NTSC";

/* device capability. */
static code dispCapabDesc_t dispCapabDesc = {
	WIDTH,
	HEIGHT,
	PNLH0,
	PNLV0,
	4,
	3,
};

/**
 * @brief	open device (register operating function).
 * @param	None.
 * @retval	return = NULL or device id.
 * @see
 * @author	Matt Wang
 * @since	2008-06-03
 * @todo	N/A
 * @bug		N/A
*/
UINT8
dispNtscOpen(
	void
)
{
	dispOperatingFunc_t dispOpFunc;


	dispOpFunc.pfOpDevNameGet    = dispOpDevNameGet;
	dispOpFunc.pfOpCapabilityGet = dispOpCapabilityGet;
	dispOpFunc.pfOpPowerOn       = dispOpPowerOn;
	dispOpFunc.pfOpPowerOff      = dispOpPowerOff;

	return dispDevOpen(&dispOpFunc, sizeof(dispOperatingFunc_t));
}

/**
 * @brief	operating function to get device name (ascii string).
 * @param	None.
 * @retval	return = device name string.
 * @see
 * @author	Matt Wang
 * @since	2008-06-03
 * @todo	N/A
 * @bug		N/A
*/
__STATIC UINT8 *
dispOpDevNameGet(
	void
)
{
	return dispDevName;
}

/**
 * @brief	operating function to get device capability.
 * @param	*pcapabDesc = [out] capability describer.
 * @param   descSize = [in] describer structure size.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-06-03
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
dispOpCapabilityGet(
	dispCapabDesc_t *pcapabDesc
)
{
	*pcapabDesc = dispCapabDesc;
}

/**
 * @brief	operating function to power on device.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-06-03
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
dispOpPowerOn(
	void
)
{
	SINT16 mtxA[9] = {0x0025,0x004b,0x000e,0x07ef,0x07dd,0x0037,0x004d,0x07c2,0x07f3};
	SINT16 mtxB[3] = {0x00};
	//-------------------------------------------------------------------------------
	dispParaSet(DISP_PARA_EDGE_GAIN_SET, 0x80, 0, 0, 0, 0); //disable edge enhancement
	//-------------------------------------------------------------------------------
	dispParaSet(DISP_PARA_GLOB_ENABLE, 0, 0, 0, 0, 0);
	/* enable tv power */
	dispParaSet(DISP_PARA_TV_DAC_POWER_ENABLE, 1, 0, 0, 0, 0);	

	/* set clk and type. */	
	dispParaSet(DISP_PARA_CLK_CFG, CLK_SRC, CLK_DIVNX, CLK_DIV1X, 0, 0);
	dispParaSet(DISP_PARA_SW_RST, DISP_DEV_RST_ALL, 0, 0, 0, 0);

	dispParaSet(DISP_PARA_DEVICE_TYPE, DEV_TYPE, 0, 0, 0, 0);

	/* set vh timing and format */
	dispParaSet(DISP_PARA_DEVICE_TOTAL_REGION, HPIXEL, VLINE, HSYNCW, VSYNCW, 0);
	dispParaSet(DISP_PARA_DEVICE_ACTIVE_REGION, PNLH0, PNLV0, PNLH1, PNLV1, 0);
	dispParaSet(DISP_PARA_DEVICE_IMG_REGION_CROP, PVX0, PVX1, PVY0, PVY1, 0);
	dispParaSet(DISP_PARA_DEVICE_IMG_REGION_RENDER, PVH0, PVH1, PVV0, PVV1, 0);
	dispParaSet(DISP_PARA_DEVICE_OSD_REGION_CROP, OSDX0, OSDX1, OSDY0, OSDY1, 0);
	dispParaSet(DISP_PARA_DEVICE_OSD_REGION_RENDER, OSDH0, OSDH1, OSDV0, OSDV1, 0);
	dispParaSet(DISP_PARA_INTERLACE_CFG, DISP_INTERLACE, 0, 0, 0, 0);
	dispParaSet(DISP_PARA_COLOR_MATRIX_TBL_CFG, (SINT16)mtxA, (SINT16)mtxB, 0, 0, 0);
	dispParaSet(DISP_PARA_COLOR_MATRIX_FMT_CFG, DISP_COLOR_MATRIX_FMT_YUV, 0, 0, 0, 0);
	dispParaSet(DISP_PARA_COLOR_MATRIX_ENABLE, 1, 0, 0, 0, 0);
	
	/* enable display */
	dispParaSet(DISP_PARA_GLOB_ENABLE, 1, 0, 0, 0, 0);
}

/**
 * @brief	operating function to power off device.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-06-03
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
dispOpPowerOff(
	void
)
{
	dispParaSet(DISP_PARA_GLOB_ENABLE, 0, 0, 0, 0, 0);
	dispParaSet(DISP_PARA_TV_DAC_POWER_ENABLE, 0, 0, 0, 0, 0);
}
#endif /* FUNC_HOST_TV */