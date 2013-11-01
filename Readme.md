QCopter Flight Control
========
* File Name   : Readme.md
* Author      : Hom19910422
* Version     : v2.0
* Update      : 2013/11/01
* Description : QCopterFlightControl Information
* Wiki        : https://github.com/Hom19910422/QCopterFlightControl/wiki

飛控板　FlightControl
========
* 控制器　STM32F405R 64Pin 168MHz DSP FPU
* 感測器　IMU_10-DOF (MPU-9150 + MS5611)
* 無線傳輸　nRF24L01P + PA + LNA
* 儲存紀錄　SD卡，使用SDIO操作
* 1個SPI接口　與攝像板連接、外擴其他裝置
* 1個USB接口　外擴其他裝置
* 2個ADC通道　測量電量
* 12個PWM通道　連接電調、LED、PPM
* 1個UART接口　可擴充 GPS 模組、藍芽模組、RS232

Hardware
========
* Use Altium Designer 13
* PCB Size : 52 * 35mm (Screws M3: 30 * 30mm)

Program
========
* QcopterFC FlightControl Pre-Alpha
* QcopterFC FlightRecorder Pre-Alpha

Test
========
* QCopterFC LED
* QCopterFC UART
* QCopterFC PWM (( TIM8目前無法輸出PWM
* QCopterFC ADC
* QCopterFC CamSPI-Master
* QCopterFC IMU
* QCopterFC NRF
* QCopterFC SDIO  
  
  
<a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/tw/deed.zh_TW"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">QCopter FlightControl</span>由<a xmlns:cc="http://creativecommons.org/ns#" href="https://plus.google.com/u/0/112822505513154783828/posts" property="cc:attributionName" rel="cc:attributionURL">Hom</a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/tw/deed.zh_TW">創用CC 姓名標示-相同方式分享 3.0 台灣 授權條款</a>釋出。<br />此作品衍生自<a xmlns:dct="http://purl.org/dc/terms/" href="https://github.com/Hom19910422" rel="dct:source">github.com</a>