/******************************************************************************\

          (c) Copyright Explore Semiconductor, Inc. Limited 2005
                           ALL RIGHTS RESERVED

--------------------------------------------------------------------------------

  File        :  EP952_If.c

  Description :  EP952 IIC Interface

\******************************************************************************/

#include <string.h>

#include "general.h"
#include "EP952_If.h"
#include "DDC_If_EP952.h"
#include "EP952SettingsData.h"
#include "i2c.h"
#include "EP952api.h"
#if Enable_HDCP
	#include "HDCP_EP952.h"
#endif
//--------------------------------------------------------------------------------------------------

//#define Little_Endian (for linux only)

//--------------------------------------------------------------------------------------------------

// Private Data
unsigned char IIC_EP952_Addr, IIC_HDCPKey_Addr;
unsigned short TempUSHORT;
unsigned char Temp_Data[15];
unsigned char W_Data[2];

// Global date for HDMI Transmiter
unsigned char is_HDCP_AVMute;
unsigned char is_AMute;
unsigned char is_VMute;
unsigned char is_HDMI;
unsigned char is_RSEN;
unsigned char Cache_EP952_DE_Control;

// Private Functions
SMBUS_STATUS IIC_Write(unsigned char IICAddr, unsigned char ByteAddr, unsigned char *Data, unsigned int Size);
SMBUS_STATUS IIC_Read(unsigned char IICAddr, unsigned char ByteAddr, unsigned char *Data, unsigned int Size);


//==================================================================================================
//
// Public Function Implementation
//

//--------------------------------------------------------------------------------------------------
// Hardware Interface

void EP952_IIC_Initial()
{
	IIC_EP952_Addr = 0x52;	  // EP952 slave address
	IIC_HDCPKey_Addr = 0xA8;  // HDCP Key (EEPROM) slave address

	// Initial Variables
	Temp_Data[0] = EP952_TX_PHY_Control_0__TERM_ON;
	EP952_Reg_Write(EP952_TX_PHY_Control_0, Temp_Data, 1);

	Temp_Data[0] = 0;
	EP952_Reg_Write(EP952_TX_PHY_Control_1, Temp_Data, 1);
}

void EP952_Info_Reset(void)
{
	int i;

	// Global date for HDMI Transmiter
	is_HDCP_AVMute = 0;
	is_AMute = 1;
	is_VMute = 1;
	is_HDMI = 0;
	is_RSEN = 0;
	Cache_EP952_DE_Control = 0x03;

	// Initial Settings
	EP952_Reg_Set_Bit(EP952_Packet_Control, EP952_Packet_Control__VTX0);
	EP952_Reg_Set_Bit(EP952_General_Control_1, EP952_General_Control_1__INT_OD);
	// Default Audio Mute
	//EP952_Reg_Set_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__AMUTE);
	//EP952_Reg_Set_Bit(EP952_Pixel_Repetition_Control, EP952_Pixel_Repetition_Control__CTS_M);
	// Default Video Mute
	//EP952_Reg_Set_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__VMUTE);

	//
	// Set Default AVI Info Frame
	//
	memset(Temp_Data, 0x00, 14);

	// Set AVI Info Frame to RGB
	Temp_Data[1] &= 0x60;
	Temp_Data[1] |= 0x00; // RGB

	// Set AVI Info Frame to 601
	Temp_Data[2] &= 0xC0;
	Temp_Data[2] |= 0x40;

	// Write AVI Info Frame
	Temp_Data[0] = 0;
	for(i=1; i<14; ++i) {
		Temp_Data[0] += Temp_Data[i];
	}
	Temp_Data[0] = ~(Temp_Data[0] - 1);
	EP952_Reg_Write(EP952_AVI_Packet, Temp_Data, 14);

	//
	// Set Default ADO Info Frame
	//
	memset(Temp_Data, 0x00, 6);

	// Write ADO Info Frame
	Temp_Data[0] = 0;
	for(i=1; i<6; ++i) {
		Temp_Data[0] += Temp_Data[i];
	}
	Temp_Data[0] = ~(Temp_Data[0] - 1);
	EP952_Reg_Write(EP952_ADO_Packet, Temp_Data, 6);

	//
	// Set Default CS Info Frame
	//
	memset(Temp_Data, 0x00, 5);

	EP952_Reg_Write(EP952_Channel_Status, Temp_Data, 5);
/*
	//
	// clear Packet_1 Info Frame
	//
	Temp_Data[0] = 0;
	for(i=EP952_Data_Packet_Header; i<= 0x5F; i++) {
		EP952_Reg_Write(i, Temp_Data, 1);
	}
*/
}

//--------------------------------------------------------------------------------------------------
//
// HDMI Transmiter (EP952-Tx Implementation)
//

void HDMI_Tx_Power_Down(void)
{
	// Software power down
	EP952_Reg_Clear_Bit(EP952_General_Control_1, EP952_General_Control_1__PU);
	DBG_printf(("< EP952 Tx Power Down >\r\n"));
}

void HDMI_Tx_Power_Up(void)
{
	// Software power up
	EP952_Reg_Set_Bit(EP952_General_Control_1, EP952_General_Control_1__PU);
	DBG_printf(("< EP952 Tx Power Up >\r\n"));
}

unsigned char HDMI_Tx_HTPLG(void)
{
	// EP952 RSEN Enable
	EP952_Reg_Clear_Bit(EP952_TX_PHY_Control_1, EP952_TX_PHY_Control_1__RESN_DIS);

	// read register
	EP952_Reg_Read(EP952_General_Control_2, Temp_Data, 1);

#if Enable_HDCP
	// check HDCP Ri Interrupt Flag
	if(Temp_Data[0] & EP952_General_Control_2__RIFE) {
		HDCP_Ext_Ri_Trigger();
	}
#endif

	// check RSEN status
	is_RSEN = (Temp_Data[0] & EP952_General_Control_2__RSEN)? 1:0;

	// return HTPLG status
	if(Temp_Data[0] & EP952_General_Control_2__HTPLG)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned char HDMI_Tx_RSEN(void)
{
	return is_RSEN;
}

void HDMI_Tx_HDMI(void)
{
	if(!is_HDMI) {
		is_HDMI = 1;
		EP952_Reg_Set_Bit(EP952_General_Control_4, EP952_General_Control_4__HDMI);
		DBG_printf(("EP952 Set to HDMI output mode\r\n"));
	}
}

void HDMI_Tx_DVI(void)
{
	if(is_HDMI) {
		is_HDMI = 0;
		EP952_Reg_Clear_Bit(EP952_General_Control_4, EP952_General_Control_4__HDMI);
		DBG_printf(("EP952 Set to DVI output mode\r\n"));
	}
}

//------------------------------------
// HDCP

void HDMI_Tx_Mute_Enable(void)
{
	is_HDCP_AVMute = 1;
	HDMI_Tx_AMute_Enable();
	HDMI_Tx_VMute_Enable();
}

void HDMI_Tx_Mute_Disable(void)
{
	is_HDCP_AVMute = 0;
	HDMI_Tx_VMute_Disable();
	HDMI_Tx_AMute_Disable();
}

void HDMI_Tx_HDCP_Enable(void)
{
	EP952_Reg_Set_Bit(EP952_General_Control_5, EP952_General_Control_5__ENC_EN);
}

void HDMI_Tx_HDCP_Disable(void)
{
	EP952_Reg_Clear_Bit(EP952_General_Control_5, EP952_General_Control_5__ENC_EN);
}

void HDMI_Tx_RPTR_Set(void)
{
	EP952_Reg_Set_Bit(EP952_General_Control_5, EP952_General_Control_5__RPTR);
}

void HDMI_Tx_RPTR_Clear(void)
{
	EP952_Reg_Clear_Bit(EP952_General_Control_5, EP952_General_Control_5__RPTR);
}

void HDMI_Tx_write_AN(unsigned char *pAN)
{
	EP952_Reg_Write(EP952_AN, pAN, 8);
}

unsigned char HDMI_Tx_AKSV_RDY(void)
{
	status = EP952_Reg_Read(EP952_General_Control_5, Temp_Data, 1);
	if(status != SMBUS_STATUS_Success) {
		DBG_printf(("ERROR: AKSV RDY - MCU IIC %d\r\n", (int)status));
		return 0;
	}
	return (Temp_Data[0] & EP952_General_Control_5__AKSV_RDY)? 1:0;
}

unsigned char HDMI_Tx_read_AKSV(unsigned char *pAKSV)
{
	int i = 0, j = 0;

	status = EP952_Reg_Read(EP952_AKSV, pAKSV, 5);
	if(status != SMBUS_STATUS_Success) {
		DBG_printf(("ERROR: AKSV read - MCU IIC %d\r\n", (int)status));
		return 0;
	}

	while (i < 5) {
		Temp_Data[0] = 1;
		while (Temp_Data[0]) {
			if (pAKSV[i] & Temp_Data[0]) j++;
			Temp_Data[0] <<= 1;
		}
		i++;
	}

	if(j != 20) {
		DBG_printf(("ERROR: AKSV read - Key Wrong\r\n"));
		return 0;
	}

	return 1;
}

void HDMI_Tx_write_BKSV(unsigned char *pBKSV)
{
	EP952_Reg_Write(EP952_BKSV, pBKSV, 5);
}

unsigned char HDMI_Tx_RI_RDY(void)
{
	EP952_Reg_Read(EP952_General_Control_5, Temp_Data, 1);
	return (Temp_Data[0] & EP952_General_Control_5__RI_RDY)? 1:0;
}

unsigned char HDMI_Tx_read_RI(unsigned char *pRI)
{
	status = EP952_Reg_Read(EP952_RI, pRI, 2);
	if(status != SMBUS_STATUS_Success) {
		DBG_printf(("ERROR: Tx Ri read - MCU IIC %d\r\n", (int)status));
		return 0;
	}
	return 1;
}

void HDMI_Tx_read_M0(unsigned char *pM0)
{
	status = EP952_Reg_Read(EP952_M0, pM0, 8);
}

SMBUS_STATUS HDMI_Tx_Get_Key(unsigned char *Key)
{
	return IIC_Read(IIC_HDCPKey_Addr, 0, Key, 512);
}

//------------------------------------
// Special for config

void HDMI_Tx_AMute_Enable(void)
{
	is_AMute = 1;
	EP952_Reg_Set_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__AMUTE);
	EP952_Reg_Set_Bit(EP952_Pixel_Repetition_Control, EP952_Pixel_Repetition_Control__CTS_M);

	EP952_Reg_Clear_Bit(EP952_IIS_Control, EP952_IIS_Control__ADO_EN);
	EP952_Reg_Clear_Bit(EP952_IIS_Control, EP952_IIS_Control__AUDIO_EN);

	DBG_printf(("< EP952 Audio_Mute_enable >\r\n"));
}

void HDMI_Tx_AMute_Disable(void)
{
	is_AMute = 0;
	EP952_Reg_Clear_Bit(EP952_Pixel_Repetition_Control, EP952_Pixel_Repetition_Control__CTS_M);
	EP952_Reg_Clear_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__AVMUTE);
	EP952_Reg_Clear_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__AMUTE);

	EP952_Reg_Set_Bit(EP952_IIS_Control, EP952_IIS_Control__ADO_EN);
	EP952_Reg_Set_Bit(EP952_IIS_Control, EP952_IIS_Control__AUDIO_EN);
	EP952_Reg_Set_Bit(EP952_IIS_Control, EP952_IIS_Control__AVI_EN);

	DBG_printf(("< EP952 Audio_Mute_disable >\r\n"));
}

void HDMI_Tx_VMute_Enable(void)
{

		is_VMute = 1;
		EP952_Reg_Set_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__VMUTE);

		DBG_printf(("< EP952 Video_Mute_enable >\r\n"));

}

void HDMI_Tx_VMute_Disable(void)
{
	EP952_Reg_Clear_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__AVMUTE);
	is_VMute = 0;

	EP952_Reg_Clear_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__VMUTE);

	DBG_printf(("< EP952 Video_Mute_disable >\r\n"));

}

void HDMI_Tx_Video_Config(PVDO_PARAMS Params)
{
	int i;
	DBG_printf(("\r\n ========== Update EP952 video Registers ==========\r\n"));

	// Disable auto transmission AVI packet
	EP952_Reg_Clear_Bit(EP952_IIS_Control, EP952_IIS_Control__AVI_EN);

	////////////////////////////////////////////////////////
	// Video Interface

	// De_Skew
	EP952_Reg_Read(EP952_General_Control_3, Temp_Data, 1);
	Temp_Data[0] &= ~0xF0;
	Temp_Data[0] |= Params->Interface & 0xF0;
	EP952_Reg_Write(EP952_General_Control_3, Temp_Data, 1);

	// input DSEL BSEL EDGE
	EP952_Reg_Read(EP952_General_Control_1, Temp_Data, 1);
	Temp_Data[0] &= ~0x0E;
	Temp_Data[0] |= Params->Interface & 0x0E;
	EP952_Reg_Write(EP952_General_Control_1, Temp_Data, 1);

	if(Params->Interface & 0x01) {
		EP952_Reg_Set_Bit(EP952_General_Control_4, EP952_General_Control_4__FMT12);
	}
	else {
		EP952_Reg_Clear_Bit(EP952_General_Control_4, EP952_General_Control_4__FMT12);
	}

	// update HVPol
	EP952_Reg_Read(EP952_General_Control_4, Temp_Data, 1);
	Params->HVPol = Temp_Data[0] & (EP952_DE_Control__VSO_POL | EP952_DE_Control__HSO_POL);

	////////////////////////////////////////////////////////
	// Sync Mode
	switch(Params->SyncMode) {
		default:
	 	case SYNCMODE_HVDE:
			// Disable E_SYNC
			EP952_Reg_Clear_Bit(EP952_General_Control_4, EP952_General_Control_4__E_SYNC);
			// Disable DE_GEN
			Cache_EP952_DE_Control &= ~EP952_DE_Control__DE_GEN;

			// Regular VSO_POL, HSO_POL
			if((Params->HVPol & VNegHPos) != (EP952_VDO_Settings[Params->VideoSettingIndex].HVRes_Type.HVPol & VNegHPos)) { // V
				Cache_EP952_DE_Control |= EP952_DE_Control__VSO_POL; // Invert
			}
			else {
				Cache_EP952_DE_Control &= ~EP952_DE_Control__VSO_POL;
			}
			if((Params->HVPol & VPosHNeg) != (EP952_VDO_Settings[Params->VideoSettingIndex].HVRes_Type.HVPol & VPosHNeg)) { // H
				Cache_EP952_DE_Control |= EP952_DE_Control__HSO_POL; // Invert
			}
			else {
				Cache_EP952_DE_Control &= ~EP952_DE_Control__HSO_POL;
			}
			DBG_printf(("EP952 Set Sync mode to (H+V+DE)input mode:HVPol(%bu)(%02X),Cache_DE_C(%bu)\r\n",Params->HVPol,(int)Temp_Data[0],Cache_EP952_DE_Control));
			break;

		case SYNCMODE_HV:
			// Disable E_SYNC
			EP952_Reg_Clear_Bit(EP952_General_Control_4, EP952_General_Control_4__E_SYNC);
			// Enable DE_GEN
			Cache_EP952_DE_Control |= EP952_DE_Control__DE_GEN;

			// Regular VSO_POL, HSO_POL
			if((Params->HVPol & VNegHPos) != (EP952_VDO_Settings[Params->VideoSettingIndex].HVRes_Type.HVPol & VNegHPos)) { // V
				Cache_EP952_DE_Control |= EP952_DE_Control__VSO_POL; // Invert
			}
			else {
				Cache_EP952_DE_Control &= ~EP952_DE_Control__VSO_POL;
			}
			if((Params->HVPol & VPosHNeg) != (EP952_VDO_Settings[Params->VideoSettingIndex].HVRes_Type.HVPol & VPosHNeg)) { // H
				Cache_EP952_DE_Control |= EP952_DE_Control__HSO_POL; // Invert
			}
			else {
				Cache_EP952_DE_Control &= ~EP952_DE_Control__HSO_POL;
			}

			// Set DE generation params
			if(Params->VideoSettingIndex < EP952_VDO_Settings_Max) {
				Cache_EP952_DE_Control &= ~0x03;

			#ifdef Little_Endian
				Cache_EP952_DE_Control |= ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_DLY)[1];
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_DLY)[0];
				EP952_Reg_Write(EP952_DE_DLY, Temp_Data, 1);
			#else	// Big Endian
				Cache_EP952_DE_Control |= ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_DLY)[0];
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_DLY)[1];
				EP952_Reg_Write(EP952_DE_DLY, Temp_Data, 1);
			#endif

				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_TOP)[0];
				EP952_Reg_Write(EP952_DE_TOP, Temp_Data, 1);

			#ifdef Little_Endian
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_CNT)[0];
				Temp_Data[1] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_CNT)[1];
				EP952_Reg_Write(EP952_DE_CNT, Temp_Data, 2);
			#else	// Big Endian
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_CNT)[1];
				Temp_Data[1] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_CNT)[0];
				EP952_Reg_Write(EP952_DE_CNT, Temp_Data, 2);
			#endif


			#ifdef Little_Endian
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_LIN)[1];
				Temp_Data[1] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_LIN)[0];
				EP952_Reg_Write(EP952_DE_LIN, Temp_Data, 2);
			#else	// Big Endian
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_LIN)[1];
				Temp_Data[1] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_LIN)[0];
				EP952_Reg_Write(EP952_DE_LIN, Temp_Data, 2);
			#endif

				DBG_printf(("EP952 DE_GEN params (DE_DLY=%u", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_DLY));
				DBG_printf((", DE_CNT=%u", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_CNT));
				DBG_printf((", DE_TOP=%u", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_TOP));
				DBG_printf((", DE_LIN=%u)", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].DE_Gen.DE_LIN));
				DBG_printf((")\r\n"));
			}
			else {
				DBG_printf(("ERROR: VideoCode overflow DE_GEN table\r\n"));
			}

			DBG_printf(("EP952 Set Sync mode to (H+V)input + DE_GEN mode\r\n"));
			break;

		case SYNCMODE_Embeded:
			// Disable DE_GEN
			Cache_EP952_DE_Control &= ~EP952_DE_Control__DE_GEN;
			// Enable E_SYNC
			EP952_Reg_Set_Bit(EP952_General_Control_4, EP952_General_Control_4__E_SYNC);

			// Set E_SYNC params
			if(Params->VideoSettingIndex < EP952_VDO_Settings_Max) {

				Temp_Data[0] = EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.CTL;
				EP952_Reg_Write(EP952_Embedded_Sync_Control, Temp_Data, 1);

				TempUSHORT = EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.H_DLY;
				if(!(Params->Interface & 0x04)) { // Mux Mode
					TempUSHORT += 2;
				}

			#ifdef Little_Endian
				Temp_Data[0] = ((unsigned char *)&TempUSHORT)[0];
				Temp_Data[1] = ((unsigned char *)&TempUSHORT)[1];
				EP952_Reg_Write(EP952_H_Delay, Temp_Data, 2);
			#else	// Big Endian
				Temp_Data[0] = ((unsigned char *)&TempUSHORT)[1];
				Temp_Data[1] = ((unsigned char *)&TempUSHORT)[0];
				EP952_Reg_Write(EP952_H_Delay, Temp_Data, 2);
			#endif


			#ifdef Little_Endian
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.H_WIDTH)[0];
				Temp_Data[1] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.H_WIDTH)[1];
				EP952_Reg_Write(EP952_H_Width, Temp_Data, 2);
			#else	// Big Endian
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.H_WIDTH)[1];
				Temp_Data[1] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.H_WIDTH)[0];
				EP952_Reg_Write(EP952_H_Width, Temp_Data, 2);
			#endif

				Temp_Data[0] = EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.V_DLY;
				EP952_Reg_Write(EP952_V_Delay, Temp_Data, 1);

				Temp_Data[0] = EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.V_WIDTH;
				EP952_Reg_Write(EP952_V_Width, Temp_Data, 1);

			#ifdef Little_Endian
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.V_OFST)[0];
				Temp_Data[1] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.V_OFST)[1];
				EP952_Reg_Write(EP952_V_Off_Set, Temp_Data, 2);
			#else	// Big Endian
				Temp_Data[0] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.V_OFST)[1];
				Temp_Data[1] = ((unsigned char *)&EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.V_OFST)[0];
				EP952_Reg_Write(EP952_V_Off_Set, Temp_Data, 2);
			#endif

				DBG_printf(("EP952 E_SYNC params (CTL=0x%02X", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.CTL));
				DBG_printf((", H_DLY=%u", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.H_DLY));
				DBG_printf((", H_WIDTH=%u", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.H_WIDTH));
				DBG_printf((", V_DLY=%u", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.V_DLY));
				DBG_printf((", V_WIDTH=%u", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.V_WIDTH));
				DBG_printf((", V_OFST=%u", (unsigned short)EP952_VDO_Settings[Params->VideoSettingIndex].E_Sync.V_OFST));
				DBG_printf((")\r\n"));

				// Regular VSO_POL, HSO_POL
				if(EP952_VDO_Settings[Params->VideoSettingIndex].HVRes_Type.HVPol & VNegHPos) { // VNeg?
					Cache_EP952_DE_Control |= EP952_DE_Control__VSO_POL;
				}
				else {
					Cache_EP952_DE_Control &= ~EP952_DE_Control__VSO_POL;
				}
				if(EP952_VDO_Settings[Params->VideoSettingIndex].HVRes_Type.HVPol & VPosHNeg) { // HNeg?
					Cache_EP952_DE_Control |= EP952_DE_Control__HSO_POL;
				}
				else {
					Cache_EP952_DE_Control &= ~EP952_DE_Control__HSO_POL;
				}
			}
			else {
				DBG_printf(("ERROR: VideoCode overflow E_SYNC table\r\n"));
			}

			DBG_printf(("EP952 Set Sync mode to (Embeded)input mode\r\n"));
			break;
	}
	EP952_Reg_Write(EP952_DE_Control, &Cache_EP952_DE_Control, 1);

	////////////////////////////////////////////////////////
	// Pixel Repetition
	EP952_Reg_Read(EP952_Pixel_Repetition_Control, Temp_Data, 1);
	Temp_Data[0] &= ~EP952_Pixel_Repetition_Control__PR;
	if(Params->VideoSettingIndex < EP952_VDO_Settings_Max) {
		Temp_Data[0] |= EP952_VDO_Settings[Params->VideoSettingIndex].AR_PR & 0x03;
	}
	EP952_Reg_Write(EP952_Pixel_Repetition_Control, Temp_Data, 1);

	////////////////////////////////////////////////////////
	// Color Space
	switch(Params->FormatIn) {
		default:
	 	case COLORFORMAT_RGB:
			EP952_Reg_Clear_Bit(EP952_General_Control_4, EP952_General_Control_4__YCC_IN | EP952_General_Control_4__422_IN);
			DBG_printf(("EP952 Set to RGB In\r\n"));
			break;
	 	case COLORFORMAT_YCC444:
			EP952_Reg_Set_Bit(EP952_General_Control_4, EP952_General_Control_4__YCC_IN);
			EP952_Reg_Clear_Bit(EP952_General_Control_4, EP952_General_Control_4__422_IN);
			DBG_printf(("EP952 Set to YCC444 In\r\n"));
			break;
	 	case COLORFORMAT_YCC422:
			EP952_Reg_Set_Bit(EP952_General_Control_4, EP952_General_Control_4__YCC_IN | EP952_General_Control_4__422_IN);
			DBG_printf(("EP952 Set to YCC422 In\r\n"));
			break;
	}
	switch(Params->FormatOut) {
		default:
	 	case COLORFORMAT_RGB:
			// Set to RGB
			if(Params->VideoSettingIndex < EP952_VDO_Settings_IT_Start) { // CE Timing
				EP952_Reg_Clear_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__YCC_OUT | EP952_Color_Space_Control__422_OUT);
				EP952_Reg_Set_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__YCC_Range); // Output limit range RGB
			}
			else { // IT Timing
				EP952_Reg_Clear_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__YCC_OUT | EP952_Color_Space_Control__422_OUT | EP952_Color_Space_Control__YCC_Range);
			}
			DBG_printf(("EP952 Set to RGB Out\r\n"));
			break;

	 	case COLORFORMAT_YCC444:
			// Set to YCC444
			EP952_Reg_Set_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__YCC_OUT);
			EP952_Reg_Clear_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__422_OUT);
			DBG_printf(("EP952 Set to YCC444 Out\r\n"));
			break;

	 	case COLORFORMAT_YCC422:
			// Set to YCC422
			EP952_Reg_Set_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__YCC_OUT | EP952_Color_Space_Control__422_OUT);
			DBG_printf(("EP952 Set to YCC422 Out\r\n"));
			break;
	}

	// Color Space
	switch(Params->ColorSpace) {
		default:
	 	case COLORSPACE_601:
			// Set to 601
			EP952_Reg_Clear_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__COLOR);
			DBG_printf(("EP952 Set to 601 color definition "));
			break;

	 	case COLORSPACE_709:
			// Set to 709
			EP952_Reg_Set_Bit(EP952_Color_Space_Control, EP952_Color_Space_Control__COLOR);
			DBG_printf(("EP952 Set to 709 color definition "));
			break;
	}
	DBG_printf(("(VIC=%d) \r\n",(int)Params->VideoSettingIndex));

	///////////////////////////////////////////////////////////////////
	// AVI Info Frame
	//

	// clear AVI Info Frame
	memset(Temp_Data, 0x00, 14);

	// AVI InfoFrame Data Byte 1
	switch(Params->FormatOut) {
		default:
	 	case COLORFORMAT_RGB:
			Temp_Data[1] |= 0x00; // AVI_Y1,Y0 = RGB
			break;

	 	case COLORFORMAT_YCC444:
			Temp_Data[1] |= 0x40; // AVI_Y1,Y0 = YCC 444
			break;

	 	case COLORFORMAT_YCC422:
			Temp_Data[1] |= 0x20; // AVI_Y1,Y0 = YCC 422
			break;
	}
	Temp_Data[1] |= 0x10; // AVI_A0 = Active Format Information valid

	//SCAN
	switch(Params->SCAN) {
		default:
		case 0:
			Temp_Data[1] &= ~0x03;	// AVI_S1,S0 = No Data
			break;
		case 1: 					// AVI_S1,S0 = overscan
			Temp_Data[1] |= 0x01;
			break;
		case 2: 					// AVI_S1,S0 = underscan
			Temp_Data[1] |= 0x02;
			break;
	}

	// AVI InfoFrame Data Byte 2
	switch(Params->ColorSpace) {
		default:
	 	case COLORSPACE_601:
			Temp_Data[2] |= 0x40;	// AVI_C1,C0 = 601
			break;

	 	case COLORSPACE_709:
			Temp_Data[2] |= 0x80;	// AVI_C1,C0 = 709
			break;
	}

	if(Params->VideoSettingIndex < EP952_VDO_Settings_Max) {
		Temp_Data[2] |= EP952_VDO_Settings[Params->VideoSettingIndex].AR_PR & 0x30; // AVI_M1,M0 : Picture Aspect Ratio
	}
	Temp_Data[2] |= Params->AFARate & 0x0F;// AVI_R3~0: Active format Aspect Ratio

	// AVI InfoFrame Data Byte 3 is 0

	// AVI InfoFrame Data Byte 4 is VIC
	if(Params->VideoSettingIndex < EP952_VDO_Settings_IT_Start) {
		Temp_Data[4] |= EP952_VDO_Settings[Params->VideoSettingIndex].VideoCode;// AVI_VIC6~0 : Vedio Identification code
	}

	// AVI InfoFrame Data Byte 5
	if(Params->VideoSettingIndex < EP952_VDO_Settings_Max) {
		Temp_Data[5] |= (EP952_VDO_Settings[Params->VideoSettingIndex].AR_PR & 0x0C) >> 2;// AVI_PR3~0 : Pixel Repetition
	}

	// AVI InfoFrame Data Byte 0 is checksum
	Temp_Data[0] = 0x91;
	for(i=1; i<6; ++i) {
		Temp_Data[0] += Temp_Data[i];
	}
	Temp_Data[0] = ~(Temp_Data[0] - 1);	// checksum

	// Write AVI InfoFrame Data Byte
	EP952_Reg_Write(EP952_AVI_Packet, Temp_Data, 14);

	// print for debug
	DBG_printf(("EP952 set AVI Info: "));
	for(i=0; i<6; ++i) {
		DBG_printf(("[%d]0x%0.2X, ",(int)i, (int)Temp_Data[i] ));
	}
	DBG_printf(("\r\n"));

	// Enable auto transmission AVI packet
	EP952_Reg_Set_Bit(EP952_IIS_Control, EP952_IIS_Control__AVI_EN | EP952_IIS_Control__GC_EN);


}

void HDMI_Tx_Audio_Config(PADO_PARAMS Params)
{
	int i;
	unsigned char N_CTS_Index;
	unsigned long N_Value, CTS_Value;
	ADSFREQ FinalFrequency;
	unsigned char FinalADSRate;

	DBG_printf(("\r\n ========== Update EP952 Audio Registers ==========\r\n"));

	////////////////////////////////////////////////////////
	// Audio Settings

	// Update WS_M, WS_POL, SCK_POL
	EP952_Reg_Read(EP952_IIS_Control, Temp_Data, 1);
	Temp_Data[0] &= ~0x07;						//clear WS_M, WS_POL, SCK_POL
	Temp_Data[0] |= Params->Interface & 0x07; 	//set WS_M, WS_POL, SCK_POL
	EP952_Reg_Write(EP952_IIS_Control, Temp_Data, 1);

	////////////////////////////////////////////////////////
	// IIS or SPDIF
	if(Params->Interface & 0x08) { // IIS
		DBG_printf(("EP952 set to IIS IN \r\n"));
		Temp_Data[0] = 0;

		// Update Flat = 0
		EP952_Reg_Clear_Bit(EP952_Packet_Control, EP952_Packet_Control__FLAT);

		// Power down OSC
		EP952_Reg_Set_Bit(EP952_General_Control_1, EP952_General_Control_1__OSC_PD);

		// Set to IIS Input
		Temp_Data[0] = EP952_General_Control_8__ADO_IIS_IN | EP952_General_Control_8__COMR_DIS;
		EP952_Reg_Write(EP952_General_Control_8, Temp_Data, 1);

		// Downsample Convert
		FinalADSRate = Params->ADSRate;
		switch(Params->ADSRate) {
			default:
			case 0: // Bypass
				//DBG_printf(("Audio ADS = 0\r\n"));
				FinalADSRate = 0;
				FinalFrequency = Params->InputFrequency;
				break;
			case 1: // 1/2
				//DBG_printf(("Audio ADS = 1_2\r\n"));
				switch(Params->InputFrequency) {
					default: // Bypass
						//DBG_printf(("Audio ADS = 0\r\n"));
						FinalADSRate = 0;
						FinalFrequency = Params->InputFrequency;
						break;
					case ADSFREQ_88200Hz:
						FinalFrequency = ADSFREQ_44100Hz;
						break;
					case ADSFREQ_96000Hz:
						FinalFrequency = ADSFREQ_48000Hz;
						break;
					case ADSFREQ_176400Hz:
						FinalFrequency = ADSFREQ_88200Hz;
						break;
					case ADSFREQ_192000Hz:
						FinalFrequency = ADSFREQ_96000Hz;
						break;
				}
				break;
			case 2: // 1/3
				//DBG_printf(("Audio ADS = 1_3\r\n"));
				switch(Params->InputFrequency) {
					default: // Bypass
						//DBG_printf(("Audio ADS = 0\r\n"));
						FinalADSRate = 0;
						FinalFrequency = Params->InputFrequency;
						break;
					case ADSFREQ_96000Hz:
						FinalFrequency = ADSFREQ_32000Hz;
						break;
				}
				break;
			case 3: // 1/4
				//DBG_printf(("Audio ADS = 1_4\r\n"));
				switch(Params->InputFrequency) {
					default: // Bypass
						//DBG_printf(("Audio ADS = 0\r\n"));
						FinalADSRate = 0;
						FinalFrequency = Params->InputFrequency;
						break;
					case ADSFREQ_176400Hz:
						FinalFrequency = ADSFREQ_44100Hz;
						break;
					case ADSFREQ_192000Hz:
						FinalFrequency = ADSFREQ_48000Hz;
						break;
				}
				break;
		}

		// Update Audio Down Sample (ADSR)
		EP952_Reg_Read(EP952_Pixel_Repetition_Control, Temp_Data, 1);
		Temp_Data[0] &= ~0x30;
		Temp_Data[0] |= (FinalADSRate << 4) & 0x30;
		EP952_Reg_Write(EP952_Pixel_Repetition_Control, Temp_Data, 1);

		///////////////////////////////////////////////////////////////
		// Channel Status

		memset(Temp_Data, 0x00, 5);

		Temp_Data[0] = (Params->NoCopyRight)? 0x04:0x00;
		Temp_Data[1] = 0x00; 			// Category code ??
		Temp_Data[2] = 0x00; 			// Channel number ?? | Source number ??
		Temp_Data[3] = FinalFrequency; 	// Clock accuracy ?? | Sampling frequency
		Temp_Data[4] = 0x01; 			// Original sampling frequency ?? | Word length ??
		EP952_Reg_Write(EP952_Channel_Status, Temp_Data, 5);

		// print for debug
		DBG_printf(("EP952 set CS Info: "));
		for(i=0; i<5; ++i) {
			DBG_printf(("0x%02X, ", (int)Temp_Data[i] ));
		}
		DBG_printf(("\r\n"));

		// set CS_M = 1 (use channel status regiater)
		EP952_Reg_Set_Bit(EP952_Pixel_Repetition_Control, EP952_Pixel_Repetition_Control__CS_M);
	}
	else { // SPIDIF
		DBG_printf(("EP952 set to SPDIF IN \r\n"));

		// power up OSC
		EP952_Reg_Clear_Bit(EP952_General_Control_1, EP952_General_Control_1__OSC_PD);

		// Set SPDIF in
		Temp_Data[0] = EP952_General_Control_8__ADO_SPDIF_IN | EP952_General_Control_8__COMR_DIS;
		EP952_Reg_Write(EP952_General_Control_8, Temp_Data, 1);

		// Update Flat = 0
		EP952_Reg_Clear_Bit(EP952_Packet_Control, EP952_Packet_Control__FLAT);

		// No Downsample
		FinalADSRate = 0;
		FinalFrequency = Params->InputFrequency;

		// Disable Down Sample and Bypass Channel Status
		EP952_Reg_Clear_Bit(EP952_Pixel_Repetition_Control, EP952_Pixel_Repetition_Control__ADSR | EP952_Pixel_Repetition_Control__CS_M);

		Params->ChannelNumber = 0;
	}

	////////////////////////////////////////////////////////
	// Set CTS/N
	if(Params->VideoSettingIndex < EP952_VDO_Settings_Max) {
		N_CTS_Index = EP952_VDO_Settings[Params->VideoSettingIndex].Pix_Freq_Type;
		if(EP952_VDO_Settings[Params->VideoSettingIndex].HVRes_Type.Vprd % 500) { // 59.94/60 Hz
			N_CTS_Index += Params->VFS;
			//DBG_printf(("EP952 Use N_CTS_Index shift(VFS) = %d\r\n", (int)Params->VFS));
		}
	}
	else {
		DBG_printf(("EP952 Use default N_CTS_Index\r\n"));
		N_CTS_Index = PIX_FREQ_25200KHz;
	}

	switch(FinalFrequency) {

		default:
		case ADSFREQ_32000Hz:
			DBG_printf(("EP952 Set to 32KHz"));
			N_Value = N_CTS_32K[N_CTS_Index].N;
			CTS_Value = N_CTS_32K[N_CTS_Index].CTS;
			break;
		case ADSFREQ_44100Hz:
			DBG_printf(("EP952 Set to 44.1KHz"));
			N_Value = N_CTS_44K1[N_CTS_Index].N;
			CTS_Value = N_CTS_44K1[N_CTS_Index].CTS;
			break;
		case ADSFREQ_48000Hz:
			DBG_printf(("EP952 Set to 48KHz"));
			N_Value = N_CTS_48K[N_CTS_Index].N;
			CTS_Value = N_CTS_48K[N_CTS_Index].CTS;
			break;
		case ADSFREQ_88200Hz:
			DBG_printf(("EP952 Set to 88.2KHz"));
			N_Value = N_CTS_44K1[N_CTS_Index].N * 2;
			CTS_Value = N_CTS_44K1[N_CTS_Index].CTS * 2;
			break;
		case ADSFREQ_96000Hz:
			DBG_printf(("EP952 Set to 96KHz"));
			N_Value = N_CTS_48K[N_CTS_Index].N * 2;
			CTS_Value = N_CTS_48K[N_CTS_Index].CTS * 2;
			break;
		case ADSFREQ_176400Hz:
			DBG_printf(("EP952 Set to 176.4KHz"));
			N_Value = N_CTS_44K1[N_CTS_Index].N * 4;
			CTS_Value = N_CTS_44K1[N_CTS_Index].CTS * 4;
			break;
		case ADSFREQ_192000Hz:
			DBG_printf(("EP952 Set to 192KHz"));
			N_Value = N_CTS_48K[N_CTS_Index].N * 4;
			CTS_Value = N_CTS_48K[N_CTS_Index].CTS * 4;
			break;
	}

	// write to EP952 - CTS.N value
	Temp_Data[0] = CTS_Value>>16;
	EP952_Reg_Write(EP952_CTS_H, Temp_Data, 1);
	Temp_Data[0] = CTS_Value>>8;
	EP952_Reg_Write(EP952_CTS_M, Temp_Data, 1);
	Temp_Data[0] = CTS_Value;
	EP952_Reg_Write(EP952_CTS_L, Temp_Data, 1);

	Temp_Data[0] = N_Value>>16;
	EP952_Reg_Write(EP952_N_H, Temp_Data, 1);
	Temp_Data[0] = N_Value>>8;
	EP952_Reg_Write(EP952_N_M, Temp_Data, 1);
	Temp_Data[0] = N_Value;
	EP952_Reg_Write(EP952_N_L, Temp_Data, 1);

	DBG_printf((" table[%d]: N=%ld, CTS=%ld (VIC=%d)\r\n", (int)N_CTS_Index, N_Value, CTS_Value, (int)Params->VideoSettingIndex));

	/*
	// for debug
	EP952_Reg_Read(EP952_CTS_H, Temp_Data, 1);
	DBG_printf(("EP952_CTS_0(Reg addr 0x60) = 0x%02X\r\n",(int)Temp_Data[0]));
	EP952_Reg_Read(EP952_CTS_M, Temp_Data, 1);
	DBG_printf(("EP952_CTS_1(Reg addr 0x61) = 0x%02X\r\n",(int)Temp_Data[0]));
	EP952_Reg_Read(EP952_CTS_L, Temp_Data, 1);
	DBG_printf(("EP952_CTS_2(Reg addr 0x62) = 0x%02X\r\n",(int)Temp_Data[0]));

	EP952_Reg_Read(EP952_N_H, Temp_Data, 1);
	DBG_printf(("EP952_N_0(Reg addr 0x63) = 0x%02X\r\n",(int)Temp_Data[0]));
	EP952_Reg_Read(EP952_N_M, Temp_Data, 1);
	DBG_printf(("EP952_N_1(Reg addr 0x64) = 0x%02X\r\n",(int)Temp_Data[0]));
	EP952_Reg_Read(EP952_N_L, Temp_Data, 1);
	DBG_printf(("EP952_N_2(Reg addr 0x65) = 0x%02X\r\n",(int)Temp_Data[0]));
	*/

	//////////////////////////////////////////////////////
	// ADO InfoFrame
	//

	// clear Default ADO InfoFrame
	memset(Temp_Data, 0x00, 6);

	// Overwrite ADO InfoFrame
	Temp_Data[1] = Params->ChannelNumber;
	Temp_Data[4] = EP952_ADO_Settings[Params->ChannelNumber].SpeakerMapping;

	// ADO InfoFrame data byte 0 is checksum
	Temp_Data[0] = 0x8F;
	for(i=1; i<6; ++i) {
		Temp_Data[0] += Temp_Data[i];
	}
	Temp_Data[0] = ~(Temp_Data[0] - 1);

	// Write ADO Info Frame back
	EP952_Reg_Write(EP952_ADO_Packet, Temp_Data, 6);

	// print for Debug
	DBG_printf(("EP952 set ADO Info: "));
	for(i=0; i<6; ++i) {
		DBG_printf(("[%d]0x%0.2X, ",(int)i, (int)Temp_Data[i] ));
	}
	DBG_printf(("\r\n"));

	// enable ADO packet
	EP952_Reg_Set_Bit(EP952_IIS_Control, EP952_IIS_Control__ACR_EN | EP952_IIS_Control__ADO_EN | EP952_IIS_Control__GC_EN | EP952_IIS_Control__AUDIO_EN);
}

//--------------------------------------------------------------------------------------------------
//
// Hardware Interface
//

SMBUS_STATUS EP952_Reg_Read(unsigned char ByteAddr, unsigned char *Data, unsigned int Size)
{
	return IIC_Read(IIC_EP952_Addr, ByteAddr, Data, Size);
}

SMBUS_STATUS EP952_Reg_Write(unsigned char ByteAddr, unsigned char *Data, unsigned int Size)
{
	//DBG_printf(("EP952_Reg_Write 0x%02X, 0x%02X, 0x%02X\r\n",(int)ByteAddr,(int)Data[0],(int)Size));
	return IIC_Write(IIC_EP952_Addr, ByteAddr, Data, Size);
}

SMBUS_STATUS EP952_Reg_Set_Bit(unsigned char ByteAddr, unsigned char BitMask)
{
	int result = 1;
	result = IIC_Read(IIC_EP952_Addr, ByteAddr, Temp_Data, 1);
	if(result == 0)
	{
		// Write back to Reg Reg_Addr
		Temp_Data[0] |= BitMask;

		return IIC_Write(IIC_EP952_Addr, ByteAddr, Temp_Data, 1);
	}
	else
	{
		return result;
	}
}

SMBUS_STATUS EP952_Reg_Clear_Bit(unsigned char ByteAddr, unsigned char BitMask)
{
	int result = 1;
	result = IIC_Read(IIC_EP952_Addr, ByteAddr, Temp_Data, 1);
	if(result == 0)
	{
		// Write back to Reg Reg_Addr
		Temp_Data[0] &= ~BitMask;

		return IIC_Write(IIC_EP952_Addr, ByteAddr, Temp_Data, 1);
	}
	else
	{
		return result;
	}
}


//==================================================================================================
//
// Private Functions
//

SMBUS_STATUS IIC_Write(unsigned char IICAddr, unsigned char ByteAddr, unsigned char *Data, unsigned int Size)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// return 0; for success
	// return 2; for No_ACK
	// return 4; for Arbitration
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// need to use customer's I2C  function

	UINT8 ret;
	UINT8 i;

	//printf ("IIC write size =%x\n",Size);

	if (Size > 255) {
		printf("IIC wt over:%lx\n", Size); //size can't large than 255
		return 0;
	}

	printf ("IIC write reg[%bx],len[%bu]:",ByteAddr,(UINT8)Size);
	for(i=0; i<(UINT8)Size; i++)
	{
		printf("%02bx ", Data[i]);
	}
	printf("\n");

	i2cDeviceAddrSet(IICAddr);
	ret = HDMI2_i2c_write(ByteAddr, Data, Size, 1);
	if (ret != SUCCESS) {
		ret = 0x02; //no ack
	}

	return ret;
}

SMBUS_STATUS IIC_Read(unsigned char IICAddr, unsigned char ByteAddr, unsigned char *Data, unsigned int Size)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// return 0; for success
	// return 2; for No_ACK
	// return 4; for Arbitration
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// need to use customer's I2C  function

	UINT8 ret;

	//printf ("IIC read size =%x\n",Size);

	if (Size > 255) {
		printf("IIC rd over:%lx\n", Size); //size can't large than 255
		return 0;
	}

	i2cDeviceAddrSet(IICAddr);
	ret = HDMI2_i2c_read(ByteAddr, Data, Size, 1);
	if (ret != SUCCESS) {
		ret = 0x02; //no ack
	}

	return ret;

}
UINT8
HDMI2_i2c_write(
	UINT8 regaddr,
	UINT8 *pregdata,
	UINT8 cnt,
	UINT8 bRetry
)
{
#define I2C_RETRY_CNT_W  10

	UINT8 mode = 0; // 0: normal mode, 1: burst mode
	UINT8 restartMode = 1;
	UINT8 subAddrEn = 1;

	UINT8 retryCnt = I2C_RETRY_CNT_W;
	UINT8 ret_tmp, ret;

	//printf("regaddr: %lx\n", (UINT32)regaddr);
	//printf("pregdata: %lx\n", (UINT32)*pregdata);
	//printf("cnt: %lx\n", (UINT32)cnt);
	while(XBYTE[0x29a0]); //wait I2C busy

	XBYTE[0x2904] = mode;
	XBYTE[0x290c] = 0x00;

	/* set write count */
	XBYTE[0x2914] = 0x01;

	ret = SUCCESS;
	while(cnt) {

		retryCnt = I2C_RETRY_CNT_W;
		do {
			/* set reg address */
			XBYTE[0x2930] = regaddr;

			/* write data */
			//printf("w->%bx\n", *pregdata);
			XBYTE[0x2931] = *pregdata;

			/* wait I2C busy */
			while(XBYTE[0x29a0]);

			/* check the ACK */
			if (XBYTE[0x29A1] & 0x01) {
				XBYTE[0x29A1] = 0x01;
				//printf("w no ack!\n");
				ret_tmp = FAIL;
			} else {
				ret_tmp = SUCCESS;
				break;
			}
		} while (bRetry && --retryCnt);

		if (ret_tmp != SUCCESS) {
			ret = FAIL;
			printf("reg(%bx),T(%bx)!\n", regaddr, I2C_RETRY_CNT_W);
		}

		regaddr++;
		pregdata++;
		cnt--;
	}

	return ret;
}


UINT8
HDMI2_i2c_read(
	UINT8 regaddr,
	UINT8 *pregdata,
	UINT8 cnt,
	UINT8 bRetry
)
{
#define I2C_RETRY_CNT_R  10

	UINT8 i;
	UINT8 rdCnt;
	UINT8 mode = 0; // 0: normal mode, 1: burst mode
	UINT8 restartMode = 1;
	UINT8 subAddrEn = 1;

	UINT8 retryCnt = I2C_RETRY_CNT_R;
	UINT8 ret_tmp, ret;

	//printf("regaddr: %lx\n", (UINT32)regaddr);
	//printf("cnt: %lx\n", (UINT32)cnt);
	while(XBYTE[0x29a0]); //wait I2C busy

	ret = SUCCESS;
	rdCnt = 0;
	while(cnt) {

		XBYTE[0x2904] = mode;
		XBYTE[0x290c] = 0x00;

		/* set reg address */
		XBYTE[0x2930] = regaddr;

		/* set read count */
		if (cnt > 8) {
			XBYTE[0x2914] = 0; //0x08 << 4;
			rdCnt = 8;
			cnt -= rdCnt;
		} else {
			XBYTE[0x2914] = cnt << 4;
			rdCnt = cnt;
			cnt = 0;
		}

		retryCnt = I2C_RETRY_CNT_R;
		do {
			/* trigger */
			XBYTE[0x2908] = 0x10 | (restartMode << 1) | subAddrEn;

			/* wait I2C busy */
			while(XBYTE[0x29a0]);

			/* check the ACK */
			if (XBYTE[0x29A1] & 0x01) {
				XBYTE[0x29A1] = 0x01;
				//printf("r no ack!\n");
				ret_tmp = FAIL;
			} else {
				ret_tmp = SUCCESS;
				break;
			}
		} while (bRetry && --retryCnt);

		if (ret_tmp == SUCCESS) {
			/* read the response data */
			for (i = 0; i < rdCnt; i++) {
				pregdata[i] = XBYTE[0x2931 + (i << 2)];
				//printf("pregdata[%bx]: %bx\n", i, pregdata[i]);
			}
		} else {
			ret = FAIL;
			printf("reg(%bx),T(%bx)!\n", regaddr, I2C_RETRY_CNT_R);
		}

		regaddr += rdCnt;
		pregdata += rdCnt;
	}

	return ret;
}

