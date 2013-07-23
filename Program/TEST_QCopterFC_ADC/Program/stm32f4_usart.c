/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NumToChar
**功能 : 數值轉字串
**輸入 : Type, Num, *CharData, NumData
**輸出 : None
**使用 : NumToChar(Type_O, 6, TrData, Data)
**=====================================================================================================*/
/*=====================================================================================================*/
void NumToChar( u8 Type, u8 Num, u8 *CharData, s32 NumData )
{
	u8 i = 0;
	u32 Tmp[48] = {0};
	u32 NumTmp = 1;

	switch(Type) {

		case Type_B:
		case Type_O:
		case Type_D:
		case Type_H:
			for(i=0; i<Num; i++) {
				Tmp[i] = ((u32)NumData) / NumTmp;
				NumTmp = NumTmp * Type;
			}
			for(i=0; i<Num; i++) {
				CharData[Num-i-1] = Tmp[i] - Tmp[i+1]*Type;
				if(CharData[Num-i-1]>9)
					CharData[Num-i-1] += 55;	// 65-10
				else
					CharData[Num-i-1] += 48;
			}
			CharData[Num] = '\0';
			break;

		case Type_I:
			if(NumData<0) {
				CharData[0] = '-';
				NumData = (~NumData) + 1;
				for(i=1; i<Num+1; i++) {
					Tmp[i-1] = ((u32)NumData) / NumTmp;
					NumTmp = NumTmp * 10;
				}
				for(i=1; i<Num+1; i++) {
					CharData[Num-i+1] = Tmp[i-1] - Tmp[i]*10;
					CharData[Num-i+1] += 48;
				}
				CharData[Num+1] = '\0';
			}
			else {
				CharData[0] = '+';
				//NumData = (~NumData) + 1;
				for(i=1; i<Num+1; i++) {
					Tmp[i-1] = ((u32)NumData) / NumTmp;
					NumTmp = NumTmp * 10;
				}
				for(i=1; i<Num+1; i++) {
					CharData[Num-i+1] = Tmp[i-1] - Tmp[i]*10;
					CharData[Num-i+1] += 48;
				}
				CharData[Num+1] = '\0';
			}
			break;

// 		case Type_F:
// 				
// 			break;
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
