QCopterFC SDIO Example
========
* Author  : [Hom](https://github.com/Hom19910422)
* Update  : 2014/01/24

Description
========
QCopterFC SDIO 搭配 FatFs 的範例程式，從一開始的 SD 卡初始化、SD 卡資訊、讀取資料夾目錄、寫檔案、讀檔案的整體測試。

Hardware
========
* LED  
PC13 - LED_B  
PC14 - LED_G  
PC15 - LED_R  

* UART  
PB10 - UART Tx  
PB11 - UART Rx  
> UARTx = USART3  
> BaudRate = 115200  
> ByteSize = 8  
> StopBits = 1  
> Parity = 'N'  
> HardwareFlowControl = None  

* SDIO  
PD8  - SDIO D0  
PD9  - SDIO D1  
PD10 - SDIO D2  
PD11 - SDIO D3  
PD12 - SDIO CK  
PD2  - SDIO CMD  
