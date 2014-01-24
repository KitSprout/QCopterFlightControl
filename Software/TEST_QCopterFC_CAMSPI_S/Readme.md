QCopterFC CamSPI Slave Example
========
* Author : [Hom](https://github.com/Hom19910422)
* Update : 2014/01/24

Description
========
QCopterFC CamSPI Slave 的範例程式，接收到指令會回傳對應資料。  

Hardware
========
* CamSPI  
PA15 - CamSPI_CS  
PB3  - CamSPI_SCK  
PB4  - CamSPI_MISO  
PB5  - CamSPI_MOSI  
> SPIx　　 = SPI3  
> Dir　　　= 2Lines_FullDuplex  
> Mode　　 = Slave  
> CPOL　　 = Low  
> CPHA　　 = 1Edge  
> FirstBit = MSB  
> DataSize = 8 bits  
> BaudRate = Prescaler_256  
