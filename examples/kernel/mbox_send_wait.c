/*
 * �����嵥����������_��������
 *
 * ������������������䷢��ʱ������������
 *
 */
#include <rtthread.h>
#include "tc_comm.h"

/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

/* ������ƿ� */
static struct rt_mailbox mb;
/* ���ڷ��ʼ����ڴ�� */
static char mb_pool[32];

static char mb_str1[] = "I'm a mail!";
static char mb_str2[] = "this is another mail!";

/* �߳�1��� */
static void thread1_entry(void* parameter)
{
	unsigned char* str;

	while (1)
	{
		/* ����������ȡ�ʼ� */
		if (rt_mb_recv(&mb, (rt_uint32_t*)&str, RT_WAITING_FOREVER) == RT_EOK)
		{
			rt_kprintf("thread1: get a mail from mailbox, the content:%s\n", str);

			/* ��ʱ20��OS Tick */
			rt_thread_delay(50);
		}
	}
}

/* �߳�2��� */
static void thread2_entry(void* parameter)
{
	rt_uint8_t count;
    unsigned char  *str;

	count = 0;
	while (1)
	{
		count ++;
		if (count & 0x1)
		{
			/* ����mb_str1��ַ�������� */
            str = mb_str1;
		}
		else
		{
			/* ����mb_str2��ַ�������� */
            str = mb_str2;
		}

        /* ��ͣ�ķ����ʼ������������ȴ�10��tick��Ȼ��ʱ */
        if( rt_mb_send_wait(&mb, (rt_uint32_t)str,10) == RT_EOK )
            rt_kprintf("thread2: sent a mail to mailbox, the content:%s\n", str);
        else
            rt_kprintf("thread2: timeout while waiting to send a mail.\n");
	}
}

int mbox_send_wait_init()
{
	/* ��ʼ��һ��mailbox */
	rt_mb_init(&mb,
		"mbt",             /* ������mbt */
		&mb_pool[0],       /* �����õ����ڴ����mb_pool */
		sizeof(mb_pool)/4,   /* ��С��mb_pool��С����4����Ϊһ���ʼ��Ĵ�С��4�ֽ� */
		RT_IPC_FLAG_FIFO); /* ����FIFO��ʽ�����̵߳ȴ� */

	/* �����߳�1 */
	tid1 = rt_thread_create("t1",
		thread1_entry, RT_NULL, /* �߳������thread1_entry, ��ڲ�����RT_NULL */
		THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
	if (tid1 != RT_NULL)
		rt_thread_startup(tid1);
	else
		tc_stat(TC_STAT_END | TC_STAT_FAILED);

	/* �����߳�2 */
	tid2 = rt_thread_create("t2",
		thread2_entry, RT_NULL, /* �߳������thread2_entry, ��ڲ�����RT_NULL */
		THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
	if (tid2 != RT_NULL)
		rt_thread_startup(tid2);
	else
		tc_stat(TC_STAT_END | TC_STAT_FAILED);

	return 0;
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
	/* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
	rt_enter_critical();

	/* ɾ���߳� */
	if (tid1 != RT_NULL && tid1->stat != RT_THREAD_CLOSE)
		rt_thread_delete(tid1);
	if (tid2 != RT_NULL && tid2->stat != RT_THREAD_CLOSE)
		rt_thread_delete(tid2);

	/* ִ������������� */
	rt_mb_detach(&mb);

	/* ���������� */
	rt_exit_critical();

	/* ����TestCase״̬ */
	tc_done(TC_STAT_PASSED);
}

int _tc_mbox_send_wait()
{
	/* ����TestCase����ص����� */
	tc_cleanup(_tc_cleanup);
	mbox_send_wait_init();

	/* ����TestCase���е��ʱ�� */
	return 300;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_mbox_send_wait, a example of mailbox send wait);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
	mbox_send_wait_init();

	return 0;
}
#endif

