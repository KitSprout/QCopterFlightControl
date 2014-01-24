QCopterFC CamSPI Master Example
========
* Author : [Hom](https://github.com/Hom19910422)
* Update : 2014/01/24

Description
========
QCopterFC CamSPI Master 的範例程式，當按下按鍵 KEY 時，CamSPI 會送出指令，並且接收回應，若正確 LED 則會發亮，最後會一起閃爍。  

Hardware
========
* LED  
PC13 - LED_B  
PC14 - LED_G  
PC15 - LED_R  

* KEY  
PB2  - KEY  

* CamSPI  
PA15 - CamSPI_CS  
PB3  - CamSPI_SCK  
PB4  - CamSPI_MISO  
PB5  - CamSPI_MOSI  
> SPIx = SPI3  
> Dir = 2Lines_FullDuplex  
> Mode = Master  
> CPOL = Low  
> CPHA = 1Edge  
> FirstBit = MSB  
> DataSize = 8 bits  
> BaudRate = Prescaler_256  
