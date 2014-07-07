QCopterFC IAP Example
========
* Author  : [Hom](http://about.me/Hom)
* Update  : 2014/07/08

Description
========
QCopterFC IAP 的範例程式，按著 KEY 開機，將進入 BOOT 模式，此時會從 UART 接收 .bin 檔案，寫入至 APP FLASH 區域並執行，若開機未按下 KEY 則直接進入 APP 模式，執行最後更新的 APP 程式。  

Hardware
========
* LED  
PC13 - LED_B  
PC14 - LED_G  
PC15 - LED_R  

* KEY  
PB2  - KEY  

* UART  
PB6 - UART Tx  
PB7 - UART Rx  
> UARTx = USART1  
> BaudRate = 115200  
> ByteSize = 8  
> StopBits = 1  
> Parity = 'N'  
> HardwareFlowControl = None  
