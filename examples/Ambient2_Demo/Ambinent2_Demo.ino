/*  MikroeElektronika Ambient2 Click Demo Example
 *
 *  Product page: https://shop.mikroe.com/ambient-2-click
 * 
 *  This click board uses OPT3001 sensor from Texas Instruments
 *  http://www.ti.com/product/OPT3001
 *  
 *  Created by Teodor Costachioiu, August 2017
 *  https://electronza.com
 *
 *  Send bug reports and feature requests to
 *  https://electronza.com/contact/
 *  
 *  Ver 0.1
 */

#include <Wire.h>
#include "Ambient2.h"

Opt3001 Ambient2;

float reading;

void setup()
{
  
  Serial.begin(115200);
  delay(100);

  // Start I2C communication
  Wire.begin();

  // Start communication with OPT3001
  Serial.println("Initializing...");
  Ambient2.begin(); 

  Serial.println("Default sensor setings:");

  // Display configuration values
  Serial.print("CONFIG register is: 0x");
  Serial.println(Ambient2.read_Config(),HEX);

  // Display device ID
  Serial.print("Device ID is:       0x");
  Serial.println(Ambient2.read_DeviceID(),HEX);

  // Display manufacturer ID
  Serial.print("Manufacturer ID is: 0x");
  Serial.println(Ambient2.read_ManufacturerID(),HEX);

  // Display HIGH limit
  Serial.print("HIGH limit is:      0x");
  Serial.println(Ambient2.read_HighLimit(),HEX);
  Serial.print("This means a High limit of ");
  Serial.print(Ambient2.register2Lux(OPT3001_HIGHLIMIT_REG));
  Serial.println (" lux");

  // Display LOW limit
  Serial.print("Low limit is:       0x");
  Serial.println(Ambient2.read_LowLimit(),HEX);
  Serial.print("This means a Low limit of ");
  Serial.print(Ambient2.register2Lux(OPT3001_LOWLIMIT_REG));
  Serial.println (" lux");

  Serial.println("------------------------------------");

  // Sensor configuration

  // Set the conversion time to 800ms
  // This is the default, you can skip it
  Ambient2.set_time800();
  // Set the conversion time to 100ms
  // Ambient2.set_time100();

  // Start the sensor in continuous mode
  Ambient2.start_continuous();
  // Other options are
  // Perform a single measurement
  // Ambient2.start_single();
  // Shutdown
  // Ambient2.shutdown();

  // Print the new configuration
  Serial.println("Configuration completed. New settings are:");
    // Display configuration values
  Serial.print("CONFIG register is: 0x");
  Serial.println(Ambient2.read_Config(),HEX);

  // Display device ID
  Serial.print("Device ID is:       0x");
  Serial.println(Ambient2.read_DeviceID(),HEX);

  // Display manufacturer ID
  Serial.print("Manufacturer ID is: 0x");
  Serial.println(Ambient2.read_ManufacturerID(),HEX);

  // Display HIGH limit
  Serial.print("HIGH limit is:      0x");
  Serial.println(Ambient2.read_HighLimit(),HEX);

  // Display LOW limit
  Serial.print("Low limit is:       0x");
  Serial.println(Ambient2.read_LowLimit(),HEX);

  Serial.println("------------------------------------");
  Serial.println(" ");
  
  
}

void loop()
{
  // Wait for measurement
  while(Ambient2.is_Measuring()){
    //Serial.println("Waiting...");
    delay(50);
  }

  // Do we have overflow?
  if (Ambient2.is_Overflow()){
     Serial.println("Overflow detected"); 
  }
  else{
     // Read the measurement
     reading = Ambient2.measure_Lux();
     Serial.print("Ambient light is :");
     Serial.print(reading);
     Serial.println(" lux");
  }
  
}
