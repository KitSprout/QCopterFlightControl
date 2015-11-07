QCopterFC UART Example
========
* Author  : [Hom](http://about.me/Hom)
* Update  : 2014/07/10

Description
========
QCopterFC UART 的範例程式，
USE_RS232：將鍵盤輸入的資料顯示在 Putty 上，同時綠色 LED 也會翻轉，
USE_VISUALSCOPE：將變數 i 透過 RS232 傳輸至電腦端，每傳一次 i 值就會加 1000，加到 32767 會從 -32768 重新開始再加，同時綠色 LED 也會閃爍。

Hardware
========
* LED  
PC13 - LED_B  
PC14 - LED_G  
PC15 - LED_R  

* UART  
PB6 - UART Tx  
PB7 - UART Rx  
> UARTx = USART1  
> BaudRate = 9600  
> ByteSize = 8  
> StopBits = 1  
> Parity = 'N'  
> HardwareFlowControl = None  
