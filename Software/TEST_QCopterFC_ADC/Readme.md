QCopterFC ADC Example
========
* Author  : [Hom](https://github.com/Hom19910422)
* Update  : 2014/01/24

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
PA4  - ADC_I  
PA5  - ADC_V  

* UART  
PB10 - UART Tx  
PB11 - UART Rx  
> UARTx = USART3  
> BaudRate = 115200  
> ByteSize = 8  
> StopBits = 1  
> Parity = 'N'  
> HardwareFlowControl = None  
