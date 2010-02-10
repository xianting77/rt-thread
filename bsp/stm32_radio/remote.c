/*
+-----------------------
|
| ������ѧϰң��
|
| Chang Logs:
| Date           Author       Notes
| 2010-01-02     aozima       The bate version.
| 2010-02-10     aozima       change printf string ���� to english.
+----------------------------------------------------
*/

#include <rtthread.h>
#include <dfs_posix.h>
#include <stm32f10x.h>

/* �ض���printf */
#define printf                   rt_kprintf
/* ��������ƫ��,��λ0.01ms */
#define remote_deviation         15
#define remote_code_len_max      100


/* ����ģʽ 0:û����,1:��ѧϰ,2:�������� */
unsigned int rem_mode = 0;

static unsigned int first_tick = 0;
static unsigned int rx_count = 0;
static unsigned short rm_code[remote_code_len_max];

struct rem_codes_typedef
{
    unsigned int len;
    unsigned short rem_code[remote_code_len_max];
};
struct rem_codes_typedef * p_rem_code_src = RT_NULL;

static const char  str1[]="KEY_UP";     /* �� */
static const char  str2[]="KEY_DOWN";   /* �� */
static const char  str3[]="KEY_LEFT";   /* �� */
static const char  str4[]="KEY_RIGHT";  /* �� */
static const char  str5[]="KEY_ENTER";  /* ȷ�� */
static const char  str6[]="KEY_RETURN"; /* ���� */
static const char * desc_key[6]= {str1,str2,str3,str4,str5,str6};


/* tim5 configure */
static void TIM5_Configuration(void)
{
    /* ʱ�Ӽ���Ƶ���� */
    {
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        /* Time Base configuration */
        /* 72M/720 = 0.01ms */
        TIM_TimeBaseStructure.TIM_Prescaler = 720-1;
        //����ģʽ:���ϼ���
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        //���¼�������ʼֵ
        TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

        TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    }

    /* �������� */
    {
        TIM_ICInitTypeDef  TIM_ICInitStructure;

        TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;/* ÿ�μ�⵽��������ʹ���һ�β��� */
        TIM_ICInitStructure.TIM_ICFilter    = 8;/* �˲� */

        TIM_ICInitStructure.TIM_Channel     = TIM_Channel_3;//ѡ��ͨ��3
        TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;//�½���
        TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//ͨ������ѡ��
        TIM_ICInit(TIM5, &TIM_ICInitStructure);

        TIM_ICInitStructure.TIM_Channel     = TIM_Channel_4;//ѡ��ͨ��3
        TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;//������
        TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;//ͨ������ѡ��
        TIM_ICInit(TIM5, &TIM_ICInitStructure);
    }

    /* ���봥��Դѡ��:�ⲿ���봥�� */
    TIM_SelectInputTrigger(TIM5, TIM_TS_ETRF);//TIM_TS_ETRF �ⲿ����
    /* ��ģʽ-��λģʽ */
    /* TIM_SlaveMode_Reset 4:ѡ�еĴ�������(TRGI)�����������³�ʼ�������������Ҳ���һ�����¼Ĵ������ź� */
    TIM_SelectSlaveMode(TIM5, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM5, TIM_MasterSlaveMode_Enable);

    /* TIM enable counter */
    TIM_Cmd(TIM5, ENABLE);

    /* Enable the CC3 and CC4 Interrupt Request */
    TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);
    TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM5 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void RCC_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* clock enable */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
}

static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* TIM5 channel 3 pin (PA.02) configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void rem_start(void)
{
    RCC_Configuration();
    GPIO_Configuration();

    /* configure TIM5 for remote and encoder */
    NVIC_Configuration();
    TIM5_Configuration();

    p_rem_code_src = rt_malloc( sizeof(struct rem_codes_typedef)*6 );
    if( p_rem_code_src != RT_NULL)
    {
        rt_memset(p_rem_code_src,0, sizeof(struct rem_codes_typedef)*6 );

        /* ���������Ϣ */
        {
            int fd,size;
            char buf[7];/* �ļ���ȡ��ʱ���� #####\r\n */
            unsigned int i;
            unsigned short tmp;
            unsigned int read_index = 0;
            unsigned int EOF_flag = 1;

            printf("\r\ndecode remote codes");
            fd = open("/resource/remote.txt",O_RDONLY,0);
            if( fd>0 )
            {
                printf("\r/resource/remote.txt open succeed");
                while( EOF_flag )
                {
                    /* ��ȡ���� */
                    size = read(fd,buf,7);
                    if( (size == 7) && (buf[5]=='\r') && buf[6]=='\n' )
                    {
                        /* ת���õ��������ݳ��� */
                        tmp =   (buf[0]-'0')*10000
                                + (buf[1]-'0')*1000
                                + (buf[2]-'0')*100
                                + (buf[3]-'0')*10
                                + (buf[4]-'0');
                        if( tmp<100 )
                        {
                            unsigned int code_len = tmp;
                            p_rem_code_src[read_index].len = code_len;
                            /* ����������ȷ��� �Ϳ�ʼ���ļ���ȡ�������� */
                            for(i=0; i<code_len; i++)
                            {
                                size = read(fd,buf,7);
                                if( (size == 7) && (buf[5]=='\r') && buf[6]=='\n' )
                                {
                                    /* ת���õ��������� */
                                    tmp =   (buf[0]-'0')*10000
                                            + (buf[1]-'0')*1000
                                            + (buf[2]-'0')*100
                                            + (buf[3]-'0')*10
                                            + (buf[4]-'0');
                                    p_rem_code_src[read_index].rem_code[i] = tmp;
                                }
                            }
                            read_index++;
                        }
                    }
                    else
                    {
                        EOF_flag = 0;
                    }
                }//while( EOF_flag )

                /* �ж��Ƿ���ȷ������������ļ� */
                if ( p_rem_code_src[0].len > 0 && p_rem_code_src[0].len < remote_code_len_max )
                {
                    /* ���ù���ģʽΪ����ʶ��ģʽ */
                    rem_mode = 2;
                    printf("\r\ndecode succeed,The remote enable\r\n");
                }
                else
                {
                    /* ���ù���ģʽΪ����ʶ��ģʽ */
                    rem_mode = 0;
                    printf("\r\nrem_codes decode fail,The remote disable\r\n");
                }
            }
            else
            {
                printf("\rrem_codes /resource/remote.txt open fail! fd:%d\r\nThe remote disbale.\r\nplease run rem_study()\r\n",fd);
            }
            close(fd);
        }/* ���������Ϣ */

    }
    else
    {
        rem_mode = 0;
        printf("\r\nmalloc rem_codes[] fail!!!\r\nThe remote disable!");
    }
}

#include <rtgui/event.h>
void rem_encoder(struct rtgui_event_kbd * p)
{
    struct rtgui_event_kbd * p_kbd_event = p;

    /* ����Ƿ������ݱ����� */
    if( (rem_mode==2) && (rt_tick_get()>first_tick+10) && (rx_count > 0) )
    {
        /* �ֶ������һ�������� */
        rm_code[0] = 0;
        rx_count = 0;

        /* ƥ�䲶������� */
        {
            unsigned int tmp;
            unsigned int err_flag = 0;
            unsigned int rem_cmp_n = 6;

            /* ѭ��ƥ������KEY */
            while( rem_cmp_n )
            {
                unsigned int tmp2 = p_rem_code_src[ 6-rem_cmp_n ].len;
                //printf("\r\nrem_cmp_n:%d  tmp2:%d",rem_cmp_n,tmp2);
                if( tmp2 )
                {

                    for(tmp=0; tmp<tmp2; tmp++)
                    {
                        /* �жϲ������Ƿ���ƫ������Χ�� */
                        if( !( (rm_code[tmp] < p_rem_code_src[6-rem_cmp_n].rem_code[tmp]+remote_deviation)
                                && (rm_code[tmp] > p_rem_code_src[6-rem_cmp_n].rem_code[tmp]-remote_deviation)) )
                        {
                            err_flag = 1;
                        }
                    }
                }
                else
                {
                    err_flag = 1;
                    printf("\r\nThe rem codes len is 0.");
                }

                if( err_flag==0 )
                {
                    /* �Ա�ȫ�����ݷ��� */
                    printf("\r\nmatch key: %s",desc_key[6-rem_cmp_n]);
                    switch( rem_cmp_n )
                    {
                    case 6:
                        p_kbd_event->key  = RTGUIK_UP;
                        break;
                    case 5:
                        p_kbd_event->key  = RTGUIK_DOWN;
                        break;
                    case 4:
                        p_kbd_event->key  = RTGUIK_LEFT;
                        break;
                    case 3:
                        p_kbd_event->key  = RTGUIK_RIGHT;
                        break;
                    case 2:
                        p_kbd_event->key  = RTGUIK_RETURN;
                        break;
                    case 1:
                        p_kbd_event->key  = RTGUIK_HOME;
                        break;
                    default:
                        break;
                    }
                    rem_cmp_n = 0;
                }
                else
                {
                    /* �ԱȲ�����,����������,�Խ�����һ�ζԱ� */
                    err_flag = 0;
                    rem_cmp_n --;
                }

            }
        }
    }//����ң��ƥ��
}

/* remote isr */
void remote_isr(void)
{
    static unsigned int clr_flag = 1;
    unsigned int tick_now  = rt_tick_get();

    /* ����ң���½��� */
    if(TIM_GetITStatus(TIM5, TIM_IT_CC3) == SET)
    {
        switch( rem_mode )
        {
        case 0://δ����
            break;
        case 1://��ѧϰ
            if( (rx_count==0) || (rx_count>90) || (tick_now>first_tick+10) )
            {
                //��Ҫ��0
                rx_count = 0;
                clr_flag = 1;
            }
            if( rx_count < remote_code_len_max )
            {
                rm_code[rx_count++] = TIM_GetCapture3(TIM5);
            }
            break;
        case 2://��������
            if( (rx_count>90) || tick_now>first_tick+10 )
            {
                rx_count = 0;
                clr_flag = 1;
            }
            if(rx_count < remote_code_len_max )
            {
                rm_code[rx_count++] = TIM_GetCapture3(TIM5);
            }
            break;
        default:
            rem_mode = 0;//����ģʽΪδ����
            break;
        }
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
    }

    /* ����ң�������� */
    if(TIM_GetITStatus(TIM5, TIM_IT_CC4) == SET)
    {
        switch( rem_mode )
        {
        case 0://δ����
            break;
        case 1://��ѧϰ
            if( rx_count < remote_code_len_max )
            {
                rm_code[rx_count++] = TIM_GetCapture4(TIM5);
            }
            break;
        case 2://��������
            if( rx_count < remote_code_len_max )
            {
                rm_code[rx_count++] = TIM_GetCapture4(TIM5);
            }
            break;
        default:
            rem_mode = 0;//����ģʽΪδ����
            break;
        }
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
    }

    //����ʱ���
    first_tick = tick_now;
    //����Ƿ���Ҫ���ü�����
    if( clr_flag )
    {
        //���ü�����
        TIM_SetCounter(TIM5,0);
        clr_flag = 0;
    }
}

#include <finsh.h>
/* ��������ѧϰ���� */
int rem_study(void)
{
    unsigned int i;

    int fd,size;
    unsigned char tmp_buf[ (remote_code_len_max+1)*7 ];

    rem_mode = 1;
    rx_count = 0;
    printf("\r\nremote studing.....");
    fd = open("/resource/remote.txt",O_WRONLY | O_TRUNC,0);
    if( !(fd<0) )
    {
        printf("\r\n/resource/remote.txt create succeed.");
    }
    else
    {
        printf("\r/resource/remote.txt create fail.\r\nabort.");
        return -1;
    }

    /* ѧϰ6������ */
    for( i=0; i<6; i++)
    {
        unsigned int is_ok = 1;
        printf("\r\npress key %s",desc_key[i]);
        while( is_ok==1 )
        {
            if( (rem_mode==1) && (rt_tick_get()>first_tick+10) && (rx_count > 0) )
            {
                unsigned int a,b;
                unsigned char * p = tmp_buf;

                printf("\r\n%s",desc_key[i]);

                b = rx_count;
                p_rem_code_src[i].len = rx_count;

                /* TIM disable counter */
                TIM_Cmd(TIM5, DISABLE);
                /* disable the CC3 and CC4 Interrupt Request */
                TIM_ITConfig(TIM5, TIM_IT_CC3, DISABLE);
                TIM_ITConfig(TIM5, TIM_IT_CC4, DISABLE);

                p[0] = rx_count / 10000 +'0';
                rx_count = rx_count % 10000;
                p[1] = rx_count / 1000  +'0';
                rx_count = rx_count % 1000;
                p[2] = rx_count / 100   +'0';
                rx_count = rx_count % 100;
                p[3] = rx_count / 10    +'0';
                rx_count = rx_count % 10;
                p[4] = rx_count        +'0';
                rx_count = 0;
                p[5] = '\r';
                p[6] = '\n';
                p += 7;

                rm_code[0] = 0;

                for( a=0; a<b; a++)
                {
                    /* �ѵ�ǰ����ֱ��д����Ʒ���� */
                    p_rem_code_src[i].rem_code[a] = rm_code[a];

                    /* Ȼ��ת�����ı���ʽ #####\r\n */
                    p[0] = rm_code[a] / 10000 +'0';
                    rm_code[a] = rm_code[a] % 10000;
                    p[1] = rm_code[a] / 1000  +'0';
                    rm_code[a] = rm_code[a] % 1000;
                    p[2] = rm_code[a] / 100   +'0';
                    rm_code[a] = rm_code[a] % 100;
                    p[3] = rm_code[a] / 10   +'0';
                    rm_code[a] = rm_code[a] % 10;
                    p[4] = rm_code[a]        +'0';
                    p[5] = '\r';
                    p[6] = '\n';
                    p += 7;
                }
                size = write(fd,(char*)tmp_buf,(b+1)*7 );
                if( size==((b+1)*7) )
                {
                    printf(" file write succeed!");
                    is_ok++;
                    rt_thread_delay( 2 );

                    /* ���´� TIM5 ���в��� */
                    TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
                    TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
                    /* TIM ENABLE counter */
                    TIM_Cmd(TIM5, ENABLE);
                    /* ENABLE the CC3 and CC4 Interrupt Request */
                    TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);
                    TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);
                }
                else
                {
                    printf(" file write fail.\r\nabort.");
                    return -1;
                }
            }
            rt_thread_delay(1);
        }//while( is_ok==1 )
    }//for( i=0; i<6; i++)
    close(fd);
    printf("\r\nremote study complete.The remote enable.\r\n");
    rem_mode = 2;
    return 0;
}
FINSH_FUNCTION_EXPORT(rem_study, rem_study);
