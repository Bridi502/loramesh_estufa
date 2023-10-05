#include "industrialli_SHT20.h"
industrialli_SHT20::industrialli_SHT20(TwoWire*wire){
_wire = wire;
}
void industrialli_SHT20::begin()
{
    //_wire = &Wire;
    //_wire->setSDA(sda);
   // _wire->setSCL(scl);
    _wire->begin();
}

void industrialli_SHT20::begin(int sda, int scl)
{
    _wire = &Wire;
    _wire->setSDA(sda);
   _wire->setSCL(scl);
    _wire->begin();
}
byte industrialli_SHT20::checkCRC(uint16_t message_from_sensor, uint8_t check_value_from_sensor){
uint32_t remainder = (uint32_t)message_from_sensor << 8;
    remainder |= check_value_from_sensor;
    uint32_t divsor = (uint32_t)SHIFTED_DIVISOR;
    for(int i = 0 ; i < 16 ; i++){
        if(remainder & (uint32_t)1 << (23 - i)){
            remainder ^= divsor;
        }
        divsor >>= 1;
    }
    return (byte)remainder;
}
uint16_t industrialli_SHT20::readRegisters(uint8_t address, uint8_t reg)
{
    _address = address;
    _reg = reg;
    _wire->beginTransmission(_address);
    _wire->write(_reg);
    if(0 != _wire->endTransmission()){   // Used Wire.endTransmission() to end a slave transmission started by beginTransmission() and arranged by write().
      return (ERROR_I2C_TIMEOUT);
    }
byte toRead;
    byte counter;
    for(counter = 0, toRead = 0 ; counter < MAX_COUNTER && toRead != 3; counter++){
        delay(DELAY_INTERVAL);
        toRead = _wire->requestFrom(_address, (uint8_t)3);
    }
    if(counter == MAX_COUNTER){
        return (ERROR_I2C_TIMEOUT);
    }
    byte msb, lsb, checksum;
    msb = _wire->read();
    lsb = _wire->read();
    checksum = _wire->read();
    uint16_t rawValue = ((uint16_t) msb << 8) | (uint16_t) lsb;
    if(checkCRC(rawValue, checksum) != 0){
        return (ERROR_BAD_CRC);
    }
    return rawValue & 0xFFFC;
}
void industrialli_SHT20::writeRegisters(uint8_t address, uint8_t reg)
{
    _address = address;
    _reg = reg;
    uint16_t config;

    _wire->beginTransmission(_address);
    _wire->write((uint8_t)_reg);
    _wire->write((uint8_t)(config >> 8));
    _wire->write((uint8_t)(config & 0xFF));
    _wire->endTransmission();
}
float industrialli_SHT20::getTemperature()
{
    uint16_t tempRaw = readRegisters(_address, TRIGGER_T_MEAS_NO_HOLD);
    float temp = tempRaw * (175.72 / 65536.0);
    temp = temp - 46.85;
    return (temp);
}
float industrialli_SHT20::getHumidity()
{
    uint16_t humiRaw = readRegisters(_address, TRIGGER_RH_MEAS_HOLD);
    float humi = humiRaw * (150.0 / 65536.0);
    humi = humi - 6.0;
    return (humi);
}