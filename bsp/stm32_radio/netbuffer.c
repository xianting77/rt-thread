#include <rthw.h>
#include <rtthread.h>

#include "netbuffer.h"

#define MP3_DECODE_MP_CNT   2
#define MP3_DECODE_MP_SZ    2560

static rt_uint8_t mempool[(MP3_DECODE_MP_SZ * 2 + 4)* 2]; // 5k x 2
static struct rt_mempool _mp;
static rt_bool_t is_inited = RT_FALSE;

rt_size_t sbuf_get_size()
{
    return MP3_DECODE_MP_SZ * 2;
}

void sbuf_init()
{
    rt_mp_init(&_mp, "mp3", &mempool[0], sizeof(mempool), MP3_DECODE_MP_SZ * 2);
}

void* sbuf_alloc()
{
	if (is_inited == RT_FALSE)
	{
		sbuf_init();
		is_inited = RT_TRUE;
	}

    return (rt_uint16_t*)rt_mp_alloc(&_mp, RT_WAITING_FOREVER);
}

void sbuf_release(void* ptr)
{
    rt_mp_free(ptr);
}

#if STM32_EXT_SRAM
/* netbuf worker stat */
#define NETBUF_STAT_FREE		0
#define NETBUF_STAT_BUFFERING	1
#define NETBUF_STAT_BUSY		2
#define NETBUF_STAT_STOPPING	3
#define NETBUF_STAT_STOPPED		4

/* net buffer module */
struct net_buffer
{
    /* read index and save index in the buffer */
	rt_size_t read_index, save_index;

    /* buffer data and size of buffer */
	rt_uint8_t* buffer_data;
	rt_size_t data_length;
	rt_size_t size;

	/* buffer ready water mater */
	rt_uint32_t ready_wm, resume_wm;
	rt_bool_t is_wait_ready, is_wait_resume;
    rt_sem_t wait_ready, wait_resume;

	/* netbuf worker stat */
	rt_uint8_t stat;
};
struct net_buffer_job
{
	rt_size_t (*fetch)(rt_uint8_t* ptr, rt_size_t len, void* parameter);
	void (*close)(void* parameter);

	void* parameter;
};

static struct net_buffer _netbuf;
static rt_mq_t _netbuf_mq = RT_NULL;

rt_size_t net_buf_read(rt_uint8_t* buffer, rt_size_t length)
{
    rt_size_t data_length, read_index;
	rt_uint32_t level;

	data_length = _netbuf.data_length;

    if ((data_length == 0) &&
		(_netbuf.stat != NETBUF_STAT_STOPPED && _netbuf.stat != NETBUF_STAT_STOPPING))
    {
    	/* set stat */
    	_netbuf.stat = NETBUF_STAT_BUFFERING;
		rt_kprintf("stat -> buffering\n");

        /* buffer is not ready. */
        _netbuf.is_wait_ready = RT_TRUE;
		rt_kprintf("wait ready, data len: %d, stat %d\n", data_length, _netbuf.stat);
		/* set buffer status to buffering */
		player_set_buffer_status(RT_TRUE);
        rt_sem_take(_netbuf.wait_ready, RT_WAITING_FOREVER);
    }

    if ((data_length <= _netbuf.ready_wm) &&
		(_netbuf.stat == NETBUF_STAT_BUFFERING))
    {
        /* buffer is not ready. */
        _netbuf.is_wait_ready = RT_TRUE;
		rt_kprintf("wait ready, data len: %d, stat %d\n", data_length, _netbuf.stat);
        rt_sem_take(_netbuf.wait_ready, RT_WAITING_FOREVER);
    }

    /* get read and save index */
    read_index = _netbuf.read_index;

    /* re-get data legnth */
    data_length = _netbuf.data_length;

	/* set the length */
	if (length > data_length) length = data_length;

	// rt_kprintf("data len: %d, read idx %d\n", data_length, read_index);
    if (data_length > 0)
    {
        /* copy buffer */
        if (_netbuf.size - read_index > length)
        {
            rt_memcpy(buffer, &_netbuf.buffer_data[read_index],
                length);
			_netbuf.read_index += length;
        }
        else
        {
            rt_memcpy(buffer, &_netbuf.buffer_data[read_index],
                _netbuf.size - read_index);
            rt_memcpy(&buffer[_netbuf.size - read_index],
                &_netbuf.buffer_data[0],
                length - (_netbuf.size - read_index));
			_netbuf.read_index = length - (_netbuf.size - read_index);
        }

		level = rt_hw_interrupt_disable();
		_netbuf.data_length -= length;
		data_length = _netbuf.data_length;

        if ((_netbuf.is_wait_resume == RT_TRUE) && data_length < _netbuf.resume_wm)
        {
        	_netbuf.is_wait_resume = RT_FALSE;
			rt_hw_interrupt_enable(level);

			rt_kprintf("resume netbuf worker\n");
            rt_sem_release(_netbuf.wait_resume);
        }
		else
		{
			rt_hw_interrupt_enable(level);
		}
    }

    return length;
}

void net_buf_add_job(rt_size_t (*fetch)(rt_uint8_t* ptr, rt_size_t len, void* parameter),
	void (*close)(void* parameter),
	void* parameter)
{
	struct net_buffer_job job;

	job.fetch = fetch;
	job.close = close;
	job.parameter = parameter;

	rt_mq_send(_netbuf_mq, (void*)&job, sizeof(struct net_buffer_job));
}

void net_buf_stop_job()
{
	rt_uint32_t level;

	level = rt_hw_interrupt_disable();
	_netbuf.stat = NETBUF_STAT_STOPPING;
	rt_kprintf("stat -> stopping\n");
	rt_hw_interrupt_enable(level);
}

static void net_buf_do_stop(struct net_buffer_job* job)
{
	/* source closed */
	job->close(job->parameter);
	_netbuf.stat = NETBUF_STAT_STOPPED;
	rt_kprintf("stat -> stopped\n");
	if (_netbuf.is_wait_ready == RT_TRUE)
	{
		/* resume the wait for buffer task */
		_netbuf.is_wait_ready = RT_FALSE;
		rt_sem_release(_netbuf.wait_ready);
	}

	rt_kprintf("job done, stat %d\n", _netbuf.stat);
}

#define NETBUF_BLOCK_SIZE  4096
static void net_buf_do_job(struct net_buffer_job* job)
{
	rt_uint32_t level;
    rt_size_t read_length, data_length;
	rt_uint8_t *ptr;

	ptr = rt_malloc(NETBUF_BLOCK_SIZE);

    while (1)
    {
    	if (_netbuf.stat == NETBUF_STAT_STOPPING)
    	{
    		net_buf_do_stop(job);
            break;
    	}

    	/* fetch data buffer */
		read_length = job->fetch(ptr, NETBUF_BLOCK_SIZE, job->parameter);
		if (read_length <= 0)
		{
			net_buf_do_stop(job);
            break;
		}
		else
		{
			/* got data length in the buffer */
			data_length = _netbuf.data_length;

			/* check avaible buffer to save */
			if ((_netbuf.size - data_length) < read_length)
			{
				rt_err_t result;

				_netbuf.is_wait_resume = RT_TRUE;
				rt_kprintf("netbuf suspend, avaible room %d\n", data_length);
				result = rt_sem_take(_netbuf.wait_resume, RT_WAITING_FOREVER);
				if (result != RT_EOK)
				{
					/* stop net buffer worker */
					net_buf_do_stop(job);
					break;
				}
			}

			/* there are free space to fetch data */
	        if ((_netbuf.size - _netbuf.save_index) < read_length)
	        {
	        	rt_memcpy(&_netbuf.buffer_data[_netbuf.save_index],
					ptr, _netbuf.size - _netbuf.save_index);
				rt_memcpy(&_netbuf.buffer_data[0],
					ptr + (_netbuf.size - _netbuf.save_index),
					read_length - (_netbuf.size - _netbuf.save_index));

				/* move save index */
				_netbuf.save_index = read_length - (_netbuf.size - _netbuf.save_index);
	        }
	        else
	        {
	        	rt_memcpy(&_netbuf.buffer_data[_netbuf.save_index],
					ptr, read_length);

				/* move save index */
				_netbuf.save_index += read_length;
				if (_netbuf.save_index >= _netbuf.size) _netbuf.save_index = 0;
	        }

			level = rt_hw_interrupt_disable();
			_netbuf.data_length += read_length;
			data_length = _netbuf.data_length;
			rt_hw_interrupt_enable(level);
		}

		rt_kprintf("buffering ... %d %c\n", (data_length * 100) / _netbuf.size, '%');

		/* set buffer position */
		player_set_position(data_length);

		if ((_netbuf.stat == NETBUF_STAT_BUFFERING) && (data_length >= _netbuf.ready_wm))
		{
			_netbuf.stat = NETBUF_STAT_BUSY;
			rt_kprintf("stat -> busy\n");

			/* notify the thread for waitting buffer ready */
			rt_kprintf("resume wait buffer\n");
			if (_netbuf.is_wait_ready == RT_TRUE)
			{
				_netbuf.is_wait_ready = RT_FALSE;
				/* set buffer status to playing */
				player_set_buffer_status(RT_FALSE);
				rt_sem_release(_netbuf.wait_ready);
			}
		}
    }

	/* release fetch buffer */
	rt_free(ptr);
}

static void net_buf_thread_entry(void* parameter)
{
	rt_err_t result;
	struct net_buffer_job job;

    while (1)
    {
    	/* get a job */
		result = rt_mq_recv(_netbuf_mq, (void*)&job, sizeof(struct net_buffer_job), RT_WAITING_FOREVER);
		if (result == RT_EOK)
		{
			_netbuf.stat = NETBUF_STAT_BUFFERING;
			rt_kprintf("stat -> buffering\n");

			/* perform the job */
			net_buf_do_job(&job);
		}
    }
}

void net_buf_init(rt_size_t size)
{
    rt_thread_t tid;

    /* init net buffer structure */
    _netbuf.read_index = _netbuf.save_index = 0;
    _netbuf.size = size; /* net buffer size */

    /* allocate buffer */
    _netbuf.buffer_data = rt_malloc(_netbuf.size);
	_netbuf.data_length = 0;

	/* set ready and resume water mater */
	_netbuf.ready_wm = _netbuf.size * 90/100;
	_netbuf.resume_wm = _netbuf.size * 80/100;

	/* set init stat */
	_netbuf.stat = NETBUF_STAT_FREE;
	rt_kprintf("stat -> free\n");

	_netbuf.wait_ready  = rt_sem_create("nready", 0, RT_IPC_FLAG_FIFO);
	_netbuf.wait_resume = rt_sem_create("nresum", 0, RT_IPC_FLAG_FIFO);
	_netbuf.is_wait_ready = RT_FALSE;
	_netbuf.is_wait_resume = RT_FALSE;

	/* crate message queue */
	_netbuf_mq = rt_mq_create("njob", sizeof(struct net_buffer_job),
		4, RT_IPC_FLAG_FIFO);

    /* create net buffer thread */
    tid = rt_thread_create("nbuf",
        net_buf_thread_entry, RT_NULL,
        1024, 22, 5);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}
#endif
