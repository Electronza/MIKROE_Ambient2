/*
  OPT3001.cpp
  Created by Teodor Costachioiu, August 2017
  https://electronza.com
  Released into the public domain.
*/

#include <Arduino.h>
#include "Ambient2.h"


/*******************************************************************************
* Function Write_Register()
* ------------------------------------------------------------------------------
* Overview: Function writes (uint16_t) to a register of the OPT3001
* Input: Register_Name (uint8_t), Value (uint16_t)
* Output: Nothing
*******************************************************************************/
void Opt3001::Write_Register(uint8_t Register_Name, uint16_t Value)
{
	// Begin Transmission 
	Wire.beginTransmission(Ambient2_Addr);

	// Send Register number
	Wire.write(Register_Name);

	// Read two bytes
	Wire.write((uint8_t)(Value >> 8));
	Wire.write((uint8_t)(Value & 0x00FF));

	// Send STOP (close communication)
	Wire.endTransmission();
	return;    
}

/*******************************************************************************
* Function Read_Register()
* ------------------------------------------------------------------------------
* Overview: Function configurates OPT3001 Ambient sensor
* Input: Register Name
* Output: Register Value (uint16_t)
*******************************************************************************/
uint16_t Opt3001::Read_Register(uint8_t Register_Name)
{
	uint8_t LSB;
	uint8_t MSB;

	/* Begin Transmission at address of device on bus */
	Wire.beginTransmission(Ambient2_Addr);
	Wire.write(Register_Name);
	Wire.endTransmission();

	/* Requests 2 bytes from Slave */
	Wire.requestFrom(Ambient2_Addr, 2);
	while(Wire.available() < 2)	{}

	/* Get data from sensor */
	MSB = Wire.read();
	LSB = Wire.read();

	return (MSB << 8) | LSB;
}

/*******************************************************************************
* Function begin()
* ------------------------------------------------------------------------------
* Overview: Function configures the Ambient2 click
* Input: Configuration value (uint16_t)
* Output: Nothing
*******************************************************************************/
void Opt3001::begin()
{
    Write_Register(OPT3001_CONFIG_REG, OPT3001_DEFAULT_CONFIG);
}

/*******************************************************************************
* Function read_DeviceID()
* ------------------------------------------------------------------------------
* Overview: Reads the value of teh DeviceID register
* Input: Nothing
* Output: DeviceID
*******************************************************************************/
uint16_t Opt3001::read_DeviceID()
{
    return Read_Register (OPT3001_DEVICEID_REG);
}

/*******************************************************************************
* Function read_ManufacturerId()
* ------------------------------------------------------------------------------
* Overview: Reads the value of the Manufacturer register
* Input: Nothing
* Output: DeviceID
*******************************************************************************/
uint16_t Opt3001::read_ManufacturerID()
{
    return Read_Register (OPT3001_MANUFACTURER_REG);
}

/*******************************************************************************
* Function read_Config()
* ------------------------------------------------------------------------------
* Overview: Reads the value of the Manufacturer register
* Input: Nothing
* Output: DeviceID
*******************************************************************************/
uint16_t Opt3001::read_Config()
{
    return Read_Register (OPT3001_CONFIG_REG);
}

/*******************************************************************************
* Function read_HighLimit()
* ------------------------------------------------------------------------------
* Overview: Reads the value of the HIGH Limit register
* Input: Nothing
* Output: HIGH limit
*******************************************************************************/
uint16_t Opt3001::read_HighLimit()
{
    return Read_Register (OPT3001_HIGHLIMIT_REG);
}

/*******************************************************************************
* Function read_LowLimit()
* ------------------------------------------------------------------------------
* Overview: Reads the value of the LOW limit register
* Input: Nothing
* Output: LOW limit (in lux))
*******************************************************************************/
uint16_t Opt3001::read_LowLimit()
{
    return Read_Register (OPT3001_LOWLIMIT_REG);
}

/*******************************************************************************
* Function write_Config_Config()
* ------------------------------------------------------------------------------
* Overview: Writes a value into the CONFIG register
* Input: value
* Output: Nothing
*******************************************************************************/
void Opt3001::write_Config(uint16_t value)
{
    Write_Register(OPT3001_CONFIG_REG, value);
}


/*******************************************************************************
* Function set_time100()
* ------------------------------------------------------------------------------
* Overview: Sets the conversion time to 100ms
* Input: Nothing
* Output: Nothing
*******************************************************************************/
void Opt3001::set_time100()
{
	// Conversion time 100 means bit 11 of CONFIG register is set to 0
	uint16_t myconfig = read_Config();
	myconfig = myconfig & 0xF7FF;
    write_Config(myconfig);
}

/*******************************************************************************
* Function set_time800()
* ------------------------------------------------------------------------------
* Overview: Sets the conversion time to 800ms
* Input: Nothing
* Output: Nothing
*******************************************************************************/
void Opt3001::set_time800()
{
	// Conversion time 100 means bit 11 of CONFIG register is set to 0
	uint16_t myconfig = Read_Register(OPT3001_CONFIG_REG);
	myconfig = myconfig | 0x800;
    write_Config(myconfig);
}

/*******************************************************************************
* Function start_single()
* ------------------------------------------------------------------------------
* Overview: Starts one single measurement
* Input: Nothing
* Output: Nothing
*******************************************************************************/
void Opt3001::start_single()
{
	// Reads the curent configuration
	uint16_t myconfig = Read_Register(OPT3001_CONFIG_REG);
	// Clear the mode bits (bits 10:9)
	// Then set single mode (bits 10:9 = 01)
	myconfig = (myconfig & 0xF9FF) | 0x200;
	// Write back the value
    write_Config(myconfig);
}

/*******************************************************************************
* Function start_continuous()
* ------------------------------------------------------------------------------
* Overview: Starts continuous measurements
* Input: Nothing
* Output: Nothing
*******************************************************************************/
void Opt3001::start_continuous()
{
	// Reads the curent configuration
	uint16_t myconfig = Read_Register(OPT3001_CONFIG_REG);
	// Clear the mode bits (bits 10:9)
	// Then set single mode (bits 10:9 = 10)
	myconfig = (myconfig & 0xF9FF) | 0x400;
	// Write back the value
    write_Config(myconfig);
}

/*******************************************************************************
* Function is_Measuring()
* ------------------------------------------------------------------------------
* Overview: Checks if conversion is completed.
* Input: Nothing
* Output: Conversion status:
* 	1 - measuring
* 	0 - completed
*******************************************************************************/
bool Opt3001::is_Measuring()
{
	// Reads the curent configuration
	uint16_t myconfig = Read_Register(OPT3001_CONFIG_REG);
	// Returns 1 if bit 7 (CRF) = 0 (measurement in progress)
	// Returns 0 if measurement is completed
	// Keep only bit 7
	myconfig = myconfig & 0x80;
    if (myconfig == 0x80){
		return 0;
	}
	else{
		return 1;
	}
}

/*******************************************************************************
* Function is_Overflow()
* ------------------------------------------------------------------------------
* Overview: Checks OVF (Overflow flag) after measurement
* Input: Nothing
* Output: OVF status:
* 	1 - Overflow
* 	0 - No overflow
*******************************************************************************/
bool Opt3001::is_Overflow()
{
	// Reads the curent configuration
	uint16_t myconfig = Read_Register(OPT3001_CONFIG_REG);
	// Keep only bit 8 (OVF)
	myconfig = myconfig & 0x100;
    if (myconfig == 0x100){
		return 1;
	}
	else{
		return 0;
	}
}

/*******************************************************************************
* Function is_FlagHigh()
* ------------------------------------------------------------------------------
* Overview: Checks HIGH flag after measurement
* Input: Nothing
* Output: High flag status:
* 	1 - Flag set
* 	0 - No flag
*******************************************************************************/
bool Opt3001::is_FlagHigh()
{
	// Reads the curent configuration
	uint16_t myconfig = Read_Register(OPT3001_CONFIG_REG);
	// Keep only bit 5
	myconfig = myconfig & 0x40;
    if (myconfig == 0x40){
		return 1;
	}
	else{
		return 0;
	}
}

/*******************************************************************************
* Function is_FlagLow()
* ------------------------------------------------------------------------------
* Overview: Checks HIGH flag after measurement
* Input: Nothing
* Output: Low flag status:
* 	1 - Flag set
* 	0 - No flag
*******************************************************************************/
bool Opt3001::is_FlagLow()
{
	// Reads the curent configuration
	uint16_t myconfig = Read_Register(OPT3001_CONFIG_REG);
	// Returns 1 if bit 7 (CRF) = 0 (measurement in progress)
	// Returns 0 if measurement is completed
	// Keep only bit 7
	myconfig = myconfig & 0x20;
    if (myconfig == 0x20){
		return 1;
	}
	else{
		return 0;
	}
}



/*******************************************************************************
* Function shutdown()
* ------------------------------------------------------------------------------
* Overview: Puts the sensor in shutdown mode, keeping all other settings
* Input: Nothing
* Output: Nothing
*******************************************************************************/
void Opt3001::shutdown()
{
	// Reads the curent configuration
	uint16_t myconfig = Read_Register(OPT3001_CONFIG_REG);
	// Clear the shutdown bits (bits 10:9)
    myconfig = myconfig & 0xF9FF;
	// Write back the value
    write_Config(myconfig);
}

/*******************************************************************************
* Function measure_Lux()
* ------------------------------------------------------------------------------
* Overview: Returns the value in Lux
* Input: Nothing
* Output: Ambient light, in Lux (float)
*******************************************************************************/
float Opt3001::measure_Lux()
{
	float lux;
	// Read the result register
	uint16_t raw = Read_Register(OPT3001_RESULT_REG);
	// Debugging: print raw velue
	// Serial.print("Raw reading: 0x");
	// Serial.println(raw,HEX);
    // Now convert to lux
	// exponent is (raw & 0xF000) >> 12
	// mantissa is  raw & 0x0FFF
	// lux is 0.01 * 2^exponent * mantissa
	// 2^exponent is the same as 1 << exponent
	// 2^0 = 1 = 0b1   = 1<<0
	// 2^1 = 2 = 0b10  = 1<<1
	// 2^2 = 4 = 0b100 = 1<<2
	// ...
	lux = 0.01 * ( 1 << ((raw & 0xF000) >> 12) ) * (raw & 0x0FFF);
	return lux;
}

/*******************************************************************************
* Function register2Lux()
* ------------------------------------------------------------------------------
* Overview: Returns the value in Lux
* Used to get the values of HIGH and LOW limit registers in lux
* Input: Nothing
* Output: Ambient light, in Lux (float)
*******************************************************************************/
float Opt3001::register2Lux(uint8_t myregister)
{
	float lux;
	// Read the result register
	uint16_t raw = Read_Register(myregister);
	// Debugging: print raw velue
	// Serial.print("Raw reading: 0x");
	// Serial.println(raw,HEX);
    // Now convert to lux
	// exponent is (raw & 0xF000) >> 12
	// mantissa is  raw & 0x0FFF
	// lux is 0.01 * 2^exponent * mantissa
	// 2^exponent is the same as 1 << exponent
	// 2^0 = 1 = 0b1   = 1<<0
	// 2^1 = 2 = 0b10  = 1<<1
	// 2^2 = 4 = 0b100 = 1<<2
	// ...
	lux = 0.01 * ( 1 << ((raw & 0xF000) >> 12) ) * (raw & 0x0FFF);
	return lux;
}


// EOF
