QCopterFC FlightControl Example
========
* Author  : [Hom](https://github.com/Hom-Wang)
* Update  : 2014/01/27

Description
========
QCopterFC 飛行控制程式，以 500Hz 讀取感測器。

Program flow
========

### 啟動初始化部分

QCopterFC_Init()
> 初始化 STM32  
> 配置所需 I/O  
> 校正電子調速器  
> 初始化 Sensor 和 RF  
> Check RF 連接  

QCopterFC_Corr(SampleRateFreg)
> 啟動 Systick, 並設定中斷為 500Hz  
> 等待 Sensor 校正和姿態系統初始化  

QCopterFC_Lock()
> 鎖住飛控版，等待解鎖訊號  

### 主程式部分

main()
> FSM_TXRX -> 無線傳輸資料  
> FSM_CTRL -> 控制訊號  
> FSM_UART -> UART 傳輸  
> FSM_DATA -> 資料轉換  

### 姿態平衡部分

SysTick_Handler()
> SEN_CORR -> 選擇是否要校正 Sensor  
> SEN_GYR -> 校正陀螺儀  
> SEN_ACC -> 校正加速度計  
> SEN_MAG -> 較正電子羅盤  
> SEN_NUMQ -> 初始化四元數和姿態系統  
> SEN_ALG -> 計算姿態角度和平衡控制  
