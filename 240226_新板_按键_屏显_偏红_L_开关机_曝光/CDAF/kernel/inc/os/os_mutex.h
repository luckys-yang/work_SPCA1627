/**************************************************************************
 *         Copyright(c) 2010 by i-Catch Technology Co., Ltd.              *
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
 * @file      os_mutex.h
 * @brief     OS mutex definition
 * @author    Phil Lin
 * @since     2011-04-01
 * @date      2011-04-01
*/

#ifndef __OS_MUTEX_H__
#define __OS_MUTEX_H__

/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/
 
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
 
/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef enum osMutexIdx_e {
	/* System mutex list start */
	
	OS_MUTEX_MEM_POOL = (unsigned char)0,
	
	/* System mutex list end */

	/* KEEP THIS ITEM AT THE BOTTOM */
	OS_MUTEX_MAX      = (unsigned char)32, 
} osMutexIdx_t;
/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/
 
/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/
 
/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/
void osMutexInit(void);
UINT8 osMutexQuery(osMutexIdx_t mutex);
UINT8 osMutexGet(osMutexIdx_t mutex);
void osMutexFree(osMutexIdx_t mutex);
#endif  /* __OS_MUTEX_H__ */

