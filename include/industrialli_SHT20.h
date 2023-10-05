#ifndef INDUSTRIALLI_SHT20_H
#define INDUSTRIALLI_SHT20_H
#include <Arduino.h>
#include <Wire.h>

#define SHT20_ADDR 0x40
#define TRIGGER_T_MEAS_HOLD 0xE3
#define TRIGGER_RH_MEAS_HOLD 0xE5
#define TRIGGER_T_MEAS_NO_HOLD 0xF3
#define TRIGGER_RH_MEAS_NO_HOLD 0x7A
#define USER_REGISTER 0x73
#define SOFT_RESET 0x7F

#define ERROR_I2C_TIMEOUT 998
#define ERROR_BAD_CRC 999
#define SHIFTED_DIVISOR  0x988000
#define MAX_WAIT 100
#define DELAY_INTERVAL 10
#define MAX_COUNTER (MAX_WAIT / DELAY_INTERVAL)

class industrialli_SHT20
{

public:
    industrialli_SHT20(TwoWire *wire = &Wire);
    void begin();
    void begin(int sda, int scl);
    byte checkCRC(uint16_t message_from_sensor, uint8_t check_value_from_sensor);
    uint16_t readRegisters(uint8_t address, uint8_t reg);
    void writeRegisters(uint8_t address, uint8_t reg);
    float getTemperature();
    float getHumidity();

private:
    TwoWire *_wire;
    uint8_t _address = SHT20_ADDR;
    uint8_t _reg;
};

#endif