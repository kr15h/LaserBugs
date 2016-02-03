#include <IRremote.h>

IRsend irsend;

int address = 0; // 3bit(~7) value 
int value = 0; //12bit(~4095) value

void setup() {

}

void loop() {
  // address
  address = address + 1;
  if(address == 8) address = 0;
  
  // value
  value = value + 1;
  if(value == 4096) value = 0;
  
  // send
  sendSignal();
  
  delay(1000);
}

void sendSignal(){
  int packet = (address << 12) + value;
  
  for (int i = 0; i < 3; i++) {
      irsend.sendSony(packet, 15); // Sony TV power code
      delay(100);
    }
}
