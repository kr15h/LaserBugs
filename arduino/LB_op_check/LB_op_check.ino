// libraries
#include <IRremote.h>
#include <EEPROM.h>

// pin assign
// out
const int MTR_A_F = 12;
const int MTR_A_B = 10;
const int MTR_A_PWM = 9;
const int MTR_B_F = 5;
const int MTR_B_B = 8;
const int MTR_B_PWM = 6;
const int MTR_STNBY = 18;
const int SOLENOID = 7;
const int LASER = 4;
const int LED_G = 13;
const int LED_R = 19;
// in
const int L_BUMPER = 2;
const int R_BUMPER = 3;
const int RECV_PIN = 11;
const int CDS = 0;
// const int JUMP_0 = 15;
// const int JUMP_1 = 16;
const int CDS_L = 1;
const int CDS_R = 2;
const int BAT_CHK = 17;

// IRrecv
IRrecv irrecv(RECV_PIN);
decode_results results;
/*
IR command list on SONY RMT-831
x  T      11419   15bit   
x  W      27803   15bit   
o  >>|    1724    12bit   turn right
o  |<<    3772    12bit   turn left
x  Mem+   215951  20bit   
x  Mem-   871311  20bit   
x  power  19613   15bit   
o  play   1436    12bit   play
o  stop   412     12bit   stop
o  >>     924     12bit
o  <<     3484    12bit
o  pause  2460    12bit
o  slow   3164    12bit

DIY IR-Commander command list 
func is 3bit, value is 12bit
func 
0 reactionLength_LSR  
1 delayTime
2 reactionLength_BMP
3 cycleLength
4 threshold
5 motorSpeed
6
7 functions*

*functions
0 play
1 stop
2 go to charge
3 save
*/
int func; // 3bit [0~7]
int value; // 12bit [0~4095]
const int sonyRemote = 0;
const int irCommander = 1;
boolean irFunction; // sonyRemote or irCommander
unsigned long timeStamp_MTR;
boolean reaction_MTR = false;
int reactionLength_MTR = 500;
boolean irCommandFlag = false;
boolean irNotification = false;
int blinkCount;
int countFlag = true;

// EEPROM
// int reactionLength_LSR_add_0 = 0;
// int reactionLength_LSR_add_1 = 1;
int reactionLength_ratio_add = 0;
int delayTime_add = 2;
// int reactionLength_BMP_add_0 = 3;
// int reactionLength_BMP_add_1 = 4;
int cycleLength_add_0 = 5;
int cycleLength_add_1 = 6;
int threshold_add = 7;
int motorSpeed_add = 8;
int reactionLength_SLND_add = 9;

// play
int motorSpeed = 150;
boolean playFlag = false;
int cycleLength = 1500;
boolean blinkFlag = false;
unsigned long shiftAmount = 0;
boolean turnRightFlag = false;
boolean turnLeftFlag = false;

// LASER DETECTION
// buffer for mean filter
const int BUFFER_LENGTH = 10;
int buffer[BUFFER_LENGTH];
int index = 0;
// detect laser
int threshold = 10; // used to be 3
// for reaction_LSR 
float reactionLength_ratio = 0.3;
unsigned long timeStamp_LSR;
boolean reaction_LSR = false;
int reactionLength_LSR = cycleLength * reactionLength_ratio;
int reactionLength_SLND = 20;
int delayTime = 0;

// bumper function
boolean bumpLstate;
boolean bumpRstate;
boolean lastBumpLstate;
boolean lastBumpRstate;
boolean bumpLreactFlag = false;
boolean bumpRreactFlag = false;
unsigned long timeStamp_BMP;
long BMPconstant = 180000;
long reactionLength_BMP = BMPconstant / motorSpeed;

// following Light for collect robots for charging
boolean followLightFlag = false;
float direction[] = {0.0, 0.0}; // -1.0 ~ 1.0 (L ~ R)

void setup() {
  Serial.begin(57600);
  pinMode(MTR_A_F, OUTPUT);
  pinMode(MTR_A_B, OUTPUT);
  pinMode(MTR_A_PWM, OUTPUT);
  pinMode(MTR_B_F, OUTPUT);
  pinMode(MTR_B_B, OUTPUT);
  pinMode(MTR_B_PWM, OUTPUT);
  pinMode(MTR_STNBY, OUTPUT);
  pinMode(SOLENOID, OUTPUT);
  pinMode(LASER, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(L_BUMPER, INPUT_PULLUP);
  pinMode(R_BUMPER, INPUT_PULLUP);
  pinMode(CDS, INPUT);
  // pinMode(JUMP_0, INPUT_PULLUP);
  // pinMode(JUMP_1, INPUT_PULLUP);
  pinMode(BAT_CHK, INPUT);
  irrecv.enableIRIn(); // Start the receiver
  digitalWrite(MTR_STNBY, HIGH); // activate motor

  // prepare mean filter 
  for(int i=0; i<BUFFER_LENGTH; i++){
    buffer[i] = analogRead(CDS);
  }
  randomSeed(analogRead(A6));

  // EEPROM reading 
 // byte a = EEPROM.read(reactionLength_LSR_add_0);
 // byte b = EEPROM.read(reactionLength_LSR_add_1);
 // reactionLength_LSR = (a << 8) + b;
 reactionLength_ratio = EEPROM.read(reactionLength_ratio_add) / 100.0;
 delayTime = EEPROM.read(delayTime_add);
 // byte c = EEPROM.read(reactionLength_BMP_add_0);
 // byte d = EEPROM.read(reactionLength_BMP_add_1);
 // reactionLength_BMP = (c << 8) + d;
 byte e = EEPROM.read(cycleLength_add_0);
 byte f = EEPROM.read(cycleLength_add_1);
 cycleLength = (e << 8) + f;
 threshold = EEPROM.read(threshold_add);
 motorSpeed = EEPROM.read(motorSpeed_add);
 reactionLength_SLND = EEPROM.read(reactionLength_SLND_add);
 reactionLength_LSR = int(cycleLength * reactionLength_ratio);
 reactionLength_BMP = int(BMPconstant / motorSpeed);
}

void loop() {
  laserDetect();
  irCommand();
  battery_check();
  bumperFunction();
  play();
  followLight();
}

void laserDetect(){
  int raw = analogRead(CDS);
  //mean filter
  int smoothedByMeanFilter = smoothByMeanFilter();

  if(!reaction_LSR && raw - smoothedByMeanFilter > threshold){  
    // Serial.println("light detected");
    timeStamp_LSR = millis();
    reaction_LSR = true;
    // shiftAmount = shiftAmount + (millis()%cycleLength);
    shiftAmount = millis() % cycleLength;
  }
  if(reaction_LSR && millis() > timeStamp_LSR + delayTime){
    digitalWrite(LASER, HIGH);
    digitalWrite(SOLENOID, HIGH);  
  }
  if(reaction_LSR && (timeStamp_LSR + reactionLength_SLND + delayTime) < millis()){
    digitalWrite(SOLENOID, LOW);
  }
  if(reaction_LSR && (timeStamp_LSR + reactionLength_LSR + delayTime) < millis()){
    digitalWrite(LASER, LOW);
    // Serial.println("reaction_LSR finished");
    reaction_LSR = false;
  }

  buffer[index] = raw;
  index = (index + 1) % BUFFER_LENGTH;

  // Serial.print(raw);
  // Serial.print(' ');
  // Serial.println(smoothedByMeanFilter);
}

int smoothByMeanFilter(){
  long sum = 0;
  for(int i=0; i<BUFFER_LENGTH; i++){
    sum += buffer[i];
  }
  return (int)(sum / BUFFER_LENGTH);
}

void dump(decode_results *results){
  if(results->bits == 12) irFunction = sonyRemote;
  else if(results->bits == 15) irFunction = irCommander;
}

void irCommand(){
  // receiving signal
  if(irrecv.decode(&results)) {
    // Serial.println(results.value);
    dump(&results);
    irrecv.resume(); // Receive the next value
    irCommandFlag = true;
    timeStamp_MTR = millis();
  }

  if(irFunction == sonyRemote){ // react to sony remote
    if(results.value == 1724){ // turn right
      if(timeStamp_MTR + reactionLength_MTR > millis() && irCommandFlag){
        digitalWrite(MTR_B_F, LOW);
        digitalWrite(MTR_B_B, HIGH);
        analogWrite(MTR_B_PWM, 100);
      }
      else{
        analogWrite(MTR_B_PWM, 0);
        irCommandFlag = false;
        irNotification = true;
      }
    }
    if(results.value == 3772){ // turn left
      if(timeStamp_MTR + reactionLength_MTR > millis() && irCommandFlag){
        digitalWrite(MTR_A_F, LOW);
        digitalWrite(MTR_A_B, HIGH);
        analogWrite(MTR_A_PWM, 100);
      }
      else{
        analogWrite(MTR_A_PWM, 0);
        irCommandFlag = false;
        irNotification = true;
      }
    }
    if(results.value == 1436 && irCommandFlag){ // play the behavior
      playFlag = true;
      irCommandFlag = false;
      irNotification = true;
    }
    if(results.value == 412 && irCommandFlag){ // stop the behavior
      playFlag = false;
      irCommandFlag = false;
      irNotification = true;
    }
  }
  else if(irFunction == irCommander && irCommandFlag){
    Serial.println("irCommand received");
    func = results.value >> 12;
    value = results.value - (func << 12);

    switch (func) {
      case 0:
        reactionLength_ratio = value / 100.0;
        reactionLength_LSR = int(cycleLength * reactionLength_ratio);
        irCommandFlag = false;
        irNotification = true;
        break;
      case 1:
        delayTime = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 2:
        // reactionLength_BMP = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 3:
        cycleLength = value;
        reactionLength_LSR = int(cycleLength * reactionLength_ratio);
        irCommandFlag = false;
        irNotification = true;
        break;
      case 4:
        threshold = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 5:
        motorSpeed = value;
        reactionLength_BMP = int(BMPconstant / motorSpeed);
        irCommandFlag = false;
        irNotification = true;
        break;
      case 6:
        reactionLength_SLND = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 7:
        switch (value) {
          case 0:
            playFlag = true;
            followLightFlag = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 1:
            playFlag = false;
            followLightFlag = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 2:
            followLightFlag = true;
            playFlag = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 3:
            //EEPROM saving function
            /*
            int reactionLength_LSR_add_0 = 0;
            int reactionLength_LSR_add_1 = 1;
            int delayTime_add = 2;
            int reactionLength_BMP_add_0 = 3;
            int reactionLength_BMP_add_1 = 4;
            int cycleLength_add_0 = 5;
            int cycleLength_add_1 = 6;
            int threshold_add = 7;
            int motorSpeed_add = 8;
            */ 
            // EEPROM.write(reactionLength_LSR_add_0, reactionLength_LSR >> 8);
            // EEPROM.write(reactionLength_LSR_add_1, reactionLength_LSR - ((reactionLength_LSR >> 8) << 8));
            EEPROM.write(reactionLength_ratio_add, int(reactionLength_ratio*100));
            EEPROM.write(delayTime_add, delayTime);
            // EEPROM.write(reactionLength_BMP_add_0, reactionLength_BMP >> 8);
            // EEPROM.write(reactionLength_BMP_add_1, reactionLength_BMP - ((reactionLength_BMP >> 8) << 8));
            EEPROM.write(cycleLength_add_0, cycleLength >> 8);
            EEPROM.write(cycleLength_add_1, cycleLength - ((cycleLength >> 8) << 8));
            EEPROM.write(threshold_add, threshold);
            EEPROM.write(motorSpeed_add, motorSpeed);
            EEPROM.write(reactionLength_SLND_add, reactionLength_SLND);
            irCommandFlag = false;
            irNotification = true;
            break;
          // default:
        }
        break;
      // default:
    }
  }
  if(irNotification){
    if(blinkCount < 5){
      if(millis()%100 > 50){
        digitalWrite(LED_R, HIGH);
        if(countFlag){
          blinkCount++;
          countFlag = false;
        }
      }
      else{
        digitalWrite(LED_R, LOW);
        if(!countFlag) countFlag = true;
      } 
    }
    else{
      Serial.println("blinkfinished");
      blinkCount = 0;
      irNotification = false;
    }
  }
}

void battery_check(){
  if(analogRead(BAT_CHK) < 200){
    if(millis() % 500 < 250) digitalWrite(LED_R, HIGH);
    else digitalWrite(LED_R, LOW);
  }
}

void bumperFunction(){
  bumpLstate = digitalRead(L_BUMPER);
  bumpRstate = digitalRead(R_BUMPER);

  if(!bumpLstate && lastBumpLstate && !bumpLreactFlag && !bumpRreactFlag){
    bumpLreactFlag = true;
    timeStamp_BMP = millis();
  }
  if(!bumpRstate && lastBumpRstate && !bumpLreactFlag && !bumpRreactFlag){
    bumpRreactFlag = true;
    timeStamp_BMP = millis();
  }

  if(bumpLreactFlag){
    if(millis() < timeStamp_BMP + reactionLength_BMP){
      digitalWrite(MTR_B_F, LOW);
      digitalWrite(MTR_B_B, HIGH);
      analogWrite(MTR_B_PWM, motorSpeed);
      digitalWrite(MTR_A_F, HIGH);
      digitalWrite(MTR_A_B, LOW);
      analogWrite(MTR_A_PWM, motorSpeed);
      digitalWrite(LED_R, HIGH);
    }
    else{
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      // analogWrite(MTR_B_PWM, 0);
      // analogWrite(MTR_A_PWM, 0);
      digitalWrite(LED_R, LOW);
      bumpLreactFlag = false;
    }
  }
  if(bumpRreactFlag){
    if(millis() < timeStamp_BMP + reactionLength_BMP){
      digitalWrite(MTR_A_F, LOW);
      digitalWrite(MTR_A_B, HIGH);
      analogWrite(MTR_A_PWM, motorSpeed);
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      analogWrite(MTR_B_PWM, motorSpeed);
      digitalWrite(LED_G, HIGH);
    }
    else{
      digitalWrite(MTR_A_F, HIGH);
      digitalWrite(MTR_A_B, LOW);
      // analogWrite(MTR_A_PWM, 0);
      // analogWrite(MTR_B_PWM, 0);
      digitalWrite(LED_G, LOW);
      bumpRreactFlag = false;
    }
  }
  lastBumpLstate = bumpLstate;
  lastBumpRstate = bumpRstate;
}

void play(){
  if(playFlag){ // "play" only works when playFlag is true
    // move forward 
    if(!bumpLreactFlag && !bumpRreactFlag){
      digitalWrite(MTR_A_F, HIGH);
      digitalWrite(MTR_A_B, LOW);
      analogWrite(MTR_A_PWM, motorSpeed);
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      analogWrite(MTR_B_PWM, motorSpeed);
    }

    //blinking and hitting
    if(((millis() + shiftAmount) % cycleLength) < reactionLength_LSR && !blinkFlag){
      blinkFlag = true;
      timeStamp_LSR = millis();
      // Serial.println("blinks");
      if(!bumpRreactFlag && !bumpLreactFlag){
        if(int(random(50)) % 2 == 1) turnRightFlag = true;
        else turnLeftFlag = true;
      }
    }
    if(blinkFlag){
      digitalWrite(LASER, HIGH);
      digitalWrite(SOLENOID, HIGH);
      // turn
      if(turnRightFlag){
        digitalWrite(MTR_B_F, LOW);
        digitalWrite(MTR_B_B, HIGH);
      }
      else if(turnLeftFlag){
        digitalWrite(MTR_A_F, LOW);
        digitalWrite(MTR_A_B, HIGH);
      }
    }
    if(blinkFlag && millis() > timeStamp_LSR + reactionLength_SLND){
      digitalWrite(SOLENOID, LOW);
    }
    if(blinkFlag && millis() > timeStamp_LSR + reactionLength_LSR){
      digitalWrite(LASER, LOW);
      if(turnRightFlag){
        digitalWrite(MTR_B_F, HIGH);
        digitalWrite(MTR_B_B, LOW);
        turnRightFlag = false;
      }
      else if(turnLeftFlag){
        digitalWrite(MTR_A_F, HIGH);
        digitalWrite(MTR_A_B, LOW);
        turnLeftFlag = false;
      }
      blinkFlag = false;
    }
  }
  else if(!followLightFlag){
    analogWrite(MTR_A_PWM, 0);
    analogWrite(MTR_B_PWM, 0);
    digitalWrite(LASER, LOW);
  }
}

void followLight(){
  if(followLightFlag){ // "followLight" only works when followLightFlag is true
  // considering charging at night, this function make collecting robots easier.
  // basically it just following light.
  // but need to figure out how to stop it...
  int brghtnsL = analogRead(CDS_L); 
  int brghtnsR = analogRead(CDS_R);
  // float brDif = map(brghtnsL - brghtnsR, -1024.0, 1024.0, -1.0, 1.0);
  float brDif = (brghtnsL - brghtnsR) / 1000.0;

  if(brDif > 0){ // means light is on the LEFT. so left wheel should be turn less.
    direction[0] = brDif;  // less power for LEFT 
    direction[1] = 1;      // full power for RIGHT
  }
  else if(brDif < 0){ 
    direction[0] = 1;          // full power for LEFT
    direction[1] = abs(brDif); // less power for RIGHT, minus change to plus.
  }

  digitalWrite(MTR_A_F, HIGH);
  digitalWrite(MTR_A_B, LOW);
  analogWrite(MTR_A_PWM, motorSpeed * direction[0]);
  digitalWrite(MTR_B_F, HIGH);
  digitalWrite(MTR_B_B, LOW);
  analogWrite(MTR_B_PWM, motorSpeed * direction[1]);

  // Serial.print("left value is ");
  // Serial.print(brghtnsL);
  // Serial.print(" right value is ");
  // Serial.print(brghtnsR);
  // Serial.print(" brDif is ");
  // Serial.println(brDif);
  // delay(10);
  }
}


