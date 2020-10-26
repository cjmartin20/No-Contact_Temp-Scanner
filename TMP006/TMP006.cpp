#include "TMP006.h"
 
#define TEMP_REG_ADDR 0x00
 
TMP006::TMP006(PinName sda, PinName scl, int addr) : m_i2c(sda, scl), m_addr(addr) {
}
 
TMP006::~TMP006() { 
}
 
 
/** TMP006 Config method.
 *  Used for configuring the sensor with desired samples.
 */
void TMP006::config(uint8_t addr, uint16_t samples)
{
    char    data[2];
 
    data[0]   = 0x02;
    data[1]   = samples | TMP006_CFG_MODEON | TMP006_CFG_DRDYEN;
 
    m_i2c.write( addr, data, 1 ); // writes the config (0x02) address with the sample rate
  
}
 
/** TMP006 Read Raw Temperature method.
 *  Used for getting the raw data off the chip.
 */
int16_t TMP006::readRawDieTemperature(uint8_t addr)
{
 
  char reg[1];
  char data[2];
  
  reg[0] = TMP006_TAMB;
  
  m_i2c.write(addr, reg, 1);
  m_i2c.read(addr, data, 2);
   
  int16_t raw = (data[0] << 8) | data[1] ;
 
  raw >>= 2;
  return raw;
}
 
/** TMP006 Read Raw Voltage method.
 *  Used for reading the raw voltage from the thermopile.
 */
int16_t TMP006::readRawVoltage(uint8_t addr) 
{  
  char reg[1];
  char data[2];
  
  reg[0] = TMP006_VOBJ;
  
  m_i2c.write(addr, reg, 1);
  m_i2c.read(addr, data, 2);
   
  int16_t raw = (data[0] << 8) | data[1] ;
  
  return raw;
}
 
/** TMP006 Read Object Temperature(C) method.
 *  Used for calculating the object temperature in celcius.
 */
double TMP006::readObjTempC(uint8_t addr) 
{
  double Tdie = readRawDieTemperature(addr);
  double Vobj = readRawVoltage(addr);
  Vobj *= 156.25;  // 156.25 nV per LSB
  Vobj /= 1000000000; // nV -> V
  Tdie *= 0.03125; // convert to celsius
  Tdie += 273.15; // convert to kelvin
 
  // Equations for calculating temperature found in section 5.1 in the user guide
  double tdie_tref = Tdie - TMP006_TREF;
  double S = (1 + TMP006_A1*tdie_tref + 
      TMP006_A2*tdie_tref*tdie_tref);
  S *= TMP006_S0;
  S /= 10000000;
  S /= 10000000;
 
  double Vos = TMP006_B0 + TMP006_B1*tdie_tref + 
    TMP006_B2*tdie_tref*tdie_tref;
 
  double fVobj = (Vobj - Vos) + TMP006_C2*(Vobj-Vos)*(Vobj-Vos);
 
  double Tobj = sqrt(sqrt(Tdie * Tdie * Tdie * Tdie + fVobj/S));
 
  Tobj -= 273.15; // Kelvin -> *C
  return Tobj;
}
 
/** TMP006 Read Object Temperature(F) method.
 *  Used for calculating the object temperature in fahrenheit.
 */
double TMP006::readObjTempF(uint8_t addr) 
{
  double Tobj = readObjTempC(addr);
  Tobj = Tobj * 9.0/5.0 + 32.0; // convert to fahrenheit
  return Tobj;
}
 
/** TMP006 Read Die Temperature(C) method.
 *  Used for calculating the die temperature in celcius.
 */
double TMP006::readDieTempC(uint8_t addr) 
{
  double Tdie = readRawDieTemperature(addr);
  Tdie *= 0.03125; // convert to celsius
  return Tdie;
}
 
/** TMP006 Read Die Temperature(F) method.
 *  Used for calculating the die temperature in fahrenheit.
 */
double TMP006::readDieTempF(uint8_t addr) 
{
  double Tdie = readDieTempC(addr);
  Tdie = Tdie * 9.0/5.0 + 32.0; // convert to fahrenheit
  return Tdie;
}