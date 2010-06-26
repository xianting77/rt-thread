/*************************************************************************************
 *
 *	File name:	sep4020.h
 *
 * Change Logs:
 * Date           Author       Notes	        
 * 2010-03-17     zchong      first version		      	
 **************************************************************************************/

#ifndef __SEP4020_H__
#define __SEP4020_H__

/*************************************
	macros for INTC
*************************************/

/*interrupt resources */

#define     INTGLOBAL		        32

#define		INT_RTC					31		        
#define		INT_DMAC				30	                
#define		INT_EMI					29              
#define		INT_MAC		  			28                  
#define		INT_TIMER1				27                  
#define		INT_TIMER2				26              
#define 	INT_TIMER3 				25              
#define		INT_UART0				24              
#define		INT_UART1				23              
#define		INT_UART2				22              
#define		INT_UART3			    21              
#define		INT_PWM					20	                
#define		INT_LCDC	   			19	            
#define		INT_I2S			    	18              
#define		INT_SSI 				17	            
#define		INT_NOTUSED16			16
#define		INT_USB 				15              
#define		INT_SMC0				14				
#define		INT_SMC1				13              
#define		INT_SDIO				12              
#define		INT_EXINT10				11              
#define		INT_EXINT9 				10              
#define		INT_EXINT8 				9               
#define		INT_EXINT7 				8               
#define		INT_EXINT6 				7               
#define		INT_EXINT5 				6               
#define		INT_EXINT4 				5               
#define		INT_EXINT3 				4               
#define		INT_EXINT2 				3               
#define		INT_EXINT1 				2               
#define		INT_EXINT0				1
#define		INT_NOTUSED0			0

/******************************************************************************************
 *  INTCģ��
 *  ��ַ: 0x10000000
 ******************************************************************************************/

#define INTC_IER        (*(volatile unsigned *)0x10000000)  /* IRQ interrupt enable register    */
#define INTC_IMR        (*(volatile unsigned *)0x10000008)  /* IRQ interrupt mask register      */
#define INTC_IFR        (*(volatile unsigned *)0x10000010)  /* IRQ���ǿ���жϼĴ���    */
#define INTC_IRSR       (*(volatile unsigned *)0x10000018)  /* IRQδ�����ж�״̬�Ĵ���  */
#define INTC_ISR        (*(volatile unsigned *)0x10000020)  /* IRQ�ж�״̬�Ĵ���        */
#define INTC_IMSR       (*(volatile unsigned *)0x10000028)  /* IRQ�����ж�״̬�Ĵ���    */
#define INTC_IFSR       (*(volatile unsigned *)0x10000030)  /* IRQ�ж�����״̬�Ĵ���    */
#define INTC_FIER       (*(volatile unsigned *)0x100000C0)  /* FIQ�ж�����Ĵ���        */
#define INTC_FIMR       (*(volatile unsigned *)0x100000C4)  /* FIQ�ж����μĴ���        */
#define INTC_FIFR       (*(volatile unsigned *)0x100000C8)  /* FIQ���ǿ���жϼĴ���    */
#define INTC_FIRSR      (*(volatile unsigned *)0x100000CC)  /* FIQδ�����ж�״̬�Ĵ���  */
#define INTC_FISR       (*(volatile unsigned *)0x100000D0)  /* FIQ�ж�״̬�Ĵ���        */
#define INTC_FIFSR      (*(volatile unsigned *)0x100000D4)  /* FIQ�ж�����״̬�Ĵ���    */
#define INTC_IPLR       (*(volatile unsigned *)0x100000D8)  /* IRQ�ж����ȼ��Ĵ���      */
#define INTC_ICR1       (*(volatile unsigned *)0x100000DC)  /* IRQ�ڲ��ж����ȼ����ƼĴ���1 */
#define INTC_ICR2       (*(volatile unsigned *)0x100000E0)  /* IRQ�ڲ��ж����ȼ����ƼĴ���2 */
#define INTC_EXICR1     (*(volatile unsigned *)0x100000E4)  /* IRQ�ⲿ�ж����ȼ����ƼĴ���1 */
#define INTC_EXICR2     (*(volatile unsigned *)0x100000E8)  /* IRQ�ⲿ�ж����ȼ����ƼĴ���2 */


/******************************************************************************************
 *  PMCģ��
 *  ��ַ: 0x10001000
 ******************************************************************************************/

#define PMC_PLTR        (*(volatile unsigned *)0x10001000)  /* PLL���ȶ�����ʱ�� */
#define PMC_PMCR        (*(volatile unsigned *)0x10001004)  /* ϵͳ��ʱ��PLL�Ŀ��ƼĴ��� */
#define PMC_PUCR        (*(volatile unsigned *)0x10001008)  /* USBʱ��PLL�Ŀ��ƼĴ��� */
#define PMC_PCSR        (*(volatile unsigned *)0x1000100C)  /* �ڲ�ģ��ʱ��Դ�����Ŀ��ƼĴ��� */
#define PMC_PDSLOW      (*(volatile unsigned *)0x10001010)  /* SLOW״̬��ʱ�ӵķ�Ƶ���� */
#define PMC_PMDR        (*(volatile unsigned *)0x10001014)  /* оƬ����ģʽ�Ĵ��� */
#define PMC_RCTR        (*(volatile unsigned *)0x10001018)  /* Reset���ƼĴ��� */
#define PMC_CLRWAKUP    (*(volatile unsigned *)0x1000101C)  /* WakeUp����Ĵ��� */


/******************************************************************************************
 *  RTCģ��
 *  ��ַ: 0x10002000
 ******************************************************************************************/

#define RTC_STA_YMD         (*(volatile unsigned *)0x10002000)  /* ��, ��, �ռ����Ĵ��� */
#define RTC_STA_HMS         (*(volatile unsigned *)0x10002004)  /* Сʱ, ����, ��Ĵ��� */
#define RTC_ALARM_ALL       (*(volatile unsigned *)0x10002008)  /* ��ʱ��, ��, ʱ, �ּĴ��� */
#define RTC_CTR             (*(volatile unsigned *)0x1000200C)  /* ���ƼĴ��� */
#define RTC_INT_EN          (*(volatile unsigned *)0x10002010)  /* �ж�ʹ�ܼĴ��� */
#define RTC_INT_STS         (*(volatile unsigned *)0x10002014)  /* �ж�״̬�Ĵ��� */
#define RTC_SAMP            (*(volatile unsigned *)0x10002018)  /* �������ڼĴ��� */
#define RTC_WD_CNT          (*(volatile unsigned *)0x1000201C)  /* Watch-Dog����ֵ�Ĵ��� */
#define RTC_WD_SEV          (*(volatile unsigned *)0x10002020)  /* Watch-Dog����Ĵ��� */ 
#define RTC_CONFIG_CHECK    (*(volatile unsigned *)0x10002024)  /* ����ʱ��ȷ�ϼĴ��� (������ʱ��֮ǰ��д0xaaaaaaaa) */
#define RTC_SOFTRESET       (*(volatile unsigned *)0x10002028)  /* �����λ���ƼĴ���, 4020�б�ȥ���� */
#define RTC_KEY0            (*(volatile unsigned *)0x1000202C)  /* ��Կ�Ĵ���0, 4020��ֻ����һ���Ĵ��� */
#define RTC_KEY1            (*(volatile unsigned *)0x10002030)  /* ��Կ�Ĵ���1 */
#define RTC_KEY2            (*(volatile unsigned *)0x10002034)  /* ��Կ�Ĵ���2 */
#define RTC_KEY3            (*(volatile unsigned *)0x10002038)  /* ��Կ�Ĵ���3 */
#define RTC_KEY4            (*(volatile unsigned *)0x1000203C)  /* ��Կ�Ĵ���4 */
#define RTC_KEY5            (*(volatile unsigned *)0x10002040)  /* ��Կ�Ĵ���5 */


/******************************************************************************************
 *  TIMERģ��
 *  ��ַ: 0x10003000
 ******************************************************************************************/

#define TIMER_T1LCR         (*(volatile unsigned *)0x10003000)  /* ͨ��1���ؼ����Ĵ��� */
#define TIMER_T1CCR         (*(volatile unsigned *)0x10003004)  /* ͨ��1��ǰ����ֵ�Ĵ��� */
#define TIMER_T1CR          (*(volatile unsigned *)0x10003008)  /* ͨ��1���ƼĴ��� */
#define TIMER_T1ISCR        (*(volatile unsigned *)0x1000300C)  /* ͨ��1�ж�״̬����Ĵ��� */
#define TIMER_T1IMSR        (*(volatile unsigned *)0x10003010)  /* ͨ��1�ж�����״̬�Ĵ��� */
#define TIMER_T2LCR         (*(volatile unsigned *)0x10003020)  /* ͨ��2���ؼ����Ĵ��� */
#define TIMER_T2CCR         (*(volatile unsigned *)0x10003024)  /* ͨ��2��ǰ����ֵ�Ĵ��� */
#define TIMER_T2CR          (*(volatile unsigned *)0x10003028)  /* ͨ��2���ƼĴ��� */
#define TIMER_T2ISCR        (*(volatile unsigned *)0x1000302C)  /* ͨ��2�ж�״̬����Ĵ��� */
#define TIMER_T2IMSR        (*(volatile unsigned *)0x10003030)  /* ͨ��2�ж�����״̬�Ĵ��� */
#define TIMER_T3LCR         (*(volatile unsigned *)0x10003040)  /* ͨ��3���ؼ����Ĵ��� */
#define TIMER_T3CCR         (*(volatile unsigned *)0x10003044)  /* ͨ��3��ǰ����ֵ�Ĵ��� */
#define TIMER_T3CR          (*(volatile unsigned *)0x10003048)  /* ͨ��3���ƼĴ��� */
#define TIMER_T3ISCR        (*(volatile unsigned *)0x1000304C)  /* ͨ��3�ж�״̬����Ĵ��� */
#define TIMER_T3IMSR        (*(volatile unsigned *)0x10003050)  /* ͨ��3�ж�����״̬�Ĵ��� */
#define TIMER_T3CAPR        (*(volatile unsigned *)0x10003054)  /* ͨ��3����Ĵ��� */
#define TIMER_T4LCR         (*(volatile unsigned *)0x10003060)  /* ͨ��4���ؼ����Ĵ��� */
#define TIMER_T4CCR         (*(volatile unsigned *)0x10003064)  /* ͨ��4��ǰ����ֵ�Ĵ��� */
#define TIMER_T4CR          (*(volatile unsigned *)0x10003068)  /* ͨ��4���ƼĴ��� */
#define TIMER_T4ISCR        (*(volatile unsigned *)0x1000306C)  /* ͨ��4�ж�״̬����Ĵ��� */
#define TIMER_T4IMSR        (*(volatile unsigned *)0x10003070)  /* ͨ��4�ж�����״̬�Ĵ��� */
#define TIMER_T4CAPR        (*(volatile unsigned *)0x10003074)  /* ͨ��4����Ĵ��� */
#define TIMER_T5LCR         (*(volatile unsigned *)0x10003080)  /* ͨ��5���ؼ����Ĵ��� */
#define TIMER_T5CCR         (*(volatile unsigned *)0x10003084)  /* ͨ��5��ǰ����ֵ�Ĵ��� */
#define TIMER_T5CR          (*(volatile unsigned *)0x10003088)  /* ͨ��5���ƼĴ��� */
#define TIMER_T5ISCR        (*(volatile unsigned *)0x1000308C)  /* ͨ��5�ж�״̬����Ĵ��� */
#define TIMER_T5IMSR        (*(volatile unsigned *)0x10003090)  /* ͨ��5�ж�����״̬�Ĵ��� */
#define TIMER_T5CAPR        (*(volatile unsigned *)0x10003094)  /* ͨ��5����Ĵ��� */
#define TIMER_T6LCR         (*(volatile unsigned *)0x100030A0)  /* ͨ��6���ؼ����Ĵ��� */
#define TIMER_T6CCR         (*(volatile unsigned *)0x100030A4)  /* ͨ��6��ǰ����ֵ�Ĵ��� */
#define TIMER_T6CR          (*(volatile unsigned *)0x100030A8)  /* ͨ��6���ƼĴ��� */
#define TIMER_T6ISCR        (*(volatile unsigned *)0x100030AC)  /* ͨ��6�ж�״̬����Ĵ��� */
#define TIMER_T6IMSR        (*(volatile unsigned *)0x100030B0)  /* ͨ��6�ж�����״̬�Ĵ��� */
#define TIMER_T6CAPR        (*(volatile unsigned *)0x100030B4)  /* ͨ��6����Ĵ��� */
#define TIMER_T7LCR         (*(volatile unsigned *)0x100030C0)  /* ͨ��7���ؼ����Ĵ��� */
#define TIMER_T7CCR         (*(volatile unsigned *)0x100030C4)  /* ͨ��7��ǰ����ֵ�Ĵ��� */
#define TIMER_T7CR          (*(volatile unsigned *)0x100030C8)  /* ͨ��7���ƼĴ��� */
#define TIMER_T7ISCR        (*(volatile unsigned *)0x100030CC)  /* ͨ��7�ж�״̬����Ĵ��� */
#define TIMER_T7IMSR        (*(volatile unsigned *)0x100030D0)  /* ͨ��7�ж�����״̬�Ĵ��� */
#define TIMER_T8LCR         (*(volatile unsigned *)0x100030E0)  /* ͨ��8���ؼ����Ĵ��� */
#define TIMER_T8CCR         (*(volatile unsigned *)0x100030E4)  /* ͨ��8��ǰ����ֵ�Ĵ��� */
#define TIMER_T8CR          (*(volatile unsigned *)0x100030E8)  /* ͨ��8���ƼĴ��� */
#define TIMER_T8ISCR        (*(volatile unsigned *)0x100030EC)  /* ͨ��8�ж�״̬����Ĵ��� */
#define TIMER_T8IMSR        (*(volatile unsigned *)0x100030F0)  /* ͨ��8�ж�����״̬�Ĵ��� */
#define TIMER_T9LCR         (*(volatile unsigned *)0x10003100)  /* ͨ��9���ؼ����Ĵ��� */
#define TIMER_T9CCR         (*(volatile unsigned *)0x10003104)  /* ͨ��9��ǰ����ֵ�Ĵ��� */
#define TIMER_T9CR          (*(volatile unsigned *)0x10003108)  /* ͨ��9���ƼĴ��� */
#define TIMER_T9ISCR        (*(volatile unsigned *)0x1000310C)  /* ͨ��9�ж�״̬����Ĵ��� */
#define TIMER_T9IMSR        (*(volatile unsigned *)0x10003110)  /* ͨ��9�ж�����״̬�Ĵ��� */
#define TIMER_T10LCR        (*(volatile unsigned *)0x10003120)  /* ͨ��10���ؼ����Ĵ��� */
#define TIMER_T10CCR        (*(volatile unsigned *)0x10003124)  /* ͨ��10��ǰ����ֵ�Ĵ��� */
#define TIMER_T10CR         (*(volatile unsigned *)0x10003128)  /* ͨ��10���ƼĴ��� */
#define TIMER_T10ISCR       (*(volatile unsigned *)0x1000312C)  /* ͨ��10�ж�״̬����Ĵ��� */
#define TIMER_T10IMSR       (*(volatile unsigned *)0x10003130)  /* ͨ��10�ж�����״̬�Ĵ��� */
#define TIMER_TIMSR         (*(volatile unsigned *)0x10003140)  /* TIMER�ж�����״̬�Ĵ��� */
#define TIMER_TISCR         (*(volatile unsigned *)0x10003144)  /* TIMER�ж�״̬����Ĵ��� */
#define TIMER_TISR          (*(volatile unsigned *)0x10003148)  /* TIMER�ж�״̬�Ĵ��� */


/******************************************************************************************
 *  PWMģ��
 *  ��ַ: 0x10004000
 ******************************************************************************************/

#define PWM1_CTRL           (*(volatile unsigned *)0x10004000)  /* PWM1���ƼĴ��� */
#define PWM1_DIV            (*(volatile unsigned *)0x10004004)  /* PWM1��Ƶ�Ĵ��� */
#define PWM1_PERIOD         (*(volatile unsigned *)0x10004008)  /* PWM1���ڼĴ��� */
#define PWM1_DATA           (*(volatile unsigned *)0x1000400C)  /* PWM1���ݼĴ��� */
#define PWM1_CNT            (*(volatile unsigned *)0x10004010)  /* PWM1�����Ĵ��� */
#define PWM1_STATUS         (*(volatile unsigned *)0x10004014)  /* PWM1״̬�Ĵ��� */
#define PWM2_CTRL           (*(volatile unsigned *)0x10004020)  /* PWM2���ƼĴ��� */
#define PWM2_DIV            (*(volatile unsigned *)0x10004024)  /* PWM2��Ƶ�Ĵ��� */
#define PWM2_PERIOD         (*(volatile unsigned *)0x10004028)  /* PWM2���ڼĴ��� */
#define PWM2_DATA           (*(volatile unsigned *)0x1000402C)  /* PWM2���ݼĴ��� */
#define PWM2_CNT            (*(volatile unsigned *)0x10004030)  /* PWM2�����Ĵ��� */
#define PWM2_STATUS         (*(volatile unsigned *)0x10004034)  /* PWM2״̬�Ĵ��� */
#define PWM3_CTRL           (*(volatile unsigned *)0x10004040)  /* PWM3���ƼĴ��� */
#define PWM3_DIV            (*(volatile unsigned *)0x10004044)  /* PWM3��Ƶ�Ĵ��� */
#define PWM3_PERIOD         (*(volatile unsigned *)0x10004048)  /* PWM3���ڼĴ��� */
#define PWM3_DATA           (*(volatile unsigned *)0x1000404C)  /* PWM3���ݼĴ��� */
#define PWM3_CNT            (*(volatile unsigned *)0x10004050)  /* PWM3�����Ĵ��� */
#define PWM3_STATUS         (*(volatile unsigned *)0x10004054)  /* PWM3״̬�Ĵ��� */
#define PWM4_CTRL           (*(volatile unsigned *)0x10004060)  /* PWM4���ƼĴ��� */
#define PWM4_DIV            (*(volatile unsigned *)0x10004064)  /* PWM4��Ƶ�Ĵ��� */
#define PWM4_PERIOD         (*(volatile unsigned *)0x10004068)  /* PWM4���ڼĴ��� */
#define PWM4_DATA           (*(volatile unsigned *)0x1000406C)  /* PWM4���ݼĴ��� */
#define PWM4_CNT            (*(volatile unsigned *)0x10004070)  /* PWM4�����Ĵ��� */
#define PWM4_STATUS         (*(volatile unsigned *)0x10004074)  /* PWM4״̬�Ĵ��� */
#define PWM_INTMASK         (*(volatile unsigned *)0x10004080)  /* PWM�ж����μĴ��� */
#define PWM_INT             (*(volatile unsigned *)0x10004084)  /* PWM�жϼĴ��� */
#define PWM_ENABLE          (*(volatile unsigned *)0x10004088)    /* PWMʹ�ܼĴ��� */


/******************************************************************************************
 *  UART0ģ��
 *  ��ַ: 0x10005000
 ******************************************************************************************/
#define UART0BASE            ((volatile unsigned *)0x10005000)
#define UART0_DLBL          (*(volatile unsigned char*)0x10005000)  /* ���������õͰ�λ�Ĵ��� */
#define UART0_RXFIFO        (*(volatile unsigned char*)0x10005000)  /* ����FIFO */
#define UART0_TXFIFO        (*(volatile unsigned char*)0x10005000)  /* ����FIFO */
#define UART0_DLBH          (*(volatile unsigned char*)0x10005004)  /* ���������ø߰�λ�Ĵ��� */
#define UART0_IER           (*(volatile unsigned char*)0x10005004)  /* �ж�ʹ�ܼĴ��� */
#define UART0_IIR           (*(volatile unsigned *)0x10005008)  /* �ж�ʶ��Ĵ��� */
#define UART0_FCR           (*(volatile unsigned *)0x10005008)  /* FIFO���ƼĴ��� */
#define UART0_LCR           (*(volatile unsigned *)0x1000500C)  /* �п��ƼĴ��� */
#define UART0_MCR           (*(volatile unsigned *)0x10005010)  /* Modem���ƼĴ��� */
#define UART0_LSR           (*(volatile unsigned *)0x10005014)  /* ��״̬�Ĵ��� */
#define UART0_MSR           (*(volatile unsigned *)0x10005018)  /* Modem״̬�Ĵ��� */


/******************************************************************************************
 *  UART1ģ��
 *  ��ַ: 0x10006000
 ******************************************************************************************/
 #define UART1BASE            ((volatile unsigned *)0x10006000)
#define UART1_DLBL          (*(volatile unsigned *)0x10006000)  /* ���������õͰ�λ�Ĵ��� */
#define UART1_RXFIFO        (*(volatile unsigned *)0x10006000)  /* ����FIFO */
#define UART1_TXFIFO        (*(volatile unsigned *)0x10006000)  /* ����FIFO */
#define UART1_DLBH          (*(volatile unsigned *)0x10006004)  /* ���������ø߰�λ�Ĵ��� */
#define UART1_IER           (*(volatile unsigned *)0x10006004)  /* �ж�ʹ�ܼĴ��� */
#define UART1_IIR           (*(volatile unsigned *)0x10006008)  /* �ж�ʶ��Ĵ��� */
#define UART1_FCR           (*(volatile unsigned *)0x10006008)  /* FIFO���ƼĴ��� */
#define UART1_LCR           (*(volatile unsigned *)0x1000600C)  /* �п��ƼĴ��� */
#define UART1_MCR           (*(volatile unsigned *)0x10006010)  /* Modem���ƼĴ��� */
#define UART1_LSR           (*(volatile unsigned *)0x10006014)  /* ��״̬�Ĵ��� */
#define UART1_MSR           (*(volatile unsigned *)0x10006018)  /* Modem״̬�Ĵ��� */


/******************************************************************************************
 *  UART2ģ��
 *  ��ַ: 0x10007000
 ******************************************************************************************/
#define UART2BASE            ((volatile unsigned *)0x10007000)
#define UART2_DLBL          (*(volatile unsigned *)0x10007000)  /* ���������õͰ�λ�Ĵ��� */
#define UART2_RXFIFO        (*(volatile unsigned *)0x10007000)  /* ����FIFO */
#define UART2_TXFIFO        (*(volatile unsigned *)0x10007000)  /* ����FIFO */
#define UART2_DLBH          (*(volatile unsigned *)0x10007004)  /* ���������ø߰�λ�Ĵ��� */
#define UART2_IER           (*(volatile unsigned *)0x10007004)  /* �ж�ʹ�ܼĴ��� */
#define UART2_IIR           (*(volatile unsigned *)0x10007008)  /* �ж�ʶ��Ĵ��� */
#define UART2_FCR           (*(volatile unsigned *)0x10007008)  /* FIFO���ƼĴ��� */
#define UART2_LCR           (*(volatile unsigned *)0x1000700C)  /* �п��ƼĴ��� */
#define UART2_MCR           (*(volatile unsigned *)0x10007010)  /* Modem���ƼĴ��� */
#define UART2_LSR           (*(volatile unsigned *)0x10007014)  /* ��״̬�Ĵ��� */
#define UART2_MSR           (*(volatile unsigned *)0x10007018)  /* Modem״̬�Ĵ��� */


/******************************************************************************************
 *  UART3ģ��
 *  ��ַ: 0x10008000
 ******************************************************************************************/
#define UART3BASE            ((volatile unsigned *)0x10008000)
#define UART3_DLBL          (*(volatile unsigned *)0x10008000)  /* ���������õͰ�λ�Ĵ��� */
#define UART3_RXFIFO        (*(volatile unsigned *)0x10008000)  /* ����FIFO */
#define UART3_TXFIFO        (*(volatile unsigned *)0x10008000)  /* ����FIFO */
#define UART3_DLBH          (*(volatile unsigned *)0x10008004)  /* ���������ø߰�λ�Ĵ��� */
#define UART3_IER           (*(volatile unsigned *)0x10008004)  /* �ж�ʹ�ܼĴ��� */
#define UART3_IIR           (*(volatile unsigned *)0x10008008)  /* �ж�ʶ��Ĵ��� */
#define UART3_FCR           (*(volatile unsigned *)0x10008008)  /* FIFO���ƼĴ��� */
#define UART3_LCR           (*(volatile unsigned *)0x1000800C)  /* �п��ƼĴ��� */
#define UART3_MCR           (*(volatile unsigned *)0x10008010)  /* Modem���ƼĴ��� */
#define UART3_LSR           (*(volatile unsigned *)0x10008014)  /* ��״̬�Ĵ��� */
#define UART3_MSR           (*(volatile unsigned *)0x10008018)  /* Modem״̬�Ĵ��� */


/******************************************************************************************
 *  SSIģ��
 *  ��ַ: 0x10009000
 ******************************************************************************************/

#define SSI_CONTROL0        (*(volatile unsigned *)0x10009000)  /* ���ƼĴ���0 */
#define SSI_CONTROL1        (*(volatile unsigned *)0x10009004)  /* ���ƼĴ���1 */
#define SSI_SSIENR          (*(volatile unsigned *)0x10009008)  /* SSIʹ�ܼĴ��� */
#define SSI_MWCR            (*(volatile unsigned *)0x1000900C)  /* Microwire���ƼĴ��� */
#define SSI_SER             (*(volatile unsigned *)0x10009010)  /* ���豸ʹ�ܼĴ��� */
#define SSI_BAUDR           (*(volatile unsigned *)0x10009014)  /* ���������üĴ��� */
#define SSI_TXFTLR          (*(volatile unsigned *)0x10009018)  /* ����FIFO��ֵ�Ĵ��� */
#define SSI_RXFTLR          (*(volatile unsigned *)0x1000901C)  /* ����FIFO��ֵ�Ĵ��� */
#define SSI_TXFLR           (*(volatile unsigned *)0x10009020)  /* ����FIFO״̬�Ĵ��� */
#define SSI_RXFLR           (*(volatile unsigned *)0x10009024)  /* ����FIFO״̬�Ĵ��� */
#define SSI_SR              (*(volatile unsigned *)0x10009028)  /* ״̬�Ĵ��� */
#define SSI_IMR             (*(volatile unsigned *)0x1000902C)  /* �ж����μĴ��� */
#define SSI_ISR             (*(volatile unsigned *)0x10009030)  /* �ж�����״̬�Ĵ��� */
#define SSI_RISR            (*(volatile unsigned *)0x10009034)  /* �ж�ԭʼ״̬�Ĵ��� */
#define SSI_TXOICR          (*(volatile unsigned *)0x10009038)  /* ����FIFO�����ж�����Ĵ��� */
#define SSI_RXOICR          (*(volatile unsigned *)0x1000903C)  /* ����FIFO�����ж�����Ĵ��� */
#define SSI_RXUICR          (*(volatile unsigned *)0x10009040)  /* ����FIFO�����ж�����Ĵ��� */
#define SSI_ICR             (*(volatile unsigned *)0x1000902C)  /* �ж�����Ĵ��� */
#define SSI_DMACR           (*(volatile unsigned *)0x1000904C)  /* DMA���ƼĴ��� */
#define SSI_DMATDLR         (*(volatile unsigned *)0x10009050)  /* DMA����״̬�Ĵ��� */
#define SSI_DMARDLR         (*(volatile unsigned *)0x10009054)  /* DMA����״̬�Ĵ��� */
#define SSI_DR              (*(volatile unsigned *)0x10009060)  /* ���ݼĴ��� */


/******************************************************************************************
 *  I2Sģ��
 *  ��ַ: 0x1000A000
 ******************************************************************************************/

#define I2S_CTRL            (*(volatile unsigned *)0x1000A000)  /* I2S���ƼĴ��� */
#define I2S_DATA            (*(volatile unsigned *)0x1000A004)  /* I2S���ݼĴ��� */
#define I2S_INT             (*(volatile unsigned *)0x1000A008)  /* I2S�жϼĴ��� */
#define I2S_STATUS          (*(volatile unsigned *)0x1000A00C)  /* I2S״̬�Ĵ��� */


/******************************************************************************************
 *  GPIOģ��
 *  ��ַ: 0x1000F000
 ******************************************************************************************/

#define GPIO_DBCLK_DIV      (*(volatile unsigned *)0x1000F000)  /* ȥë�̲���ʱ�ӷ�Ƶ�����üĴ��� */
#define GPIO_PORTA_DIR      (*(volatile unsigned *)0x1000F004)  /* A��˿���������������üĴ��� */
#define GPIO_PORTA_SEL      (*(volatile unsigned *)0x1000F008)  /* A��˿�ͨ����;ѡ�����üĴ��� */
#define GPIO_PORTA_INCTL    (*(volatile unsigned *)0x1000F00C)  /* A��˿�ͨ����;����ʱ�������üĴ��� */
#define GPIO_PORTA_INTRCTL  (*(volatile unsigned *)0x1000F010)  /* A��˿��жϴ����������üĴ��� */
#define GPIO_PORTA_INTRCLR  (*(volatile unsigned *)0x1000F014)  /* A��˿�ͨ����;�ж�������üĴ��� */
#define GPIO_PORTA_DATA     (*(volatile unsigned *)0x1000F018)  /* A��˿�ͨ����;�������üĴ��� */
#define GPIO_PORTB_DIR      (*(volatile unsigned *)0x1000F01C)  /* B��˿���������������üĴ��� */
#define GPIO_PORTB_SEL      (*(volatile unsigned *)0x1000F020)  /* B��˿�ͨ����;ѡ�����üĴ��� */
#define GPIO_PORTB_DATA     (*(volatile unsigned *)0x1000F024)  /* B��˿�ͨ����;�������üĴ��� */
#define GPIO_PORTC_DIR      (*(volatile unsigned *)0x1000F028)  /* C��˿���������������üĴ��� */
#define GPIO_PORTC_SEL      (*(volatile unsigned *)0x1000F02C)  /* C��˿�ͨ����;ѡ�����üĴ��� */
#define GPIO_PORTC_DATA     (*(volatile unsigned *)0x1000F030)  /* C��˿�ͨ����;�������üĴ��� */
#define GPIO_PORTD_DIR      (*(volatile unsigned *)0x1000F034)  /* D��˿���������������üĴ��� */
#define GPIO_PORTD_SEL      (*(volatile unsigned *)0x1000F038)  /* D��˿�ͨ����;ѡ�����üĴ��� */
#define GPIO_PORTD_SPECII   (*(volatile unsigned *)0x1000F03C)  /* D��˿�ר����;2ѡ�����üĴ��� */
#define GPIO_PORTD_DATA     (*(volatile unsigned *)0x1000F040)  /* D��˿�ͨ����;�������üĴ��� */
#define GPIO_PORTE_DIR      (*(volatile unsigned *)0x1000F044)  /* E��˿���������������üĴ��� */
#define GPIO_PORTE_SEL      (*(volatile unsigned *)0x1000F048)  /* E��˿�ͨ����;ѡ�����üĴ��� */
#define GPIO_PORTE_DATA     (*(volatile unsigned *)0x1000F04C)  /* E��˿�ͨ����;�������üĴ��� */
#define GPIO_PORTF_DIR      (*(volatile unsigned *)0x1000F050)  /* F��˿���������������üĴ��� */
#define GPIO_PORTF_SEL      (*(volatile unsigned *)0x1000F054)  /* F��˿�ͨ����;ѡ�����üĴ��� */
#define GPIO_PORTF_INCTL    (*(volatile unsigned *)0x1000F058)  /* F��˿�ͨ����;����ʱ�������üĴ��� */
#define GPIO_PORTF_INTRCTL  (*(volatile unsigned *)0x1000F05C)  /* F��˿��жϴ����������üĴ��� */
#define GPIO_PORTF_INTRCLR  (*(volatile unsigned *)0x1000F060)  /* F��˿�ͨ����;�ж�������üĴ��� */
#define GPIO_PORTF_DATA     (*(volatile unsigned *)0x1000F064)  /* F��˿�ͨ����;�������üĴ��� */
#define GPIO_PORTG_DIR      (*(volatile unsigned *)0x1000F068)  /* G��˿���������������üĴ��� */
#define GPIO_PORTG_SEL      (*(volatile unsigned *)0x1000F06C)  /* G��˿�ͨ����;ѡ�����üĴ��� */
#define GPIO_PORTG_DATA     (*(volatile unsigned *)0x1000F070)  /* G��˿�ͨ����;�������üĴ��� */
#define GPIO_PORTH_DIR      (*(volatile unsigned *)0x1000F074)  /* H��˿���������������üĴ��� */
#define GPIO_PORTH_SEL      (*(volatile unsigned *)0x1000F078)  /* H��˿�ͨ����;ѡ�����üĴ��� */
#define GPIO_PORTH_DATA     (*(volatile unsigned *)0x1000F07c)  /* H��˿�ͨ����;�������üĴ��� */
#define GPIO_PORTI_DIR      (*(volatile unsigned *)0x1000F080)  /* I��˿���������������üĴ��� */
#define GPIO_PORTI_SEL      (*(volatile unsigned *)0x1000F084)  /* I��˿�ͨ����;ѡ�����üĴ��� */
#define GPIO_PORTI_DATA     (*(volatile unsigned *)0x1000F088)  /* I��˿�ͨ����;�������üĴ��� */


/******************************************************************************************
 *  SMC0ģ��
 *  ��ַ: 0x1000C000
 ******************************************************************************************/

#define SMC0_CTRL           (*(volatile unsigned *)0x1000C000)  /* SMC0���ƼĴ��� */
#define SMC0_INT            (*(volatile unsigned *)0x1000C004)  /* SMC0�жϼĴ��� */
#define SMC0_FD             (*(volatile unsigned *)0x1000C008)  /* SMC0������Ԫʱ��Ĵ��� */
#define SMC0_CT             (*(volatile unsigned *)0x1000C00C)  /* SMC0�ַ�����ʱ��Ĵ��� */
#define SMC0_BT             (*(volatile unsigned *)0x1000C010)  /* SMC0�鴫��ʱ��Ĵ��� */


/******************************************************************************************
 *  SMC1ģ��
 *  ��ַ: 0x1000D000
 ******************************************************************************************/

#define SMC1_CTRL           (*(volatile unsigned *)0x1000D000)  /* SMC1���ƼĴ��� */
#define SMC1_INT            (*(volatile unsigned *)0x1000D004)  /* SMC1�жϼĴ��� */
#define SMC1_FD             (*(volatile unsigned *)0x1000D008)  /* SMC1������Ԫʱ��Ĵ��� */
#define SMC1_CT             (*(volatile unsigned *)0x1000D00C)  /* SMC1�ַ�����ʱ��Ĵ��� */
#define SMC1_BT             (*(volatile unsigned *)0x1000D010)  /* SMC1�鴫��ʱ��Ĵ��� */


/******************************************************************************************
 *  USBDģ��
 *  ��ַ: 0x1000E000
 ******************************************************************************************/

#define USBD_PROTOCOLINTR           (*(volatile unsigned *)0x1000E000)  /* USBЭ���жϼĴ��� */
#define USBD_INTRMASK               (*(volatile unsigned *)0x1000E004)  /* USB�ж����μĴ��� */
#define USBD_INTRCTRL               (*(volatile unsigned *)0x1000E008)  /* USB�ж����Ϳ��ƼĴ��� */
#define USBD_EPINFO                 (*(volatile unsigned *)0x1000E00C)  /* USB��˵�״̬�Ĵ��� */
#define USBD_BCONFIGURATIONVALUE    (*(volatile unsigned *)0x1000E010)  /* SET_CCONFIGURATION��¼ */
#define USBD_BMATTRIBUTES           (*(volatile unsigned *)0x1000E014)  /* ��ǰ�������ԼĴ��� */
#define USBD_DEVSPEED               (*(volatile unsigned *)0x1000E018)  /* ��ǰ�豸�����ٶȼĴ��� */
#define USBD_FRAMENUMBER            (*(volatile unsigned *)0x1000E01C)  /* ��¼��ǰSOF���ڵ�֡�� */
#define USBD_EPTRANSACTIONS0        (*(volatile unsigned *)0x1000E020)  /* ��¼�´�Ҫ��Ĵ������ */
#define USBD_EPTRANSACTIONS1        (*(volatile unsigned *)0x1000E024)  /* ��¼�´�Ҫ��Ĵ������ */
#define USBD_APPIFUPDATE            (*(volatile unsigned *)0x1000E028)  /* �ӿںſ��ٸ��¼Ĵ��� */
#define USBD_CFGINTERFACE0          (*(volatile unsigned *)0x1000E02c)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE1          (*(volatile unsigned *)0x1000E030)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE2          (*(volatile unsigned *)0x1000E034)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE3          (*(volatile unsigned *)0x1000E038)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE4          (*(volatile unsigned *)0x1000E03c)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE5          (*(volatile unsigned *)0x1000E040)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE6          (*(volatile unsigned *)0x1000E044)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE7          (*(volatile unsigned *)0x1000E048)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE8          (*(volatile unsigned *)0x1000E04c)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE9          (*(volatile unsigned *)0x1000E050)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE10         (*(volatile unsigned *)0x1000E054)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE11         (*(volatile unsigned *)0x1000E058)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE12         (*(volatile unsigned *)0x1000E05c)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE13         (*(volatile unsigned *)0x1000E060)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE14         (*(volatile unsigned *)0x1000E064)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE15         (*(volatile unsigned *)0x1000E068)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE16         (*(volatile unsigned *)0x1000E06c)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE17         (*(volatile unsigned *)0x1000E070)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE18         (*(volatile unsigned *)0x1000E074)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE19         (*(volatile unsigned *)0x1000E078)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE20         (*(volatile unsigned *)0x1000E07c)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE21         (*(volatile unsigned *)0x1000E080)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE22         (*(volatile unsigned *)0x1000E084)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE23         (*(volatile unsigned *)0x1000E088)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE24         (*(volatile unsigned *)0x1000E08c)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE25         (*(volatile unsigned *)0x1000E090)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE26         (*(volatile unsigned *)0x1000E094)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE27         (*(volatile unsigned *)0x1000E098)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE28         (*(volatile unsigned *)0x1000E09c)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE29         (*(volatile unsigned *)0x1000E0a0)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE30         (*(volatile unsigned *)0x1000E0a4)  /* ��¼�ӿڵ�ֵ */
#define USBD_CFGINTERFACE31         (*(volatile unsigned *)0x1000E0a8)  /* ��¼�ӿڵ�ֵ */
#define USBD_PKTPASSEDCTRL          (*(volatile unsigned *)0x1000E0ac)  /* ��¼�ɹ����յİ��� */
#define USBD_PKTDROPPEDCTRL         (*(volatile unsigned *)0x1000E0b0)  /* ��¼��ʧ�İ��� */
#define USBD_CRCERRCTRL             (*(volatile unsigned *)0x1000E0b4)  /* ��¼CRC����İ��� */
#define USBD_BITSTUFFERRCTRL        (*(volatile unsigned *)0x1000E0b8)  /* ��¼λ������İ��� */
#define USBD_PIDERRCTRL             (*(volatile unsigned *)0x1000E0bc)  /* ��¼PID����İ��� */
#define USBD_FRAMINGERRCTL          (*(volatile unsigned *)0x1000E0c0)  /* ��¼��SYNC��EOP�İ��� */
#define USBD_TXPKTCTRL              (*(volatile unsigned *)0x1000E0c4)  /* ��¼���Ͱ������� */
#define USBD_STATCTRLOV             (*(volatile unsigned *)0x1000E0c8)  /* ��¼ͳ�ƼĴ��������� */
#define USBD_TXLENGTH               (*(volatile unsigned *)0x1000E0cc)  /* ��¼ÿ��IN������������� */
#define USBD_RXLENGTH               (*(volatile unsigned *)0x1000E0d0)  /* ��¼OUT������������� */
#define USBD_RESUME                 (*(volatile unsigned *)0x1000E0d4)  /* USB���ѼĴ��� */
#define USBD_READFLAG               (*(volatile unsigned *)0x1000E0d8)  /* ���첽״̬�Ĵ�����־ */
#define USBD_RECEIVETYPE            (*(volatile unsigned *)0x1000E0dc)  /* ����״̬�Ĵ��� */
#define USBD_APPLOCK                (*(volatile unsigned *)0x1000E0e0)  /* ���źżĴ��� */
#define USBD_EP0OUTADDR             (*(volatile unsigned *)0x1000E100)  /* �˵�0�˵�źͷ��� */
#define USBD_EP0OUTBMATTR           (*(volatile unsigned *)0x1000E104)  /* �˵�0���ͼĴ��� */
#define USBD_EP0OUTMAXPKTSIZE       (*(volatile unsigned *)0x1000E108)  /* �˵�0�����ߴ�Ĵ��� */
#define USBD_EP0OUTIFNUM            (*(volatile unsigned *)0x1000E10c)  /* �˵�0�ӿںżĴ��� */
#define USBD_EP0OUTSTAT             (*(volatile unsigned *)0x1000E110)  /* �˵�0״̬�Ĵ��� */
#define USBD_EP0OUTBMREQTYPE        (*(volatile unsigned *)0x1000E114)  /* �˵�0 SETUP���������� */
#define USBD_EP0OUTBREQUEST         (*(volatile unsigned *)0x1000E118)  /* �˵�0 SETUP������������ */
#define USBD_EP0OUTWVALUE           (*(volatile unsigned *)0x1000E11c)  /* �˵�0 SETUP��������ֵ */
#define USBD_EP0OUTWINDEX           (*(volatile unsigned *)0x1000E120)  /* �˵�0 SETUP������������ */
#define USBD_EP0OUTWLENGTH          (*(volatile unsigned *)0x1000E120)  /* �˵�0 SETUP�������󳤶� */
#define USBD_EP0OUTSYNCHFRAME       (*(volatile unsigned *)0x1000E128)  /* �˵�0ͬ����֡�� */
#define USBD_EP1OUTADDR             (*(volatile unsigned *)0x1000E12c)  /* �˵�1����˵�źͷ��� */
#define USBD_EP1OUTBMATTR           (*(volatile unsigned *)0x1000E130)  /* �˵�1������ͼĴ��� */
#define USBD_EP1OUTMAXPKTSIZE       (*(volatile unsigned *)0x1000E134)  /* �˵�1��������ߴ�Ĵ��� */
#define USBD_EP1OUTIFNUM            (*(volatile unsigned *)0x1000E138)  /* �˵�1����ӿںżĴ��� */
#define USBD_EP1OUTSTAT             (*(volatile unsigned *)0x1000E13c)  /* �˵�1���״̬�Ĵ��� */
#define USBD_EP1OUTBMREQTYPE        (*(volatile unsigned *)0x1000E140)  /* �˵�1���SETUP������������ */
#define USBD_EP1OUTBREQUEST         (*(volatile unsigned *)0x1000E144)  /* �˵�1���SETUP������������ */
#define USBD_EP1OUTWVALUE           (*(volatile unsigned *)0x1000E148)  /* �˵�1���SETUP��������ֵ */
#define USBD_EP1OUTWINDX            (*(volatile unsigned *)0x1000E14c)  /* �˵�1���SETUP������������ */
#define USBD_EP1OUTWLENGH           (*(volatile unsigned *)0x1000E150)  /* �˵�1���SETUP���������򳤶� */
#define USBD_EP1OUTSYNCHFRAME       (*(volatile unsigned *)0x1000E154)  /* �˵�1���ͬ����֡�� */
#define USBD_EP1INADDR              (*(volatile unsigned *)0x1000E158)  /* �˵�1����˵�źͷ��� */
#define USBD_EP1INBMATTR            (*(volatile unsigned *)0x1000E15c)  /* �˵�1�������ͼĴ��� */
#define USBD_EP1INMAXPKTSIZE        (*(volatile unsigned *)0x1000E160)  /* �˵�1���������ߴ�Ĵ��� */
#define USBD_EP1INIFNUM             (*(volatile unsigned *)0x1000E164)  /* �˵�1����ӿںżĴ��� */
#define USBD_EP1INSTAT              (*(volatile unsigned *)0x1000E168)  /* �˵�1����״̬�Ĵ��� */
#define USBD_EP1INBMREQTYPE         (*(volatile unsigned *)0x1000E16c)  /* �˵�1����SETUP������������ */
#define USBD_EP1INBREQUEST          (*(volatile unsigned *)0x1000E170)  /* �˵�1����SETUP������������ */
#define USBD_EP1INWVALUE            (*(volatile unsigned *)0x1000E174)  /* �˵�1����SETUP��������ֵ */
#define USBD_EP1INWINDEX            (*(volatile unsigned *)0x1000E178)  /* �˵�1����SETUP������������ */
#define USBD_EP1INWLENGTH           (*(volatile unsigned *)0x1000E17c)  /* �˵�1����SETUP���������򳤶� */
#define USBD_EP1INSYNCHFRAME        (*(volatile unsigned *)0x1000E180)  /* �˵�1����ͬ����֡�� */
#define USBD_EP2OUTADDR             (*(volatile unsigned *)0x1000E184)  /* �˵�2����˵�źͷ��� */
#define USBD_EP2OUTBMATTR           (*(volatile unsigned *)0x1000E188)  /* �˵�2������ͼĴ��� */
#define USBD_EP2OUTMAXPKTSIZE       (*(volatile unsigned *)0x1000E18c)  /* �˵�2��������ߴ�Ĵ��� */
#define USBD_EP2OUTIFNUM            (*(volatile unsigned *)0x1000E190)  /* �˵�2����ӿںżĴ��� */
#define USBD_EP2OUTSTAT             (*(volatile unsigned *)0x1000E194)  /* �˵�2���״̬�Ĵ��� */
#define USBD_EP2OUTBMREQTYPE        (*(volatile unsigned *)0x1000E198)  /* �˵�2���SETUP������������ */
#define USBD_EP2OUTBREQUEST         (*(volatile unsigned *)0x1000E19c)  /* �˵�2���SETUP������������ */
#define USBD_EP2OUTWVALUE           (*(volatile unsigned *)0x1000E1a0)  /* �˵�2���SETUP��������ֵ */
#define USBD_EP2OUTWINDEX           (*(volatile unsigned *)0x1000E1a4)  /* �˵�2���SETUP������������ */
#define USBD_EP2OUTWLENGTH          (*(volatile unsigned *)0x1000E1a8)  /* �˵�2���SETUP���������򳤶� */
#define USBD_EP2OUTSYNCHFRAME       (*(volatile unsigned *)0x1000E1ac)  /* �˵�2���ͬ����֡�� */
#define USBD_EP2INADDR              (*(volatile unsigned *)0x1000E1b0)  /* �˵�2����˵�źͷ��� */
#define USBD_EP2INBMATTR            (*(volatile unsigned *)0x1000E1b4)  /* �˵�2�������ͼĴ��� */
#define USBD_EP2INMAXPKTSIZE        (*(volatile unsigned *)0x1000E1b8)  /* �˵�2���������ߴ�Ĵ��� */
#define USBD_EP2INIFNUM             (*(volatile unsigned *)0x1000E1bc)  /* �˵�2����ӿںżĴ��� */
#define USBD_EP2INSTAT              (*(volatile unsigned *)0x1000E1c0)  /* �˵�2����״̬�Ĵ��� */
#define USBD_EP2INBMREQTYPE         (*(volatile unsigned *)0x1000E1c4)  /* �˵�2����SETUP������������ */
#define USBD_EP2INBREQUEST          (*(volatile unsigned *)0x1000E1c8)  /* �˵�2����SETUP������������ */
#define USBD_EP2INWVALUE            (*(volatile unsigned *)0x1000E1cc)  /* �˵�2����SETUP��������ֵ */
#define USBD_EP2INWINDEX            (*(volatile unsigned *)0x1000E1d0)  /* �˵�2����SETUP������������ */
#define USBD_EP2INWLENGTH           (*(volatile unsigned *)0x1000E1d4)  /* �˵�2����SETUP���������򳤶� */
#define USBD_EP2INSYNCHFRAME        (*(volatile unsigned *)0x1000E1d8)  /* �˵�2����ͬ����֡�� */
#define USBD_RXFIFO                 (*(volatile unsigned *)0x1000E200)  /* ����FIFO */
#define USBD_TXFIFO                 (*(volatile unsigned *)0x1000E300)  /* ����FIFO */


/******************************************************************************************
 *  EMIģ��
 *  ��ַ: 0x11000000
 ******************************************************************************************/

#define EMI_CSACONF                 (*(volatile unsigned *)0x11000000)  /* CSA�������üĴ��� */
#define EMI_CSBCONF                 (*(volatile unsigned *)0x11000004)  /* CSB�������üĴ��� */
#define EMI_CSCCONF                 (*(volatile unsigned *)0x11000008)  /* CSC�������üĴ��� */
#define EMI_CSDCONF                 (*(volatile unsigned *)0x1100000c)  /* CSD�������üĴ��� */
#define EMI_CSECONF                 (*(volatile unsigned *)0x11000010)  /* CSE�������üĴ��� */
#define EMI_CSFCONF                 (*(volatile unsigned *)0x11000014)  /* CSF�������üĴ��� */
#define EMI_SDCONF1                 (*(volatile unsigned *)0x11000018)  /* SDRAMʱ�����üĴ���1 */
#define EMI_SDCONF2                 (*(volatile unsigned *)0x1100001c)  /* SDRAMʱ�����üĴ���2, SDRAM��ʼ���õ���������Ϣ */
#define EMI_REMAPCONF               (*(volatile unsigned *)0x11000020)  /* Ƭѡ�ռ估��ַӳ��REMAP���üĴ��� */
#define EMI_NAND_ADDR1              (*(volatile unsigned *)0x11000100)  /* NAND FLASH�ĵ�ַ�Ĵ���1 */
#define EMI_NAND_COM                (*(volatile unsigned *)0x11000104)  /* NAND FLASH�Ŀ����ּĴ��� */
#define EMI_NAND_STA                (*(volatile unsigned *)0x1100010c)  /* NAND FLASH��״̬�Ĵ��� */
#define EMI_ERR_ADDR1               (*(volatile unsigned *)0x11000110)  /* ����������ĵ�ַ�Ĵ���1 */
#define EMI_ERR_ADDR2               (*(volatile unsigned *)0x11000114)  /* ����������ĵ�ַ�Ĵ���2 */
#define EMI_NAND_CONF1              (*(volatile unsigned *)0x11000118)  /* NAND FLASH������������1 */
#define EMI_NAND_INTR               (*(volatile unsigned *)0x1100011c)  /* NAND FLASH�жϼĴ��� */
#define EMI_NAND_ECC                (*(volatile unsigned *)0x11000120)  /* ECCУ����ɼĴ��� */
#define EMI_NAND_IDLE               (*(volatile unsigned *)0x11000124)  /* NAND FLASH���мĴ��� */
#define EMI_NAND_CONF2              (*(volatile unsigned *)0x11000128)  /* NAND FLASH������������2 */
#define EMI_NAND_ADDR2              (*(volatile unsigned *)0x1100012c)  /* NAND FLASH�ĵ�ַ�Ĵ���2 */
#define EMI_NAND_ID                 (*(volatile unsigned *)0x11000130)  /* NAND FLASH��ID�Ĵ���*/
#define EMI_NAND_DATA               (*(volatile unsigned *)0x11000200)  /* NAND FLASH�����ݼĴ��� */


/******************************************************************************************
 *  DMACģ��
 *  ��ַ: 0x11001000
 ******************************************************************************************/

#define DMAC_INTSTATUS              (*(volatile unsigned *)0x11001020)  /* DAMC�ж�״̬�Ĵ����� */
#define DMAC_INTTCSTATUS            (*(volatile unsigned *)0x11001050)  /* DMAC��������ж�״̬�Ĵ��� */
#define DMAC_INTTCCLEAR             (*(volatile unsigned *)0x11001060)  /* DMAC��������ж�״̬����Ĵ��� */
#define DMAC_INTERRORSTATUS         (*(volatile unsigned *)0x11001080)  /* DMAC��������ж�״̬�Ĵ��� */
#define DMAC_INTERRCLR              (*(volatile unsigned *)0x11001090)  /* DMAC��������ж�״̬����Ĵ��� */
#define DMAC_ENBLDCHNS              (*(volatile unsigned *)0x110010b0)  /* DMACͨ��ʹ��״̬�Ĵ��� */
#define DMAC_C0SRCADDR              (*(volatile unsigned *)0x11001000)  /* DMAC��0Դ��ַ�Ĵ��� */
#define DMAC_C0DESTADD              (*(volatile unsigned *)0x11001004)  /* DMAC��0Ŀ�ĵ�ַ�Ĵ��� */
#define DMAC_C0CONTROL              (*(volatile unsigned *)0x1100100c)  /* DMAC��0���ƼĴ��� */
#define DMAC_C0CONFIGURATION        (*(volatile unsigned *)0x11001010)  /* DMAC��0���üĴ��� */
#define DMAC_C0DESCRIPTOR           (*(volatile unsigned *)0x11001014)  /* DMAC��0�����ַ�Ĵ��� */
#define DMAC_C1SRCADDR              (*(volatile unsigned *)0x11001100)  /* DMAC��1Դ��ַ�Ĵ��� */
#define DMAC_C1DESTADDR             (*(volatile unsigned *)0x11001104)  /* DMAC��1Ŀ�ĵ�ַ�Ĵ��� */
#define DMAC_C1CONTROL              (*(volatile unsigned *)0x1100110c)  /* DMAC��1���ƼĴ��� */
#define DMAC_C1CONFIGURATION        (*(volatile unsigned *)0x11001110)  /* DMAC��1���üĴ��� */
#define DMAC_C1DESCRIPTOR           (*(volatile unsigned *)0x11001114)  /* DMAC��1�����ַ�Ĵ��� */
#define DMAC_C2SRCADDR              (*(volatile unsigned *)0x11001200)  /* DMAC��2Դ��ַ�Ĵ��� */
#define DMAC_C2DESTADDR             (*(volatile unsigned *)0x11001204)  /* DMAC��2Ŀ�ĵ�ַ�Ĵ��� */
#define DMAC_C2CONTROL              (*(volatile unsigned *)0x1100120c)  /* DMAC��2���ƼĴ��� */
#define DMAC_C2CONFIGURATION        (*(volatile unsigned *)0x11001210)  /* DMAC��2���üĴ��� */
#define DMAC_C2DESCRIPTOR           (*(volatile unsigned *)0x11001214)  /* DMAC��2�����ַ�Ĵ��� */
#define DMAC_C3SRCADDR              (*(volatile unsigned *)0x11001300)  /* DMAC��3Դ��ַ�Ĵ��� */
#define DMAC_C3DESTADDR             (*(volatile unsigned *)0x11001304)  /* DMAC��3Ŀ�ĵ�ַ�Ĵ��� */
#define DMAC_C3CONTROL              (*(volatile unsigned *)0x1100130c)  /* DMAC��3���ƼĴ��� */
#define DMAC_C3CONFIGURATION        (*(volatile unsigned *)0x11001310)  /* DMAC��3���üĴ��� */
#define DMAC_C3DESCRIPTOR           (*(volatile unsigned *)0x11001314)  /* DMAC��3�����ַ�Ĵ��� */
#define DMAC_C4SRCADDR              (*(volatile unsigned *)0x11001400)  /* DMAC��4Դ��ַ�Ĵ��� */
#define DMAC_C4DESTADDR             (*(volatile unsigned *)0x11001404)  /* DMAC��4Ŀ�ĵ�ַ�Ĵ��� */
#define DMAC_C4CONTROL              (*(volatile unsigned *)0x1100140c)  /* DMAC��4���ƼĴ��� */
#define DMAC_C4CONFIGURATION        (*(volatile unsigned *)0x11001410)  /* DMAC��4���üĴ��� */
#define DMAC_C4DESCRIPTOR           (*(volatile unsigned *)0x11001414)  /* DMAC��4�����ַ�Ĵ��� */
#define DMAC_C5SRCADDR              (*(volatile unsigned *)0x11001500)  /* DMAC��5Դ��ַ�Ĵ��� */
#define DMAC_C5DESTADDR             (*(volatile unsigned *)0x11001504)  /* DMAC��5Ŀ�ĵ�ַ�Ĵ��� */
#define DMAC_C5CONTROL              (*(volatile unsigned *)0x1100150c)  /* DMAC��5���ƼĴ��� */
#define DMAC_C5CONFIGURATION        (*(volatile unsigned *)0x11001510)  /* DMAC��5���üĴ��� */
#define DMAC_C5DESCRIPTOR           (*(volatile unsigned *)0x11001514)  /* DMAC��5�����ַ�Ĵ��� */


/******************************************************************************************
 *  LCDCģ��
 *  ��ַ: 0x11002000
 ******************************************************************************************/

#define LCDC_SSA                    (*(volatile unsigned *)0x11002000)  /* ��Ļ��ʼ��ַ�Ĵ��� */
#define LCDC_SIZE                   (*(volatile unsigned *)0x11002004)  /* ��Ļ�ߴ�Ĵ��� */
#define LCDC_PCR                    (*(volatile unsigned *)0x11002008)  /* ������üĴ��� */
#define LCDC_HCR                    (*(volatile unsigned *)0x1100200c)  /* ˮƽ���üĴ��� */
#define LCDC_VCR                    (*(volatile unsigned *)0x11002010)  /* ��ֱ���üĴ��� */
#define LCDC_PWMR                   (*(volatile unsigned *)0x11002014)  /* PWM�Աȶȿ��ƼĴ��� */
#define LCDC_LECR                   (*(volatile unsigned *)0x11002018)  /* ʹ�ܿ��ƼĴ��� */
#define LCDC_DMACR                  (*(volatile unsigned *)0x1100201c)  /* DMA���ƼĴ��� */
#define LCDC_LCDISREN               (*(volatile unsigned *)0x11002020)  /* �ж�ʹ�ܼĴ��� */
#define LCDC_LCDISR                 (*(volatile unsigned *)0x11002024)  /* �ж�״̬�Ĵ��� */
#define LCDC_LGPMR                  (*(volatile unsigned *)0x11002040)  /* �Ҷȵ�ɫӳ��Ĵ����� */
                                                                        /*(0x40-0x7c 16��32bit�Ĵ���) */


/******************************************************************************************
 *  MACģ��
 *  ��ַ: 0x11003000
 ******************************************************************************************/

#define MAC_CTRL                    (*(volatile unsigned *)0x11003000)  /* MAC���ƼĴ��� */
#define MAC_INTSRC                  (*(volatile unsigned *)0x11003004)  /* MAC�ж�Դ�Ĵ��� */
#define MAC_INTMASK                 (*(volatile unsigned *)0x11003008)  /* MAC�ж����μĴ��� */
#define MAC_IPGT                    (*(volatile unsigned *)0x1100300c)  /* ����֡����Ĵ��� */
#define MAC_IPGR1                   (*(volatile unsigned *)0x11003010)  /* �ȴ����ڼĴ��� */
#define MAC_IPGR2                   (*(volatile unsigned *)0x11003014)  /* �ȴ����ڼĴ��� */
#define MAC_PACKETLEN               (*(volatile unsigned *)0x11003018)  /* ֡���ȼĴ��� */
#define MAC_COLLCONF                (*(volatile unsigned *)0x1100301c)  /* ��ײ�ط��Ĵ��� */
#define MAC_TXBD_NUM                (*(volatile unsigned *)0x11003020)  /* �����������Ĵ��� */
#define MAC_FLOWCTRL                (*(volatile unsigned *)0x11003024)  /* ���ؼĴ��� */
#define MAC_MII_CTRL                (*(volatile unsigned *)0x11003028)  /* PHY���ƼĴ��� */
#define MAC_MII_CMD                 (*(volatile unsigned *)0x1100302c)  /* PHY����Ĵ��� */
#define MAC_MII_ADDRESS             (*(volatile unsigned *)0x11003030)  /* PHY��ַ�Ĵ��� */
#define MAC_MII_TXDATA              (*(volatile unsigned *)0x11003034)  /* PHYд���ݼĴ��� */
#define MAC_MII_RXDATA              (*(volatile unsigned *)0x11003038)  /* PHY�����ݼĴ��� */
#define MAC_MII_STATUS              (*(volatile unsigned *)0x1100303c)  /* PHY״̬�Ĵ��� */
#define MAC_ADDR0                   (*(volatile unsigned *)0x11003040)  /* MAC��ַ�Ĵ��� */
#define MAC_ADDR1                   (*(volatile unsigned *)0x11003044)  /* MAC��ַ�Ĵ��� */
#define MAC_HASH0                   (*(volatile unsigned *)0x11003048)  /* MAC HASH�Ĵ��� */
#define MAC_HASH1                   (*(volatile unsigned *)0x1100304c)  /* MAC HASH�Ĵ��� */
#define MAC_TXPAUSE                 (*(volatile unsigned *)0x11003050)  /* MAC����֡�Ĵ��� */
#define MAC_BD                      /* 0x4*/


#endif

/*******************END OF FILE*************************/
