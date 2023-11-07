#include <Arduino.h>
#include <HardwareSerial.h>
#include "display_7seg.h"
#include "industrialli_SHT20.h"
#include <industrialli_loraMesh.h>

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
// MASTER
uint16_t addr = 0;

// SLAVE
//uint16_t addr = 1;
bool sendNow = false;

// transmissao de mensagem

int msgSize;

// HardwareSerial Serial2(USART2_RX, USART2_TX);
display_7seg display;
industrialli_SHT20 SHT20;

HardwareSerial Serial1(PB7, PB6);
TIM_TypeDef *Instance = TIM3;
HardwareTimer *MyTim = new HardwareTimer(Instance);

TIM_TypeDef *Instance1 = TIM14;
HardwareTimer *MyTim1 = new HardwareTimer(Instance1);
LoRaMESH lora(&Serial2);
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
  Serial2.setRx(USART2_RX);
  Serial2.setTx(USART2_TX);
  // Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(9600);

  /************************* LoRa *************************/
  delay(5000);
  lora.begin(false);

  /*
   if (!lora.setnetworkId(addr))
    {
      Serial1.println("Erro ao definir o novo ID");
      while (1)
        ;
    }

    Serial1.println("ID configurado com sucesso!");
    delay(2000);

  */

  /*
    if (!lora.config_bps(BW500, SF_LoRa_7, CR4_5))
  {
    Serial1.println("Erro ao configurar bps");
    while (1)
      ;
  }

  Serial1.println("Parametros LoRa configurados com sucesso!");
 delay(2000);
  */

  /*
    if (!lora.config_class(LoRa_CLASS_C, LoRa_WINDOW_15s))
   {
     Serial1.println("Erro ao configurar a classe");
     while (1)
       ;
   }
   Serial1.println("Modo de operacao configurado com sucesso!");
  delay(2000);
   */

  /*
    if (!lora.setpassword(123))
    {
      Serial1.println("Erro ao gravar a senha ou a senha gravada não condiz com a senha definida");
      while (1)
        ;
    }

    Serial1.println("Senha configurada com sucesso!");
   delay(2000);
  */

  Serial1.println("LocalID: " + String(lora.localId));
  Serial1.println("UniqueID: " + String(lora.localUniqueId));
  Serial1.println("Pass <= 65535: " + String(lora.registered_password));

  MyTim->setOverflow(200, HERTZ_FORMAT); // 10 Hz
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
  // Serial1.print(temp);
  // Serial1.print(" ");
  // Serial1.print(humi);
  if (addr == 1)
  {
    int function = lora.read_function();
    Serial1.println(function);
    if (function == 3)
    {
      lora.write_node_temp(0, temp);
    }
    else if (function == 4)
    {
      lora.write_node_humi(0, humi);
    }

    if (sendNow)
    {
      // lora.write_node_temp(0, temp);
    }
    else
    {
      // lora.write_node_humi(0, humi);
    }
  }
  else if (addr == 0)
  {
    if (sendNow)
    {Serial1.print("Pendindo temperatura do node 1...");
      lora.write_node_function(1, 3);
       Serial1.print("Temp: ");
      Serial1.print(lora.read_NodeTemp());
      Serial1.println("°C");
    }
    else
    {
      Serial1.print("Pendindo umidade do node 1...");
      lora.write_node_function(1, 4);
      Serial1.print("Umi: ");
      Serial1.print(lora.read_NodeHumi());
      Serial1.println("%");
    }

    // Serial1.print(" ");
    // Serial1.println(lora.read_NodeHumi());
  }
  delay(1000);
  sendNow = !sendNow;

  /*

  //ENVIO E RECEBIMENTO DE INT
    int function = 1234;
     byte functionArray[4] = {
        ((uint8_t*)&function)[0],
        ((uint8_t*)&function)[1],
        ((uint8_t*)&function)[2],
        ((uint8_t*)&function)[3]
     };

     //receiver
      int b;
      ((uint8_t*)&b)[0] = functionArray[0];
      ((uint8_t*)&b)[1] = functionArray[1];
      ((uint8_t*)&b)[2] = functionArray[2];
      ((uint8_t*)&b)[3] = functionArray[3];



  //ENVIO E RECEBIMENTO DE FLOAT
    //sender
     byte tempArray[4] = {
        ((uint8_t*)&temp)[0],
        ((uint8_t*)&temp)[1],
        ((uint8_t*)&temp)[2],
        ((uint8_t*)&temp)[3]
     };

     //receiver
      float a;
      ((uint8_t*)&a)[0] = tempArray[0];
      ((uint8_t*)&a)[1] = tempArray[1];
      ((uint8_t*)&a)[2] = tempArray[2];
      ((uint8_t*)&a)[3] = tempArray[3];

  //ENVIAR E RECEBER INT E FLOAT DE UMA VEZ
      uint8_t all[8] = {
        ((uint8_t*)&temp)[0],
        ((uint8_t*)&temp)[1],
        ((uint8_t*)&temp)[2],
        ((uint8_t*)&temp)[3],
        ((uint8_t*)&function)[4],
        ((uint8_t*)&function)[5],
        ((uint8_t*)&function)[6],
        ((uint8_t*)&function)[7]
      };
  float c;
  int d;

      ((uint8_t*)&c)[0] = all[0];
      ((uint8_t*)&c)[1] = all[1];
      ((uint8_t*)&c)[2] = all[2];
      ((uint8_t*)&c)[3] = all[3];
      ((uint8_t*)&d)[0] = all[4];
      ((uint8_t*)&d)[1] = all[5];
      ((uint8_t*)&d)[2] = all[6];
      ((uint8_t*)&d)[3] = all[7];

      Serial1.print(function);
      Serial1.print(" ");
      Serial1.print(b);
      Serial1.print(" ");
      Serial1.print(temp);
      Serial1.print(" ");
      Serial1.print(a);
      Serial1.print(" ");
      Serial1.print(d);
      Serial1.print(" ");
      Serial1.println(c);
      /////////////////////////////////////////////////////////////






  */

  /*
  //converte um elemento do array para int
    msgSize = sizeof(bufferPayload);
String number = "";
for(int i = 0; i < msgSize; i++){
  number += (uint8_t)bufferPayload[i];

}
  num = number.toInt();
  */

  /*
              String number = "";
                number += (uint16_t)bufferPayload[0];
                number += (uint16_t)bufferPayload[1];
                number += (uint16_t)bufferPayload[2];
                number += (uint16_t)bufferPayload[3];
                int num = number.toInt();
  */

  /*
 Serial1.print(" ");
Serial1.print(sizeof(bufferPayload));
Serial1.print(" ");
Serial1.print(num, DEC);
Serial1.println(" ");
  */

  // Serial1.print(number);
  /*
  Serial1.print(" ");
  if (num == 123456789){
    Serial1.println("OK!");
  }
  else{
    Serial1.println("NOT OK!");
  }
  */

  // Serial1.print(temp);
  // Serial2.print(temp);

  // delay(1000);

  // delay(1000);
}

void cycleDigit()
{
  noInterrupts();
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
  interrupts();
}

void cycleInfo()
{
  info++;
}
