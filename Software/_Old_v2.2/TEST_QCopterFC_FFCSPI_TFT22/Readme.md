QCopterFC FFCSPI-TFT2.2 Example
========
* Author  : [Hom](http://about.me/home)
* Update  : 2014/06/10

Description
========
QCopterFC FFCSPI-TFT2.2 的範例程式，透過 FFCSPI 讀寫 2.2 吋螢幕，並顯示彩條。

Hardware
========
* LED  
PC13 - LED_B  
PC14 - LED_G  
PC15 - LED_R  

* KEY  
PB2  - KEY  

* FFCSPI - TFT2.2  
PA4 - TFT22_CST  
PA5 - TFT22_SCK  
PA6 - TFT22_SDO  
PA7 - TFT22_SDI  
PC0 - TFT22_RST  
PC1 - TFT22_DC  
> SPIx = SPI1  
> Dir = 2Lines_FullDuplex  
> Mode = Slave  
> CPOL = High  
> CPHA = 2Edge  
> FirstBit = MSB  
> DataSize = 8 bits  
> BaudRate = Prescaler_2 
