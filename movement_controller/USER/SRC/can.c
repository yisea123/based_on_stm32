/**
  *-----------------------------------------------------------------------------
  * @file    can.c
  * @author  V
  * @version V1.0
  * @date    27-NOV-2017
  * @brief   This file contains all the functions prototypes for the can communication 
  *          driver library.
  *------------------------------------------------------------------------------
  */
#include "can.h"

/** extern struct declaration -------------------------------------------------*/
CAN_SendMessage_TypeDef         CAN_SendMessage;
MasterCMD_SortTypeDef           *pCAN_Rec_Buff;
MasterCMD_SortTypeDef           CAN_Rec_Buff[10] = {0};

uint8_t buffIndexStart = 0;
uint8_t buffIndexEnd   = 0;
uint8_t buff_ZeroEmpty = 0;

uint32_t CAN_station_ID = 0X00324;
/**
 * @brief: CAN_GPIO configurate
 */
void CAN_GPIO_Configuration(CAN_TypeDef* CANx)
{
    if(CANx == CAN1)
    {
        CAN_DeInit(CAN1);
        /*------------------------- CAN GPIO CONFIG---------------------------------*/
        /* CAN Periph clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitTypeDef    GPIO_InitStruction;
        /** Configure CAN RX pin */
        GPIO_InitStruction.GPIO_Pin   = GPIO_Pin_11;
        GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStruction);
        /** Configure CAN TX pin */
        GPIO_InitStruction.GPIO_Pin   = GPIO_Pin_12;
        GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStruction);
    }
    if(CANx == CAN2)
    {
        CAN_DeInit(CAN2);
        /*------------------------- CAN GPIO CONFIG---------------------------------*/
        /* CAN Periph clock enable */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        GPIO_InitTypeDef        GPIO_InitStruction;
        /** Configure CAN RX pin */
        GPIO_InitStruction.GPIO_Pin   = GPIO_Pin_12;
        GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStruction);
        /** Configure CAN TX pin */
        GPIO_InitStruction.GPIO_Pin   = GPIO_Pin_13;
        GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStruction);
    }
}
/*-------------------- function point --------------------------*/
//void (*pFunc_Arry[])(uint16_t, uint16_t);
//typedef void (*pFunc)(uint16_t);

/*-------------------- function point end ----------------------*/

/**
 * CAN GPIO:
 * CAN_TX -> PB9;     CAN_RX -> PB8
 * need GPIO_Pin remap
 *
 * CAN_SJW:重新同步跳跃时间单元.         范围:CAN_SJW_1tq~ CAN_SJW_4tq
 * CAN_BS2:时间段2的时间单元.           范围:CAN_BS2_1tq~CAN_BS2_8tq;
 * CAN_BS1:时间段1的时间单元.           范围:CAN_BS1_1tq ~CAN_BS1_16tq
 * CAN_Prescaler :波特率分频器.        范围:1~1024;  tq=(brp)*tpclk1
 * 波特率=Fpclk1/((CAN_SJW+CAN_BS2+CAN_BS1+1+1)*CAN_Prescaler);
 * Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Configurate(CAN1, CAN_Mode_Normal, CAN_SJW_1tq, CAN_BS1_9tq, CAN_BS2_8tq, 4);
 * 则波特率为:36M/((8+9+1)*4)=500Kbps
 * 返回值:0,初始化OK;
 * 其他,初始化失败;
*/
/**
 *    CAN1 and CAN2 configuration
 *
 *    @param  CANx               param set to define which CAN port need to be set, param is CAN1, CAN2
 *    @param  CAN_ModeSet        CAN work model set, normal mode or loop mode can choose
 *    @param  CAN_SJW_TimeSet    like the words meaning
 *    @param  CAN_BS1_TimeSet    like the words meaning 
 *    @param  CAN_BS2_TimeSet    like the words meaning
 *    @param  CAN_Prescaler_Set  prescaler  of the CAN Clok, can be devide the clock speed
 *    
 */
void CAN_Configurate(CAN_TypeDef* CANx, uint8_t CAN_ModeSet, uint8_t CAN_SJW_TimeSet, uint8_t CAN_BS1_TimeSet,
                     uint8_t CAN_BS2_TimeSet, uint8_t CAN_Prescaler_Set)
{
            /*----------------- CAN1 Configuration ------------------------------------------*/
    if (CANx == CAN1)
    {
        CAN_InitTypeDef     CAN_InitStructure;

        CAN_GPIO_Configuration(CAN1);
        /** open CAN clock -------------------------------------------------------*/
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); // can clock
        /*------------------------- CAN MODE CONFIG---------------------------------*/
        /** CAN default speed configurate to 500Kbps. ------------------------------*/
        //CAN单元设置
        CAN_InitStructure.CAN_TTCM          = DISABLE;               //非时间触发通信模式
        CAN_InitStructure.CAN_ABOM          = DISABLE;               //软件自动离线管理
        CAN_InitStructure.CAN_AWUM          = DISABLE;               //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
        CAN_InitStructure.CAN_NART          = ENABLE;                //报文自动传送, 从FIFO_0 到FIFO_3
        CAN_InitStructure.CAN_RFLM          = DISABLE;               //报文不锁定,新的覆盖旧的
        CAN_InitStructure.CAN_TXFP          = DISABLE;               //优先级由报文标识符决定
        CAN_InitStructure.CAN_Mode          = CAN_ModeSet;              //模式设置： mode: 0,普通模式;1,回环模式;
        /** set CAN BUS bps------------------------------------*/
        CAN_InitStructure.CAN_SJW       = CAN_SJW_TimeSet;                
        CAN_InitStructure.CAN_BS1       = CAN_BS1_TimeSet;          
        CAN_InitStructure.CAN_BS2       = CAN_BS2_TimeSet;          
        CAN_InitStructure.CAN_Prescaler = CAN_Prescaler_Set;        
        CAN_Init(CAN1, &CAN_InitStructure);                             //初始化CAN1

        /*-------------------- CAN Fileter Configuuration ---------------------*/
        /** 
         * configuration statement:
         * 1. configurate CAN in standard ID mode.
         * 2. filter run in ID marsk mode.              CAN_FxR1
         * 3. accept standard ID range between 0x0324 to 0x0327.
         * 4. CAN default speed configurate to 500Kbps. 
        */
        CAN_FilterInitTypeDef     CAN_FilterInitStructure;  
        CAN_FilterInitStructure.CAN_FilterNumber         = 0;                       //过滤器0
        CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;   //屏蔽位模式
        CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;   //32位宽
        CAN_FilterInitStructure.CAN_FilterIdHigh         = ((CAN_station_ID << 21) & 0xFFFF0000) >> 16;                   //32位ID
        CAN_FilterInitStructure.CAN_FilterIdLow          = ((CAN_station_ID << 21)|CAN_ID_STD|CAN_RTR_DATA) & 0xffff;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;                 //32位MASK
        CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;        //过滤器0关联到FIFO0
        CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;                  //激活过滤器0
        CAN_FilterInit(&CAN_FilterInitStructure);                                   //滤波器初始化

        /*--------------------- CAN NVIC Configuration ----------------------*/


        NVIC_InitTypeDef    NVIC_InitStructure;
        CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);                                    //FIFO0消息挂号中断允许.
        NVIC_InitStructure.NVIC_IRQChannel                   = CAN1_RX0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                   // 主优先级为1
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;                   // 次优先级为0
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    /*----------------- CAN2 Configuration ------------------------------------------*/
    if (CANx == CAN2)
    {
        CAN_GPIO_Configuration(CAN2);
        /*------------------------- CAN MODE CONFIG---------------------------------*/
        CAN_InitTypeDef         CAN_InitStructure;
        /** open CAN clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); // can clock
        //CAN单元设置
        CAN_InitStructure.CAN_TTCM = DISABLE;                           //非时间触发通信模式
        CAN_InitStructure.CAN_ABOM = DISABLE;                           //软件自动离线管理
        CAN_InitStructure.CAN_AWUM = DISABLE;                           //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
        CAN_InitStructure.CAN_NART = DISABLE;                           //禁止报文自动传送
        CAN_InitStructure.CAN_RFLM = DISABLE;                           //报文不锁定,新的覆盖旧的
        CAN_InitStructure.CAN_TXFP = ENABLE;                            //优先级由报文标识符决定
        CAN_InitStructure.CAN_Mode = CAN_ModeSet;                       //模式设置： mode: 0,普通模式;1,回环模式;
        //设置波特率
        CAN_InitStructure.CAN_SJW       = CAN_SJW_TimeSet;              //重新同步跳跃宽度(Tsjw)CAN_SJW_TimeSet+1个时间单位  CAN_SJW_1tq     CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
        CAN_InitStructure.CAN_BS1       = CAN_BS1_TimeSet;              //CAN_BS1_TimeSet = CAN_BS1_TimeSet+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
        CAN_InitStructure.CAN_BS2       = CAN_BS2_TimeSet;              //CAN_BS2_TimeSet = CAN_BS2_TimeSet+1个时间单位CAN_BS2_1tq ~    CAN_BS2_8tq
        CAN_InitStructure.CAN_Prescaler = CAN_Prescaler_Set;            //分频系数(Fdiv)为brp+1
        CAN_Init(CAN1, &CAN_InitStructure);                             //初始化CAN1

        /*-------------------- CAN Fileter Configuuration ---------------------*/
        CAN_FilterInitTypeDef       CAN_FilterInitStructure;
        CAN_FilterInitStructure.CAN_FilterNumber         = 0;                           //过滤器0
        CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;       //屏蔽位模式
        CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;       //32位宽
        CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;                      //32位ID
        CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;                      //32位MASK
        CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;            //过滤器0关联到FIFO0
        CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;                      //激活过滤器0
        CAN_FilterInit(&CAN_FilterInitStructure);      

/*--------------------- CAN NVIC Configuration ----------------------*/
        NVIC_InitTypeDef NVIC_InitStructure;
        CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);                                        //FIFO0消息挂号中断允许.
        NVIC_InitStructure.NVIC_IRQChannel                   = CAN1_RX0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;                       // 主优先级为0
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;                       // 次优先级为1
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
}


/**
 *    @brief CAN_ConfigurateByMarco description, define CAN BUS bps through #define command, @refer file can.h
 *
 *    @param  CANx     CAN_TypeDef using CAN1 or CAN2 as input parameter    
 *    @param  CAN_BPS  define parameter, @refer file can.h, #define CAN_PRESCALER_SPEED_CONFIG
 */
#ifdef      CAN_PRESCALER_SPEED_CONFIG

    void CAN_ConfigurateByMarco(CAN_TypeDef *CANx, uint8_t CAN_BPS)
    {
        /*----------------- CAN1 Configuration ------------------------------------------*/
        if (CANx == CAN1)
        {
            CAN_GPIO_Configuration(CAN1);
            /*------------------------- CAN GPIO CONFIG---------------------------------*/
            /** open CAN clock */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); // can clock
            /*------------------------- CAN MODE CONFIG---------------------------------*/
            /** CAN default speed configurate to 500Kbps. ------------------------------*/
            CAN_InitTypeDef CAN_InitStructure;
            //CAN单元设置
            CAN_InitStructure.CAN_TTCM = DISABLE;     //非时间触发通信模式
            CAN_InitStructure.CAN_ABOM = DISABLE;     //软件自动离线管理
            CAN_InitStructure.CAN_AWUM = DISABLE;     //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
            CAN_InitStructure.CAN_NART = ENABLE;      //报文自动传送, 从FIFO_0 到FIFO_3
            CAN_InitStructure.CAN_RFLM = DISABLE;     //报文不锁定,新的覆盖旧的
            CAN_InitStructure.CAN_TXFP = DISABLE;     //优先级由报文标识符决定
            CAN_InitStructure.CAN_Mode = CAN_ModeSet; //模式设置： mode: 0,普通模式;1,回环模式;
            /** set CAN BUS bps------------------------------------*/

            if (CAN_BPS == CAN_BAUDRATE_500KBPS)
            {
                CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;
                CAN_InitStructure.CAN_BS1       = CAN_BS1_9tq;
                CAN_InitStructure.CAN_BS2       = CAN_BS2_8tq;
                CAN_InitStructure.CAN_Prescaler = 4;
            }
            else if(CAN_BPS == CAN_BAUDRATE_1MBPS)
            {
                CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
                CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
                CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
                CAN_InitStructure.CAN_Prescaler = 4;
            }
            else if(CAN_BPS == CAN_BAUDRATE_250KBPS)
            {
                CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
                CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
                CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
                CAN_InitStructure.CAN_Prescaler = 4;
            }
            else if(CAN_BPS == CAN_BAUDRATE_125KBPS)
            {
                CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
                CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
                CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
                CAN_InitStructure.CAN_Prescaler = 4;
            }


            CAN_Init(CAN1, &CAN_InitStructure); //初始化CAN1
            /*-------------------- CAN Fileter Configuuration ---------------------*/
            /** 
             * configuration statement:
             * 1. configurate CAN in standard ID mode.
             * 2. filter run in ID marsk mode.              CAN_FxR1
             * 3. accept standard ID range between 0x0324 to 0x0327.
             * 4. CAN default speed configurate to 500Kbps. 
            */
            CAN_FilterInitTypeDef CAN_FilterInitStructure;
            CAN_FilterInitStructure.CAN_FilterNumber         = 0;                                                               //过滤器0
            CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;                                             //屏蔽位模式
            CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;                                            //32位宽
            CAN_FilterInitStructure.CAN_FilterIdHigh         = ((CAN_station_ID << 21) & 0xFFFF0000) >> 16;                     //32位ID
            CAN_FilterInitStructure.CAN_FilterIdLow          = ((CAN_station_ID << 21) | CAN_ID_STD | CAN_RTR_DATA) & 0xffff;
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;                                                      //0xfff8;   //32位MASK
            CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;                                                       //0xffff;
            CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;                                        //过滤器0关联到FIFO0
            CAN_FilterInitStructure.CAN_FilterActivation     =  ENABLE;                                                      //激活过滤器0
            CAN_FilterInit(&CAN_FilterInitStructure);                                                                   //滤波器初始化

            /*--------------------- CAN NVIC Configuration ----------------------*/

            NVIC_InitTypeDef NVIC_InitStructure;
            CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);                                                                    //FIFO0消息挂号中断允许.
            NVIC_InitStructure.NVIC_IRQChannel                   = CAN1_RX0_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                                                   // 主优先级为0
            NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;                                                          // 次优先级为1
            NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
        }
        /*----------------- CAN2 Configuration ------------------------------------------*/
        if (CANx == CAN2)
        {
        //     CAN_DeInit(CAN2);
        // #ifdef AC200
        //     /*------------------------- CAN GPIO CONFIG---------------------------------*/
        //     /* CAN Periph clock enable */
        //     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        //     /** open CAN clock */
        //     RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); // can clock

        //     GPIO_InitTypeDef GPIO_InitStruction;
        //     /** Configure CAN RX pin */
        //     GPIO_InitStruction.GPIO_Pin = GPIO_Pin_12;
        //     GPIO_InitStruction.GPIO_Mode = GPIO_Mode_IPU;
        //     GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        //     GPIO_Init(GPIOB, &GPIO_InitStruction);
        //     /** Configure CAN TX pin */
        //     GPIO_InitStruction.GPIO_Pin = GPIO_Pin_13;
        //     GPIO_InitStruction.GPIO_Mode = GPIO_Mode_AF_PP;
        //     GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        //     GPIO_Init(GPIOB, &GPIO_InitStruction);

        //     /*------------------------- CAN MODE CONFIG---------------------------------*/
        //     CAN_InitTypeDef CAN_InitStructure;
        //     //CAN单元设置
        //     CAN_InitStructure.CAN_TTCM = DISABLE;     //非时间触发通信模式
        //     CAN_InitStructure.CAN_ABOM = DISABLE;     //软件自动离线管理
        //     CAN_InitStructure.CAN_AWUM = DISABLE;     //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
        //     CAN_InitStructure.CAN_NART = DISABLE;     //禁止报文自动传送
        //     CAN_InitStructure.CAN_RFLM = DISABLE;     //报文不锁定,新的覆盖旧的
        //     CAN_InitStructure.CAN_TXFP = ENABLE;      //优先级由报文标识符决定
        //     CAN_InitStructure.CAN_Mode = CAN_ModeSet; //模式设置： mode: 0,普通模式;1,回环模式;
        //     //设置波特率
        //     CAN_InitStructure.CAN_SJW = CAN_SJW_TimeSet;         //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq     CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
        //     CAN_InitStructure.CAN_BS1 = CAN_BS1_TimeSet;         //Tbs1 = tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
        //     CAN_InitStructure.CAN_BS2 = CAN_BS2_TimeSet;         //Tbs2 = tbs2+1个时间单位CAN_BS2_1tq ~    CAN_BS2_8tq
        //     CAN_InitStructure.CAN_Prescaler = CAN_Prescaler_Set; //分频系数(Fdiv)为brp+1
        //     CAN_Init(CAN1, &CAN_InitStructure);                  //初始化CAN1

        //     /*-------------------- CAN Fileter Configuuration ---------------------*/
        //     CAN_FilterInitTypeDef CAN_FilterInitStructure;
        //     CAN_FilterInitStructure.CAN_FilterNumber = 0;                    //过滤器0
        //     CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;  //屏蔽位模式
        //     CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; //32位宽
        //     CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;               //32位ID
        //     CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
        //     CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //32位MASK
        //     CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
        //     CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //过滤器0关联到FIFO0
        //     CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;               //激活过滤器0
        //     CAN_FilterInit(&CAN_FilterInitStructure);
        // #endif
    /*--------------------- CAN NVIC Configuration ----------------------*/
        //NVIC_InitTypeDef NVIC_InitStructure;
        //CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //FIFO0消息挂号中断允许.
        //NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
        //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 主优先级为1
        //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 次优先级为0
        //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        //NVIC_Init(&NVIC_InitStructure);
        }
    }
#endif



/**
 *@brief: CAN1 received message interrupt
 */
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
    CAN_Receive(CAN1, 0, &RxMessage);
    CAN_DataCopy(&RxMessage);
}


/**
 *    [Can_Send_Message description]
 *
 *    @param  struct type about CAN_SendData, @refer to CAN_SendMessage_TypeDef in file "can.h" 
 *    @return  return 0: message send success
 *                    1: message send failed
 */
ErrorStatus CAN_Send_Message(CAN_SendMessage_TypeDef *CAN_SendData)
{
    uint8_t     mbox;
    CanTxMsg    TxMessage;
    uint16_t tickCounter =  0;
    TxMessage.StdId    = CAN_SendData->CAN_Standard_ID_No;              // standard ID set
    // TxMessage.ExtId = CAN_SendData->CAN_ExtenID;                     // extern ID set
    TxMessage.IDE      = CAN_Id_Standard;                               // defien CAN sent ID mode， TxMessage.IDE = CAN_Id_Standard;                               // defien CAN sent ID mode
    TxMessage.RTR      = CAN_RTR_Data;                                  // define CAN data frame
    TxMessage.DLC      = CAN_SendData ->CAN_SendDataLength;             // define how large data wil be send
    for (tickCounter =  0; tickCounter < (CAN_SendData ->CAN_SendDataLength); tickCounter++)
        {
            TxMessage.Data[tickCounter] = CAN_SendData ->CAN_DATA[tickCounter];
        }
    mbox =  CAN_Transmit(CAN1, &TxMessage);
    while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (tickCounter < 0XFFF))
        tickCounter++;                                                  // waitting send success
    if (tickCounter >=  0XFFF)
        return ERROR;
    return SUCCESS;
}
/**
 * @brief: copy data and CAN ID to CAN_Rec_Buff from System can data. 
 */
void CAN_DataCopy(CanRxMsg *CAN_Rec)
{
    if (buffIndexEnd == 9)
    {

        CAN_SendMessage.CAN_SendDataLength = 8;
        CAN_SendMessage.CAN_DATA[0] = AC200_Busy;
        CAN_SendMessage.CAN_DATA[1] = 0x00;
        CAN_SendMessage.CAN_DATA[2] = 0x00;
        CAN_SendMessage.CAN_DATA[3] = 0x00;
        CAN_SendMessage.CAN_DATA[4] = 0x00;
        CAN_SendMessage.CAN_DATA[5] = 0x00;
        CAN_SendMessage.CAN_DATA[6] = 0x00;
        CAN_SendMessage.CAN_DATA[7] = 0x00;
        CAN_Send_Message(&CAN_SendMessage);
    }
    else if(buffIndexEnd == 0)
    {
        CAN_Rec_Buff[0].emptyIndex                 = 1;
        CAN_Rec_Buff[0].MasterCMD_ID               = ((uint16_t)CAN_Rec->StdId & 0x7ff); // STM32 CAN ID Max 0x7fff
        CAN_Rec_Buff[0].MasterCMD_ActionSortNumber = ((uint16_t)CAN_Rec->Data[0] << 8) | ((uint16_t)CAN_Rec->Data[1]);
        CAN_Rec_Buff[0].MasterCMD_Reserved         = CAN_Rec->Data[2];
        CAN_Rec_Buff[0].MasterCMD_DataLength       = CAN_Rec->Data[3];
        CAN_Rec_Buff[0].MasterCMD_Param_1          = CAN_Rec->Data[4];
        CAN_Rec_Buff[0].MasterCMD_Param_2          = CAN_Rec->Data[5];
        CAN_Rec_Buff[0].MasterCMD_Param_3          = CAN_Rec->Data[6];
        CAN_Rec_Buff[0].MasterCMD_Param_4          = CAN_Rec->Data[7];

        buffIndexEnd = 1;
    }
    else
    {
        CAN_Rec_Buff[buffIndexEnd].emptyIndex                 = 1;
        CAN_Rec_Buff[buffIndexEnd].MasterCMD_ID               = ((uint16_t)CAN_Rec->StdId & 0x7ff); // STM32 CAN ID Max 0x7fff
        CAN_Rec_Buff[buffIndexEnd].MasterCMD_ActionSortNumber = ((uint16_t)CAN_Rec->Data[0] << 8) | ((uint16_t)CAN_Rec->Data[1]);
        CAN_Rec_Buff[buffIndexEnd].MasterCMD_Reserved         = CAN_Rec->Data[2];
        CAN_Rec_Buff[buffIndexEnd].MasterCMD_DataLength       = CAN_Rec->Data[3];
        CAN_Rec_Buff[buffIndexEnd].MasterCMD_Param_1          = CAN_Rec->Data[4];
        CAN_Rec_Buff[buffIndexEnd].MasterCMD_Param_2          = CAN_Rec->Data[5];
        CAN_Rec_Buff[buffIndexEnd].MasterCMD_Param_3          = CAN_Rec->Data[6];
        CAN_Rec_Buff[buffIndexEnd].MasterCMD_Param_4          = CAN_Rec->Data[7];

        buffIndexEnd += 1;
    }
}
/**
 * @brief: Sort CAN received buff after one action is done.
 * @param: None
 * @return: None
 */
void CAN_RecDataSort(void)
{
    uint8_t x = 0;
    
    for(; x < buffIndexEnd; x++)
    {
        CAN_Rec_Buff[x] = CAN_Rec_Buff[x+1];
    }
    if(buffIndexEnd > 0)
        buffIndexEnd = buffIndexEnd - 1;
    memset(&CAN_Rec_Buff[x+1], 0, sizeof(CAN_Rec_Buff[x+1]));
    
}
/**
 * @brief: Analysis CAN Received data, Classify them by Received CAN Standard ID.
 * @param: ST fireware library defined Rec data function.
 * @return: return the analysis answer, success or error.
 */
ErrorStatus CAN_RecAnalysis(void)
{
    ErrorStatus  CAN_Rec_Flag;
    switch (CAN_Rec_Buff[0].MasterCMD_ID)
    {
		default:
			CAN_Rec_Flag = ERROR;
		break;
        case(MasterCMD):
        {
            CAN_Rec_Flag = SUCCESS;
			CAN_ActionAnalysis_MasterCMD();
        }break;
        case(MasterCMD_ENCO):
        {
            CAN_ActionAnalysis_ENCO();
        }
        case(MasterCMD_MOTOR):
        {
            CAN_Rec_Flag = SUCCESS;
			CAN_ActionAnalysis_Motor();
        }break;
        case(MasterCMD_IO):
        {
            CAN_Rec_Flag = SUCCESS;
			CAN_ActionAnalysis_IO();
        }break;
        case(MasterCMD_Search):
        {
            CAN_Rec_Flag = SUCCESS;
			CAN_ActionAnalysis();
        }break;
        case(MasterCMD_Data):
        {
            CAN_Rec_Flag = SUCCESS;
			CAN_ActionAnalysis();
        }break;
        case(MasterCMD_HoldOn):
        {
            CAN_Rec_Flag = SUCCESS;
			CAN_ActionAnalysis();
        }break;
        case(MasterCMD_Recover):
        {
            CAN_Rec_Flag = SUCCESS;
			CAN_ActionAnalysis();
        }break;
    }
    CAN_RecDataSort();  //sort data

    
    #ifdef AC200_DEBUG
    if (CAN_Rec_Flag == SUCCESS)
        Blink(CAN_RecMode, 100);
    #endif
    return CAN_Rec_Flag;
}
/**
 * @brief: 
 */
void CAN_ActionAnalysis_MasterCMD(void)
{
    memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
    CAN_RecDataSort();  //sort data
}

/**
 * @brief: 
 */
void CAN_ActionAnalysis_Motor(void)
{
    ErrorStatus     CAN_ActionAnalysisFlag;
    CAN_ActionAnalysisFlag = ERROR;
    uint8_t analysisTemp = 0;
    switch (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber)
    {
        default:
            CAN_ActionAnalysisFlag = ERROR;
            CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
            CAN_SendMessage.CAN_SendDataLength = 8;
            CAN_SendMessage.CAN_DATA[0] = AC200_ActionAnalysisError;
            CAN_SendMessage.CAN_DATA[1] = (0xff & (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
            CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
            CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
            CAN_SendMessage.CAN_DATA[4] = 0x00;
            CAN_SendMessage.CAN_DATA[5] = 0x00;
            CAN_SendMessage.CAN_DATA[6] = 0x00;
            CAN_SendMessage.CAN_DATA[7] = 0x00;
            CAN_Send_Message(&CAN_SendMessage);
            memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            break;
        case(AC200_MotorBreak):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case(AC200_MotorStop):
        {
            DC_MOTOR_Stop();

            CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
            CAN_SendMessage.CAN_SendDataLength = 8;
            CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
            CAN_SendMessage.CAN_DATA[1] = (0xff & (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
            CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
            CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
            CAN_SendMessage.CAN_DATA[4] = 0x00;
            CAN_SendMessage.CAN_DATA[5] = 0x00;
            CAN_SendMessage.CAN_DATA[6] = 0x00;
            CAN_SendMessage.CAN_DATA[7] = 0x00;
            CAN_Send_Message(&CAN_SendMessage);
            memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
        }break;
        case (AC200_MotorShutdown):
        {
            CAN_ActionAnalysisFlag = DC_MOTOR_Shutdown();
            if (CAN_ActionAnalysisFlag == SUCCESS)
            {
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
            }
            else
            {
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
            }
            CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
            CAN_SendMessage.CAN_SendDataLength = 8;
            CAN_SendMessage.CAN_DATA[1] = (0xff && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
            CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
            CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
            CAN_SendMessage.CAN_DATA[4] = 0x00;
            CAN_SendMessage.CAN_DATA[5] = 0x00;
            CAN_SendMessage.CAN_DATA[6] = 0x00;
            CAN_SendMessage.CAN_DATA[7] = 0x00;
            CAN_Send_Message(&CAN_SendMessage);
            memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
        }break;
        case (AC200_MotorReady):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_MotorStart):
        {
            /** 开启后定时器TIM2 采样 ------------------------------*/
//            Timmer_TIM2(ENABLE);
            
            /** parameter 1 for motor dir set */
            /** parameter 2 for motor speed set */
            CAN_ActionAnalysisFlag = DC_MOTOR_Start(CAN_Rec_Buff[0].MasterCMD_Param_1, CAN_Rec_Buff[0].MasterCMD_Param_2);
            
            if (CAN_ActionAnalysisFlag == SUCCESS)
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
            else
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;

            CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
            CAN_SendMessage.CAN_SendDataLength = 8;
            CAN_SendMessage.CAN_DATA[1] = (0xff & (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
            CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
            CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
            CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
            CAN_SendMessage.CAN_DATA[5] = CAN_Rec_Buff[0].MasterCMD_Param_2;
            CAN_SendMessage.CAN_DATA[6] = 0x00;
            CAN_SendMessage.CAN_DATA[7] = 0x00;
            CAN_Send_Message(&CAN_SendMessage);
            memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
        }break;
        case (AC200_MotorRun):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_MotorSpeedUp):
        {
            analysisTemp = DC_MOTOR_SpeedUp(CAN_Rec_Buff[0].MasterCMD_Param_1);
            if (analysisTemp == SUCCESS)
            {
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
            }
            else
            {
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
            }
            CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
            CAN_SendMessage.CAN_SendDataLength = 8;
            CAN_SendMessage.CAN_DATA[1] = (0xff & (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
            CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
            CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
            CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
            CAN_SendMessage.CAN_DATA[5] = 0x00;
            CAN_SendMessage.CAN_DATA[6] = 0x00;
            CAN_SendMessage.CAN_DATA[7] = 0x00;
            CAN_Send_Message(&CAN_SendMessage);
            memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
        }break;
        case (AC200_MotorReduceSpeed):
        {
            analysisTemp = DC_MOTOR_ReduceSpeed(CAN_Rec_Buff[0].MasterCMD_Param_1);
            if (analysisTemp == SUCCESS)
            {
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
            }
            else
            {
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
            }
            CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
            CAN_SendMessage.CAN_SendDataLength = 8;
            CAN_SendMessage.CAN_DATA[1] = (0xff & (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
            CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
            CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
            CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
            CAN_SendMessage.CAN_DATA[5] = 0x00;
            CAN_SendMessage.CAN_DATA[6] = 0x00;
            CAN_SendMessage.CAN_DATA[7] = 0x00;
            CAN_Send_Message(&CAN_SendMessage);
            memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
        }break;
        case (AC200_MotorChangeDir):
        {
            /**-----------------------------------------
             * Forwared : 0;
             * Reversed : !0;
             */
            DC_MOTOR_DirectionSet(CAN_Rec_Buff[0].MasterCMD_Param_1, CAN_Rec_Buff[0].MasterCMD_Param_2);
                
            CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
            CAN_SendMessage.CAN_SendDataLength = 8;
            CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
            CAN_SendMessage.CAN_DATA[1] = (0xff & (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
            CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
            CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
            CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
            CAN_SendMessage.CAN_DATA[5] = 0x00;
            CAN_SendMessage.CAN_DATA[6] = 0x00;
            CAN_SendMessage.CAN_DATA[7] = 0x00;
            CAN_Send_Message(&CAN_SendMessage);
            memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
        }break;
        case (AC200_MotorStartSpeedSet):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_MotorMaxSpeedSet):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_MotorMinSpeedSet):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_MotorAcceSet):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_MotorMax_PWM_DutyCycleSet):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_MotorMin_PWM_DutyCycleSet):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_Motor_PWM_FreqSet):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_Motor_SelfCheck):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
        case (AC200_Motor_SpeedCheck):
        {
            CAN_ActionAnalysisFlag = SUCCESS;
        }break;
    }
    CAN_RecDataSort();  //sort data
}

/**
 * @brief: 
 */
void CAN_ActionAnalysis_ENCO(void)
{
    ErrorStatus     CAN_ActionAnalysisFlag;
    CAN_ActionAnalysisFlag = ERROR;
    switch (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber)
    {
        default:
        {
            CAN_ActionAnalysisFlag = ERROR;
            CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
            CAN_SendMessage.CAN_SendDataLength = 8;
            CAN_SendMessage.CAN_DATA[0] = AC200_ActionAnalysisError;
            CAN_SendMessage.CAN_DATA[1] = 0x40;
            CAN_SendMessage.CAN_DATA[2] = 0x01;
            CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
            CAN_SendMessage.CAN_DATA[4] = 0x00;
            CAN_SendMessage.CAN_DATA[5] = 0x00;
            CAN_SendMessage.CAN_DATA[6] = 0x00;
            CAN_SendMessage.CAN_DATA[7] = 0x00;
            CAN_Send_Message(&CAN_SendMessage);
            memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
        }break;
        case(AC200_ENCO_ON):
        {
            ;
        }break;
        case (AC200_ENCO_OFF):
        {
            ;
        }break;
        case (AC200_ENCO_SelfCheck):
        {
            ;
        }break;
        case (AC200_ENCO_DataRead):
        {
            CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
            CAN_SendMessage.CAN_SendDataLength = 8;
            CAN_SendMessage.CAN_DATA[0] = AC200_ActionAnalysisReserved;
            CAN_SendMessage.CAN_DATA[1] = 0xff & (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8);
            CAN_SendMessage.CAN_DATA[2] = 0xff & CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
            CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
            CAN_SendMessage.CAN_DATA[4] = 0xff;
            CAN_SendMessage.CAN_DATA[5] = 0xff;
            CAN_SendMessage.CAN_DATA[6] = 0xff;
            CAN_SendMessage.CAN_DATA[7] = 0xff;
            CAN_Send_Message(&CAN_SendMessage);
            memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
        }break;
    }
    CAN_RecDataSort();  //sort data
}
/**
 * @brief: 
 */
void CAN_ActionAnalysis_IO(void)
{
    ErrorStatus     CAN_ActionAnalysisFlag;
    CAN_ActionAnalysisFlag = ERROR;
    if ((CAN_Rec_Buff[0].MasterCMD_ActionSortNumber > AC200_Read_IO_Part_Start) && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber < AC200_Read_IO_Part_End))
    {
        uint8_t readValueTemp = 0xff;
        switch (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber)
        {
            default:
            {
                CAN_ActionAnalysisFlag = ERROR;
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionAnalysisError;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Read_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
                
            }break;
            case (AC200_Read_IO_1):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(1);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Read_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Read_IO_1 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_2):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(2);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Read_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Read_IO_2 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_3):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(3);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Read_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Read_IO_3 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_4):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(4);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Read_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Read_IO_4 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_5):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(5);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Read_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Read_IO_5 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_6):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(6);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Read_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Read_IO_6 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
        }
    }
    else if ((CAN_Rec_Buff[0].MasterCMD_ActionSortNumber > AC200_Write_IO_Part_Start) && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber < AC200_Write_IO_Part_End))
    {
        switch (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber)
        {
            default:
            {
                CAN_ActionAnalysisFlag = ERROR;
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = ((CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[1] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber & 0xff;
                CAN_SendMessage.CAN_DATA[2] = 0x00;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = 0x00;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;

            case (AC200_Write_IO_1):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                CAN_ActionAnalysisFlag = WriteSinglePin(1, CAN_Rec_Buff[0].MasterCMD_Param_1);
                if (ERROR == CAN_ActionAnalysisFlag)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Write_IO_1 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case(AC200_Write_IO_2):
            {
                CAN_ActionAnalysisFlag = SUCCESS;

				CAN_ActionAnalysisFlag = WriteSinglePin(2, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Write_IO_2 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Write_IO_3): 
            {
                CAN_ActionAnalysisFlag = SUCCESS;
				CAN_ActionAnalysisFlag = WriteSinglePin(3, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Write_IO_3 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Write_IO_4):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
				CAN_ActionAnalysisFlag = WriteSinglePin(4, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Write_IO_4 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Write_IO_5):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
				CAN_ActionAnalysisFlag = WriteSinglePin(5, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Write_IO_5 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case(AC200_Write_IO_6):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
				CAN_ActionAnalysisFlag = WriteSinglePin(6, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = (AC200_Write_IO_6 & 0xff);
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
        }

    }
    else
        CAN_ActionAnalysisFlag = ERROR;
    /** sort data ------------------------------------------------*/    
    CAN_RecDataSort();  //sort data
}
/**
 * @brief: function to Analysis the data which CAN BUS received.
 * @param: NULL
 * @return: ErrorStatus, success or ERROR.
 */
ErrorStatus CAN_ActionAnalysis(void)
{
    ErrorStatus     CAN_ActionAnalysisFlag;
    CAN_ActionAnalysisFlag = ERROR;
    if ((CAN_Rec_Buff[0].MasterCMD_ActionSortNumber > AC200_Motor_Part_Start) && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber < AC200_Motor_Part_End))
    {
        uint8_t analysisTemp = 0;
        switch (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber)
        {
			default:
				CAN_ActionAnalysisFlag = ERROR;
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionAnalysisError;
                CAN_SendMessage.CAN_DATA[1] = (0xff && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
                CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = 0x00;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
                break;
            case(AC200_MotorBreak):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case(AC200_MotorStop):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_MotorShutdown):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_MotorReady):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_MotorStart):
            {

                /** parameter 1 for motor dir set */
                /** parameter 2 for motor speed set */
                DC_MOTOR_Periph(DISABLE);
                DC_MOTOR_Start(CAN_Rec_Buff[0].MasterCMD_Param_1, CAN_Rec_Buff[0].MasterCMD_Param_2);
//                DC_MOTOR_Periph(ENABLE);
//                //TIM_Cmd(TIM1, ENABLE);
//               // DC_MOTOR_Ready(ENABLE);
                
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                CAN_SendMessage.CAN_DATA[1] = (0xff && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
                CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = CAN_Rec_Buff[0].MasterCMD_Param_2;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_MotorRun):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_MotorSpeedUp):
            {
                analysisTemp = DC_MOTOR_SpeedUp(CAN_Rec_Buff[0].MasterCMD_Param_1);
                if (analysisTemp == SUCCESS)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = (0xff && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
                CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_MotorReduceSpeed):
            {
                analysisTemp = DC_MOTOR_ReduceSpeed(CAN_Rec_Buff[0].MasterCMD_Param_1);
                if (analysisTemp == SUCCESS)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = (0xff && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
                CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_MotorChangeDir):
            {
                /**
                 * Forwared : 0;
                 * Reversed : !0;
                 */
                DC_MOTOR_DirectionSet(CAN_Rec_Buff[0].MasterCMD_Param_1, CAN_Rec_Buff[0].MasterCMD_Param_2);
                if (analysisTemp == SUCCESS)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = (0xff && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8));
                CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_MotorStartSpeedSet):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_MotorMaxSpeedSet):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_MotorMinSpeedSet):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_MotorAcceSet):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_MotorMax_PWM_DutyCycleSet):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_MotorMin_PWM_DutyCycleSet):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_Motor_PWM_FreqSet):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_Motor_SelfCheck):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
            case (AC200_Motor_SpeedCheck):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
            }break;
        }
    }
    else if ((CAN_Rec_Buff[0].MasterCMD_ActionSortNumber > AC200_ENCO_Part_Start) && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber < AC200_ENCO_Part_End))
    {
        CAN_ActionAnalysisFlag = SUCCESS;
        switch (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber)
        {
            default:
            {
                CAN_ActionAnalysisFlag = ERROR;
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionAnalysisError;
                CAN_SendMessage.CAN_DATA[1] = 0x40;
                CAN_SendMessage.CAN_DATA[2] = 0x01;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = 0x00;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case(AC200_ENCO_ON):
            case (AC200_ENCO_OFF):
            case (AC200_ENCO_SelfCheck):
            case (AC200_ENCO_DataRead):
            {
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionAnalysisReserved;
                CAN_SendMessage.CAN_DATA[1] = 0xff && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8);
                CAN_SendMessage.CAN_DATA[2] = 0xff && CAN_Rec_Buff[0].MasterCMD_ActionSortNumber;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = 0xff;
                CAN_SendMessage.CAN_DATA[5] = 0xff;
                CAN_SendMessage.CAN_DATA[6] = 0xff;
                CAN_SendMessage.CAN_DATA[7] = 0xff;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
        }
    }
    else if ((CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >= AC200_Read_IO_Part_Start) && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber < AC200_Read_IO_Part_End))
    {
        uint8_t readValueTemp = 0xff;
        switch (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber)
        {
            default:
            {
                CAN_ActionAnalysisFlag = ERROR;
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionAnalysisError;
                CAN_SendMessage.CAN_DATA[1] = 0x40;
                CAN_SendMessage.CAN_DATA[2] = 0x01;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = 0x00;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
                
            }break;
            case (AC200_Read_IO_1):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(1);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = 0x40;
                CAN_SendMessage.CAN_DATA[2] = 0x01;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_2):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(2);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = 0x40;
                CAN_SendMessage.CAN_DATA[2] = 0x02;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_3):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(3);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = 0x40;
                CAN_SendMessage.CAN_DATA[2] = 0x03;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_4):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(4);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = 0x40;
                CAN_SendMessage.CAN_DATA[2] = 0x04;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_5):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(5);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = 0x40;
                CAN_SendMessage.CAN_DATA[2] = 0x05;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Read_IO_6):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                readValueTemp = ReadSinglePin(6);
                if (0xff == readValueTemp)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = 0x40;
                CAN_SendMessage.CAN_DATA[2] = 0x06;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = readValueTemp;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
        }
    }
    else if ((CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >= AC200_Write_IO_Part_Start) && (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber < AC200_Write_IO_Part_End))
    {
        switch (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber)
        {
            default:
            {
                CAN_ActionAnalysisFlag = ERROR;
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = AC200_ActionAnalysisError;
                CAN_SendMessage.CAN_DATA[1] = ((CAN_Rec_Buff[0].MasterCMD_ActionSortNumber) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = CAN_Rec_Buff[0].MasterCMD_ActionSortNumber & 0xff;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = 0x00;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;

            case (AC200_Write_IO_1):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
                CAN_ActionAnalysisFlag = WriteSinglePin(1, CAN_Rec_Buff[0].MasterCMD_Param_1);
                if (ERROR == CAN_ActionAnalysisFlag)
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
                }
                else
                {
                    CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
                }
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = AC200_Write_IO_1 & 0xff;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case(AC200_Write_IO_2):
            {
                CAN_ActionAnalysisFlag = SUCCESS;

				CAN_ActionAnalysisFlag = WriteSinglePin(2, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = AC200_Write_IO_2 & 0xff;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Write_IO_3): 
            {
                CAN_ActionAnalysisFlag = SUCCESS;
				CAN_ActionAnalysisFlag = WriteSinglePin(3, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = AC200_Write_IO_3 & 0xff;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Write_IO_4):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
				CAN_ActionAnalysisFlag = WriteSinglePin(4, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = AC200_Write_IO_4 & 0xff;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case (AC200_Write_IO_5):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
				CAN_ActionAnalysisFlag = WriteSinglePin(5, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = AC200_Write_IO_5 & 0xff;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
            case(AC200_Write_IO_6):
            {
                CAN_ActionAnalysisFlag = SUCCESS;
				CAN_ActionAnalysisFlag = WriteSinglePin(6, CAN_Rec_Buff[0].MasterCMD_Param_1);
				if(ERROR == CAN_ActionAnalysisFlag)
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionError;
				}
				else
				{
					CAN_SendMessage.CAN_DATA[0] = AC200_ActionDone;
				}
                CAN_SendMessage.CAN_Standard_ID_No = MasterCMD_IO;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[1] = ((AC200_Write_IO_Part_Start >> 8) & 0xff);
                CAN_SendMessage.CAN_DATA[2] = AC200_Write_IO_6 & 0xff;
                CAN_SendMessage.CAN_DATA[3] = RESERVED_NUMBER;
                CAN_SendMessage.CAN_DATA[4] = CAN_Rec_Buff[0].MasterCMD_Param_1;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
            }break;
        }

    }
    else
        CAN_ActionAnalysisFlag = ERROR;
    /** sort data ------------------------------------------------*/    
    CAN_RecDataSort();  //sort data

    return CAN_ActionAnalysisFlag;
}

/**
 * @brief: AC200 system check protocols
 * @param: NULL
 * @return: return check status, SUCCESS or ERROR.
 */
ErrorStatus AC200_System_Check(void)
{
    return SUCCESS;
}

/**
 * @brief: AC200 System Init Handshake Protocol.
 *         When power on and system checked Init Protocol will send message to Master Device through CAN BUS.
 * @param: NULL
 * @return: NULL
 */
void AC200_Init_Protocol(void)
{
        ;
}

/**
 * 
 */
void CAN_DataMerge(void)
{
    //    CAN_Rec_Buff[1].MasterCMD_ID_H = 0x01;
}

/*-------------------- function point --------------------------*/
// pFunc funcitonPoint(uint16_t x)
// {
//     ;
// }

/*-------------------- function point end ----------------------*/


/*-------------------------------------- END LINE -----------------------------------------*/




