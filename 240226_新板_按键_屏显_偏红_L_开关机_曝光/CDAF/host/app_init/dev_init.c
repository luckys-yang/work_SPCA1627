/***************************************************************************
 * File: dev_init.c
 * Author: Yang
 * Date: 2024-03-07 13:48:46
 * description: 
 -----------------------------------
�豸��ʼ�����
 -----------------------------------
****************************************************************************/
#include "general.h"

#include "hal_device.h"
#include "hal_spi.h"
#include "func_ptr.h"

#include "dev_init.h"
#include "device.h"
#include "main.h"
#include "initio.h"
#include "timer.h"
#include "gpio.h"
#include "gpio_api.h"
#include "key_scan.h"
#include "pb_general.h"

#include "sp1k_hal_api.h"
#include "sp1k_front_api.h"
#include "sp1k_gpio_api.h"
#include "sp1k_disp_api.h"
#include "sp1k_osd_api.h"
#include "sp1k_pb_api.h"
#include "sp1k_aud_api.h"
#include "sp1k_util_api.h"
#include "sp1k_zoom_api.h"

#include "app_def.h"
#include "app_init.h"
#include "app_msg.h"
#include "app_battery.h"
#include "app_dev_disp.h"
#include "app_dev_storage.h"
#include "app_menu.h"
#include "app_menu_micro.h"
#include "app_ui_para.h"
#include "app_audio_play.h"
#include "app_gpio.h"
#include "app_sts_pwroff.h"
#include "sp1k_dos_api.h"
#include "app_cal_api.h"

#include "host_func_opt.h"
#include "initio.h"
#include "app_ui_osd.h"

#if SUPPORT_SENSOR_SWITCH
UINT8 sensor_switch = 1;    // 1:��ͷ 0:����
#endif

UINT8 enterMacroadjust = 0; // ����Ӧ�ó���������־λ

UINT8 enterVersionshow = 0;

UINT8 enterPCCamflag = 0;

extern panelStatus_t G_PanelStatus;
extern UINT8 TV_Open;

/**
* @param    None
* @retval   None
* @brief    ������Ƶ��ʼ������
**/
UINT8 appAudioInit(void)
{
    uiPara_t *pPara = appUiParaGet();  // ��ȡUI����

    sp1kAudioVolumeSet(VOLUME_10);  // ������Ƶ����Ϊ10

    // ���ڸ�����Ƶ¼��
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_MIN_VOL, 15);  // �����Զ�������Ƶ���С����Ϊ15
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_MAX_VOL, 100);  // �����Զ�������Ƶ��������Ϊ100
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_DEFAULT_GAIN_IDX, 34);  // �����Զ�������Ƶ�Ĭ����������Ϊ34
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_GAIN_DOWN_SPEED, 4);  // �����Զ�������ƵĽ��������ٶ�Ϊ4
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_GAIN_UP_SPEED, 30);  // �����Զ�������Ƶ���������ٶ�Ϊ30
    sp1kAudParamSet(SP1K_AUD_PARAM_ALC_OPT, 1);  // �����Զ��������

    return SUCCESS;  // ���سɹ�״̬
}

/**
* @param    None
* @retval   return = SUCCESS / FAIL
* @brief    ��ʼ��ǰ���豸
**/
UINT8 appFrontInit(void)
{
    UINT8 sensorId, actId;

    sensorId = sp1kFrontSensorInit();  // ǰ�ô�������ʼ��
    sp1kFrontDevActSet(sensorId);  // ����ǰ���豸����
    sp1kFrontPowerOn(sensorId);  // ǰ���豸�ϵ�

    actId = sp1kFrontDevActGet();  // ��ȡǰ���豸����ID

    if (SUCCESS == sp1kFrontHighQualityDigitalZoomCapabilityGet(actId))
    {
        sp1kFrontHighQualityDigitalZoomModeSet(actId, 1);  // ���֧�ָ��������ֱ佹��������Ϊģʽ1
    }
    else
    {
        sp1kFrontHighQualityDigitalZoomModeSet(actId, 0);  // �����֧�ָ��������ֱ佹��������Ϊģʽ0
    }

    return SUCCESS;  // ���سɹ�״̬
}

/**
* @param    None
* @retval   return = SUCCESS / FAIL
* @brief    ��ʼ���佹�豸
**/
UINT8 appZoomDevInit(void)
{
    sp1kZoomDevInit();  // ��ʼ���佹�豸

    if(VIDEO_NEW_ZOOM == 0 && STILL_NEW_ZOOM == 0)
    {
        sp1kZoomDevActSet(SP1K_ZOOM_LOGI_STILL_MODE, SP1K_ZOOM_LOGI_BLOCKING_TYPE);  // ���þ�̬ģʽ�ı佹�߼�Ϊ��������

        sp1kZoomDevActSet(SP1K_ZOOM_LOGI_VIDEO_MODE, SP1K_ZOOM_LOGI_BLOCKING_TYPE);  // ������Ƶģʽ�ı佹�߼�Ϊ��������
    }
    else
    {
        sp1kZoomDevActSet(SP1K_ZOOM_LOGI_STILL_MODE, SP1K_ZOOM_LOGI_NONBLOCKING_TYPE);  // ���þ�̬ģʽ�ı佹�߼�Ϊ����������

        sp1kZoomDevActSet(SP1K_ZOOM_LOGI_VIDEO_MODE, SP1K_ZOOM_LOGI_NONBLOCKING_TYPE);  // ������Ƶģʽ�ı佹�߼�Ϊ����������
    }

    return SUCCESS;  // ���سɹ�״̬
}

/**
* @param    None
* @retval   return = SUCCESS / FAIL
* @brief    ��ʼ������GPIO���ŵ����ã���������GPIO�Ĺ��ܡ��������ֵ��
**/
UINT8 appDispInit(void)
{
    UINT8 pnlType, pnlSts;
    UINT16 decWidth, decHeight;
    uiPara_t *pPara = appUiParaGet();  // ��ȡUI�����ṹ��ָ��

    /*
    	Panel driver logic install process
    	The flow is NOT the physic driver, so the panel was still not been opened
    */
    sp1kDispPanelInit();  // ��ʼ����ʾ���
    sp1kDispTvNtscInit();  // ��ʼ��TV NTSC
    sp1kDispTvPalInit();  // ��ʼ��TV PAL
    sp1kDispHdmiInit();  // ��ʼ��HDMI

    /* parioty: TV > HDMI > LCD */
    if (IS_GPIO_TV_PLUGIN())  // ���TV����
    {
        TV_Open = 1;  // TV�򿪱�־��Ϊ1
        if (pPara->TVMode == TVMODE_NTSC)  // ���TVģʽΪNTSC
        {
            pnlType = DISP_DEV_NTSC;  // ��ʾ�豸������ΪNTSC
            pnlSts  = PANEL_STATUS_NTSC;  // ���״̬��ΪNTSC
        }
        else
        {
            pnlType = DISP_DEV_PAL;  // ��ʾ�豸������ΪPAL
            pnlSts  = PANEL_STATUS_PAL;  // ���״̬��ΪPAL
        }
    }
    /*The open image must be showed on the LCD while HDMI in.*/
    /* else if (IS_GPIO_HDMI_PLUGIN()) {
    	pnlType = DISP_DEV_HDMI;
    	pnlSts  = PANEL_STATUS_LCD;
    }*/
    else
    {
        pnlType = DISP_DEV_PANEL;  // ��ʾ�豸������Ϊ���
        pnlSts  = PANEL_STATUS_LCD;  // ���״̬��ΪLCD
    }

    /* panel physic open here */
    sp1kDispDevActSet(pnlType);  // ������ʾ�豸����
    sp1kDispPowerOn(pnlType);  // ��ʾ�豸�ϵ�
    G_PanelStatus = pnlSts;  // ȫ�����״̬��Ϊ��ǰ���״̬

    if (pPara->OpenPic)
    {
        //sp1kHalJpegDecodeWait();
        if (IS_DISP_MODE_TVOUT())
        {
            sp1kHalWait(500); //wait TV contect ready
        }
        sp1kPbDecodeSizeGet(&decWidth, &decHeight);  // ��ȡ����Ŀ�Ⱥ͸߶�
        sp1kDispImgShow(sp1kPbDispBuffGet(1), decWidth, decHeight, 100);  // ��ʾͼƬ
    }

    return SUCCESS;  // ���سɹ�״̬
}

/**
* @param    None
* @retval   None
* @brief    ��ʼ��GPIO����
**/
void appGpioInit(void)
{
    // ������1527���� @090520
#if TAG_TODO
#if(_HW_SOLUTION_ == _HW_TK_MODEL_B_ || _HW_SOLUTION_ == _HW_RDK_A_)
    // ʹ��������
    sp1kSpeaker_ENCfg();  // ����������ʹ������
#endif
#endif

    // ���оƬIDΪ0x1627��0x1626
    if((pkgChipIdGet() == 0x1627) || (pkgChipIdGet() == 0x1626))
    {
        gpioPullSelSet(6, 0xFF, 0xFF);   // ����GPIO 6����������
        gpioPullEnSet(7, 0xF0, 0xF0);    // ����GPIO 7����������
        gpioPullEnSet(8, 0x0F, 0x0F);    // ����GPIO 8���������ã�DGPIO0~7���ߵ�ƽ
    }

    sp1kSDWPDectCfg();  // ����SD��д�������

    sp1kTV_PlugCfg(TV_PLUG, 0);  // ����TV������
    sp1kHDMI_PlugCfg();  // ����HDMI������
    sp1kUSB_PlugCfg();  // ����USB������

    // ��ӿ���⼫������
    //sp1kSD_CD_PolaritySet(0);
    sp1kSD_CdCfg();  // ����SD��������

#if TAG_TODO
    // ���ѡ��LED����
    sp1kLED_SelfCfg();  // ��������LED
    sp1kLED_SelfSet(1);  // ��������LED״̬Ϊ����
    ///
    sp1kLED_BusyCfg();  // ����æ״̬LED
    sp1kLED_BusySet(1); // �ر�״̬LED
#endif

    appChargePinCfg();  // �����������

#if (_HW_SOLUTION_ == _HW_CUSTOM_MODEL_A_)
    /*�������ϵ�*/
    gpioByteFuncSet(GPIO_BYTE_GEN2, 0x80, 0x80);  // ����ͨ��IO���Ź���
    gpioByteOutSet(GPIO_BYTE_GEN2, 0x80, 0x00);   // ����ͨ��IO�������ֵ
    gpioByteDirSet(GPIO_BYTE_GEN2, 0x80, 0x80);   // ����ͨ��IO���ŷ���

    /*lcd�������gpio6*/
    gpioByteFuncSet(GPIO_BYTE_GEN0, 0x40, 0x40);  // ����ͨ��IO���Ź���
    gpioByteOutSet(GPIO_BYTE_GEN0, 0x40, 0x40);   // ����ͨ��IO�������ֵ
    gpioByteDirSet(GPIO_BYTE_GEN0, 0x40, 0x40);   // ����ͨ��IO���ŷ���
#endif

#if (_HW_SOLUTION_ == _HW_RDK_A_)
#if TAG_TODO
    //lcd��Դ
    sp1kGpioByteOutSet(GPIO_BYTE_P1, 0x80, 0x00);  // ����ͨ��IO�������ֵ
    sp1kGpioByteDirSet(GPIO_BYTE_P1, 0x80, 0xff);  // ����ͨ��IO���ŷ���

    //��������Դ
    sp1kGpioByteOutSet(GPIO_BYTE_GEN2, 0x01, 0x00); // ����ͨ��IO�������ֵ
    sp1kGpioByteDirSet(GPIO_BYTE_GEN2, 0x01, 0xff); // ����ͨ��IO���ŷ���
#endif
#endif

    // GPIO 0 ���Զ��Խ�
    appAFCtrl(0);  // �����Զ��Խ�

    // GPIO 1 �����
    appSnapFlashLed(0);  // ���������

    // GPIO 2
#if SUPPORT_SENSOR_SWITCH
    appSensorPWDNSwitch(sensor_switch);  // ���ƴ���������(��ͷ/����)
#endif

    app_set_led(1);  // ����LED״̬Ϊ����

    // GPIO 6 SPIд����
#if SUPPORT_SPI_WRITE_CONTROL
    appSetGpio6SpiWP(1);  // ����GPIO 6 SPIд����
#endif

    // GPIO 8 ����ʹ��
#if SUPPORT_SPEAKER_CONTROL
    app_set_speaker(1);  // ����������ʹ��
#endif

    // GPIO 10 ���Ź�
#if SUPPORT_MCU_WTD
    appSetWTDHeartbeat(1);  // ���ÿ��Ź�����
#endif
}

/**
* @param    None
* @retval   None
* @brief    ��ʼ��RTC��ʵʱʱ�ӣ�ģ��
**/
void appRtcInit(void)
{
    dateStc_t DateInfo;  // �������ڽṹ�����DateInfo�����ڴ洢������Ϣ

    DateInfo.Year = 22;  // �������Ϊ22��
    DateInfo.Month = 1;  // �����·�Ϊ1��
    DateInfo.Day = 1;    // ��������Ϊ1��
    DateInfo.Hour = 0;   // ����СʱΪ0ʱ
    DateInfo.Minute = 0; // ���÷���Ϊ0��
    DateInfo.Second = 0; // ��������Ϊ0��

    sp1kRTCDateInit(&DateInfo);  // ��DateInfo�ṹ�崫�룬��ʼ��RTC����ʱ��
}

/**
* @param    None
* @retval   None
* @brief    �����豸��ʼ������
**/
void hostDevInit(void)
{
    uiPara_t *pPara;  // �û��������ָ��
    UINT16 adcVal = 0;  // ADC ֵ
    UINT8 enterCardUpdateFlag = 0;  // ���뿨������־

    // ��ʼ�� GPIO
    appGpioInit();
    sp1kHalWait(100);  // ��ʱ

    // ���������
#if 0
    sp1kADCSarSel(0x02);
    sp1kHalWait(50);
    adcVal = sp1kADCValueSnapIntr();
    sp1kHalWait(50);
    if ((adcVal >= (391 - 30) && adcVal <= (391 + 30)) || (adcVal >= (336 - 30) && adcVal <= (336 + 30)))
    {
        enterCardUpdateFlag = 1;
    }
#endif

    // ΢�����ģʽ���
#if 0
    if ((adcVal >= (626 - 30) && adcVal <= (626 + 30)) || (adcVal >= (587 - 30) && adcVal <= (587 + 30)))
    {
        enterMacroadjust = 1;
    }
#endif

    // USB ģʽ���
    sp1kADCSarSel(0x00);
    sp1kHalWait(50);
    adcVal = sp1kADCValueSnapIntr();
    if ((adcVal >= (600 - 100) && adcVal <= (600 + 100)) && (HAL_GpioByteInGet(HAL_GPIO_BYTE_DISP0, 0x04, 0)) && (!sp1kUSB_PlugGet()))
    {
        enterVersionshow = 1;
    }
    else if ((adcVal >= (600 - 100) && adcVal <= (600 + 100)) && (HAL_GpioByteInGet(HAL_GPIO_BYTE_DISP0, 0x04, 0)) && (sp1kUSB_PlugGet()))
    {
        enterPCCamflag = 1;
    }

    // ��USBʱ��Ҫ��������
#if 1
    if (!sp1kUSB_PlugGet())
    {
        sp1kADCSarSel(0x00);
        sp1kHalWait(50);
        adcVal = sp1kADCValueSnapIntr();
        if (adcVal >= (600 - 100) && adcVal <= (600 + 100))
        {
            sp1kHalWait(50);
            adcVal = sp1kADCValueSnapIntr();
            if (!(adcVal >= (600 - 100) && adcVal <= (600 + 100)))
            {
                appPowerHoldPullDown();
            }
        }
        else
        {
            appPowerHoldPullDown();
        }
    }
    sp1kADCSarSel(0x00);
#endif

    // SPI д����֧��
#ifdef DISK_DRV_SPI
#if SUPPORT_SPI_WRITE_CONTROL
    appSpiWpSet(1);
#else
    // ע�� SPI ��д�����ص�����
    sp1kSfCallbackReg(SP1K_SPI_CALLBACK_WP_SET, pFunc_SfWPSet);
#endif
#endif

    /* �����û��������� */
    appUiParaInit(PARA_INIT_FROM_UDF);
    pPara = appUiParaGet();

    appUiItemOrderCfg();  // �û�������Ŀ˳������

    /* �����ģʽ */
    appChargeModeChk();

    if(appChargeModeGet())  // ������ڳ��ģʽ
    {
        /*
        	�ڳ��ģʽ�µı���ͼƬ�������
        	��ǰ����������Ϊ��ʾ�����δ�򿪡�
        */
        appImgDecode(0x38);
    }
    else     /* �ǳ��ģʽ */
    {
        /*
           ��ͼƬ�������
           ��ǰ����������Ϊ��ʾ�����δ�򿪡�
        */
        appOpenImgDec(pPara->OpenPic);

        /*
        	��һ�ε�ؼ��
        	�����⵽��ص����ͣ���ϵͳ���ػ���
        */
        appBattInit();

        /*
        	��Դ�����
        	�����ʱû�а��µ�Դ������ϵͳ���ػ���
        */
#if (_HW_SOLUTION_ != _HW_EVB_)
        if (!prjDbgMode)
        {
            if (sp1kBtnChk(SP1K_BTN_POWER) != KEY_STATUS_PRESSED)
            {
                appPowerShutDown();
            }
        }
#endif
    }

    /* ���� */
    appDispInit();

    // ����������
    if (!sp1kSD_CdGet())
    {
        appStorageMount(K_MEDIA_SDF);
#if SUPPORT_MCU_WTD
        appMCUWDTEnterCardISPMode();
#endif
    }

    /* ��Ƶ��ʼ�� */
    appAudioInit();

    if(!appChargeModeGet())
    {
        appVoicePlay(SP1K_MSG_UNKNOWN_HOST, VOICE_TYPE_OPEN, VOICE_PLAYLEVLE_HIGH);

#if SUPPORT_MCU_WTD
        appSetWTDHeartbeat(0);
#endif
#if SUPPORT_MCU_WTD
        appSetWTDHeartbeat(1);
#endif
        sp1kHalWait(907);  /*mantis 42008*/
    }
    sp1kAudioVolumeSet(pPara->Volume);  // ������Ƶ����

    /* ǰ������ͷ��ʼ�� */
    appFrontInit();

    /* �佹�豸��ʼ�� */
    appZoomDevInit();

    /* RTC ���ڳ�ʼ�� */
    appRtcInit();
}

/**
* @param    None
* @retval   None
* @brief    ������ʼ�����������ڳ�ʼ���豸����
**/
void appButtonInit(void) USING_0
{
    devAttr_t *pDevAttr;  // �豸����ָ��

    sp1kBtnDisableAll();  // �������а���

    pDevAttr = sp1kDevHdlGet();  // ��ȡ�豸���
    pDevAttr[DEV_ID_KEYBROAD].devSts = DEV_STS_RDY;  // ���ü����豸״̬Ϊ����
    pDevAttr[DEV_ID_KEYBROAD].devEn = 1;  // ���ü����豸
    pDevAttr[DEV_ID_KEYBROAD].devType = DEV_TYPE_KEYBROAD_COM;  // ���ü����豸����Ϊͨ�ü���
}

/**
* @param    mode ģʽ����
* @retval   return = SUCCESS / FAIL ����ִ�н�����ɹ���ʧ��
* @brief    ��ͼ����빦��
**/
UINT8 appOpenImgDec(UINT8 mode) USING_0
{
    UINT32 resID;  // ��ԴID
    uiPara_t *pPara;  // UI����ָ��

    pPara = appUiParaGet();  // ��ȡUI����

    if (mode == PB_OPENPIC_OFF)
    {
        resID = 0;  // ���ģʽΪ�ر�ͼ���ܣ�����ԴIDΪ0��
    }
    else if (mode == PB_OPENPIC_USER_DEF)
    {
        /* ָ���û��Զ�������ͼ�� */
        resID = 0x800;  // ���ģʽΪ�û��Զ���ͼ������ԴIDΪ0x800��
    }
    else 
    {
        /* ָ��Ĭ������ͼ�� */
        resID = 0x0d;  // ����ָ��Ĭ������ͼ����ԴIDΪ0x0d��
    }

    appImgDecode(resID);  // ����ͼ����뺯������ָ����ԴID��ͼ��

    return SUCCESS;  // ����ִ�гɹ���
}

/**
* @param    resId Ҫ�����ͼ����ԴID
* @retval   return = SUCCESS / FAIL ����ִ�н�����ɹ���ʧ��
* @brief    ͼ��������
**/
UINT8 appImgDecode(UINT32 resId) USING_0
{
    UINT32 freeBuffAddr = K_SDRAM_TotalSize - 0x100000;  // ���л�������ַ�����ڽ�����ͼ�����ݴ洢��
    UINT16 decWidth = PB_DECODE_WIDTH;  // ������
    UINT16 decHeight = PB_DECODE_HEIGHT;  // ����߶�

    sp1kPbDecodeSizeSet(decWidth, decHeight);  // ���ý���ߴ�
    sp1kPbDispBuffSwitch();  // �л���ʾ������

    if(resId)
    {
        // �����ԴID��Ч�������ͼ�񱳾����뺯������ͼ����ʾ��
        appBGImgDec(resId, NULL, freeBuffAddr, sp1kPbDispBuffGet(1), 0);
    }
    else 
    {
        // ��������ɫ��ʾ��������
        sp1kPbFillBlack(sp1kPbDispBuffGet(1), decWidth, decWidth);
    }

    return SUCCESS;  // ����ִ�гɹ���
}

/**
* @param    None
* @retval   None
* @brief    ��ʼ���豸
**/
void devInit(void)
{
    keyScanInit();    // ��ʼ������ɨ��

    appButtonInit();  // ��ʼ��Ӧ�ð�ť

    TIMER0_Start();   // ������ʱ��0

    appInit();        // Ӧ�ó�ʼ��
}
