/*
 * File      : startup.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-08-31     Bernard      first implementation
 */

#include <rthw.h>
#include <rtthread.h>

#include "board.h"

#ifdef RT_USING_DFS
#include "sd.h"
#endif

#ifdef RT_USING_LWIP
#include "dev_emac.h"
#include "netif/ethernetif.h"
#endif

#ifdef RT_USING_FINSH
#include <finsh.h>
extern void finsh_system_init(void);
#endif

/**
 * @addtogroup sam7x256
 */

/*@{*/
#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#endif

#ifdef __GNUCC__
extern unsigned char __bss_start;
extern unsigned char __bss_end;
#endif

extern void rt_hw_interrupt_init(void);
extern int  rt_application_init(void);

#ifdef RT_USING_DEVICE
#include "dev_serial.h"
#endif

#ifdef RT_USING_FINSH
extern void finsh_system_init(void);
#endif

#ifdef RT_USING_DBGU
#include "dev_dbgu.h"
#endif
/**
 * This function will startup RT-Thread RTOS.
 */
void rtthread_startup(void)
{
	/* init hardware interrupt */
	rt_hw_interrupt_init();

	/* init board */
	rt_hw_board_init();
	
	rt_show_version();

	/* init tick */
	rt_system_tick_init();

	/* init kernel object */
	rt_system_object_init();

	/* init timer system */
	rt_system_timer_init();

#ifdef RT_USING_HEAP
#ifdef __CC_ARM
	rt_system_heap_init((void*)&Image$$RW_IRAM1$$ZI$$Limit, (void*)0x00210000);
#elif __ICCARM__
    rt_system_heap_init(__segment_end("HEAP"), (void*)0x00210000);
#else
	rt_system_heap_init(&__bss_end, 0x00210000);
#endif
#endif

	/* init scheduler system */
	rt_system_scheduler_init();

#ifdef RT_USING_HOOK /* if the hook is used */
	/* set idle thread hook */
	//rt_thread_idle_sethook(led_flash);
#endif

#ifdef RT_USING_DEVICE

#ifdef RT_USING_DBGU
	/* init dbgu */
	rt_hw_dbgu_init();
#endif

	/* init hardware serial device */
	rt_hw_serial_init();

#ifdef RT_USING_LWIP
	eth_system_device_init();

	/* register AT91 EMAC device */
	sam7xether_register("E0");
#endif

#ifdef RT_USING_DFS
	rt_hw_sdcard_init();
#endif

	/*init all registed devices*/
	rt_device_init_all();
#endif

	/* init application */
	rt_application_init();

#ifdef RT_USING_FINSH
	/* init finsh */
	finsh_system_init();
	finsh_set_device("dbgu");
#endif

	/* init idle thread */
	rt_thread_idle_init();

	/* start scheduler */
	rt_system_scheduler_start();

	/* never reach here */
	return ;
}

int main (void)
{
	/* invoke rtthread_startup */
	rtthread_startup();
	
	return 0;
}

/*@}*/
