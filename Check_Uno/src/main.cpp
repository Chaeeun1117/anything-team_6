#include <LiquidCrystal.h>
#include <Arduino.h>
#include <Wire.h>
#include <stdlib.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); 
const byte address[6] = "00001";

LiquidCrystal lcd(7,6,5,4,3,2);
enum direction {UP, DOWN, RIGHT, LEFT};


void print_init(){
    byte up1[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00001,
        B00010,
        B00100,
        B01000
    };
    byte up2[8] = {
        B00100,
        B01110,
        B10101,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100
    };

    byte down4[8] = { // left4
        B01000,
        B00100,
        B00010,
        B00001,
        B00000,
        B00000,
        B00000,
        B00000
    };
    byte down5[8] = {
        B00100,
        B00100,
        B00100,
        B00100,
        B00100,
        B10101,
        B01110,
        B00100
    };
    byte down6[8] = { // right6
        B00010,
        B00100,
        B01000,
        B10000,
        B00000,
        B00000,
        B00000,
        B00000
    };

    byte right3[8] = {
        B00000,
        B00000,
        B10000,
        B01000,
        B00100,
        B00010,
        B11111,
        B00000,
    };

    byte left1[8] = {
        B00000,
        B00000,
        B00001,
        B00010,
        B00100,
        B01000,
        B11111,
        B00000
    };

    lcd.createChar(1, up1);
    lcd.createChar(2, up2);
    lcd.createChar(3, down4);
    lcd.createChar(4, down5);
    lcd.createChar(5, down6);
    lcd.createChar(6, right3);
    lcd.createChar(7, left1);
}

void print_arrow(int i, direction j){ //i는 슬롯(0~4) j는 화살표 방향
    if(j==UP){
        lcd.setCursor(3*i, 0);
        lcd.write(byte(1));

        lcd.setCursor(3*i+1, 0);
        lcd.write(byte(2));

        lcd.setCursor(3*i+2, 0);
        lcd.write(byte(B10100100));

        lcd.setCursor(3*i+1, 1);
        lcd.write(byte(B01111100));
    }
    else if (j==DOWN){
        lcd.setCursor(3*i+1, 0);
        lcd.write(byte(B01111100));

        lcd.setCursor(3*i, 1);
        lcd.write(byte(3));

        lcd.setCursor(3*i+1, 1);
        lcd.write(byte(4));

        lcd.setCursor(3*i+2, 1);
        lcd.write(byte(5));
    }
    else if (j==RIGHT){
        lcd.setCursor(3*i, 0);
        lcd.write(byte(B01011111));

        lcd.setCursor(3*i+1, 0);
        lcd.write(byte(B01011111));

        lcd.setCursor(3*i+2, 0);
        lcd.write(byte(6));

        lcd.setCursor(3*i+2, 1);
        lcd.write(byte(5));
    }
    else if (j==LEFT){
        lcd.setCursor(3*i, 0);
        lcd.write(byte(7));

        lcd.setCursor(3*i+1, 0);
        lcd.write(byte(B01011111));

        lcd.setCursor(3*i+2, 0);
        lcd.write(byte(B01011111));

        lcd.setCursor(3*i, 1);
        lcd.write(byte(3));
    }
}




void setup() {
    lcd.begin(16, 2);
    print_init();

    radio.begin();
    radio.openWritingPipe(address); // 데이터를 보낼 수신의 주소를 설정합니다.
    radio.setPALevel(RF24_PA_MIN); // 전원공급에 관한 파워레벨을 설정합니다.
    radio.stopListening();  //모듈을 송신기로 설정합니다.   

}

void loop() {
    int j;

    lcd.clear();
    for(int i=0; i<5; i++){
        j = rand()%4;
        print_arrow(i, (direction)j);
    }
    delay(2000);
}
