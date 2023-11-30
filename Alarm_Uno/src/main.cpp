#include <stdlib.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9,8);
const byte address[6] = "01101";

void setup() {
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);

    radio.stopListening();
}

void loop() {
    //임시 RF24 수신 테스트 코드
    const char text[] = "Hello World";
    radio.write(&text, sizeof(text));
    delay(1000);
}
