/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_sdio.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define DMAx_CHANNEL      DMA_Channel_4
#define DMAx_STREAM       DMA2_Stream3
#define DMAx_FLAG_FEIF    DMA_FLAG_FEIF3
#define DMAx_FLAG_DMEIF   DMA_FLAG_DMEIF3
#define DMAx_FLAG_TEIF    DMA_FLAG_TEIF3
#define DMAx_FLAG_HTIF    DMA_FLAG_HTIF3
#define DMAx_FLAG_TCIF    DMA_FLAG_TCIF3
#define DMAx_IRQn         DMA2_Stream3_IRQn
/*=====================================================================================================*/
/*=====================================================================================================*/
#define SDIO_STATIC_FLAGS               ((u32)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((u32)0x00010000)

#define SD_OCR_ADDR_OUT_OF_RANGE        ((u32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((u32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((u32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((u32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((u32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((u32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((u32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((u32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((u32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((u32)0x00200000)
#define SD_OCR_CC_ERROR                 ((u32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((u32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((u32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((u32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((u32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((u32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((u32)0x00004000)
#define SD_OCR_ERASE_RESET              ((u32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((u32)0x00000008)
#define SD_OCR_ERRORBITS                ((u32)0xFDFFE008)

#define SD_R6_GENERAL_UNKNOWN_ERROR     ((u32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((u32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((u32)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((u32)0x80100000)
#define SD_HIGH_CAPACITY                ((u32)0x40000000)
#define SD_STD_CAPACITY                 ((u32)0x00000000)
#define SD_CHECK_PATTERN                ((u32)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((u32)0x0000FFFF)
#define SD_ALLZERO                      ((u32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((u32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((u32)0x00010000)
#define SD_CARD_LOCKED                  ((u32)0x02000000)

#define SD_DATATIMEOUT                  ((u32)0xFFFFFFFF)
#define SD_0TO7BITS                     ((u32)0x000000FF)
#define SD_8TO15BITS                    ((u32)0x0000FF00)
#define SD_16TO23BITS                   ((u32)0x00FF0000)
#define SD_24TO31BITS                   ((u32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((u32)0x01FFFFFF)

#define SD_HALFFIFO                     ((u32)0x00000008)
#define SD_HALFFIFOBYTES                ((u32)0x00000020)

#define SD_CCCC_LOCK_UNLOCK             ((u32)0x00000080)
#define SD_CCCC_WRITE_PROT              ((u32)0x00000040)
#define SD_CCCC_ERASE                   ((u32)0x00000020)

#define SDIO_SEND_IF_COND               ((u32)0x00000008)
/*=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error CmdError( void );
static SD_Error CmdResp1Error( u8 cmd );
static SD_Error CmdResp7Error( void );
static SD_Error CmdResp3Error( void );
static SD_Error CmdResp2Error( void );
static SD_Error CmdResp6Error( u8 cmd, u16 *prca );
static SD_Error SDEnWideBus( FunctionalState NewState );
static SD_Error IsCardProgramming( u8 *pstatus );
static SD_Error FindSCR( u16 rca, u32 *pscr );
/*=====================================================================================================*/
/*=====================================================================================================*/
SD_CardInfo SDCardInfo;

static u32 CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1;
static u32 CSD_Tab[4], CID_Tab[4], RCA = 0;
static u8 SDSTATUS_Tab[16];
__IO u32 StopCondition = 0;
__IO SD_Error TransferError = SD_OK;
__IO u32 TransferEnd = 0, DMAEndOfTransfer = 0;
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_Init
**功能 : Initialize
**輸入 : None
**輸出 : status
**使用 : status = SD_Init();
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_Init( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  SDIO_InitTypeDef SDIO_InitStruct;

  __IO SD_Error errorstatus = SD_OK;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);

  /* NVIC */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStruct.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  NVIC_InitStruct.NVIC_IRQChannel = DMAx_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStruct);

  /* GPIO */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8,  GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,  GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2,  GPIO_AF_SDIO);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  errorstatus = SD_PowerON();
  if(errorstatus != SD_OK)
    return(errorstatus);

  errorstatus = SD_InitializeCards();
  if(errorstatus != SD_OK)
    return(errorstatus);

  /*!< Configure the SDIO peripheral */
  /*!< SDIO_CK = SDIOCLK / (SDIO_TRANSFER_CLK_DIV + 2) */
  /*!< on STM32F4xx devices, SDIOCLK is fixed to 48MHz */
  SDIO_InitStruct.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;
  SDIO_InitStruct.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStruct.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStruct.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStruct.SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStruct.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
  SDIO_Init(&SDIO_InitStruct);

  errorstatus = SD_GetCardInfo(&SDCardInfo);
  if(errorstatus == SD_OK)
    errorstatus = SD_SelectDeselect((u32)(SDCardInfo.RCA << 16));
  if(errorstatus == SD_OK)
    errorstatus = SD_EnableWideBusOperation(SDIO_BusWide_4b);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_GetStatus
**功能 : 讀取 Status
**輸入 : None
**輸出 : Status
**使用 : Status = SD_GetStatus();
**=====================================================================================================*/
/*=====================================================================================================*/
SDTransferState SD_GetStatus( void )
{
  SDCardState cardstate =  SD_CARD_TRANSFER;

  cardstate = SD_GetState();

  if(cardstate == SD_CARD_TRANSFER)
    return(SD_TRANSFER_OK);
  else if(cardstate == SD_CARD_ERROR)
    return(SD_TRANSFER_ERROR);
  else
    return(SD_TRANSFER_BUSY);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_GetState
**功能 : 讀取 State
**輸入 : None
**輸出 : State
**使用 : State = SD_GetState();
**=====================================================================================================*/
/*=====================================================================================================*/
SDCardState SD_GetState( void )
{
  u32 resp = 0;

  if(SD_Detect()== SD_PRESENT) {
    if(SD_SendStatus(&resp) != SD_OK)
      return SD_CARD_ERROR;
    else
      return (SDCardState)((resp >> 9) & 0x0F);
  }
  else
    return SD_CARD_ERROR;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_Detect
**功能 : Detect
**輸入 : None
**輸出 : 
**使用 : SD_Detect()
**=====================================================================================================*/
/*=====================================================================================================*/
u8 SD_Detect( void )
{
  return SD_PRESENT;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_PowerON
**功能 : Power ON
**輸入 : None
**輸出 : status
**使用 : status = SD_PowerON();
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_PowerON( void )
{
  SDIO_InitTypeDef SDIO_InitStruct;
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;

  __IO SD_Error errorstatus = SD_OK;
  u32 response = 0, count = 0, validvoltage = 0;
  u32 SDType = SD_STD_CAPACITY;

  /*!< Power ON Sequence -----------------------------------------------------*/
  /*!< Configure the SDIO peripheral */
  /*!< SDIO_CK = SDIOCLK / (SDIO_INIT_CLK_DIV + 2) */
  /*!< on STM32F4xx devices, SDIOCLK is fixed to 48MHz */
  /*!< SDIO_CK for initialization should not exceed 400 KHz */
  SDIO_InitStruct.SDIO_ClockDiv = SDIO_INIT_CLK_DIV;
  SDIO_InitStruct.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStruct.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStruct.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStruct.SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStruct.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
  SDIO_Init(&SDIO_InitStruct);

  SDIO_SetPowerState(SDIO_PowerState_ON);
  SDIO_ClockCmd(ENABLE);

  SDIO_CmdInitStruct.SDIO_Argument = 0x0;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_GO_IDLE_STATE;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_No;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdError();
  if(errorstatus != SD_OK)
    return(errorstatus);

  SDIO_CmdInitStruct.SDIO_Argument = SD_CHECK_PATTERN;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SDIO_SEND_IF_COND;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp7Error();
  if(errorstatus == SD_OK) {
    CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0; /*!< SD Card 2.0 */
    SDType = SD_HIGH_CAPACITY;
  }
  else {
    SDIO_CmdInitStruct.SDIO_Argument = 0x00;
    SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_CMD;
    SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStruct);
    errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
  }
  SDIO_CmdInitStruct.SDIO_Argument = 0x00;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_CMD;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);
  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

  if(errorstatus == SD_OK) {
    while ((!validvoltage) && (count < SD_MAX_VOLT_TRIAL)) {
      SDIO_CmdInitStruct.SDIO_Argument = 0x00;
      SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_CMD;
      SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStruct);

      errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

      if(errorstatus != SD_OK)
        return(errorstatus);

      SDIO_CmdInitStruct.SDIO_Argument = SD_VOLTAGE_WINDOW_SD | SDType;
      SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SD_APP_OP_COND;
      SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStruct);

      errorstatus = CmdResp3Error();
      if(errorstatus != SD_OK)
        return(errorstatus);

      response = SDIO_GetResponse(SDIO_RESP1);
      validvoltage = (((response >> 31) == 1) ? 1 : 0);
      count++;
    }
    if(count >= SD_MAX_VOLT_TRIAL) {
      errorstatus = SD_INVALID_VOLTRANGE;
      return(errorstatus);
    }

    if(response &= SD_HIGH_CAPACITY)
      CardType = SDIO_HIGH_CAPACITY_SD_CARD;
  }

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_PowerOFF
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_PowerOFF( void )
{
  SDIO_SetPowerState(SDIO_PowerState_OFF);

  return(SD_OK);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_InitializeCards
**功能 : Initialize Cards
**輸入 : None
**輸出 : status
**使用 : status = SD_InitializeCards();
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_InitializeCards( void )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;

  SD_Error errorstatus = SD_OK;
  u16 rca = 0x01;

  if(SDIO_GetPowerState() == SDIO_PowerState_OFF) {
    errorstatus = SD_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

  if(SDIO_SECURE_DIGITAL_IO_CARD != CardType) {
    /*!< Send CMD2 ALL_SEND_CID */
    SDIO_CmdInitStruct.SDIO_Argument = 0x0;
    SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_ALL_SEND_CID;
    SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Long;
    SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStruct);

    errorstatus = CmdResp2Error();

    if(SD_OK != errorstatus)
      return(errorstatus);

    CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
    CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
  }
  if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) ||  (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) ||  (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType)
    ||  (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
    /*!< Send CMD3 SET_REL_ADDR with argument 0 */
    /*!< SD Card publishes its RCA. */
    SDIO_CmdInitStruct.SDIO_Argument = 0x00;
    SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SET_REL_ADDR;
    SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStruct);

    errorstatus = CmdResp6Error(SD_CMD_SET_REL_ADDR, &rca);

    if(SD_OK != errorstatus)
      return(errorstatus);
  }

  if(SDIO_SECURE_DIGITAL_IO_CARD != CardType) {
    RCA = rca;

    /*!< Send CMD9 SEND_CSD with argument as card's RCA */
    SDIO_CmdInitStruct.SDIO_Argument = (u32)(rca << 16);
    SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SEND_CSD;
    SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Long;
    SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStruct);

    errorstatus = CmdResp2Error();

    if(SD_OK != errorstatus)
      return(errorstatus);

    CSD_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
    CSD_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CSD_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CSD_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
  }

  errorstatus = SD_OK; /*!< All cards get intialized */

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_GetCardInfo
**功能 : Get Card Info
**輸入 : *cardinfo
**輸出 : status
**使用 : status = SD_GetCardInfo(&SDCardInfo);
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_GetCardInfo( SD_CardInfo* cardinfo )
{
  u8 tmp = 0;

  SD_Error errorstatus = SD_OK;

  cardinfo->CardType = (u8)CardType;
  cardinfo->RCA = (u16)RCA;

  /*!< Byte 0 */
  tmp = (u8)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;

  /*!< Byte 1 */
  tmp = (u8)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  /*!< Byte 2 */
  tmp = (u8)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  /*!< Byte 3 */
  tmp = (u8)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  /*!< Byte 4 */
  tmp = (u8)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  /*!< Byte 5 */
  tmp = (u8)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  /*!< Byte 6 */
  tmp = (u8)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; /*!< Reserved */

  if((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0)) {
    cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

    /*!< Byte 7 */
    tmp = (u8)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

    /*!< Byte 8 */
    tmp = (u8)((CSD_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    /*!< Byte 9 */
    tmp = (u8)((CSD_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;

    /*!< Byte 10 */
    tmp = (u8)((CSD_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
    
    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
    cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
    cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
    cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }
  else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {

    /*!< Byte 7 */
    tmp = (u8)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    /*!< Byte 8 */
    tmp = (u8)((CSD_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_csd.DeviceSize |= (tmp << 8);

    /*!< Byte 9 */
    tmp = (u8)((CSD_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.DeviceSize |= (tmp);

    /*!< Byte 10 */
    tmp = (u8)((CSD_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->CardCapacity = ((uint64_t)cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
    cardinfo->CardBlockSize = 512;
  }

  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /*!< Byte 11 */
  tmp = (u8)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /*!< Byte 12 */
  tmp = (u8)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /*!< Byte 13 */
  tmp = (u8)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  /*!< Byte 14 */
  tmp = (u8)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  /*!< Byte 15 */
  tmp = (u8)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;

  /*!< Byte 0 */
  tmp = (u8)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;

  /*!< Byte 1 */
  tmp = (u8)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  /*!< Byte 2 */
  tmp = (u8)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  /*!< Byte 3 */
  tmp = (u8)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  /*!< Byte 4 */
  tmp = (u8)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  /*!< Byte 5 */
  tmp = (u8)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  /*!< Byte 6 */
  tmp = (u8)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  /*!< Byte 7 */
  tmp = (u8)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  /*!< Byte 8 */
  tmp = (u8)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  /*!< Byte 9 */
  tmp = (u8)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  /*!< Byte 10 */
  tmp = (u8)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  /*!< Byte 11 */
  tmp = (u8)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  /*!< Byte 12 */
  tmp = (u8)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  /*!< Byte 13 */
  tmp = (u8)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /*!< Byte 14 */
  tmp = (u8)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  /*!< Byte 15 */
  tmp = (u8)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_GetCardStatus
**功能 : Get Card Status
**輸入 : *cardstatus
**輸出 : status
**使用 : status = SD_GetCardStatus(&SDCardStatus);
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_GetCardStatus( SD_CardStatus* cardstatus )
{
  SD_Error errorstatus = SD_OK;
  u8 tmp = 0;

  errorstatus = SD_SendSDStatus((u32 *)SDSTATUS_Tab);

  if(errorstatus != SD_OK)
    return(errorstatus);

  /*!< Byte 0 */
  tmp = (u8)((SDSTATUS_Tab[0] & 0xC0) >> 6);
  cardstatus->DAT_BUS_WIDTH = tmp;

  /*!< Byte 0 */
  tmp = (u8)((SDSTATUS_Tab[0] & 0x20) >> 5);
  cardstatus->SECURED_MODE = tmp;

  /*!< Byte 2 */
  tmp = (u8)((SDSTATUS_Tab[2] & 0xFF));
  cardstatus->SD_CARD_TYPE = tmp << 8;

  /*!< Byte 3 */
  tmp = (u8)((SDSTATUS_Tab[3] & 0xFF));
  cardstatus->SD_CARD_TYPE |= tmp;

  /*!< Byte 4 */
  tmp = (u8)(SDSTATUS_Tab[4] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA = tmp << 24;

  /*!< Byte 5 */
  tmp = (u8)(SDSTATUS_Tab[5] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 16;

  /*!< Byte 6 */
  tmp = (u8)(SDSTATUS_Tab[6] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 8;

  /*!< Byte 7 */
  tmp = (u8)(SDSTATUS_Tab[7] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp;

  /*!< Byte 8 */
  tmp = (u8)((SDSTATUS_Tab[8] & 0xFF));
  cardstatus->SPEED_CLASS = tmp;

  /*!< Byte 9 */
  tmp = (u8)((SDSTATUS_Tab[9] & 0xFF));
  cardstatus->PERFORMANCE_MOVE = tmp;

  /*!< Byte 10 */
  tmp = (u8)((SDSTATUS_Tab[10] & 0xF0) >> 4);
  cardstatus->AU_SIZE = tmp;

  /*!< Byte 11 */
  tmp = (u8)(SDSTATUS_Tab[11] & 0xFF);
  cardstatus->ERASE_SIZE = tmp << 8;

  /*!< Byte 12 */
  tmp = (u8)(SDSTATUS_Tab[12] & 0xFF);
  cardstatus->ERASE_SIZE |= tmp;

  /*!< Byte 13 */
  tmp = (u8)((SDSTATUS_Tab[13] & 0xFC) >> 2);
  cardstatus->ERASE_TIMEOUT = tmp;

  /*!< Byte 13 */
  tmp = (u8)((SDSTATUS_Tab[13] & 0x3));
  cardstatus->ERASE_OFFSET = tmp;
 
  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_EnableWideBusOperation
**功能 : Enable Wide Bus Operation
**輸入 : None
**輸出 : status
**使用 : status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_EnableWideBusOperation( u32 WideMode )
{
  SDIO_InitTypeDef SDIO_InitStruct;

  SD_Error errorstatus = SD_OK;

  if(SDIO_MULTIMEDIA_CARD == CardType) {
    errorstatus = SD_UNSUPPORTED_FEATURE;
    return(errorstatus);
  }
  else if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
    if(SDIO_BusWide_8b == WideMode) {
      errorstatus = SD_UNSUPPORTED_FEATURE;
      return(errorstatus);
    }
    else if(SDIO_BusWide_4b == WideMode) {
      errorstatus = SDEnWideBus(ENABLE);

      if(SD_OK == errorstatus) {
        /*!< Configure the SDIO peripheral */
        SDIO_InitStruct.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV; 
        SDIO_InitStruct.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStruct.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStruct.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStruct.SDIO_BusWide = SDIO_BusWide_4b;
        SDIO_InitStruct.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStruct);
      }
    }
    else {
      errorstatus = SDEnWideBus(DISABLE);

      if(SD_OK == errorstatus) {
        /*!< Configure the SDIO peripheral */
        SDIO_InitStruct.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV; 
        SDIO_InitStruct.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStruct.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStruct.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStruct.SDIO_BusWide = SDIO_BusWide_1b;
        SDIO_InitStruct.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStruct);
      }
    }
  }

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_SelectDeselect
**功能 : Select Deselect
**輸入 : addr
**輸出 : status
**使用 : status = SD_SelectDeselect((u32)(SDCardInfo.RCA << 16));
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_SelectDeselect( u32 addr )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;

  SD_Error errorstatus = SD_OK;

  /*!< Send CMD7 SDIO_SEL_DESEL_CARD */
  SDIO_CmdInitStruct.SDIO_Argument =  (u32)addr;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SEL_DESEL_CARD;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SEL_DESEL_CARD);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_ReadBlock
**功能 : Read Block
**輸入 : *readbuff, ReadAddr, BlockSize
**輸出 : Status
**使用 : Status = SD_ReadBlock(buff, sector<<9, SDCardInfo.CardBlockSize);
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_ReadBlock( u8* readbuff, uint64_t ReadAddr, u16 BlockSize )
{
  DMA_InitTypeDef DMA_InitStruct;
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;
  SDIO_DataInitTypeDef SDIO_DataInitStruct;

  SD_Error errorstatus = SD_OK;

  TransferError = SD_OK;
  TransferEnd = 0;
  StopCondition = 0;

  SDIO->DCTRL = 0x0;

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
    BlockSize = 512;
    ReadAddr /= 512;
  }

  /* Set Block Size for Card */ 
  SDIO_CmdInitStruct.SDIO_Argument = (u32)BlockSize;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
  if(SD_OK != errorstatus)
    return(errorstatus);

  SDIO_DataInitStruct.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStruct.SDIO_DataLength = BlockSize;
  SDIO_DataInitStruct.SDIO_DataBlockSize = (u32)9 << 4;
  SDIO_DataInitStruct.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStruct.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStruct.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStruct);

  /*!< Send CMD17 READ_SINGLE_BLOCK */
  SDIO_CmdInitStruct.SDIO_Argument = (u32)ReadAddr;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_READ_SINGLE_BLOCK;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_READ_SINGLE_BLOCK);
  if(errorstatus != SD_OK)
    return(errorstatus);

  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);
  SDIO_DMACmd(ENABLE);

  DMA_ClearFlag(DMAx_STREAM, DMAx_FLAG_FEIF | DMAx_FLAG_DMEIF | DMAx_FLAG_TEIF | DMAx_FLAG_HTIF | DMAx_FLAG_TCIF);
  DMA_Cmd(DMAx_STREAM, DISABLE);
  DMA_DeInit(DMAx_STREAM);

  DMA_InitStruct.DMA_Channel = DMAx_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)SDIO_FIFO_ADDRESS;
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)readbuff;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize = BlockSize;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
  DMA_Init(DMAx_STREAM, &DMA_InitStruct);
  DMA_ITConfig(DMAx_STREAM, DMA_IT_TC, ENABLE);
  DMA_FlowControllerConfig(DMAx_STREAM, DMA_FlowCtrl_Peripheral);
  DMA_Cmd(DMAx_STREAM, ENABLE);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_ReadMultiBlocks
**功能 : Read Multi Blocks
**輸入 : *readbuff, ReadAddr, BlockSize, NumberOfBlocks
**輸出 : Status
**使用 : Status = SD_ReadMultiBlocks(buff, sector<<9, SDCardInfo.CardBlockSize, count);
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_ReadMultiBlocks( u8* readbuff, uint64_t ReadAddr, u16 BlockSize, u32 NumberOfBlocks )
{
  DMA_InitTypeDef DMA_InitStruct;
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;
  SDIO_DataInitTypeDef SDIO_DataInitStruct;

  SD_Error errorstatus = SD_OK;

  TransferError = SD_OK;
  TransferEnd = 0;
  StopCondition = 1;

  SDIO->DCTRL = 0x0;

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
    BlockSize = 512;
    ReadAddr /= 512;
  }

  /*!< Set Block Size for Card */
  SDIO_CmdInitStruct.SDIO_Argument = (u32)BlockSize;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
  if(SD_OK != errorstatus)
    return(errorstatus);

  SDIO_DataInitStruct.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStruct.SDIO_DataLength = (NumberOfBlocks * BlockSize);
  SDIO_DataInitStruct.SDIO_DataBlockSize = (u32) 9 << 4;
  SDIO_DataInitStruct.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStruct.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStruct.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStruct);

  /*!< Send CMD18 READ_MULT_BLOCK with argument data address */
  SDIO_CmdInitStruct.SDIO_Argument = (u32)ReadAddr;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_READ_MULT_BLOCK;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_READ_MULT_BLOCK);
  if(errorstatus != SD_OK)
    return(errorstatus);

  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);

  DMA_ClearFlag(DMAx_STREAM, DMAx_FLAG_FEIF | DMAx_FLAG_DMEIF | DMAx_FLAG_TEIF | DMAx_FLAG_HTIF | DMAx_FLAG_TCIF);
  DMA_Cmd(DMAx_STREAM, DISABLE);
  DMA_DeInit(DMAx_STREAM);

  DMA_InitStruct.DMA_Channel = DMAx_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)SDIO_FIFO_ADDRESS;
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)readbuff;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize = (NumberOfBlocks * BlockSize);
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
  DMA_Init(DMAx_STREAM, &DMA_InitStruct);
  DMA_ITConfig(DMAx_STREAM, DMA_IT_TC, ENABLE);
  DMA_FlowControllerConfig(DMAx_STREAM, DMA_FlowCtrl_Peripheral);
  DMA_Cmd(DMAx_STREAM, ENABLE);

  SDIO_DMACmd(ENABLE);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_WaitReadOperation
**功能 : Wait Read Operation
**輸入 : None
**輸出 : Status
**使用 : Status = SD_WaitReadOperation();
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_WaitReadOperation( void )
{
  SD_Error errorstatus = SD_OK;

  u32 timeout = 0;

  timeout = SD_DATATIMEOUT;
  while((DMAEndOfTransfer == 0x00) && (TransferEnd == 0) && (TransferError == SD_OK) && (timeout > 0))
    timeout--;

  DMAEndOfTransfer = 0x00;

  timeout = SD_DATATIMEOUT;
  while(((SDIO->STA & SDIO_FLAG_RXACT)) && (timeout > 0))
    timeout--;

  if(StopCondition == 1) {
    errorstatus = SD_StopTransfer();
    StopCondition = 0;
  }
  if((timeout == 0) && (errorstatus == SD_OK))
    errorstatus = SD_DATA_TIMEOUT;

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  if(TransferError != SD_OK)
    return(TransferError);
  else
    return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_WriteBlock
**功能 : Write Block
**輸入 : *writebuff, WriteAddr, BlockSize
**輸出 : Status
**使用 : Status = SD_WriteBlock((u8 *)buff, sector<<9, SDCardInfo.CardBlockSize);
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_WriteBlock( u8* writebuff, uint64_t WriteAddr, u16 BlockSize )
{
  DMA_InitTypeDef DMA_InitStruct;
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;
  SDIO_DataInitTypeDef SDIO_DataInitStruct;

  SD_Error errorstatus = SD_OK;

  TransferError = SD_OK;
  TransferEnd = 0;
  StopCondition = 0;

  SDIO->DCTRL = 0x0;

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
    BlockSize = 512;
    WriteAddr /= 512;
  }

  /* Set Block Size for Card */ 
  SDIO_CmdInitStruct.SDIO_Argument = (u32)BlockSize;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
  if(SD_OK != errorstatus)
    return(errorstatus);

  SDIO_CmdInitStruct.SDIO_Argument = (u32)WriteAddr;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_WRITE_SINGLE_BLOCK;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_WRITE_SINGLE_BLOCK);
  if(errorstatus != SD_OK)
    return(errorstatus);

  SDIO_DataInitStruct.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStruct.SDIO_DataLength = BlockSize;
  SDIO_DataInitStruct.SDIO_DataBlockSize = (u32) 9 << 4;
  SDIO_DataInitStruct.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStruct.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStruct.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStruct);

  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);

  DMA_ClearFlag(DMAx_STREAM, DMAx_FLAG_FEIF | DMAx_FLAG_DMEIF | DMAx_FLAG_TEIF | DMAx_FLAG_HTIF | DMAx_FLAG_TCIF);
  DMA_Cmd(DMAx_STREAM, DISABLE);
  DMA_DeInit(DMAx_STREAM);

  DMA_InitStruct.DMA_Channel = DMAx_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)SDIO_FIFO_ADDRESS;
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)writebuff;
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStruct.DMA_BufferSize = BlockSize;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
  DMA_Init(DMAx_STREAM, &DMA_InitStruct);
  DMA_ITConfig(DMAx_STREAM, DMA_IT_TC, ENABLE);
  DMA_FlowControllerConfig(DMAx_STREAM, DMA_FlowCtrl_Peripheral);
  DMA_Cmd(DMAx_STREAM, ENABLE);

  SDIO_DMACmd(ENABLE);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_WriteMultiBlocks
**功能 : Write Multi Blocks
**輸入 : *writebuff, WriteAddr, BlockSize, NumberOfBlocks
**輸出 : Status
**使用 : Status = SD_WriteMultiBlocks((u8 *)buff, sector<<9, SDCardInfo.CardBlockSize, count);
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_WriteMultiBlocks( u8* writebuff, uint64_t WriteAddr, u16 BlockSize, u32 NumberOfBlocks )
{
  DMA_InitTypeDef DMA_InitStruct;
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;
  SDIO_DataInitTypeDef SDIO_DataInitStruct;

  SD_Error errorstatus = SD_OK;

  TransferError = SD_OK;
  TransferEnd = 0;
  StopCondition = 1;

  SDIO->DCTRL = 0x0;

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
    BlockSize = 512;
    WriteAddr /= 512;
  }

  /* Set Block Size for Card */ 
  SDIO_CmdInitStruct.SDIO_Argument = (u32)BlockSize;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
  if(SD_OK != errorstatus)
    return(errorstatus);

  /*!< To improve performance */
  SDIO_CmdInitStruct.SDIO_Argument = (u32)(RCA << 16);
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_CMD;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
  if(errorstatus != SD_OK)
    return(errorstatus);

  /*!< To improve performance */
  SDIO_CmdInitStruct.SDIO_Argument = (u32)NumberOfBlocks;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SET_BLOCK_COUNT;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCK_COUNT);
  if(errorstatus != SD_OK)
    return(errorstatus);

  /*!< Send CMD25 WRITE_MULT_BLOCK with argument data address */
  SDIO_CmdInitStruct.SDIO_Argument = (u32)WriteAddr;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_WRITE_MULT_BLOCK;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_WRITE_MULT_BLOCK);
  if(SD_OK != errorstatus)
    return(errorstatus);

  SDIO_DataInitStruct.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStruct.SDIO_DataLength = (NumberOfBlocks * BlockSize);
  SDIO_DataInitStruct.SDIO_DataBlockSize = (u32) 9 << 4;
  SDIO_DataInitStruct.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStruct.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStruct.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStruct);

  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);
  SDIO_DMACmd(ENABLE);

  DMA_ClearFlag(DMAx_STREAM, DMAx_FLAG_FEIF | DMAx_FLAG_DMEIF | DMAx_FLAG_TEIF | DMAx_FLAG_HTIF | DMAx_FLAG_TCIF);
  DMA_Cmd(DMAx_STREAM, DISABLE);
  DMA_DeInit(DMAx_STREAM);

  DMA_InitStruct.DMA_Channel = DMAx_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)SDIO_FIFO_ADDRESS;
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)writebuff;
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStruct.DMA_BufferSize = (u32)(NumberOfBlocks * BlockSize);
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
  DMA_Init(DMAx_STREAM, &DMA_InitStruct);
  DMA_ITConfig(DMAx_STREAM, DMA_IT_TC, ENABLE);
  DMA_FlowControllerConfig(DMAx_STREAM, DMA_FlowCtrl_Peripheral);

  DMA_Cmd(DMAx_STREAM, ENABLE);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_WaitWriteOperation
**功能 : Wait Write Operation
**輸入 : None
**輸出 : Status
**使用 : Status = SD_WaitWriteOperation();
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_WaitWriteOperation( void )
{
  SD_Error errorstatus = SD_OK;

  u32 timeout = 0;

  timeout = SD_DATATIMEOUT;
  while ((DMAEndOfTransfer == 0x00) && (TransferEnd == 0) && (TransferError == SD_OK) && (timeout > 0))
    timeout--;

  DMAEndOfTransfer = 0x00;

  timeout = SD_DATATIMEOUT;
  while(((SDIO->STA & SDIO_FLAG_TXACT)) && (timeout > 0))
    timeout--;  

  if(StopCondition == 1) {
    errorstatus = SD_StopTransfer();
    StopCondition = 0;
  }

  if((timeout == 0) && (errorstatus == SD_OK))
    errorstatus = SD_DATA_TIMEOUT;

  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  if(TransferError != SD_OK)
    return(TransferError);
  else
    return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_GetTransferState
**功能 : Get Transfer State
**輸入 : None
**輸出 : state
**使用 : state = SD_GetTransferState();
**=====================================================================================================*/
/*=====================================================================================================*/
SDTransferState SD_GetTransferState( void )
{
  if(SDIO->STA & (SDIO_FLAG_TXACT | SDIO_FLAG_RXACT))
    return(SD_TRANSFER_BUSY);
  else
    return(SD_TRANSFER_OK);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_StopTransfer
**功能 : Stop Transfer
**輸入 : None
**輸出 : status
**使用 : status = SD_StopTransfer();
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_StopTransfer( void )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;

  SD_Error errorstatus = SD_OK;

  /*!< Send CMD12 STOP_TRANSMISSION  */
  SDIO_CmdInitStruct.SDIO_Argument = 0x0;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_STOP_TRANSMISSION;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_Erase
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_Erase( uint64_t startaddr, uint64_t endaddr )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;

  SD_Error errorstatus = SD_OK;
  u32 delay = 0;
  __IO u32 maxdelay = 0;
  u8 cardstate = 0;

  /*!< Check if the card coomnd class supports erase command */
  if(((CSD_Tab[1] >> 20) & SD_CCCC_ERASE) == 0) {
    errorstatus = SD_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

  maxdelay = 120000 / ((SDIO->CLKCR & 0xFF) + 2);

  if(SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED) {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
    startaddr /= 512;
    endaddr /= 512;
  }
  
  /*!< According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
  if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
    /*!< Send CMD32 SD_ERASE_GRP_START with argument as addr  */
    SDIO_CmdInitStruct.SDIO_Argument =(u32)startaddr;
    SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SD_ERASE_GRP_START;
    SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStruct);

    errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_START);
    if(errorstatus != SD_OK)
      return(errorstatus);

    /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
    SDIO_CmdInitStruct.SDIO_Argument = (u32)endaddr;
    SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SD_ERASE_GRP_END;
    SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStruct);

    errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_END);
    if(errorstatus != SD_OK)
      return(errorstatus);
  }

  /*!< Send CMD38 ERASE */
  SDIO_CmdInitStruct.SDIO_Argument = 0;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_ERASE;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_ERASE);

  if(errorstatus != SD_OK)
    return(errorstatus);

  for(delay = 0; delay < maxdelay; delay++)
  {}

  /*!< Wait till the card is in programming state */
  errorstatus = IsCardProgramming(&cardstate);
  delay = SD_DATATIMEOUT;
  while((delay > 0) && (errorstatus == SD_OK) && ((SD_CARD_PROGRAMMING == cardstate) || (SD_CARD_RECEIVING == cardstate))) {
    errorstatus = IsCardProgramming(&cardstate);
    delay--;
  }

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_SendStatus
**功能 : Send Status
**輸入 : *pcardstatus
**輸出 : status
**使用 : status = SD_SendStatus(&resp);
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_SendStatus( u32* pcardstatus )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;

  SD_Error errorstatus = SD_OK;

  if(pcardstatus == NULL) {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  SDIO_CmdInitStruct.SDIO_Argument = (u32) RCA << 16;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SEND_STATUS;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);
  if(errorstatus != SD_OK)
    return(errorstatus);

  *pcardstatus = SDIO_GetResponse(SDIO_RESP1);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_SendSDStatus
**功能 : Send SD Status
**輸入 : *psdstatus
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_SendSDStatus( u32* psdstatus )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;
  SDIO_DataInitTypeDef SDIO_DataInitStruct;

  SD_Error errorstatus = SD_OK;
  u32 count = 0;

  if(SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED) {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  /*!< Set block size for card if it is not equal to current block size for card. */
  SDIO_CmdInitStruct.SDIO_Argument = 64;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
  if(errorstatus != SD_OK)
    return(errorstatus);

  /*!< CMD55 */
  SDIO_CmdInitStruct.SDIO_Argument = (u32) RCA << 16;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_CMD;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
  if(errorstatus != SD_OK)
    return(errorstatus);

  SDIO_DataInitStruct.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStruct.SDIO_DataLength = 64;
  SDIO_DataInitStruct.SDIO_DataBlockSize = SDIO_DataBlockSize_64b;
  SDIO_DataInitStruct.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStruct.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStruct.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStruct);

  /*!< Send ACMD13 SD_APP_STAUS  with argument as card's RCA.*/
  SDIO_CmdInitStruct.SDIO_Argument = 0;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SD_APP_STAUS;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SD_APP_STAUS);
  if(errorstatus != SD_OK)
    return(errorstatus);

  while(!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))) {
    if(SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET) {
      for(count = 0; count < 8; count++)
        *(psdstatus + count) = SDIO_ReadData();
      psdstatus += 8;
    }
  }

  if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SD_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SD_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if(SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SD_RX_OVERRUN;
    return(errorstatus);
  }
  else if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SD_START_BIT_ERR;
    return(errorstatus);
  }

  count = SD_DATATIMEOUT;
  while((SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) && (count > 0)) {
    *psdstatus = SDIO_ReadData();
    psdstatus++;
    count--;
  }
  /*!< Clear all the static status flags*/
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_ProcessIRQSrc
**功能 : SD IRQ
**輸入 : None
**輸出 : status;
**使用 : status = SD_ProcessIRQSrc();
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_ProcessIRQSrc( void )
{ 
  if(SDIO_GetITStatus(SDIO_IT_DATAEND) != RESET) {
    TransferError = SD_OK;
    SDIO_ClearITPendingBit(SDIO_IT_DATAEND);
    TransferEnd = 1;
  }
  else if(SDIO_GetITStatus(SDIO_IT_DCRCFAIL) != RESET) {
    SDIO_ClearITPendingBit(SDIO_IT_DCRCFAIL);
    TransferError = SD_DATA_CRC_FAIL;
  }
  else if(SDIO_GetITStatus(SDIO_IT_DTIMEOUT) != RESET) {
    SDIO_ClearITPendingBit(SDIO_IT_DTIMEOUT);
    TransferError = SD_DATA_TIMEOUT;
  }
  else if(SDIO_GetITStatus(SDIO_IT_RXOVERR) != RESET) {
    SDIO_ClearITPendingBit(SDIO_IT_RXOVERR);
    TransferError = SD_RX_OVERRUN;
  }
  else if(SDIO_GetITStatus(SDIO_IT_TXUNDERR) != RESET) {
    SDIO_ClearITPendingBit(SDIO_IT_TXUNDERR);
    TransferError = SD_TX_UNDERRUN;
  }
  else if(SDIO_GetITStatus(SDIO_IT_STBITERR) != RESET) {
    SDIO_ClearITPendingBit(SDIO_IT_STBITERR);
    TransferError = SD_START_BIT_ERR;
  }

  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_TXFIFOHE |
                SDIO_IT_RXFIFOHF | SDIO_IT_TXUNDERR | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, DISABLE);

  return(TransferError);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_ProcessDMAIRQ
**功能 : SD DMA IRQ
**輸入 : None
**輸出 : None
**使用 : SD_ProcessDMAIRQ();
**=====================================================================================================*/
/*=====================================================================================================*/
void SD_ProcessDMAIRQ( void )
{
  if(DMA2->LISR & DMAx_FLAG_TCIF) {
    DMAEndOfTransfer = 0x01;
    DMA_ClearFlag(DMAx_STREAM, DMAx_FLAG_FEIF | DMAx_FLAG_TCIF);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SD_HighSpeed
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
SD_Error SD_HighSpeed( void )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;
  SDIO_DataInitTypeDef SDIO_DataInitStruct;

  SD_Error errorstatus = SD_OK;
  u32 scr[2] = {0, 0};
  u32 SD_SPEC = 0 ;
  u8 hs[64] = {0} ;
  u32 count = 0, *tempbuff = (u32 *)hs;
  TransferError = SD_OK;
  TransferEnd = 0;
  StopCondition = 0;

  SDIO->DCTRL = 0x0;

  /*!< Get SCR Register */
  errorstatus = FindSCR(RCA, scr);
  if(errorstatus != SD_OK)
    return(errorstatus);

  /* Test the Version supported by the card*/ 
  SD_SPEC = (scr[1]  & 0x01000000)||(scr[1]  & 0x02000000);

  if(SD_SPEC != SD_ALLZERO) {
    /* Set Block Size for Card */
    SDIO_CmdInitStruct.SDIO_Argument = (u32)64;
    SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
    SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStruct);
    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
    if(errorstatus != SD_OK)
      return(errorstatus);
    SDIO_DataInitStruct.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStruct.SDIO_DataLength = 64;
    SDIO_DataInitStruct.SDIO_DataBlockSize = SDIO_DataBlockSize_64b ;
    SDIO_DataInitStruct.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
    SDIO_DataInitStruct.SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStruct.SDIO_DPSM = SDIO_DPSM_Enable;
    SDIO_DataConfig(&SDIO_DataInitStruct);

    /*!< Send CMD6 switch mode */
    SDIO_CmdInitStruct.SDIO_Argument = 0x80FFFF01;
    SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_HS_SWITCH;
    SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStruct);

    errorstatus = CmdResp1Error(SD_CMD_HS_SWITCH);
    if(errorstatus != SD_OK)
      return(errorstatus);

    while(!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))) {
      if(SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET) {
        for(count = 0; count < 8; count++)
          *(tempbuff + count) = SDIO_ReadData();
        tempbuff += 8;
      }
    }

    if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
      SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
      errorstatus = SD_DATA_TIMEOUT;
      return(errorstatus);
    }
    else if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
      SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
      errorstatus = SD_DATA_CRC_FAIL;
      return(errorstatus);
    }
    else if(SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) {
      SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
      errorstatus = SD_RX_OVERRUN;
      return(errorstatus);
    }
    else if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
      SDIO_ClearFlag(SDIO_FLAG_STBITERR);
      errorstatus = SD_START_BIT_ERR;
      return(errorstatus);
    }
    count = SD_DATATIMEOUT;
    while((SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) && (count > 0)) {
      *tempbuff = SDIO_ReadData();
      tempbuff++;
      count--;
    }

    /*!< Clear all the static flags */
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);

    /* Test if the switch mode HS is ok */
    if((hs[13]& 0x2)==0x2)
      errorstatus = SD_OK;
    else
      errorstatus = SD_UNSUPPORTED_FEATURE;
  }

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CmdError
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error CmdError( void )
{
  SD_Error errorstatus = SD_OK;
  u32 timeout;

  timeout = SDIO_CMD0TIMEOUT; /*!< 10000 */

  while ((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) == RESET))
    timeout--;

  if(timeout == 0) {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CmdResp7Error
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error CmdResp7Error( void )
{
  SD_Error errorstatus = SD_OK;
  u32 status;
  u32 timeout = SDIO_CMD0TIMEOUT;

  status = SDIO->STA;

  while(!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) && (timeout > 0)) {
    timeout--;
    status = SDIO->STA;
  }

  if((timeout == 0) || (status & SDIO_FLAG_CTIMEOUT)) {
    /*!< Card is not V2.0 complient or card does not support the set voltage range */
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }

  if(status & SDIO_FLAG_CMDREND) {
    /*!< Card is SD V2.0 compliant */
    errorstatus = SD_OK;
    SDIO_ClearFlag(SDIO_FLAG_CMDREND);
    return(errorstatus);
  }

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CmdResp1Error
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error CmdResp1Error( u8 cmd )
{
  SD_Error errorstatus = SD_OK;
  u32 status;
  u32 response_r1;

  status = SDIO->STA;

  while(!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
    status = SDIO->STA;

  if(status & SDIO_FLAG_CTIMEOUT) {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if(status & SDIO_FLAG_CCRCFAIL) {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  if(SDIO_GetCommandResponse() != cmd) {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  SDIO_ClearFlag(SDIO_STATIC_FLAGS);
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if((response_r1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
    return(errorstatus);
  if(response_r1 & SD_OCR_ADDR_OUT_OF_RANGE)
    return(SD_ADDR_OUT_OF_RANGE);
  if(response_r1 & SD_OCR_ADDR_MISALIGNED)
    return(SD_ADDR_MISALIGNED);
  if(response_r1 & SD_OCR_BLOCK_LEN_ERR)
    return(SD_BLOCK_LEN_ERR);
  if(response_r1 & SD_OCR_ERASE_SEQ_ERR)
    return(SD_ERASE_SEQ_ERR);
  if(response_r1 & SD_OCR_BAD_ERASE_PARAM)
    return(SD_BAD_ERASE_PARAM);
  if(response_r1 & SD_OCR_WRITE_PROT_VIOLATION)
    return(SD_WRITE_PROT_VIOLATION);
  if(response_r1 & SD_OCR_LOCK_UNLOCK_FAILED)
    return(SD_LOCK_UNLOCK_FAILED);
  if(response_r1 & SD_OCR_COM_CRC_FAILED)
    return(SD_COM_CRC_FAILED);
  if(response_r1 & SD_OCR_ILLEGAL_CMD)
    return(SD_ILLEGAL_CMD);
  if(response_r1 & SD_OCR_CARD_ECC_FAILED)
    return(SD_CARD_ECC_FAILED);
  if(response_r1 & SD_OCR_CC_ERROR)
    return(SD_CC_ERROR);
  if(response_r1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
    return(SD_GENERAL_UNKNOWN_ERROR);
  if(response_r1 & SD_OCR_STREAM_READ_UNDERRUN)
    return(SD_STREAM_READ_UNDERRUN);
  if(response_r1 & SD_OCR_STREAM_WRITE_OVERRUN)
    return(SD_STREAM_WRITE_OVERRUN);
  if(response_r1 & SD_OCR_CID_CSD_OVERWRIETE)
    return(SD_CID_CSD_OVERWRITE);
  if(response_r1 & SD_OCR_WP_ERASE_SKIP)
    return(SD_WP_ERASE_SKIP);
  if(response_r1 & SD_OCR_CARD_ECC_DISABLED)
    return(SD_CARD_ECC_DISABLED);
  if(response_r1 & SD_OCR_ERASE_RESET)
    return(SD_ERASE_RESET);
  if(response_r1 & SD_OCR_AKE_SEQ_ERROR)
    return(SD_AKE_SEQ_ERROR);
  
  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CmdResp3Error
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error CmdResp3Error( void )
{
  SD_Error errorstatus = SD_OK;
  u32 status;

  status = SDIO->STA;

  while(!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
    status = SDIO->STA;

  if(status & SDIO_FLAG_CTIMEOUT) {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);
  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CmdResp2Error
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error CmdResp2Error( void )
{
  SD_Error errorstatus = SD_OK;
  u32 status;

  status = SDIO->STA;

  while(!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
    status = SDIO->STA;

  if(status & SDIO_FLAG_CTIMEOUT) {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL) {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CmdResp6Error
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error CmdResp6Error( u8 cmd, u16 *prca )
{
  SD_Error errorstatus = SD_OK;
  u32 status;
  u32 response_r1;

  status = SDIO->STA;

  while(!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
    status = SDIO->STA;

  if(status & SDIO_FLAG_CTIMEOUT) {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if(status & SDIO_FLAG_CCRCFAIL) {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  if(SDIO_GetCommandResponse() != cmd) {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  SDIO_ClearFlag(SDIO_STATIC_FLAGS);
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if(SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED))) {
    *prca = (u16) (response_r1 >> 16);
    return(errorstatus);
  }

  if(response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
    return(SD_GENERAL_UNKNOWN_ERROR);
  if(response_r1 & SD_R6_ILLEGAL_CMD)
    return(SD_ILLEGAL_CMD);
  if(response_r1 & SD_R6_COM_CRC_FAILED)
    return(SD_COM_CRC_FAILED);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SDEnWideBus
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error SDEnWideBus( FunctionalState NewState )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;

  SD_Error errorstatus = SD_OK;

  u32 scr[2] = {0, 0};

  if(SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED) {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  errorstatus = FindSCR(RCA, scr);

  if(errorstatus != SD_OK) {
    return(errorstatus);
  }

  if(NewState == ENABLE) {
    if((scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO) {
      SDIO_CmdInitStruct.SDIO_Argument = (u32) RCA << 16;
      SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_CMD;
      SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStruct);

      errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

      if(errorstatus != SD_OK)
        return(errorstatus);

      /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
      SDIO_CmdInitStruct.SDIO_Argument = 0x2;
      SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_SD_SET_BUSWIDTH;
      SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStruct);

      errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);

      if(errorstatus != SD_OK)
        return(errorstatus);

      return(errorstatus);
    }
    else {
      errorstatus = SD_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }
  else {
    if((scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO) {
      SDIO_CmdInitStruct.SDIO_Argument = (u32) RCA << 16;
      SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_CMD;
      SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStruct);

      errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

      if(errorstatus != SD_OK)
        return(errorstatus);

      SDIO_CmdInitStruct.SDIO_Argument = 0x00;
      SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_SD_SET_BUSWIDTH;
      SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStruct);

      errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);

      if(errorstatus != SD_OK)
        return(errorstatus);

      return(errorstatus);
    }
    else {
      errorstatus = SD_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : IsCardProgramming
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error IsCardProgramming( u8 *pstatus )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;

  SD_Error errorstatus = SD_OK;
  __IO u32 respR1 = 0, status = 0;

  SDIO_CmdInitStruct.SDIO_Argument = (u32) RCA << 16;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SEND_STATUS;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  status = SDIO->STA;
  while(!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
    status = SDIO->STA;

  if(status & SDIO_FLAG_CTIMEOUT) {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if(status & SDIO_FLAG_CCRCFAIL) {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  status = (u32)SDIO_GetCommandResponse();

  /*!< Check response received is of desired command */
  if(status != SD_CMD_SEND_STATUS) {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  /*!< We have received response, retrieve it for analysis  */
  respR1 = SDIO_GetResponse(SDIO_RESP1);

  /*!< Find out card status */
  *pstatus = (u8)((respR1 >> 9) & 0x0000000F);

  if((respR1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
    return(errorstatus);
  if(respR1 & SD_OCR_ADDR_OUT_OF_RANGE)
    return(SD_ADDR_OUT_OF_RANGE);
  if(respR1 & SD_OCR_ADDR_MISALIGNED)
    return(SD_ADDR_MISALIGNED);
  if(respR1 & SD_OCR_BLOCK_LEN_ERR)
    return(SD_BLOCK_LEN_ERR);
  if(respR1 & SD_OCR_ERASE_SEQ_ERR)
    return(SD_ERASE_SEQ_ERR);
  if(respR1 & SD_OCR_BAD_ERASE_PARAM)
    return(SD_BAD_ERASE_PARAM);
  if(respR1 & SD_OCR_WRITE_PROT_VIOLATION)
    return(SD_WRITE_PROT_VIOLATION);
  if(respR1 & SD_OCR_LOCK_UNLOCK_FAILED)
    return(SD_LOCK_UNLOCK_FAILED);
  if(respR1 & SD_OCR_COM_CRC_FAILED)
    return(SD_COM_CRC_FAILED);
  if(respR1 & SD_OCR_ILLEGAL_CMD)
    return(SD_ILLEGAL_CMD);
  if(respR1 & SD_OCR_CARD_ECC_FAILED)
    return(SD_CARD_ECC_FAILED);
  if(respR1 & SD_OCR_CC_ERROR)
    return(SD_CC_ERROR);
  if(respR1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
    return(SD_GENERAL_UNKNOWN_ERROR);
  if(respR1 & SD_OCR_STREAM_READ_UNDERRUN)
    return(SD_STREAM_READ_UNDERRUN);
  if (respR1 & SD_OCR_STREAM_WRITE_OVERRUN)
    return(SD_STREAM_WRITE_OVERRUN);
  if(respR1 & SD_OCR_CID_CSD_OVERWRIETE)
    return(SD_CID_CSD_OVERWRITE);
  if(respR1 & SD_OCR_WP_ERASE_SKIP)
    return(SD_WP_ERASE_SKIP);
  if(respR1 & SD_OCR_CARD_ECC_DISABLED)
    return(SD_CARD_ECC_DISABLED);
  if(respR1 & SD_OCR_ERASE_RESET)
    return(SD_ERASE_RESET);
  if(respR1 & SD_OCR_AKE_SEQ_ERROR)
    return(SD_AKE_SEQ_ERROR);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : FindSCR
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static SD_Error FindSCR( u16 rca, u32 *pscr )
{
  SDIO_CmdInitTypeDef SDIO_CmdInitStruct;
  SDIO_DataInitTypeDef SDIO_DataInitStruct;

  u32 index = 0;
  SD_Error errorstatus = SD_OK;
  u32 tempscr[2] = {0, 0};

  /*!< Set Block Size To 8 Bytes */
  /*!< Send CMD55 APP_CMD with argument as card's RCA */
  SDIO_CmdInitStruct.SDIO_Argument = (u32)8;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);
  if(errorstatus != SD_OK)
    return(errorstatus);

  /*!< Send CMD55 APP_CMD with argument as card's RCA */
  SDIO_CmdInitStruct.SDIO_Argument = (u32) RCA << 16;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_APP_CMD;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
  if(errorstatus != SD_OK)
    return(errorstatus);

  SDIO_DataInitStruct.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStruct.SDIO_DataLength = 8;
  SDIO_DataInitStruct.SDIO_DataBlockSize = SDIO_DataBlockSize_8b;
  SDIO_DataInitStruct.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStruct.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStruct.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStruct);


  /*!< Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
  SDIO_CmdInitStruct.SDIO_Argument = 0x0;
  SDIO_CmdInitStruct.SDIO_CmdIndex = SD_CMD_SD_APP_SEND_SCR;
  SDIO_CmdInitStruct.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStruct.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStruct);

  errorstatus = CmdResp1Error(SD_CMD_SD_APP_SEND_SCR);
  if(errorstatus != SD_OK)
    return(errorstatus);

  while(!(SDIO->STA & (SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))) {
    if(SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) {
      *(tempscr + index) = SDIO_ReadData();
      index++;
    }
  }

  if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SD_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SD_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if(SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SD_RX_OVERRUN;
    return(errorstatus);
  }
  else if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SD_START_BIT_ERR;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  *(pscr + 1) = ((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);
  *(pscr) = ((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);

  return(errorstatus);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
