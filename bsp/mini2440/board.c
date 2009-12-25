/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-24     Bernard      first implementation
 * 2006-05-05     Bernard      add DATA_COUNT definition
 * 2006-10-05     Alsor.Z      for s3c2410x porting
 * 2007-11-20     Yi.Qiu	     add lcd,touch,console
 */

#include <rtthread.h>
#include <rthw.h>

#include "board.h"

/**
 * @addtogroup mini2440
 */
/*@{*/

extern rt_uint32_t PCLK, FCLK, HCLK, UCLK;
extern rt_uint8_t asc16_font[];
extern rt_uint16_t _rt_hw_framebuffer[];

extern void rt_hw_clock_init(void);
extern void rt_hw_lcd_init(void);
extern void rt_hw_mmu_init(void);
extern void rt_hw_touch_init(void);

extern void rt_kbd_init(void);
extern void rt_console_init(rt_uint8_t*, rt_uint8_t*, rt_uint8_t);

extern void rt_hw_get_clock(void);
extern void rt_hw_set_dividor(rt_uint8_t hdivn, rt_uint8_t pdivn);
extern void rt_hw_set_clock(rt_uint8_t sdiv, rt_uint8_t pdiv, rt_uint8_t mdiv);

static rt_uint32_t timer_load_val = 0;

#define UART0	((struct uartport *)U0BASE)
struct serial_int_rx uart0_int_rx;
struct serial_device uart0 =
{
	UART0,
	&uart0_int_rx,
	RT_NULL
};
struct rt_device uart0_device;

/**
 * This function will handle rtos timer
 */
void rt_timer_handler(int vector)
{
	/* reset TDATA0 */
	TCNTB4 = timer_load_val;

	rt_tick_increase();
}

/**
 * This function will handle serial
 */
void rt_serial_handler(int vector)
{
	INTSUBMSK |= (BIT_SUB_RXD0);

	rt_hw_serial_isr(&uart0_device);

	SUBSRCPND |= BIT_SUB_RXD0;

	/* Unmask sub interrupt (RXD0) */
	INTSUBMSK  &=~(BIT_SUB_RXD0);
}

/**
 * This function will handle init uart
 */
void rt_hw_uart_init(void)
{
	int i;
	/* UART0 port configure */
	GPHCON |= 0xAA;
	/* PULLUP is disable */
	GPHUP |= 0xF;

	/* FIFO enable, Tx/Rx FIFO clear */
	uart0.uart_device->ufcon = 0x1;
	/* disable the flow control */
	uart0.uart_device->umcon = 0x0;
	/* Normal,No parity,1 stop,8 bit */
	uart0.uart_device->ulcon = 0x3;
	/*
	 * tx=level,rx=edge,disable timeout int.,enable rx error int.,
	 * normal,interrupt or polling
	 */
	uart0.uart_device->ucon = 0x245;
	/* Set uart0 bps */
	uart0.uart_device->ubrd = (rt_int32_t)(PCLK / (BPS * 16)) - 1;
	/* output PCLK to UART0/1, PWMTIMER */
	CLKCON |= 0x0D00;

	for (i = 0; i < 100; i++);

	/* install uart isr */
	INTSUBMSK &= ~(BIT_SUB_RXD0);

	rt_hw_interrupt_install(INTUART0, rt_serial_handler, RT_NULL);
	rt_hw_interrupt_umask(INTUART0);
}

/**
 * This function will init s3ceb2410 board
 */
void rt_hw_board_init()
{
	/* initialize the system clock */
	rt_hw_clock_init();

	/* Get the clock */
	rt_hw_get_clock();

	/* initialize uart */
	rt_hw_uart_init();

	/* initialize mmu */
	rt_hw_mmu_init();

	/* initialize console */
	//rt_console_init(&_rt_hw_framebuffer[0], &asc16_font[0], 2);

#ifdef RT_USING_RTGUI
	rt_hw_touch_init();
#endif

	/* install interrupt handler */
	rt_hw_interrupt_install(INTTIMER4, rt_timer_handler, RT_NULL);
	rt_hw_interrupt_umask(INTTIMER4);

	/* stop timer 	*/
	/* TCON = 0x0;	*/
}

/*@}*/
