#include <Arduino.h>
#include <HardwareSerial.h>
#include "display_7seg.h"
#include "industrialli_SHT20.h"
#define DEBUG PB12
#define USART2_TX PA2
#define USART2_RX PA3
#define LORA_TRANSP_TX PA4
#define LORA_TRANSP_RX PA5

#define I2C1_SCL PB8
#define I2C1_SDA PB9
#define GPIO_07 PA15
#define GPIO_02 PB3
#define GPIO_01 PB4
#define GPIO_00 PB5
#define GPIO_06 PD0
#define GPIO_05 PD1
#define GPIO_04 PD2
#define GPIO_03 PD3
int digit;
int symbolTemp1;
int symbolTemp2;
int symbolHumi1;
int symbolHumi2;
int symbolAddr1;
int symbolAddr2;
int symbolAddr3;
float temp;
float humi;
int info;
int addr = 15;

display_7seg display;
industrialli_SHT20 SHT20;

// HardwareSerial Serial2(USART2_RX, USART2_TX);
// HardwareSerial Serial(PB7, PB6);
TIM_TypeDef *Instance = TIM3;
HardwareTimer *MyTim = new HardwareTimer(Instance);

TIM_TypeDef *Instance1 = TIM14;
HardwareTimer *MyTim1 = new HardwareTimer(Instance1);

void cycleDigit();
void cycleInfo();
void setup()
{
  // put your setup code here, to run once:
  display.begin();
  SHT20.begin(I2C1_SDA, I2C1_SCL);
  pinMode(DEBUG, OUTPUT);
  pinMode(GPIO_00, INPUT);
  pinMode(GPIO_01, INPUT);
  pinMode(GPIO_02, INPUT);
  pinMode(GPIO_03, INPUT);
  pinMode(GPIO_04, INPUT);
  pinMode(GPIO_05, INPUT);
  pinMode(GPIO_06, INPUT);
  pinMode(GPIO_07, INPUT);

  Serial.begin(115200);
  // Serial1.begin(115200);
  Serial2.begin(115200);

  MyTim->setOverflow(120, HERTZ_FORMAT); // 10 Hz
  MyTim->attachInterrupt(cycleDigit);
  MyTim->resume();

  MyTim1->setOverflow(5000000, MICROSEC_FORMAT); // 10 Hz
  MyTim1->attachInterrupt(cycleInfo);
  MyTim1->resume();
}

void loop()
{
  // put your main code here, to run repeatedly:
  /*
    digitalWrite(DEBUG, !digitalRead(DEBUG));
    for (int i = 0; i < 14; i++)
    {
      display.segmentDisplay(i);
      delay(100);
    }

  */
  temp = SHT20.getTemperature();
  humi = SHT20.getHumidity();

  Serial.print(symbolTemp1);
  Serial.print(" ");
  Serial.println(symbolTemp2);
}

void cycleDigit()
{
  if (info > 3)
  {
    info = 1;
  }
  switch (info)
  {
  case 1:
    if (digit > 3)
    {
      digit = 1;
    }
    symbolTemp1 = display.remainderFirst(temp);
    symbolTemp2 = display.remainderSecond(temp);
    display.lightDigit(digit, symbolTemp1, symbolTemp2, 10);
    digit++;
    break;

  case 2:
    if (digit > 3)
    {
      digit = 1;
    }
    symbolHumi1 = display.remainderFirst(humi);
    symbolHumi2 = display.remainderSecond(humi);
    display.lightDigit(digit, symbolHumi1, symbolHumi2, 11);
    digit++;
    break;

  case 3:
    if (digit > 3)
    {
      digit = 1;
    }
    symbolAddr1 = display.remainderAddrFirst(addr);
    symbolAddr2 = display.remainderAddrSecond(addr);
    symbolAddr3 = display.remainderAddrThrid(addr);
    display.lightDigit(digit, symbolAddr1, symbolAddr2, symbolAddr3);
    digit++;
    break;

  default:
    break;
  }
}

void cycleInfo()
{

  info++;
}
