/**************************************************************************
 *         Copyright(c) 2011 by i-Catch Technology Co., Ltd.              *
 *                                                                        *
 *  This software is copyrighted by and is the property of i-Catch Tech-  *
 *  nology Co., Ltd. All rights are reserved by i-Catch Technology Co.,   *
 *  Ltd. This software may only be used in accordance with the            *
 *  corresponding license agreement. Any unauthorized use, duplication,   *
 *  distribution, or disclosure of this software is expressly forbidden.  *
 *                                                                        *
 *  This Copyright notice "M U S T" not be removed or modified without    *
 *  prior written consent of i-Catch Technology Co., Ltd.                 *
 *                                                                        *
 *  i-Catch Technology Co., Ltd. reserves the right to modify this        *
 *  software without notice.                                              *
 *                                                                        *
 *  i-Catch Technology Co., Ltd.                                          *
 *  19-1, Innovation First Road, Science-Based Industrial Park,           *
 *  Hsin-Chu, Taiwan, R.O.C.                                              *
 *                                                                        *
 *************************************************************************/
 
/**
 * @file      func_gui_def.h
 * @brief     GUI callback function definition
 * @author    Phil Lin
 * @since     2011-09-02
 * @date      2011-09-02
*/

#ifndef __FUNC_GUI_DEF_H__
#define __FUNC_GUI_DEF_H__

/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/
 
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#ifdef SYS_GUI_INTERNAL
#define SYS_GUI_DCL 
#else
#define SYS_GUI_DCL static 
#endif 

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
 
/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef UINT8 (code *pFuncGui)(void *ptr);

/* MUST Sync with the array "funcGuiCbArray" */
typedef enum funcGuiHdlGrp_e{

	FUNC_GUI_HDL_IDX_STILL = 0,
	FUNC_GUI_HDL_IDX_STILLPB,
	FUNC_GUI_HDL_IDX_VIDEO,
	FUNC_GUI_HDL_IDX_MENU,
	
} funcGuiHdlGrp_t;

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/
 
/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/
 
/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/
pFuncGui funcGuiHdlGet(funcGuiHdlGrp_t grp, UINT8 idx);
#endif  /* __FUNC_GUI_DEF_H__ */

