QCopter Flight Control
========
* File Name   : README.md
* Author      : Hom19910422
* Version     : v1.1
* Date        : 2013/08/01
* Description : QCopterFlightControl Information
* Wiki        : https://github.com/Hom19910422/QCopterFlightControl/wiki

飛控板　FlightControl
========
* 控制器　STM32F405RG 64Pin 168MHz DSP FPU
* 感測器　IMU 10DOF (MPU-9150 + MS5611)
* 無線傳輸　nRF24L01P + PA + LNA
* 儲存紀錄　SD卡，使用SDIO操作
* 1個SPI接口　與攝像板連接、外擴其他裝置
* 1個USB接口　與攝像板連接、外擴其他裝置
* 2個ADC通道　測量電量
* 12個PWM通道　連接電調、LED、PPM
* 1個USART接口　可擴充GPS模組、藍芽模組、電腦傳輸RS232

Hardware
========
* Use Altium Designer 13

Test
========
* QCopterFC LED
* QCopterFC USART
* QCopterFC PWM
* QCopterFC ADC
* QCopterFC CamSPI-Master
* QCopterFC IMU
* QCopterFC NRF
* QCopterFC SDIO

Program
========
* QcopterFC FlightControl
* QcopterFC FlightRecorder
