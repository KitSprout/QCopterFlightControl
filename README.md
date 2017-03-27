QCopterFC
========
[![GitHub version](https://img.shields.io/badge/QFC%20version-v2.2-brightgreen.svg)](https://github.com/KitSprout/QCopterFlightControl/tree/b4c5a7a2395dcd7b22ef3b991fd287610df9cdff)
[![GitHub version](https://img.shields.io/badge/QFCs%20version-v1.0-brightgreen.svg)](https://github.com/KitSprout/QCopterFlightControl)
[![GitHub license](https://img.shields.io/badge/license-%20MIT%20%2F%20CC%20BY--SA%204.0-blue.svg)](https://github.com/KitSprout/QCopterFlightControl/blob/master/LICENSE)
[![GitHub pcb library](https://img.shields.io/badge/pcb%20library-%20v2.16-yellow.svg)](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.16)


QCopterFC 是一個基於 STM32F4 的飛行控制器，主要用來實現濾波、平衡、控制等演算法的平台。集成微控制器 9 DOF 慣性測量元件以及氣壓計，目前有 QCopterFC 與 QCopterFCs 兩種版本，前者使用芯片效能較高、功能多、硬體完善，後者功能較為簡潔，以擴充方式實現不同需求，靈活度高。目前開發著重於 QCopterFCs，想了解 QCopterFC 的可以進入該分支 [branche](https://github.com/KitSprout/QCopterFlightControl/tree/QCopterFC)

Hardware
========
* 控制器　 : [STM32F411CE](http://www.st.com/en/microcontrollers/stm32f411ce.html) 48Pin 100MHz DSP FPU
* 感測器　 : [MPU9250](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/) + [LPS22HB](http://www.st.com/en/mems-and-sensors/lps22hb.html)
* PCB 尺寸 : 26 * 26mm ( Screws M3: 20 * 20mm )
* 設計軟體 [Altium Designer 17](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.16](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.16) )

View
========
<img src="https://lh3.googleusercontent.com/hb8YwQ8Ec85S0KuNUv23IW5hVOOJd3HtoLyhCc_ewsBfWaHWFq-GK0lpd6p8lotzPEFO63HJqHt8eRFjoHVPh-yu8As_p10CcrOxU9T4Z2tguE1ZeKuA3aF_piJyBL9Q43xECsaNKdOSHea7TLN_n13cv-YwFM5sQt3GBK3w16776-crhO-xnq6c3-y0J_Q-i_TdPjB5IF7g6zqfCzOGtjkPWgSKYzrlsawMiUDjCECHNydLpvCOWgm8XUk98ggVBch3fB0FsZXrZ8lPN1Ndcb4YteeeF39NTjCWwyea06AOzlnYOz4Yb_wX4r-_by7VktfoHO6HqoAG3k0nBfntnOOxx1YurUwll7JzTWTuxpNwEceQurpDsfCVKChVb6c8sB8wST68EyWeRlsvBWRqAZ9-kD-xiPoKhYJbvWoJrAaVeiipRXQt5wfoffubktiUFPTv1soReYC1DCR7FgdaBKA6Hct5mE1CQgHNKwo3l2M9_gR7JE4PrTDwk9larMSnmEeNOwpyR6XhRNbwFobl-4-ivuqQ_4ECZ2UbdLQSfB8xXap4h4G1dxdPUYvjjKpCjlAQ1suHtBwerTZ6stzjMXZpGdSjnKqiXVs5kFTWYE2JFCP9VJNddMPoMSI8FKkG_6cz-iSAwk1ltZ8j6huv6denR9O1G54KGrwJ5igz_A=w2000" />

<br />
[more photo](https://goo.gl/photos/LVRgoCt4GyYr4dVZ8)...

Config
========
<img src="https://lh3.googleusercontent.com/HODzDcYpuODZuabYmN20IMYbJdJaIFH0Sw3fbejPAIb3Up0hfpcDMytvB1PYuj723lh3RTHvxK7uXtjmYwT9x-U5-RboC4SACkH3mJD9CgDE06FQL0xPmQ-7X70lqk7s4RX_Q9NK4wgG4_ZgkcChi28rQYznVMlyrC_gqUhawVCIrU4FqyksJ3L-4---ZRBS9UzPEjI0I_mvxj3gXRfFESvjtJeaZ16Hhl-8o6EFV4-t3tACQb_dtx57coNcU6x9MXAhL1OGfx2DVrvEf3dKYUgyGd4BXl5yOi9ca2wj_28pIm2iPmv8_RanhMO3YUw7byMfJ10DV4GOVDMbwPCsKbRKDYXNx3AUE2GtCAVfnJ8n7VSprwDHWcwL-OeF9BtNa8R3XzWBqZGmBR6r-3899bB67hxklWQ6qBiBQvF_9T2AsUmOvRJ2o9_GcHXMOG8TRATWUzxE_qFEUJDRkygEq2wW2ALnEycC3oyEM_AcoitW8AVWf9AH_dWttldoWBPW4SKf34hSKQItQJ2DKg_0VzFZ6dg5V0NZHadAy_VHrkXgO_bQo9NoFdKEiC0d-Udh_15_nNiOheb_IP-lG9wrnf-eJAi0n9nXZEEP9X8pbqW5QEULlT6bSYByeqnzhtssXkTEU2n_8OYeN8kSC_pMYzfw2nRfk751nFaJqsHP7g=w1700" />

Schematic
========
<img src="https://lh3.googleusercontent.com/LxfR6NXUhPvNYK_5ZHkASRROVcRPEzpZZTRx1cusujp_UBrBxYnp4_gs6fmFRAFsbo9vgOgxgzUdgiasDcyJaFDV7tenHQqklTYcimBtUdxUmTXLpPbgyAr5G5nQj-STVegWTKdtnvULsPjpV_csHxeGtfc26CRcrVf41MQ_8YjgP3l_odrIccikS2i9hl81MSGV4VAh2vXIhYOm6wHuB_suR4Sk5GyNee7QjbKxb5nYRn6W4tuXPla-PTCkik0phSdVRVgVRO9Z9MeBiQsm7CS7QM3HJuTRRQXbI0aAxGp1EAewzotExpQFcFqc3ie62w_F2fyc4J9UgGku4k8i-lsrJqB2eGZ4x7o6VHVgYXcUKBMlhhg_fXV5-7PCBcqzepxTA8iFiQdDGFrZvBcGMvuNKi8Wl3s5N3K5fwmoP0ojd83Z8N1fZgpqxAg44XBgNMEDlo3zccUe_5ig3KflLiHuYukkA2qDS_laRNUU0cERDjqUBKFAoP-6I0wglmI5S3gCBql7iNHEWKvrr2I3OlUppR2eC40FWRr-04l18-HbvYaQg_wl7jyQ6blKEVrjc1Ewqmg8Cb_nv_y69qxlSyAVE1DhKdOzz1Bbm7w6eIpaZxsWGJi1X-Ee5DIvzIUNIPlU1PM9C01wfioIdIYL1ahMIgcKLURyXtpww9ADBw=w2000" />

Get it
========
Ruten Shop
