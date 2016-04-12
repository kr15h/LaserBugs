#include <IRremote.h>

IRsend irsend;

byte data[4];
// byte byteData[4];
unsigned long mergedNumber;
unsigned int id; // 8bit 0~255
int op; // 2bit 0~3, 0[==] 1[!=] 2[<] 3[>]
int address; // 3bit 0~7 
unsigned long value; // 16bit 0~65535

void setup() {
  Serial.begin(57600);
}

void loop() {
  if(Serial.available() > 0){
    Serial.readBytes(data, 4);
    // for(int i=0; i<4; i++){
    //   byteData[i] = data[i];
    // }
    unsigned long data0 = data[0];
    unsigned long data1 = data[1];
    unsigned long data2 = data[2];
    unsigned long data3 = data[3];

    mergedNumber = (data0 << 21) +
                   (data1 << 16) +
                   (data2 << 8) + 
                   data3;

    // code below are just for monitoring
    // id = byteData[0];
    // op = byteData[1] >> 3; 
    // address = byteData[1] - (op << 3);
    // value = (byteData[2] << 8) + byteData[3];
    id = data0;
    op = data1 >> 3; 
    address = data1 - (op << 3);
    value = (data2 << 8) + data3;

    
    // Serial.println(mergedNumber);
    Serial.print(id);
    Serial.print(' ');
    Serial.print(op);
    Serial.print(' ');
    Serial.print(address);
    Serial.print(' ');
    Serial.print(value);
    Serial.print(' ');
    Serial.print(data0);
    Serial.print(' ');
    Serial.print(data1);
    Serial.print(' ');  
    Serial.print(data2);
    Serial.print(' ');
    Serial.print(data3);
    Serial.print(' ');
    Serial.println(mergedNumber);
    sendSignal(mergedNumber);
  }
}

void sendSignal(unsigned long packet){
  // for (int i = 0; i < 3; i++) {
    // irsend.sendSony(packet, 29); 
    irsend.sendSony(packet, 29);
  //   delay(50);
  // }
}
