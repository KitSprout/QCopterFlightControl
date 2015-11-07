QCopterFC ADC Example
========
* Author  : [Hom](about.me/Hom)
* Update  : 2014/05/06

Description
========
QCopterFC ADC 的範例程式，連續讀取 ADC_I 和 ADC_V 資料，同時 LED_G 會一直閃爍。

Hardware
========
* LED  
PC13 - LED_B  
PC14 - LED_G  
PC15 - LED_R  

* KEY  
PB2  - KEY  

* ADC  
PC4  - ADC_I  
PC5  - ADC_V  

* UART  
PB6 - UART Tx  
PB7 - UART Rx  
> UARTx = USART1  
> BaudRate = 115200  
> ByteSize = 8  
> StopBits = 1  
> Parity = 'N'  
> HardwareFlowControl = None  
