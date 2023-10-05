#ifndef DISPLAY_7SEG_H
#define DISPLAY_7SEG_H
#include <Arduino.h>

#define B PA10
#define D PA11
#define C PA12
#define G PB13
#define E PB14
#define A PB15
#define F PC7
#define DIG1 PB2
#define DIG2 PB10
#define DIG3 PB11

class display_7seg
{
private:
    int _digit;
    int _symbol;
    float _num;
    int _addr;

public:
    void begin();
    void segmentDisplay(int symbol);
    void lightDigit(int digit, int symbol1, int symbol2, int symbol3);
    int remainderFirst(float num);
    int remainderSecond(float num);
    int remainderAddrFirst(int num);
    int remainderAddrSecond(int num);
    int remainderAddrThrid(int num);
};

void display_7seg::begin()
{

    pinMode(DIG1, OUTPUT);
    pinMode(DIG2, OUTPUT);
    pinMode(DIG3, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(F, OUTPUT);
    pinMode(G, OUTPUT);

    digitalWrite(DIG1, LOW);
    digitalWrite(DIG2, HIGH);
    digitalWrite(DIG3, LOW);
}

void display_7seg::segmentDisplay(int symbol)
{
    _symbol = symbol;
    switch (_symbol)
    {
    case 0:
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, LOW);
        digitalWrite(E, LOW);
        digitalWrite(F, LOW);
        digitalWrite(G, HIGH);
        break;

    case 1:
        digitalWrite(A, HIGH);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, HIGH);
        digitalWrite(E, HIGH);
        digitalWrite(F, HIGH);
        digitalWrite(G, HIGH);
        break;

    case 2:
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, HIGH);
        digitalWrite(D, LOW);
        digitalWrite(E, LOW);
        digitalWrite(F, HIGH);
        digitalWrite(G, LOW);
        break;

    case 3:
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, LOW);
        digitalWrite(E, HIGH);
        digitalWrite(F, HIGH);
        digitalWrite(G, LOW);
        break;

    case 4:
        digitalWrite(A, HIGH);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, HIGH);
        digitalWrite(E, HIGH);
        digitalWrite(F, LOW);
        digitalWrite(G, LOW);
        break;

    case 5:
        digitalWrite(A, LOW);
        digitalWrite(B, HIGH);
        digitalWrite(C, LOW);
        digitalWrite(D, LOW);
        digitalWrite(E, HIGH);
        digitalWrite(F, LOW);
        digitalWrite(G, LOW);
        break;

    case 6:
        digitalWrite(A, LOW);
        digitalWrite(B, HIGH);
        digitalWrite(C, LOW);
        digitalWrite(D, LOW);
        digitalWrite(E, LOW);
        digitalWrite(F, LOW);
        digitalWrite(G, LOW);
        break;

    case 7:
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, HIGH);
        digitalWrite(E, HIGH);
        digitalWrite(F, HIGH);
        digitalWrite(G, HIGH);
        break;

    case 8:
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, LOW);
        digitalWrite(E, LOW);
        digitalWrite(F, LOW);
        digitalWrite(G, LOW);
        break;

    case 9:
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, LOW);
        digitalWrite(E, HIGH);
        digitalWrite(F, LOW);
        digitalWrite(G, LOW);
        break;

    case 10: // C
        digitalWrite(A, LOW);
        digitalWrite(B, HIGH);
        digitalWrite(C, HIGH);
        digitalWrite(D, LOW);
        digitalWrite(E, LOW);
        digitalWrite(F, LOW);
        digitalWrite(G, HIGH);
        break;

    case 11: // U
        digitalWrite(A, HIGH);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, LOW);
        digitalWrite(E, LOW);
        digitalWrite(F, LOW);
        digitalWrite(G, HIGH);
        break;

    case 12: // E
        digitalWrite(A, LOW);
        digitalWrite(B, HIGH);
        digitalWrite(C, HIGH);
        digitalWrite(D, LOW);
        digitalWrite(E, LOW);
        digitalWrite(F, LOW);
        digitalWrite(G, LOW);
        break;

    case 13: // r
        digitalWrite(A, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(C, HIGH);
        digitalWrite(D, HIGH);
        digitalWrite(E, LOW);
        digitalWrite(F, HIGH);
        digitalWrite(G, LOW);
        break;

    default:
        break;
    }
}

void display_7seg::lightDigit(int digit, int symbol1, int symbol2, int symbol3)
{
    _digit = digit;
    switch (_digit)
    {
    case 1:
        segmentDisplay(symbol1);
        digitalWrite(DIG1, LOW);
        digitalWrite(DIG2, HIGH);
        digitalWrite(DIG3, HIGH);
        break;

    case 2:
        segmentDisplay(symbol2);
        digitalWrite(DIG1, HIGH);
        digitalWrite(DIG2, LOW);
        digitalWrite(DIG3, HIGH);
        break;

    case 3:
        segmentDisplay(symbol3);
        digitalWrite(DIG1, HIGH);
        digitalWrite(DIG2, HIGH);
        digitalWrite(DIG3, LOW);
        break;

    default:
        break;
    }
}

int display_7seg::remainderFirst(float num){
    _num = num;
    if(_num >= 99.00){
        return 9;
    }
    int floatToInt = _num * 100;
    int remainderFirst = (floatToInt / 1000) % 10;
    return remainderFirst;
}

int display_7seg::remainderSecond(float num){
    _num = num;
      if(_num >= 99.00){
        return 9;
    }
    int floatToInt = _num * 100;
    int remainderSecond = (floatToInt / 100) % 10;
    return remainderSecond;
}

int display_7seg::remainderAddrFirst(int addr){
    _addr = addr;
    int remainderFirst = (_addr / 100) % 10;
    return remainderFirst;
}

int display_7seg::remainderAddrSecond(int addr){
    _addr = addr;
    int remainderSecond = (_addr / 10) % 10;
    return remainderSecond;
}

int display_7seg::remainderAddrThrid(int addr){
    _addr = addr;
    int remainderThrid = (_addr) % 10;
    return remainderThrid;
}
#endif
