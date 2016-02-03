#include <IRremote.h>

IRsend irsend;

char data[2];
byte highOrder;
byte lowOrder;
int mergedNumber;
int address;
int value;

void setup() {
  Serial.begin(57600);
}

void loop() {
  if(Serial.available() > 0){
    Serial.readBytes(data, 2);
    highOrder = data[0];
    lowOrder = data[1];
    mergedNumber = (highOrder << 8) + lowOrder;
    // code below are just for monitoring 
    address = mergedNumber >> 12;
    value = mergedNumber - (address << 12);
    Serial.print(address);
    Serial.print(' ');
    Serial.println(value);
    sendSignal(mergedNumber);
  }
}

void sendSignal(int packet){
  for (int i = 0; i < 5; i++) {
    irsend.sendSony(packet, 15); 
    delay(100);
  }
}