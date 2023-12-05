#include <LiquidCrystal.h>
#include <Arduino.h>
#include <Wire.h>
#include <stdlib.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

#include <time.h>

#define MAX 5
#define btn1 1
#define btn2 2
#define btn3 3
#define btn4 4
#define resetbtn 5

int randNumber[MAX];


RF24 radio(9,8);
const byte address[6] = "01101";

LiquidCrystal lcd(7,6,5,4,3,2);
enum direction {UP, DOWN, RIGHT, LEFT};


void random_sign(){
        // 랜덤값 생성 코드
    for(int i = 0; i < MAX ; i++)
    {
        randNumber[i] = rand() % 4 + 1;
    }

    //lcd 화살표 출력 코드
    lcd.clear();
    for(int i = 0; i < MAX; i++){
        print_arrow(i, (direction)randNumber[i]-1);
    }
    delay(2000);
    return;
}

int input_btn(){
    //버튼 입력 함수
    int btn1_state = 0,
        btn2_state = 0,
        btn3_state = 0,
        btn4_state = 0,
        resetbtn_state = 0;
    
    btn1_state = digitalRead(btn1);
    btn2_state = digitalRead(btn2);
    btn3_state = digitalRead(btn3);
    btn4_state = digitalRead(btn4);
    resetbtn_state = digitalRead(resetbtn);

    if (btn1_state == 1)
    {
        return 1;
    }
    else if(btn2_state == 1)
    {
        return 2;
    }
    else if(btn3_state == 1)
    {
        return 3;
    }
    else if(btn4_state == 1)
    {
        return 4;
    }
    else
    {
        return 5;
    }
}

int check_btn(int rand_numbers, int btn_value){
    //랜덤 생성 넘버와 비교
    if(rand_numbers == btn_value){
        return 1;
    }
    else{
        return 0;
    }
}



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
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);

    radio.startListening();

    pinMode(btn1, INPUT);
    pinMode(btn2, INPUT);
    pinMode(btn3, INPUT);
    pinMode(btn4, INPUT);
    pinMode(resetbtn, INPUT);
}

void loop() {

    int cnt = 0;

    while (cnt < 3)
    {
        random_sign();
        int check_cnt = 0;
        int reset_cnt = 0;
        while (1)
        {
            int btn_value = input_btn();


            if(check_cnt > 5)//버튼 횟수 5회 이상
                break;

            if (reset_cnt > 3)//리셋 버튼 횟수 3회 이상
            {
                return;
            }
            
            if (btn_value == 5)//리셋 버튼 눌렀을 때
            {
                reset_cnt++;
                continue;
            }
            
            if(check_btn(randNumber[check_cnt], btn_value - 1)){//버튼 값 확인
                //맞았을 때
                check_cnt++;
            }
            else
            {
                //틀렸습니다.
                cnt = -1;// 카운트 초기화
                break;
            }
        }
        cnt++;
    }
    
    //임시 화살표 테스트 코드
    // lcd.clear();
    // int j;
    // for(int i=0; i<5; i++){
    //     j = rand()%4;
    //     print_arrow(i, (direction)j);
    // }
    // delay(2000);

    //임시 RF24 수신 테스트 코드
    if (radio.available()) {
        char text[32] = "";
        radio.read(&text, sizeof(text));
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(text);
    }
}
