 /*****************************************************************************
 *==================******************************************================*
 *==================**   Author: Mohamed Samir Hashish      **================*
 *==================**               NTI                    **================*
 *==================******************************************================*/
 /*****************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "util/delay.h"
#include "TWI_int.h"
#include "EXT_EEPROM_int.h"
#include "EXT_EEPROM_private.h"




void H_EXT_EEPROM_void_Init(void)
{
	M_TWI_void_Init();
}


/***************************************************************************/

void H_EXT_EEPROM_void_Write(u16 Copy_u16ByteNo , u8 Copy_u8Data)
{


M_TWI_u8_StartCondition();

M_TWI_u8_SendSlaveAddressWrite( EXT_EEPROM_ADDRESS_SEQ | (u8)(Copy_u16ByteNo>>8)   );

M_TWI_u8_SendByte( (u8)Copy_u16ByteNo);

M_TWI_u8_SendByte(Copy_u8Data);

M_TWI_void_StopCondition();

	_delay_ms(10);
}

/***************************************************************************/
u8   H_EXT_EEPROM_void_Read(u16 Copy_u16ByteNo)
{
	u8 data = 0;
	u8 Local_u8Data=0 ;
	data=M_TWI_u8_StartCondition();
	data=M_TWI_u8_SendSlaveAddressWrite(EXT_EEPROM_ADDRESS_SEQ | (u8)(Copy_u16ByteNo>>8));
	data=M_TWI_u8_SendByte((u8)Copy_u16ByteNo);
	data=M_TWI_u8_RepeatedStart();
	data=M_TWI_u8_SendSlaveAddressRead( EXT_EEPROM_ADDRESS_SEQ | (u8)(Copy_u16ByteNo>>8));
	data=M_TWI_u8_ReadByte(&Local_u8Data);
	M_TWI_void_StopCondition();

	return Local_u8Data;
}
/***************************************************************************/
void H_EXT_EEPROM_void_Write_Str(u16 ByteNo , u8* DATA)
{
	s8 i = -1 ;
	u8 BN=ByteNo ;
       BN--;
	do
	{
		i++ ;
		BN++;
		H_EXT_EEPROM_void_Write(BN , DATA[i]);

	}while( DATA[i] != '\0' ) ;


}
/***************************************************************************/
void H_EXT_EEPROM_void_Read_Str(u16 ByteNo , u8* DATA)
{
	s8 i = -1 ;
	u8 BN=ByteNo ;
       BN--;
	do
	{
		i++ ;
		BN++;
		DATA[i]= H_EXT_EEPROM_void_Read(BN);


	}while( DATA[i] != '\0' ) ;


}

