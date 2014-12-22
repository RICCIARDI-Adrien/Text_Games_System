/** @file Driver_I2C.c
 * @see Driver_I2C.h for description.
 * @author Adrien RICCIARDI
 */
#include <Driver_I2C.h>
#include <system.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Bus address of the external EEPROM when requesting a write operation. */
#define EEPROM_ADDRESS_WRITE 0xA0
/** Bus address of the external EEPROM when requesting a read operation. */
#define EEPROM_ADDRESS_READ 0xA1

/** Send a Start condition over the I2C bus. */
#define I2C_SEND_START() \
{ \
	pir1.SSPIF = 0; \
	sspcon2.SEN = 1; \
}

/** Send a Stop condition over the I2C bus. */
#define I2C_SEND_STOP() sspcon2.PEN = 1

/** Wait until the previous I2C operation has finished. */
#define I2C_WAIT_OPERATION_END() \
{ \
	while (!pir1.SSPIF); \
	pir1.SSPIF = 0; \
}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
#if 0
static void I2CWaitDeviceBusy(void)
{
	// The busy EEPROM does not acknowledge a write command
	do
	{
		// Send a start condition to the bus
		/*sspcon2.SEN = 1;
		while (sspcon2.SEN); // Wait for start condition completion*/
		I2C_SEND_START();
		I2C_WAIT_OPERATION_END();
		
		// Send device address and write request
		/*pir1.SSPIF = 0;
		sspbuf = EEPROM_ADDRESS_WRITE;
		while (!pir1.SSPIF); // Wait for transmission end*/
		sspbuf = EEPROM_ADDRESS_WRITE;
		I2C_WAIT_OPERATION_END();
	}
	while (sspcon2.ACKSTAT);
	
	// Send stop condition to abort write operation
	/*sspcon2.PEN = 1;
	while (sspcon2.PEN); // Wait for stop condition completion*/
	I2C_SEND_STOP();
	I2C_WAIT_OPERATION_END();
}
#endif

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void I2CInitialize(void)
{
	// Set I2C pins as inputs
	trisc.3 = 1;
	trisc.4 = 1;
	
	sspstat = 0x00; // 400KHz speed mode, I2C input levels
	sspcon2 = 0x00; // Reset communication flags
	sspadd = 2; // Baud_Rate = Fosc / (4 * (SSPADD + 1)) => SSPADD = (Fosc / (4 * Baud_Rate)) - 1 ; here ~333 KHz
	sspcon = 0x28; // Enable I2C module in Master mode
}

void I2CSetCurrentAddress(unsigned short Address)
{
	// Send a start condition to the bus
	I2C_SEND_START();
	I2C_WAIT_OPERATION_END();
	
	// Send device address and write request in order to set the device internal address register
	sspbuf = EEPROM_ADDRESS_WRITE;
	I2C_WAIT_OPERATION_END();
	
	// Send address high byte
	sspbuf = Address >> 8;
	I2C_WAIT_OPERATION_END();
	
	// Send address low byte
	sspbuf = Address;
	I2C_WAIT_OPERATION_END();
	
	// Send stop condition to abort write operation
	I2C_SEND_STOP();
	I2C_WAIT_OPERATION_END();
}

unsigned char I2CReadNextByte(void)
{
	// Send a start condition to the bus
	I2C_SEND_START();
	I2C_WAIT_OPERATION_END();
	
	// Send device address and read request
	sspbuf = EEPROM_ADDRESS_READ;
	I2C_WAIT_OPERATION_END();
	
	// Receive a byte from the device
	sspcon2.RCEN = 1;
	I2C_WAIT_OPERATION_END();
	
	// Send a Not Acknowledge to the device
	sspcon2.ACKDT = 1; // NACK
	sspcon2.ACKEN = 1;
	I2C_WAIT_OPERATION_END();
	
	// Send stop condition
	I2C_SEND_STOP();
	I2C_WAIT_OPERATION_END();
	
	return sspbuf;
}

void I2CWriteByte(unsigned short Address, unsigned char Byte)
{
	// Can't directly write a byte, the datasheet indicates that the address must be set for each write operation
	// I was not able to make the "acknowledge polling" mode works so I used a delay

	// Send a start condition to the bus
	I2C_SEND_START();
	I2C_WAIT_OPERATION_END();

	// Send device address and write request
	sspbuf = EEPROM_ADDRESS_WRITE;
	I2C_WAIT_OPERATION_END();
	
	// Send address high byte
	sspbuf = Address >> 8;
	I2C_WAIT_OPERATION_END();

	// Send address low byte
	sspbuf = Address;
	I2C_WAIT_OPERATION_END();
	
	// Send data byte
	sspbuf = Byte;
	I2C_WAIT_OPERATION_END();

	// Send stop condition
	I2C_SEND_STOP();
	I2C_WAIT_OPERATION_END();
	
	// Wait the maximum time required by the EEPROM to complete the write operation
	delay_ms(5);
	//I2CWaitDeviceBusy();
}