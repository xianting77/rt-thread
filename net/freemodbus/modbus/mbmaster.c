/*
 * File      : mbmaster.c
 * This file is part of freemodbus in RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-04-04     yi.qiu      first version
 */
 
/* ----------------------- Platform includes --------------------------------*/
#include "rtthread.h"
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbfunc.h"

eMBErrorCode eMBMReadHoldingRegisters  (UCHAR ucSlaveAddress, USHORT usRegStartAddress, 
			UBYTE ubNRegs, USHORT arusBufferOut[]) 
{
	static UCHAR ucMBFrame[5];
	eMBErrorCode eStatus = MB_ENOERR;
	eMBEventType eEvent;
	static UCHAR ucRcvAddress;
	static USHORT usLength;

	/* make up request frame */	
	ucMBFrame[0] = MB_FUNC_READ_HOLDING_REGISTER;
	ucMBFrame[1] = (UCHAR)(usRegStartAddress >> 8);
	ucMBFrame[2] = (UCHAR)(usRegStartAddress);
	ucMBFrame[3] = (UCHAR)(ubNRegs >> 8);
	ucMBFrame[4] = (UCHAR)(ubNRegs);	

	 
		rt_kprintf("send frame [%x%x%x%x%x]\n", 
		ucMBFrame[0], ucMBFrame[1], ucMBFrame[2], ucMBFrame[3], ucMBFrame[4]); 
	
	
	/* send request frame to slave device */
	eStatus = eMBRTUSend( ucSlaveAddress, ucMBFrame, 5 );

	/* wait on receive event */
	if( xMBPortEventGet( &eEvent ) == TRUE )
	{		
		eStatus = eMBRTUReceive( &ucRcvAddress, &ucMBFrame, &usLength );
		if( eStatus == MB_ENOERR )
		{
			/* Check if the frame is for us. If not ignore the frame. */
			if( ( ucRcvAddress == ucSlaveAddress ) || ( ucRcvAddress == MB_ADDRESS_BROADCAST ) )
			{
				/* parse and restore data */
				rt_kprintf("parse and restore date here\n");
			}
		}
	}
	else eStatus = MB_ETIMEDOUT;

	return eStatus;
}

