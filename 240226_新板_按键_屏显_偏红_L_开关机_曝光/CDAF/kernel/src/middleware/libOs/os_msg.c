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
#include "general.h"
#include "os_msg.h"
#include "string.h"
#define SYS_MSG_INTERNAL // for libSys internal use
#include "func_msg_def.h"

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#define MSG_BUF_MAX  10
/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *                         G L O B A L    D A T A                         *
**************************************************************************/
static msg_t xdata msgBuf[MSG_CTRL_TYPE_MAX][MSG_BUF_MAX];  // 存放消息数组

static MainMsgQue_t xdata msgQueCtrl[MSG_CTRL_TYPE_MAX];
//static BIT data isMsgNull[MSG_CTRL_TYPE_MAX];


/**************************************************************************
 *				   E X T E R N A L	  R E F E R E N C E S				  *
 **************************************************************************/
extern SYS_MSG_DCL pFuncMsg code funcMsgRcvArray[];

/**
* @param    None
* @retval   None
* @brief    初始化消息队列 - 2010-5-4
**/
void osMsgInit (void)
{
	UINT8 i;
	MainMsgQue_t* pMsg;

	// 遍历消息控制类型，对每个消息队列进行初始化
	for (i = 0; i < MSG_CTRL_TYPE_MAX; i++) 
    {
		pMsg = &msgQueCtrl[i];
		
		// 设置消息队列的起始位置、结束位置、入队和出队指针等信息
		pMsg->QueStart = &msgBuf[i][0];
		pMsg->QueEnd = &msgBuf[i][MSG_BUF_MAX];
		pMsg->QueIn = &msgBuf[i][0];
		pMsg->QueOut = &msgBuf[i][0];
		pMsg->QueEntries = 0;
		pMsg->QueSize = MSG_BUF_MAX;
		pMsg->MsgBuf = &msgBuf[i][0];
		pMsg->bLock = 0;
		
		// 清空消息缓冲区并设置锁定标志为未锁定
		memset(pMsg->MsgBuf, 0, sizeof(msg_t) * (UINT32)MSG_BUF_MAX);
		//isMsgNull[i] = TRUE;
	}
}

/**
* @param    None
* @retval   返回值为 TRUE（非空）或 FALSE（空）
* @brief    查询消息队列是否为空
**/
UINT8 osMsgQuery (void)
{
	UINT8 i;
	MainMsgQue_t* pMsg;
	UINT8 ret;

	ret = FALSE;
	// 遍历消息控制类型，检查每个消息队列的消息计数是否为零
	for (i = 0; i < MSG_CTRL_TYPE_MAX; i++) 
    {
		pMsg = &msgQueCtrl[i];
		if (pMsg->QueEntries != 0) 
        {
			ret = TRUE;
			break;
		}
	}
	
	return ret;
}

/**
 * @param    msg: 消息类型 @osMsgList_t
 * @retval   返回值为 SUCCESS（成功）或 FAIL（失败）
 * @brief    将消息（事件）安全地发布到操作系统的消息队列中
**/
#pragma disable // 禁用中断，以防止在执行消息发布过程中被中断干扰
UINT8 osMsgPost (UINT16 msg)
{
	UINT8 status; // 用于保存临界区状态。
	UINT16 msgGrp; // 消息组ID。
	MainMsgQue_t* pMsg; // 指向消息队列的指针。

	// 从消息中提取消息组ID
	msgGrp = (msg & MSG_CTRL_IDX_GROUP_MASK) >> MSG_CTRL_IDX_GROUP_SHIFT;

	// 如果消息组ID超出允许的最大值，则返回失败
	if (msgGrp >= MSG_CTRL_TYPE_MAX) 
    {
		return FAIL;
	}
	
	ENTER_CRITICAL(status);
	// 进入临界区，保护代码段不受中断影响

	pMsg = &msgQueCtrl[msgGrp];	
	// 获取对应消息组ID的消息队列控制结构体的地址。

	// 如果消息队列已满，则退出临界区并返回失败
	if (pMsg->QueEntries >= pMsg->QueSize) 
    {
		EXIT_CRITICAL(status);
		return FAIL;
	}
	
	// 将消息放入队列中，并更新指针和计数
	*pMsg->QueIn++ = msg;
	pMsg->QueEntries++;
	if (pMsg->QueIn == pMsg->QueEnd) 
    {
		pMsg->QueIn = pMsg->QueStart;
	}
	
	EXIT_CRITICAL(status); // 退出临界区，恢复中断状态
	return SUCCESS;
}


/**
* @param    msg: 消息类型 @osMsgList_t
* @retval   返回值为 SUCCESS（成功）或 FAIL（失败）
* @brief    强制将消息（事件）安全地发布到操作系统的消息队列中
**/
#pragma disable // 禁用中断，以防止在执行消息发布过程中被中断干扰
UINT8 osMsgPostForce (UINT16 msg)
{
	UINT8 status; // 用于保存临界区状态。
	UINT16 msgGrp; // 消息组ID。
	MainMsgQue_t* pMsg; // 指向消息队列的指针。

	// 从消息中提取消息组ID
	msgGrp = (msg & MSG_CTRL_IDX_GROUP_MASK) >> MSG_CTRL_IDX_GROUP_SHIFT;

	// 如果消息组ID超出允许的最大值，则返回失败
	if (msgGrp >= MSG_CTRL_TYPE_MAX) 
    {
		return FAIL;
	}
		
	ENTER_CRITICAL(status);
	// 进入临界区，保护代码段不受中断影响

	pMsg = &msgQueCtrl[msgGrp];
	// 获取对应消息组ID的消息队列控制结构体的地址。

	// 如果消息队列已满，则退出临界区并返回失败
	if (pMsg->QueEntries >= pMsg->QueSize) 
    {
		//isMsgNull[msgGrp] = FALSE; // 标记消息组为非空，这行代码被注释了。
		*pMsg->QueIn = msg; // 将消息放入队列中，但不移动输入指针。这可能是一个错误，因为它会导致消息覆盖。
		//printf( "MsgQue over flow!\n" ); // 打印消息队列溢出的警告，这行代码被注释了。
		EXIT_CRITICAL(status); // 退出临界区。
		return FAIL; // 返回失败。
	}
	
	// 将消息放入队列中，并更新指针和计数
	*pMsg->QueIn++ = msg;
	pMsg->QueEntries++;
	if (pMsg->QueIn == pMsg->QueEnd) 
    {
		pMsg->QueIn = pMsg->QueStart;
	}
	
	EXIT_CRITICAL(status); // 退出临界区，恢复中断状态
	return SUCCESS; // 操作成功，返回成功状态。
}

/**
* @param    msg: 要插入的消息
* @retval   返回值为 SUCCESS（成功）或 FAIL（失败）
* @brief    将消息插入到消息队列的前面 - 2010-5-4
**/
#pragma disable //禁用中断
UINT8 osMsgPostFront (UINT16 msg)
{
	UINT8 status;
	UINT16 msgGrp;
	MainMsgQue_t* pMsg;

	// 从消息中提取消息组ID
	msgGrp = (msg & MSG_CTRL_IDX_GROUP_MASK) >> MSG_CTRL_IDX_GROUP_SHIFT;
	if (msgGrp >= MSG_CTRL_TYPE_MAX) 
    {
		return FAIL;
	}
	
	ENTER_CRITICAL(status);

	pMsg = &msgQueCtrl[msgGrp];
	if (pMsg->QueEntries >= pMsg->QueSize) 
    {
		// 如果消息队列已满，则退出临界区并返回失败
		EXIT_CRITICAL(status);
		return FAIL;
	}
	//isMsgNull[msgGrp] = FALSE;
	// 将消息插入到消息队列的前面
	if (pMsg->QueOut == pMsg->QueStart) 
    {
		pMsg->QueOut = pMsg->QueEnd;
		*pMsg->QueOut = msg;
	} 
    else 
    {
		*(--pMsg->QueOut) = msg;
	}
	pMsg->QueEntries++;
	
	EXIT_CRITICAL(status);
	return SUCCESS;
}

/**
* @param    msg: 要等待的消息
* @retval   返回值为 SUCCESS（成功）或 FAIL（失败）
* @brief    等待指定消息出现在消息队列中
**/
UINT8 osMsgWait(UINT16 msg)
{
	while(osMsgSeek(msg) == FALSE); // 循环等待指定消息出现在消息队列中
	return SUCCESS;
}

/**
* @param    msg: 存储接受到的消息的指针
* @retval   返回值为 SUCCESS（成功）或 FAIL（失败）
* @brief    接受消息
**/
UINT8 osMsgAccept (UINT16* msg)
{
	UINT8 i, ret;
	MainMsgQue_t* pMsg;
	
	ret = FAIL; // 默认返回失败

	// 按照优先级顺序检查消息队列，将优先级高的消息作为接受的消息
	for (i = MSG_CTRL_TYPE_HOST; i < MSG_CTRL_TYPE_MAX; i++) 
    {
		pMsg = &msgQueCtrl[i];
		if (pMsg->QueEntries != 0) 
        {
			if (msg) 
            {
				*msg = *pMsg->QueOut; // 将接受到的消息存储到传入的指针中
			}
			*pMsg->QueOut++ = SP1K_MSG_NULL; // 将队列中的消息标记为无效
			pMsg->QueEntries--; // 减少消息队列中的消息计数
			if (!pMsg->QueEntries) 
            {
				//isMsgNull[msgGrp] = TRUE; // 如果队列为空，则标记为消息组为空
			}
			if (pMsg->QueOut == pMsg->QueEnd) 
            {
				pMsg->QueOut = pMsg->QueStart; // 如果出队指针到达队列末尾，则重置为队列开始
			}
			ret = SUCCESS; // 设置返回值为成功
			break;
		}
	}
	
	if (ret != SUCCESS) 
    {
		if (msg)
        {
			*msg = SP1K_MSG_NULL; // 如果没有接受到消息，则将消息设为无效
		}
	}
	
	return ret;
}

/**
* @param    msgGrp: 要清空的消息组
* @retval   None
* @brief    清空消息队列
**/
#pragma disable // 禁用中断
void osMsgFlush (UINT8 msgGrp)
{
	UINT8 status;
	MainMsgQue_t* pMsg;
	UINT8 i;

	ENTER_CRITICAL(status);
	
	// 遍历消息控制类型，对每个消息队列进行清空操作
	for (i = 0; i < MSG_CTRL_TYPE_MAX; i++) 
    {
		pMsg = &msgQueCtrl[i];
		
		// 在消息锁定模式下，此操作将被中止
		if (pMsg->bLock || (msgGrp & (0x01 << i)) == 0x00) 
        {
			continue;
		}
		
		pMsg->QueIn = pMsg->QueStart; // 将入队指针重置为队列开始
		pMsg->QueOut = pMsg->QueStart; // 将出队指针重置为队列开始
		pMsg->QueEntries = 0; // 消息队列中的消息计数清零
		memset(pMsg->MsgBuf, 0, sizeof(msg_t) * (UINT32)MSG_BUF_MAX); // 清空消息缓冲区
		//isMsgNull[i] = TRUE;
	}
	
	EXIT_CRITICAL(status);
}

/**
* @param    msg: 要搜索的消息
* @retval   返回值为 TRUE（成功）或 FALSE（失败）
* @brief    搜索消息缓冲区中是否存在指定的消息 - 2010-5-4
**/
UINT8 osMsgSeek(UINT16 msg)
{
	UINT8 i, j;

	// 遍历消息缓冲区，搜索指定的消息
	for (i = 0; i < MSG_CTRL_TYPE_MAX; i++) 
    {
		for (j = 0; j < MSG_BUF_MAX; j++) 
        {
			if (msgBuf[i][j] == msg) 
            {
				return TRUE;
			}
		}
	}

	// 没有找到指定的消息
	return FALSE;
}

/**
* @param    msg: 要查找的消息
* @retval   返回值为 SUCCESS（找到）或 FAIL（未找到）
* @brief    在中断模式下查找消息
**/
UINT8 osMsgIntrSeek(UINT16 msg)
{
	UINT8 i, j;

	// 遍历消息类型和缓冲区，查找消息
	for (i = 0; i < MSG_CTRL_TYPE_MAX; i++) 
    {
		for (j = 0; j < MSG_BUF_MAX; j++) 
        {
			if (msgBuf[i][j] == msg) 
            {
				return TRUE; // 找到消息，返回成功
			}
		}
	}

	return FALSE; // 未找到消息，返回失败
}

/**
* @param    bLock: 是否锁定消息的标志位
* @param    msgGrp: 要锁定或解锁的消息组
* @retval   返回值为 SUCCESS（找到）或 FAIL（未找到）
* @brief    在中断模式下查找消息
**/
void osMsgLock(UINT8 bLock, msgCtrlFlush_t msgGrp)
{
	UINT8 i;
	
	// 遍历消息控制类型，根据传入的消息组对相应消息队列进行锁定或解锁操作
	for (i = 0; i < MSG_CTRL_TYPE_MAX; i++) 
    {
		// 在消息锁定模式下，此操作将被中止
		if ((msgGrp & (0x01 << i)) != 0x00) 
        {
			msgQueCtrl[i].bLock = bLock;
		}
	}
}

/**
* @param    NONE
* @retval   NONE
* @brief    接收消息
**/
void osMsgRcv(void)
{
	pFuncMsg* pf = &funcMsgRcvArray[0];

	// 调用消息接收控制器
	if (*pf != NULL) 
    {
		(*pf)();
	}
}


