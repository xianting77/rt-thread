#ifndef SPI_FLASH_H_INCLUDED
#define SPI_FLASH_H_INCLUDED

/*
user for AT45DB161.
copy form : http://www.ourdev.cn/bbs/bbs_content.jsp?bbs_sn=737106
thanks to gxlujd.
*/
#define AT45DB_BUFFER_1_WRITE                 0x84	/* д���һ������ */
#define AT45DB_BUFFER_2_WRITE                 0x87	/* д��ڶ������� */
#define AT45DB_BUFFER_1_READ                  0xD4	/* ��ȡ��һ������ */
#define AT45DB_BUFFER_2_READ                  0xD6	/* ��ȡ�ڶ������� */
#define AT45DB_B1_TO_MM_PAGE_PROG_WITH_ERASE  0x83	/* ����һ������������д�����洢��������ģʽ��*/
#define AT45DB_B2_TO_MM_PAGE_PROG_WITH_ERASE  0x86	/* ���ڶ�������������д�����洢��������ģʽ��*/
#define AT45DB_MM_PAGE_TO_B1_XFER             0x53	/* �����洢����ָ��ҳ���ݼ��ص���һ������    */
#define AT45DB_MM_PAGE_TO_B2_XFER             0x55	/* �����洢����ָ��ҳ���ݼ��ص��ڶ�������    */
#define AT45DB_PAGE_ERASE                     0x81	/* ҳɾ����ÿҳ512/528�ֽڣ� */
#define AT45DB_SECTOR_ERASE                   0x7C	/* ����������ÿ����128K�ֽڣ�*/
#define AT45DB_READ_STATE_REGISTER            0xD7	/* ��ȡ״̬�Ĵ��� */
#define AT45DB_MM_PAGE_READ                   0xD2	/* ��ȡ����������ָ��ҳ */
#define AT45DB_MM_PAGE_PROG_THRU_BUFFER1      0x82  /* ͨ��������д���������� */

/*
user for SST25VF**
*/
#define SPI_FLASH_CMD_RDSR                    0x05  /* ��״̬�Ĵ���     */
#define SPI_FLASH_CMD_WRSR                    0x01  /* д״̬�Ĵ���     */
#define SPI_FLASH_CMD_EWSR                    0x50  /* ʹ��д״̬�Ĵ��� */
#define SPI_FLASH_CMD_WRDI                    0x04  /* �ر�дʹ��       */
#define SPI_FLASH_CMD_WREN                    0x06  /* ��дʹ��       */
#define SPI_FLASH_CMD_READ                    0x03  /* ������           */
#define SPI_FLASH_CMD_FAST_READ               0x0B  /* ���ٶ�           */
#define SPI_FLASH_CMD_BP                      0x02  /* �ֽڱ��         */
#define SPI_FLASH_CMD_AAIP                    0xAD  /* �Զ���ַ������� */
#define SPI_FLASH_CMD_ERASE_4K                0x20  /* ��������:4K      */
#define SPI_FLASH_CMD_ERASE_32K               0x52  /* ��������:32K     */
#define SPI_FLASH_CMD_ERASE_64K               0xD8  /* ��������:64K     */
#define SPI_FLASH_CMD_ERASE_full              0xC7  /* ȫƬ����         */
#define SPI_FLASH_CMD_JEDEC_ID                0x9F  /* �� JEDEC_ID      */
#define SPI_FLASH_CMD_EBSY                    0x70  /* ��SOæ���ָʾ */
#define SPI_FLASH_CMD_DBSY                    0x80  /* �ر�SOæ���ָʾ */

//---------------------------------

/*
 * WARNING: !!! ENABLING DMA MODE MAY DESTROY YOUR DATA IN THE SPI FLASH !!!
 * Don't set SPI_FLASH_USE_DMA to 1 unless you know what you're doing!
 * However, readonly access is just fine. :)
 */
#define SPI_FLASH_USE_DMA         1
#define FLASH_SECTOR_SIZE	      512
#define SST_WRITE_BUFFER          1

extern void rt_hw_spi_flash_init(void);

#endif /* SPI_FLASH_H_INCLUDED */
