QCopterFC UART Example
========
* Author  : [Hom](https://github.com/Hom19910422)
* Update  : 2014/01/18

Description
========
QCopterFC UART 的範例程式，將變數 i 透過 RS232 傳輸至電腦端，每傳一次 i 值就會加 1，加到 255 會從 0 重新開始再加，同時綠色 LED 也會閃爍。

Hardware
========
* LED  
PC13 - LED_B  
PC14 - LED_G  
PC15 - LED_R  

* UART  
PB10 - USART3 Tx  
PB11 - USART3 Rx  
> BaudRate = 115200  
> ByteSize = 8  
> StopBits = 1  
> Parity = 'N'  
> HardwareFlowControl = None  
