#include <Arduino.h>
#include <HardwareSerial.h>
#include "display_7seg.h"
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

display_7seg display;

// HardwareSerial Serial2(USART2_RX, USART2_TX);
HardwareSerial Serial1(PB7, PB6);
 TIM_TypeDef *Instance = TIM3;
HardwareTimer *MyTim = new HardwareTimer(Instance);

void cycleDigit();
void setup()
{
  // put your setup code here, to run once:
  display.begin();
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
  Serial1.begin(115200);
  Serial2.begin(115200);

    MyTim->setOverflow(10, HERTZ_FORMAT); // 10 Hz
  MyTim->attachInterrupt(cycleDigit);
  MyTim->resume();
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(DEBUG, !digitalRead(DEBUG));
  for (int i = 0; i < 14; i++)
  {
    display.segmentDisplay(i);
    delay(500);
  }

  //Serial1.println("Hello1");

}

void cycleDigit(){
  if(digit > 3){
    digit = 1;
  }
  display.lightDigit(digit);
  digit++;


}


