#include <IRremote.h>
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

boolean reportFlag = false;
int count = 0;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
//    Serial.println(results.value, HEX);
    if(results.value == 1111){
      reportFlag = true;
    }
    irrecv.resume(); // Receive the next value
  }
  if(reportFlag){
    count++;
    Serial.println(count);
    reportFlag = false;
  }
}



