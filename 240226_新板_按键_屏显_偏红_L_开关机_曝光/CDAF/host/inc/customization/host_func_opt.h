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
 * @file      host_func_opt.h
 * @brief     options for the function on host level
 * @author    Phil Lin
 * @since     2010-01-01
 * @date      2010-01-01
*/

#ifndef __HOST_FUNC_OPT_H__
#define __HOST_FUNC_OPT_H__

/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/* function support for calibration mode */
#define FUNC_HOST_CAL    1

/* function support for DPS */
#define FUNC_HOST_DPS    1

/* function support for copy to card */
#define FUNC_HOST_CP2CARD    1

/* function support for photoframe in preview mode */
#define FUNC_HOST_PF_PV	   1
/* function support for photoframe in playback mode */
#define FUNC_HOST_PF_PB    1

/* function support for display */
#define FUNC_HOST_DISP    1

/* function support for Motion detect */
#define FUNC_HOST_MD    0

/* function support for Panorama */
#define FUNC_HOST_PANORAMA    0

/* function support for HDMI */
#define FUNC_HOST_HDMI    1

/* function support for TV out */
#define FUNC_HOST_TV    1

/* function support for badpixel */
#define FUNC_HOST_BADPIXEL    1

/* function support for IGBT */
#define FUNC_HOST_IGBT_FLASH    0

/* function support for touchPanel */
#define FUNC_HOST_TOUCH_PANEL    0

/* function support for fixtrue */
#define FUNC_HOST_AF_FIXTURE	0

/* function support for fixtrue */
#define FUNC_HOST_AF_FOCUS_ONE_WIN	1




// ���������Ϣ
#define DBG_MODE					0

// ֧���޿�����
#define SUPPORT_NOCARD_OPERATION	1

// ֧���ļ�����
#define SUPPORT_LOCK_FILE			1

// ���ݵ�ǰ״̬����USB���ͼ��
#define SUPPORT_USB_ICON			1

// ֧����Ļ����
#define SUPPORT_PANEL_PROTECT		1

// ���ݵ�ǰ״̬����LED��˸��ʽ
#define SUPPORT_UPDATE_LED			1

// SPIд��������
#define SUPPORT_SPI_WRITE_CONTROL	1

// GPIO6 ���ƹ���ʹ��
#define SUPPORT_SPEAKER_CONTROL		1

// ֧��MCU�����Ź�
#define SUPPORT_MCU_WTD				1

#define SUPPORT_SENSOR_SWITCH			1

/* ��ʱ��������Ƿ����仯����������仯���AWB������ر�AWB */
#define SUPPORT_AWB_ADJ					0

// ֧����Ԥ��������ʾ��ǰʱ��
#define SUPPORT_RTC_OSD_UPDATE			1


/*
	֧��AF����
*/
#define SUPPORT_AF						1
#ifdef SUPPORT_AF
/* ֧������ʽ�̶����� Զ��/΢�� */
#define SUPPORT_FIXED_AF				1

/* ֧������ǰ�Զ��Խ� */
#define SUPPORT_SNAP_AF					1
#endif

/*¼��EV����*/
#define SUPPORT_VIDEO_EV					0


// �Զ����Թ���
#define SUPPORT_AUTO_TEST				1
#if SUPPORT_AUTO_TEST
#define SUPPORT_AUTO_TEST_FILENAME		"D:\\TEST.TXT"
#endif

// �汾��Ϣ
#define FW_VERSION						ID_STR_DC403S_5647_9851_221220

/**************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

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

#endif  /* __HOST_FUNC_OPT_H__ */

