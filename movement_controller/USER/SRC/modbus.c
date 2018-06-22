#include "modbus.h"
/**
 * frame style:
 * 1. station code -> 2.address -> 3. function -> 4.frameMark -> 5.
 */

/******************* extern parament init *******************************/
//ModBusProtocolTypeDef	         ModBus;
//ModeBusFrameStatusTypeDef        ModeBusFrameStatus;
//ModeBusFrameTypeDef              ModeBusFrame;

/******************** extern parament init ********************************/
Modbus_FrameTypedef 		Modbus_Frame;
ModBusTypedef 				ModBus;
/****************************************************************/

 Modbus_status analysisFrame(Modbus_FrameTypedef *frame)
{
    return OK;
}
void packFrame(void)
{
    ;
}
void sendFrame(void)
{
    ;
}























//m_protocol_dev_typedef m_ctrl_dev;	
///**
// * 
// */

//m_result mb_unpack_frame(m_frame_typedef *fx)
//{
//	uint16_t rxchkval  = 0;   	 	
//	uint16_t calchkval = 0;			
//	uint8_t datalen    = 0; 				
//	fx->datalen        = 0; 				
//	if (m_ctrl_dev.rxlen > M_MAX_FRAME_LENGTH || m_ctrl_dev.rxlen < M_MIN_FRAME_LENGTH)return MR_FRAME_FORMAT_ERR; 
//	datalen = m_ctrl_dev.rxbuf[3];		// frame length bit
//	switch (m_ctrl_dev.checkmode)
//	{
//	case M_FRAME_CHECK_SUM:							
//		calchkval = mc_check_sum(m_ctrl_dev.rxbuf, datalen + 4);
//		rxchkval  = m_ctrl_dev.rxbuf[datalen + 4];
//		break;
//	case M_FRAME_CHECK_XOR:							
//		calchkval = mc_check_xor(m_ctrl_dev.rxbuf, datalen + 4);
//		rxchkval  = m_ctrl_dev.rxbuf[datalen + 4];
//		break;
//	case M_FRAME_CHECK_CRC8:						
//		calchkval = mc_check_crc8(m_ctrl_dev.rxbuf, datalen + 4);
//		rxchkval  = m_ctrl_dev.rxbuf[datalen + 4];
//		break;
//	case M_FRAME_CHECK_CRC16:						
//		calchkval = mc_check_crc16(m_ctrl_dev.rxbuf, datalen + 4);
//		rxchkval  = ((uint16_t)m_ctrl_dev.rxbuf[datalen + 4] << 8) + m_ctrl_dev.rxbuf[datalen + 5];
//		break;
//	}
//	m_ctrl_dev.rxlen = 0;			//
//	m_ctrl_dev.frameok = 0;		//
//	if (calchkval == rxchkval)		//CRC16 check pass
//	{
//		fx->address  = m_ctrl_dev.TxbuffTemp[0];
//		fx->function = m_ctrl_dev.TxbuffTemp[1];
//		fx->count    = m_ctrl_dev.TxbuffTemp[2];
//		fx->datalen  = m_ctrl_dev.TxbuffTemp[3];
//		if (fx->datalen)
//		{		
//			for (datalen = 0; datalen < fx->datalen; datalen++)
//			{
//				fx->TxbuffTemp[datalen] = m_ctrl_dev.rxbuf[4 + datalen];		
//			}
//		}
//		fx->chkval = rxchkval;	
//	} 
//	else 
//		return MR_FRAME_CHECK_ERR;
//	return MR_OK;
//}



//void mb_packsend_frame(m_frame_typedef *fx)
//{
//	uint16_t i;
//	uint16_t calchkval = 0;			
//	uint16_t framelen = 0;						

//	if (m_ctrl_dev.checkmode == M_FRAME_CHECK_CRC16)
//		framelen = 6 + fx->datalen;
//	else 
//		framelen = 5 + fx->datalen;
//	fx ->TxbuffTemp[0] = fx->address;
//	fx ->TxbuffTemp[1] = fx->function;
//	fx ->TxbuffTemp[2] = fx->count;
//	fx ->TxbuffTemp[3] = fx->datalen;
//	for (i = 0; i < fx->datalen; i++)
//	{
//		TxbuffTemp[4 + i] = fx->data[i];
//	}
//	switch (m_ctrl_dev.checkmode)
//	{
//	case M_FRAME_CHECK_SUM:							
//		calchkval = mc_check_sum(sendbuf, fx->datalen + 4);
//		break;
//	case M_FRAME_CHECK_XOR:							
//		calchkval = mc_check_xor(sendbuf, fx->datalen + 4);
//		break;
//	case M_FRAME_CHECK_CRC8:						
//		calchkval = mc_check_crc8(sendbuf, fx->datalen + 4);
//		break;
//	case M_FRAME_CHECK_CRC16:						
//		calchkval = mc_check_crc16(sendbuf, fx->datalen + 4);
//		break;
//	}

//	if (m_ctrl_dev.checkmode == M_FRAME_CHECK_CRC16)		
//	{
//		sendbuf[4 + fx->datalen] = (calchkval >> 8) & 0XFF; 	
//		sendbuf[5 + fx->datalen] = calchkval & 0XFF;			
//	} else sendbuf[4 + fx->datalen] = calchkval & 0XFF;
//	mp_send_data(sendbuf, framelen);	
//	myfree(SRAMIN, sendbuf);			
//}


//m_result mb_init(uint8_t checkmode)
//{
//	m_ctrl_dev.rxbuf = mymalloc(SRAMIN, M_MAX_FRAME_LENGTH);
//	m_ctrl_dev.rxlen = 0;
//	m_ctrl_dev.frameok = 0;
//	m_ctrl_dev.checkmode = checkmode;
//	if (m_ctrl_dev.rxbuf)
//		return MR_OK;
//	else
//		return MR_MEMORY_ERR;
//}


//void mb_destroy(void)
//{
//	myfree(SRAMIN, m_ctrl_dev.rxbuf);	
//	m_ctrl_dev.rxlen = 0;
//	m_ctrl_dev.frameok = 0;
//}







