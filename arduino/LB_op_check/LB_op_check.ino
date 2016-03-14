
// #define boardproduction1
#define boardproduction2

#ifdef boardproduction1
  // #define boardproduction 1
  #define MTR_A_F 12
  #define MTR_A_B 10
  #define MTR_A_PWM 9
  #define MTR_B_F 5
  #define MTR_B_B 8
  #define MTR_B_PWM 6
  #define MTR_STNBY 18
  #define SOLENOID 7
  #define LASER 4
  #define LED_G 13
  #define LED_R 19
  // in
  #define L_BUMPER 2
  #define R_BUMPER 3
  #define RECV_PIN 11
  #define CDS 0
  // #define JUMP_0 15
  // #define JUMP_1 16
  #define CDS_L 1
  #define CDS_R 2
  #define BAT_CHK 17  
#endif

#ifdef boardproduction2
  // #define boardproduction 2
  #define MTR_A_F 12
  #define MTR_A_B 10
  #define MTR_A_PWM 9
  #define MTR_B_F 5
  #define MTR_B_B 8
  #define MTR_B_PWM 6
  #define JUMP A4
  // #define JUMP 18
  #define SOLENOID 7
  #define LASER 4
  #define LED_G 13
  #define LED_R A5
  // in
  #define L_BUMPER 2
  #define R_BUMPER 3
  #define RECV_PIN 11
  #define CDS A0
  #define CDS_L A1
  #define CDS_R A2
  #define BAT_CHK A3
#endif

// libraries
#include <IRremote.h>
#include <EEPROM.h>

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
0 Cycle Length (0-4096 ms)
1 Laser Reaction Length (%)
2 Motor Speed (0-255 PWM value)
3 Threshold of Light Detect (0-255)
4 Solenoid On Time (0-255 ms)
5 -
6 -
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
boolean readFromEEPROM = false; 
int cycleLength_add_0 = 0;
int cycleLength_add_1 = 1;
int laser_reactionLength_ratio_add = 2;
int motorSpeed_add = 3;
int threshold_add = 4;
int reactionLength_SLND_add = 5;

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
float laser_reactionLength_ratio = 0.3;
unsigned long timeStamp_LSR;
boolean reaction_LSR = false;
int reactionLength_LSR = cycleLength * laser_reactionLength_ratio;
int reactionLength_SLND = 20;
// int delayTime = 0;

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
  pinMode(SOLENOID, OUTPUT);
  pinMode(LASER, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(L_BUMPER, INPUT_PULLUP);
  pinMode(R_BUMPER, INPUT_PULLUP);
  pinMode(CDS, INPUT);
  // pinMode(JUMP_1, INPUT_PULLUP);
  pinMode(BAT_CHK, INPUT);
  irrecv.enableIRIn(); // Start the receiver

  #ifdef boardproduction1
    pinMode(MTR_STNBY, OUTPUT);
    digitalWrite(MTR_STNBY, HIGH); // activate motor  
  #endif
  #ifdef boardproduction2
    pinMode(JUMP, INPUT_PULLUP);
  #endif
  // prepare mean filter 
  for(int i=0; i<BUFFER_LENGTH; i++){
    buffer[i] = analogRead(CDS);
  }
  randomSeed(analogRead(A6));

  // EEPROM reading 
  if(readFromEEPROM){
    byte a = EEPROM.read(cycleLength_add_0);
    byte b = EEPROM.read(cycleLength_add_1);
    cycleLength = (a << 8) + b;
    laser_reactionLength_ratio = EEPROM.read(laser_reactionLength_ratio_add) / 100.0;
    motorSpeed = EEPROM.read(motorSpeed_add);
    threshold = EEPROM.read(threshold_add);
    reactionLength_SLND = EEPROM.read(reactionLength_SLND_add);
  }
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
  // Serial.println(raw);
  //mean filter
  int smoothedByMeanFilter = smoothByMeanFilter();

  if(!reaction_LSR && raw - smoothedByMeanFilter > threshold){  
    // Serial.println("light detected");
    timeStamp_LSR = millis();
    reaction_LSR = true;
    // shiftAmount = shiftAmount + (millis()%cycleLength);
    shiftAmount = millis() % cycleLength;
  }
  if(reaction_LSR && millis() > timeStamp_LSR){
    // digitalWrite(LASER, HIGH);
    // digitalWrite(SOLENOID, HIGH);  
    digitalWrite(LASER, LOW);
    digitalWrite(SOLENOID, LOW);  
  }
  if(reaction_LSR && (timeStamp_LSR + reactionLength_SLND) < millis()){
    // digitalWrite(SOLENOID, LOW);
    digitalWrite(SOLENOID, HIGH);
  }
  if(reaction_LSR && (timeStamp_LSR + reactionLength_LSR) < millis()){
    // digitalWrite(LASER, LOW);
    digitalWrite(LASER, HIGH);
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
        cycleLength = value;
        reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio);
        irCommandFlag = false;
        irNotification = true;
        break;
      case 1:
        laser_reactionLength_ratio = value / 100.0;
        reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio);
        irCommandFlag = false;
        irNotification = true;
        break;
      case 2:
        motorSpeed = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 3:
        threshold = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 4:
        reactionLength_SLND = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 5:
        irCommandFlag = false;
        irNotification = true;
        break;
      case 6:
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
            int cycleLength_add_0 = 0;
            int cycleLength_add_1 = 1;
            int laser_reactionLength_ratio_add = 2;
            int motorSpeed_add = 3;
            int threshold_add = 4;
            int reactionLength_SLND_add = 5;
            */ 
            EEPROM.write(cycleLength_add_0, cycleLength >> 8);
            EEPROM.write(cycleLength_add_1, cycleLength - ((cycleLength >> 8) << 8));
            EEPROM.write(laser_reactionLength_ratio_add, int(laser_reactionLength_ratio * 100));
            EEPROM.write(motorSpeed_add, motorSpeed);
            EEPROM.write(threshold_add, threshold);
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
        // digitalWrite(LED_R, HIGH);
        digitalWrite(LED_R, LOW);
        if(countFlag){
          blinkCount++;
          countFlag = false;
        }
      }
      else{
        // digitalWrite(LED_R, LOW);
        digitalWrite(LED_R, HIGH);
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


