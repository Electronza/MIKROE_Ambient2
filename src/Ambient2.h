/*
  Ambient2.h
  Created by Teodor Costachioiu, August 2017
  https://electronza.com
  Released into the public domain.
*/

#ifndef __ambient2_h__
#define __ambient2_h__

#include <Wire.h>

// Ambient2 click has the default address 0x44
// Other possible addresses are 0x45, 0x46 and 0x47
// by changing SMD jumpers on Ambient2 click board
#define Ambient2_Addr 		  0x44

// OPT3001 Internal registers
#define OPT3001_RESULT_REG        0x00
#define OPT3001_CONFIG_REG        0x01
#define OPT3001_LOWLIMIT_REG      0x02
#define OPT3001_HIGHLIMIT_REG     0x03
// The manufacturer ID reads 5449h. 
// In ASCII code, this register reads TI.
#define OPT3001_MANUFACTURER_REG  0x7E
#define OPT3001_DEVICEID_REG      0x7F


// de curatat un pic explicatiile

/* 	CONFIG REGISTER BITS: RN3 RN2 RN1 RN0 CT M1 M0 OVF CRF FH FL L Pol ME FC1 FC0
	RN3 to RN0 = Range select:
                    1100 by default, enables auto-range 
	CT = Conversion time bit
                    0 = 100ms conversion time
                    1 = 800ms conversion time (default)
	M1 to M0 = Mode bits
                    00 = Shutdown mode
                    01 = Single shot mode
                    10 = Continuous conversion (default)
                    11 = Continuous conversion
	OVF (Bit 8)     Overflow flag. When set the conversion result is overflown.
	CRF (Bit 7)     Conversion ready flag. Sets at end of conversion. Clears by read or write of the Configuration register.
	FH (Bit 6)      Flag high bit. Read only. Sets when result is higher that TH register. Clears when Config register is 
					read or when Latch bit is 0 and the result goes bellow TH register.
	FL (Bit 5)      Flag low bit. Read only. Sets when result is lower that TL register. Clears when Config register is read 
                    or when Latch bit is 0 and the result goes above TL register.
	L (Bit 4)       Latch bit. Read/write bit. Default 1, Controls Latch/transparent functionality of FH and FL bits. When 
                    L = 1 the Alert pin works in window comparator mode with Latched functionality When L = 0 the Alert pin 
                    works in transparent mode and the two limit registers provide the hysteresis.
	Pol (Bit 3)     Polarity. Read/write bit. Default 0, Controls the active state of the Alert pin. Pol = 0 means Alert 
                    active low.
	ME (Bit 2)      Exponent mask. In fixed range modes masks the exponent bits in the result register to 0000.
	FC1 to FC0  -   Fault count bits. Read/write bits. Default 00 - the first fault will trigger the alert pin.
*/

// Default is 0b1100 1000 0001 0000
#define OPT3001_DEFAULT_CONFIG    0xC810

/*
 * This line needs to be enabled to support interrupts from the OPT3001.
 * Also remember to bridge solder jumper SJ1 (EN-INT) on the PCB.
 * Currently your code also need to attach a interrupt handler for pin 12.
 */
//#define OPT_INTERRUPT_PIN     12
class Opt3001
{
  public:
	void     begin();
	uint16_t read_DeviceID();
	uint16_t read_ManufacturerID();
	uint16_t read_HighLimit();
	uint16_t read_LowLimit();
	uint16_t read_Config();
	void     write_Config(uint16_t);
    void     set_time100();
	void     set_time800();
	void     start_single();
	void     start_continuous();
	bool     is_Measuring();
	bool     is_Overflow();
	bool     is_FlagHigh();
	bool     is_FlagLow();
	void     shutdown();
	float    measure_Lux();
	float    register2Lux(uint8_t);
	

//#if defined(OPT_INTERRUPT_PIN)
	//uint8_t	 interruptPin();
//#endif
  private:
    void     Write_Register(uint8_t Register_Name, uint16_t Value);
	uint16_t Read_Register(uint8_t Register_Name);
};

#endif // __ambient2_h__
