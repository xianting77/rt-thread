/*
 * File      : readwrite.c
 * This file is part of RT-TestCase in RT-Thread RTOS
 * COPYRIGHT (C) 2010, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-02-10     Bernard      first version
 */
#include <rtthread.h>
#include <dfs_posix.h>

/* �ļ���д���� */
void readwrite(const char* filename)
{
	int fd;

	/* ֻд�� */

	/* ֻд����ĩβ��Ӵ� */

	/* ֻ���򿪽�������У�� */

	/* lseek���ԣ�������У������ */
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(readwrite, perform file read and write test);
#endif
