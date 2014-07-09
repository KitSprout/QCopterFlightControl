[QCopterFC](https://github.com/QCopter/QCopterFlightControl)
========
* Author  : [Hom](https://about.me/Hom)
* Version : v2.2
* Update  : 2014/07/09

Description
========
QCopterFC 是一個基於 STM32F4 的飛行控制器，可以應用於固定翼、旋翼飛行器上面，用來實現濾波、平衡、控制等演算法的平台。感測器部分使用 SmartIMU，其集成微控制器 STM32F401C、9 DOF 慣性測量元件 MPU9250 以及氣壓計 MS5611，SmartIMU 提供了兩種操作模式，讀取感測器原始資料與讀取當下姿態角度，可以透過指令選擇，無線傳輸部分使用工作於 2.4GHz 頻段的無線傳輸模組 nRF24L01P，傳輸飛行器上相關資訊，同時也可以藉由該模組從外部接收飛行控制指令，另外板上還有 Micro USB 與 Micro SD，並且引出獨立的 UART、SPI、ADC、PWM、CAN，使的 QCopterFC 可以有更多的應用、擴充。

License
========
* 硬體(Hardware)採用 [CC BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW) 方式授權 
  
　　<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/80x15.png" /></a>  
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> QCopterFC v2.2 </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="http://about.me/Hom" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 4.0 國際 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : [STM32F405R](http://www.st.com/web/catalog/mmc/FM141/SC1169/SS1577/LN1035/PF252144) 64Pin 168MHz DSP FPU
* 感測器　 : [SmartIMU](https://github.com/Hom-Wang/SmartIMU) ( [STM32F401C](http://www.st.com/web/en/catalog/mmc/FM141/SC1169/SS1577/LN1810) + [MPU-9250](http://www.invensense.com/mems/gyro/mpu9250.html) + [MS5611](http://www.meas-spec.com/product/pressure/MS5611-01BA03.aspx) )
* 無線傳輸 : [nRF24L01P](http://www.nordicsemi.com/eng/Products/2.4GHz-RF/nRF24L01P) + PA + LNA
* 儲存紀錄 : Micro SD，使用 SDIO 操作
* 外接介面 : 1*UART、1*SPI ( FFCSPI )、2*ADC、1*USB ( Micro )、1*CAN、8*PWM
* PCB 尺寸 : 51.5 * 35mm ( Screws M3: 30 * 30mm )
* 設計軟體 [Altium Designer 14](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v0.12](https://github.com/OpenPCB/AltiumDesigner_PcbLibrary/releases/tag/v0.12) )  

 > 預計更新：  
 > ．飛行方向標示  
 > ．微調元件間距  

<img src="https://lh6.googleusercontent.com/-DM7NHDq6rQs/U1UsC_ElBbI/AAAAAAAAHq0/5NbuoV3Cs6A/s1600/QCopterFC_System_v2.2.png" />

Related Documents
========
* [Update Records - Hackpad](https://hom.hackpad.com/QCopterFC-EvK2EHj4bqH)
* [Datasheet & BOM - Google Drive](https://drive.google.com/folderview?id=0BzL2wwAot6oPS0thRUVrb0VadTQ&usp=sharing)

Software
========
* QCopterFC FlightControl
* QCopterFC FlightRecorder
* [QCopterFC ADC](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_ADC)
* [QCopterFC FFCSPI_TFT22](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_FFCSPI_TFT22) ( Use [TFT_2.2-inch](https://github.com/OpenPCB/TFT_2.2-inch) )
* [QCopterFC FFCSPI_TFT22_WF](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_FFCSPI_TFT22_WF)
* [QCopterFC FLASH](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_FLASH)
* [QCopterFC FreeRTOS_LED](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_FreeRTOS_LED) ( Use [FreeRTOS](http://www.freertos.org/) 8.0.1 )
* [QCopterFC IAP](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_IAP)
* [QCopterFC IAP_LED](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_IAP_LED)
* [QCopterFC IMU](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_IMU) ( Use [SmartIMU](https://github.com/Hom-Wang/SmartIMU) )
* [QCopterFC IMU_WF](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_IMU_WF)
* [QCopterFC LED](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_LED)
* [QCopterFC NRF](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_NRF) ( Use [nRF24L01+](https://www.nordicsemi.com/chi/node_176/2.4GHz-RF/nRF24L01P) )
* [QCopterFC PWM](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_PWM)
* [QCopterFC SDIO](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_SDIO) ( Use [Fatfs](http://elm-chan.org/fsw/ff/00index_e.html) 0.10a )
* [QCopterFC UART](https://github.com/QCopter/QCopterFlightControl/tree/master/Software/TEST_QCopterFC_UART)

View
========
<img src="https://lh5.googleusercontent.com/-aCFF4G5FxdU/U5ctHNIhsVI/AAAAAAAAJEI/X5uot4Gjqeg/s1600/DSC_2243.jpg" />
<img src="https://lh3.googleusercontent.com/-PsoV7JZEWkc/U5ctGThWgKI/AAAAAAAAJEE/fsjt34W0jXk/s1600/DSC_2242.jpg" />
<br />
更多圖片 [Google+ albums](https://plus.google.com/u/0/photos/+文宏王Hom/albums/5899377395636747681)

Config
========
．Vin 建議輸入 5v ~ 6v
<img src="https://lh3.googleusercontent.com/-i1jYrr84_Tc/U1lOvLOxa9I/AAAAAAAAH1I/iJ3X6pUUzP4/s1600/MechanicalTop.png" />
<img src="https://lh6.googleusercontent.com/-ccBIlU9MAfQ/U1lOvAvklgI/AAAAAAAAH1E/YsOaFJj8hXk/s1600/MechanicalBottom.png" />

Schematic
========
<img src="https://lh6.googleusercontent.com/-H14qpF6-0r0/U1UsEegXJiI/AAAAAAAAHrQ/8hxHSZB0SXY/s1600/QCopterFC_v2.2_Sch_1.png" />
<img src="https://lh4.googleusercontent.com/-e7WyY739wD4/U1UsFOjysDI/AAAAAAAAHrw/TC9kQGoledM/s1600/QCopterFC_v2.2_Sch_2.png" />
