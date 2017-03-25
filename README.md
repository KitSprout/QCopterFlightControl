QCopterFC
========
[![GitHub version](https://img.shields.io/badge/QFC%20version-v2.2-brightgreen.svg)](https://github.com/KitSprout/QCopterFlightControl/tree/b4c5a7a2395dcd7b22ef3b991fd287610df9cdff)
[![GitHub version](https://img.shields.io/badge/QFCs%20version-v1.0-brightgreen.svg)](https://github.com/KitSprout/QCopterFlightControl)
[![GitHub license](https://img.shields.io/badge/license-%20MIT%20%2F%20CC%20BY--SA%204.0-blue.svg)](https://github.com/KitSprout/QCopterRemoteControl/blob/master/LICENSE)
[![GitHub pcb library](https://img.shields.io/badge/pcb%20library-%20v2.16-yellow.svg)](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.16)


QCopterFC 是一個基於 STM32F4 的飛行控制器，主要用來實現濾波、平衡、控制等演算法的平台。集成微控制器 9 DOF 慣性測量元件以及氣壓計，目前遙控器有 QCopterFC 與 QCopterFCs 兩種版本，前者使用芯片效能較高、功能多、硬體完善，後者功能較為簡潔，以擴充方式實現不同需求，較為靈活。目前開發著重於 QCopterFCs，想了解 QCopterFC 的可以進入該分支 [branche](https://github.com/KitSprout/QCopterFlightControl/tree/QCopterFC)

Hardware
========
* 控制器　 : [STM32F411CE](http://www.st.com/en/microcontrollers/stm32f411ce.html) 48Pin 100MHz DSP FPU
* 感測器　 : [MPU9250](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/) + [LPS22HB](http://www.st.com/en/mems-and-sensors/lps22hb.html)
* PCB 尺寸 : 26 * 26mm ( Screws M3: 20 * 20mm )
* 設計軟體 [Altium Designer 17](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.16](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.16) )

View
========
<img src="" />

<br />
[more photo...](https://goo.gl/photos/LVRgoCt4GyYr4dVZ8)

Config
========
<img src="" />

Schematic
========
<img src="" />

Get it
========
Ruten Shop
