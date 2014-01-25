/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_spi.h"
#include "module_nrf24l01.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define CMD_R_REG             ((u8)0x00)
#define CMD_W_REG             ((u8)0x20)
#define CMD_R_RX_PLOAD        ((u8)0x61)
#define CMD_W_TX_PLOAD        ((u8)0xA0)
#define CMD_FLUSH_TX          ((u8)0xE1)
#define CMD_FLUSH_RX          ((u8)0xE2)
#define CMD_REUSE_TX_PL       ((u8)0xE3)
#define CMD_RX_PL_WID         ((u8)0x50)
#define CMD_W_ACK_PLOAD(P)    ((u8)(0xA8|(P&0x07)))
#define CMD_W_TX_PLOAD_NOACK  ((u8)0xB0)
#define CMD_NOP               ((u8)0xFF)
/*=====================================================================================================*/
/*=====================================================================================================*/
u8 TxBuf[NRF_TX_PL_WIDTH] = {0};
u8 RxBuf[NRF_RX_PL_WIDTH] = {0};

static u8 TX_ADDRESS[NRF_PIPE_WIDTH] = { 0x34,0x43,0x10,0x10,0x01 };   // 定義一個靜態發送地址
static u8 RX_ADDRESS[NRF_PIPE_WIDTH] = { 0x34,0x43,0x10,0x10,0x01 };
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF24L01_Config
**功能 : nRF24L01 配置 & 設定
**輸入 : None
**輸出 : None
**使用 : NRF24L01_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void NRF24L01_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

  /* CSN PB12 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  NRF_CSN = 1;

  /* SCK PB13 */	/* MISO PB14 */	/* MOSI PB15 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  /* CE PC4 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  NRF_CE = 0;

  /* IRQ PC5 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   // 雙線全雙工
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;                        // 主模式
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;                    // 數據大小 8 位
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;                           // 時鐘極性，空閒時為低
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;                         // 第 1 個邊沿有效，上升沿為采樣時刻
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;                            // NSS 信號由軟體產生
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;   // 4 分頻，10.5 MHz
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;                   // 高位在前
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(NRF_SPI, &SPI_InitStruct);

  SPI_Cmd(NRF_SPI, ENABLE);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF24L01_Init
**功能 : nRF24L01 初始化
**輸入 : None
**輸出 : None
**使用 : NRF24L01_Init();
**=====================================================================================================*/
/*=====================================================================================================*/
void NRF24L01_Init( u8 NRF_INIT_MODE )
{
  NRF_CE = 0;
  NRF_SetAddr(NRF_TX_ADDR, TX_ADDRESS);             // 設定 TX 地址
  NRF_SetAddr(NRF_RX_ADDR_P0, RX_ADDRESS);          // 設定 RX 地址
  NRF_WriteReg(CMD_W_REG | NRF_EN_AA, 0x01);        // 致能通道0的自動應答
  NRF_WriteReg(CMD_W_REG | NRF_EN_RXADDR, 0x01);    // 致能 data Pipe 0 的接收地址
  NRF_WriteReg(CMD_W_REG | NRF_SETUP_RETR, 0x1A);   // 設定自動重發間隔時間:500us + 86us;最大自動重發次數:10次
  NRF_SetChannel(NRF_CHANAL);                       // 設定傳輸通道(頻率)
  NRF_SetDataRate(NRF_RATE_2Mbps);                  // 設定傳輸速率 2Mbps
//  NRF_WriteReg(CMD_W_REG | NRF_RX_PW_P0, NRF_TX_PL_WIDTH);  // 設定通道0的有效數據寬度

  switch(NRF_INIT_MODE) {

    case NRF_MODE_FTLR:
      NRF_WriteReg(CMD_W_REG | NRF_CONFIG, 0x0E);   // Power UP，Enable 16bit CRC，TX Mode，no mask
      NRF_FlushRxFIFO();
      NRF_FlushTxFIFO();
      break;

    case NRF_MODE_FRLT:
      NRF_FlushRxFIFO();
      NRF_FlushTxFIFO();
      NRF_WriteReg(CMD_W_REG | NRF_CONFIG, 0x0F);   // Power UP，Enable 16bit CRC，RX Mode，no mask
      break;
  }

  NRF_WriteReg(CMD_RX_PL_WID, 0x73);
  NRF_WriteReg(CMD_W_REG | NRF_DYNPD, 0x01);
  NRF_WriteReg(CMD_W_REG | NRF_FEATURE, 0x07);

  NRF_CE = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_ReadReg
**功能 : 讀暫存器
**輸入 : ReadAddr
**輸出 : ReadData
**使用 : Status = NRF_ReadReg(CMD_R_REG | NRF_STATUS);
**=====================================================================================================*/
/*=====================================================================================================*/
static u8 NRF_ReadReg( u8 ReadAddr )
{
  u8 ReadData;

  NRF_CSN = 0;
  SPI_WriteByte(NRF_SPI, ReadAddr);
  ReadData = SPI_ReadByte(NRF_SPI);
  NRF_CSN = 1;

  return ReadData;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_WriteReg
**功能 : 寫暫存器
**輸入 : WriteAddr, WriteData
**輸出 : None
**使用 : NRF_WriteReg(CMD_W_REG | NRF_STATUS, Status);
**=====================================================================================================*/
/*=====================================================================================================*/
static void NRF_WriteReg( u8 WriteAddr, u8 WriteData )
{
  NRF_CSN = 0;
  SPI_WriteByte(NRF_SPI, WriteAddr);
  SPI_WriteByte(NRF_SPI, WriteData);
  NRF_CSN = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_ReadBuf
**功能 : 連續讀暫存器
**輸入 : ReadAddr, *ReadBuf, Bytes
**輸出 : None
**使用 : NRF_ReadBuf(CMD_R_RX_PLOAD, RxBuf, RX_PLOAD_WIDTH);
**=====================================================================================================*/
/*=====================================================================================================*/
static void NRF_ReadBuf( u8 ReadAddr, u8 *ReadBuf, u8 Bytes )
{
  u8 i = 0;

  NRF_CSN = 0;
  SPI_WriteByte(NRF_SPI, ReadAddr);
  for(i=0; i<Bytes; i++)
    ReadBuf[i] = SPI_ReadByte(NRF_SPI);
  NRF_CSN = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_WriteBuf
**功能 : 連續寫暫存器
**輸入 : WriteAddr, *WriteBuf, Bytes
**輸出 : None
**使用 : NRF_WriteBuf(CMD_W_TX_PLOAD, TxBuf, TX_PLOAD_WIDTH);
**=====================================================================================================*/
/*=====================================================================================================*/
static void NRF_WriteBuf( u8 WriteAddr, u8 *WriteBuf, u8 Bytes )
{
  u8 i;

  NRF_CSN = 0;
  SPI_WriteByte(NRF_SPI, WriteAddr);
  for(i=0; i<Bytes; i++)
    SPI_WriteByte(NRF_SPI, WriteBuf[i]);
  NRF_CSN = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_NOP
**功能 : NOP
**輸入 : None
**輸出 : None
**使用 : Status = NRF_NOP();
**=====================================================================================================*/
/*=====================================================================================================*/
//static u8 NRF_NOP( void )
//{
//  u8 Status;

//  NRF_CSN = 0;
//  Status = SPI_RW(NRF_SPI, NRF_CMD_NOP);
//  NRF_CSN = 1;

//  return Status;
//}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_GetStatus
**功能 : 讀取狀態
**輸入 : None
**輸出 : Status
**使用 : Status = NRF_GetStatus();
**=====================================================================================================*/
/*=====================================================================================================*/
//static u8 NRF_GetStatus( void )
//{
//  return NRF_NOP();
//}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_SetChannel
**功能 : 設定傳輸通道
**輸入 : Channel
**輸出 : None
**使用 : NRF_SetChannel(NRF_CHANAL);
**=====================================================================================================*/
/*=====================================================================================================*/
static void NRF_SetChannel( u8 Channel )
{
  if(Channel<126)
    NRF_WriteReg(CMD_W_REG | NRF_RF_CH, Channel);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_SetDataRate
**功能 : 設定傳輸速率
**輸入 : DataRate
**輸出 : None
**使用 : NRF_SetDataRate(NRF_RATE_2Mbps);
**=====================================================================================================*/
/*=====================================================================================================*/
static void NRF_SetDataRate( u8 DataRate )
{
  switch(DataRate) {

    case NRF_RATE_250Kbps:
      NRF_WriteReg(CMD_W_REG | NRF_RF_SETUP, NRF_RATE_250Kbps);
      break;

    case NRF_RATE_1Mbps:
      NRF_WriteReg(CMD_W_REG | NRF_RF_SETUP, NRF_RATE_1Mbps);
      break;

    case NRF_RATE_2Mbps:
      NRF_WriteReg(CMD_W_REG | NRF_RF_SETUP, NRF_RATE_2Mbps);
      break;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_SetAddr
**功能 : 設定傳輸地址
**輸入 : PipeAddr, *Addr
**輸出 : None
**使用 : NRF_SetAddr(NRF_RX_ADDR_P0, RX_ADDRESS);
**=====================================================================================================*/
/*=====================================================================================================*/
static void NRF_SetAddr( u8 RegAddr, u8 *Address )
{
  u8 PipeLen = 0;

  switch(RegAddr) {
    case NRF_TX_ADDR:
    case NRF_RX_ADDR_P0:
    case NRF_RX_ADDR_P1:
      PipeLen = NRF_PIPE_WIDTH;
      break;
    case NRF_RX_ADDR_P2:
    case NRF_RX_ADDR_P3:
    case NRF_RX_ADDR_P4:
    case NRF_RX_ADDR_P5:
      PipeLen = 1;
      break;
  }

  NRF_WriteBuf(CMD_W_REG | RegAddr, Address, PipeLen);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_FlushTxFIFO
**功能 : 刷新 Tx FIFO
**輸入 : None
**輸出 : None
**使用 : NRF_FlushTxFIFO();
**=====================================================================================================*/
/*=====================================================================================================*/
static void NRF_FlushTxFIFO( void )
{
  NRF_CSN = 0;
  SPI_RW(NRF_SPI, CMD_FLUSH_TX);
  NRF_CSN = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_FlushRxFIFO
**功能 : 刷新 Rx FIFO
**輸入 : None
**輸出 : None
**使用 : NRF_FlushRxFIFO();
**=====================================================================================================*/
/*=====================================================================================================*/
static void NRF_FlushRxFIFO( void )
{
  NRF_CSN = 0;
  SPI_RW(NRF_SPI, CMD_FLUSH_RX);
  NRF_CSN = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_Check
**功能 : nRF24L01檢測
**輸入 : None
**輸出 : Status
**使用 : Status = NRF_Check();
**=====================================================================================================*/
/*=====================================================================================================*/
u8 NRF_Check( void )
{
  u8 i;
  u8 SendBuf[5] = {0xc2, 0xb5, 0x81, 0xd0, 0xa9};
  u8 RecvBuf[5] = {0};

  NRF_WriteBuf(CMD_W_REG | NRF_TX_ADDR, SendBuf, 5);
  NRF_ReadBuf(CMD_R_REG | NRF_TX_ADDR, RecvBuf, 5);

  for(i=0; i<5; i++)
    if(RecvBuf[i]!=SendBuf[i])
      return ERROR;

  return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_TX_Mode
**功能 : 轉成 TX 模式
**輸入 : None
**輸出 : None
**使用 : NRF_TX_Mode();
**=====================================================================================================*/
/*=====================================================================================================*/
//void NRF_TX_Mode( void )
//{
//  NRF_CE = 0;
//  NRF_WriteReg(CMD_W_REG | NRF_CONFIG, 0x0E);   // Power UP，Enable 16bit CRC，TX Mode，no mask
//  NRF_CE = 1;
//}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_RX_Mode
**功能 : 轉成 RX 模式
**輸入 : None
**輸出 : None
**使用 : NRF_RX_Mode();
**=====================================================================================================*/
/*=====================================================================================================*/
void NRF_RX_Mode( void )
{
  NRF_CE = 0;
  NRF_WriteReg(CMD_W_REG | NRF_CONFIG, 0x0F);   // Power UP，Enable 16bit CRC，RX Mode，no mask
  NRF_CE = 1;
  Delay_10us(13);   // After 130μs nRF24L01+ monitors the air for incoming communication.
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_TxData
**功能 : 傳送資料
**輸入 : *TxBuf
**輸出 : None
**使用 : NRF_TxData(TxBuf);
**=====================================================================================================*/
/*=====================================================================================================*/
void NRF_TxData( u8 *TxBuf )
{
  NRF_CE = 0;
  NRF_WriteReg(CMD_W_REG | NRF_CONFIG, 0x0E);   // Power UP，Enable 16bit CRC，TX Mode，no mask
  NRF_SetAddr(NRF_TX_ADDR, TX_ADDRESS);         // 設定 TX 地址
  NRF_WriteBuf(CMD_W_TX_PLOAD, TxBuf, NRF_TX_PL_WIDTH); // 寫入資料
  NRF_CE = 1;
  Delay_10us(1);  // A high pulse on CE starts the transmission. The minimum pulse width on CE is 10μs.
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_RxData
**功能 : 接收資料
**輸入 : *RxBuf
**輸出 : None
**使用 : NRF_RxData(TxBuf);
**=====================================================================================================*/
/*=====================================================================================================*/
void NRF_RxData( u8 *RxBuf )
{
  NRF_ReadBuf(CMD_R_RX_PLOAD, RxBuf, NRF_RX_PL_WIDTH);
//  NRF_FlushRxFIFO();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_TxPacket
**功能 : 傳送封包
**輸入 : *TxBuf
**輸出 : Status
**使用 : NRF_TxPacket(TxBuf);
**=====================================================================================================*/
/*=====================================================================================================*/
u8 NRF_TxPacket( u8 *TxBuf )
{
  u8 Status;

  NRF_TxData(TxBuf);

  while(NRF_IRQ!=0);
  Status = NRF_ReadReg(CMD_R_REG | NRF_STATUS);
  NRF_WriteReg(CMD_W_REG | NRF_STATUS, Status);
  NRF_FlushTxFIFO();

  if(Status&NRF_STA_MAX_RT)
    return NRF_STA_MAX_RT;
  else if(Status&NRF_STA_TX_DS)
    return NRF_STA_TX_DS;
  else
    return ERROR;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NRF_RxPacket
**功能 : 接收封包
**輸入 : *RxBuf
**輸出 : Status
**使用 : NRF_RxPacket(RxBuf);
**=====================================================================================================*/
/*=====================================================================================================*/
u8 NRF_RxPacket( u8 *RxBuf )
{
  u8 Status;

  NRF_CE = 1;
  while(NRF_IRQ!=0);
  NRF_CE = 0;

  Status = NRF_ReadReg(CMD_R_REG | NRF_STATUS);
  NRF_WriteReg(CMD_W_REG | NRF_STATUS, Status);

  if(Status&NRF_STA_RX_DR) {
    NRF_RxData(RxBuf);
    return NRF_STA_RX_DR;
  }
  else
    return ERROR;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
