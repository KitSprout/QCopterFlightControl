QCopterFC IMU Example
========
* Author  : [Hom](https://github.com/Hom19910422)
* Update  : 2014/01/24

Description
========
QCopterFC IMU 的範例程式，以 500Hz 讀取感測器，並透過 RS232 將資料傳送至 PC 端的 VisualScope 做顯示。

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
> BaudRate = 9600  
> ByteSize = 8  
> StopBits = 1  
> Parity = 'N'  
> HardwareFlowControl = None  

* I2C  
PB8  - I2C_SCL  
PB9  - I2C_SDA  
> I2Cx       = I2C1  
> ClockSpeed = 400KHz  
