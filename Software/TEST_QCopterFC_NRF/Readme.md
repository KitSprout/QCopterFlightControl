QCopterFC NRF Example
========
* Author  : [Hom](https://github.com/Hom19910422)
* Update  : 2014/01/26

Description
========
QCopterFC NRF 的範例程式，透過定義 NRF_MODE 來決定先發射還是先接收，
先發送的為 FTLR，先接收的為 FRLT，FTLR 板子會先傳送資料至 FRLT，
FRLT 收到後會直接回傳，並將資料透過 RS232 在 PC 端顯示，FTLR 則會將
接收到的資料加上資料地址的數值發送至 FRLT，不斷循環。

Hardware
========
* LED  
PC13 - LED_B  
PC14 - LED_G  
PC15 - LED_R  

* KEY  
PB2  - KEY  

* UART  
PB10 - UART Tx  
PB11 - UART Rx  
> UARTx = USART3  
> BaudRate = 115200  
> ByteSize = 8  
> StopBits = 1  
> Parity = 'N'  
> HardwareFlowControl = None  

* NRF  
PB12 - CSN  
PB13 - SCK  
PB14 - MISO  
PB15 - MOSI  
PC4  - CE  
PC5  - IRQ  
> SPIx　　 = SPI2  
> Dir　　　= 2Lines_FullDuplex  
> Mode　　 = Master  
> CPOL　　 = Low  
> CPHA　　 = 1Edge  
> FirstBit = MSB  
> DataSize = 8 bits  
> BaudRate = Prescaler_4  
