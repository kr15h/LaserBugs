#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
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

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
/*
note:
because irremote using different timer on Leonardo Board,
board2 cannot use D6 pin as PWM output.
*/
#define MTR_A_F 12
#define MTR_A_B 10
#define MTR_A_PWM 9
#define MTR_B_F 6 // used to be 5
#define MTR_B_B 8
#define MTR_B_PWM 5 // used to be 6
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
id is 8bit(0~255)
op is 2bit(0~4) [==, !=, <, >]
address is 3bit(0~7)
value is 16bit(0~65535)

address
0 Cycle Length (0-4095 ms)
1 Laser Reaction Length (%)
2 Motor Speed (0-255 PWM value)
3 Threshold of Light Detect (0-255)
4 Solenoid On Time (0-255 ms)
5 Sequence bit (16bit)[0000000000000000]
6 usage length (4bit)
7 functions*

*functions
0 -
1 play
2 stop
3 go to charge
4 save
5 sequence mode on
6 sequence mode off
7 sequence count reset
8 cue
*/
const int myId = 14;
/*
memo about id
0 means for everything
1st 5 Prototypes take 1 ~ 5
2nd 5 Prototypes take 11 ~ 15
*/
int id; // 8bit [0~255]
int op; // 2bit [0~4] (==, !=, <, >)
int address; // 3bit [0~7]
unsigned int value; // 16bit [0~65535]
boolean correspond = false;
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
unsigned long ignoreThis = 4294967295;

// EEPROM
boolean readFromEEPROM = false;
int cycleLength_add_0 = 0;
int cycleLength_add_1 = 1;
int laser_reactionLength_ratio_add = 2;
int motorSpeed_add = 3;
int threshold_add = 4;
int reactionLength_SLND_add = 5;

// play
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
int motorSpeed = 125; //
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
int motorSpeed = 125; // 250 for slow motor
#endif

boolean playFlag = false;
boolean lastPlayFlag = false;
unsigned int cycleLength = 476; // = bpm126
boolean bangFlag = false;
boolean bangFlag_LSR = false;
boolean bangFlag_SLND = false;
unsigned long shiftAmount = 0;
boolean turnDirection = false; // true is left, false is left
boolean turnFlag = false;
// boolean turnRightFlag = false;
// boolean turnLeftFlag = false;
const int debounce = 4;
boolean rotateReactFlag = false;

// LASER DETECTION
// buffer for mean filter
const int BUFFER_LENGTH = 60;
int buffer[BUFFER_LENGTH];
int index = 0;
// detect laser
int threshold = 10; // used to be 3
// for bangFlag
float laser_reactionLength_ratio = 0.2;
unsigned long timeStamp_LSR;
// boolean bangFlag= false;
unsigned int reactionLength_LSR = cycleLength * laser_reactionLength_ratio;
unsigned int reactionLength_SLND = 20;

// bumper function
boolean bumpLstate;
boolean bumpRstate;
boolean lastBumpLstate;
boolean lastBumpRstate;
boolean bumpLreactFlag = false;
boolean bumpRreactFlag = false;
unsigned long timeStamp_BMP;
// long BMPconstant = 180000;
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
long BMPconstant = 180000; //180000 * 0.5
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
long BMPconstant = 180000; // 500000
#endif
//id 1 ~ 5 has various gear ratio. should have some different BMPconstant

long reactionLength_BMP = BMPconstant / motorSpeed;

// following Light for collect robots for charging
boolean followLightFlag = false;
float direction[] = {0.0, 0.0}; // -1.0 ~ 1.0 (L ~ R)

// sequence functions
boolean sequenceMode = false;
unsigned int sequence;
boolean beat[16];
int loopDigit = 16;
int seqCount = 0;
boolean seqCountFlag = true;

void setup() {
  if(id == 5) BMPconstant = 100000;
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

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  pinMode(MTR_STNBY, OUTPUT);
  digitalWrite(MTR_STNBY, HIGH); // activate motor
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  pinMode(JUMP, INPUT_PULLUP);
#endif
  // prepare mean filter
  for (int i = 0; i < BUFFER_LENGTH; i++) {
    buffer[i] = analogRead(CDS);
  }
  // randomSeed(analogRead(A6));

  // EEPROM reading
  if (readFromEEPROM) {
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
  /*
  note:
  "laserDetect_React()" turns "bangFlag" Booleans on and off.
  and does digitalWrite for Laser and Solenoid.
  "play()" only turns booleans on and off.
  */
  laserDetect_React();
  irCommand();
  battery_check();
  bumperFunction();
  play();
  followLight();
}

void laserDetect_React() {
  int raw = analogRead(CDS);
  // Serial.println(raw);
  //mean filter
  int smoothedByMeanFilter = smoothByMeanFilter();

  if (!bangFlag_LSR && raw - smoothedByMeanFilter > threshold) {
    timeStamp_LSR = millis();
    bangFlag_LSR = true;
    bangFlag_SLND = true;
    turnFlag = true;
    // shiftAmount = shiftAmount + (millis()%cycleLength);
    shiftAmount = millis() % cycleLength;
  }
  if (bangFlag_SLND) {
    digitalWrite(SOLENOID, HIGH);
    // Serial.println("solonoid on");
  }
  if (millis() > timeStamp_LSR + reactionLength_SLND) {
    digitalWrite(SOLENOID, LOW);
    bangFlag_SLND = false;
  }
  if (bangFlag_LSR) {
    digitalWrite(LASER, HIGH);
    // Serial.println("laser on");
  }
  if (millis() > timeStamp_LSR + reactionLength_LSR) {
    digitalWrite(LASER, LOW);
    bangFlag_LSR = false;
  }
  // if(sequenceMode){
  //   if (millis() > timeStamp_LSR + reactionLength_LSR / 2) {
  //     digitalWrite(LASER, LOW);
  //     bangFlag_LSR = false;
  //   }
  // }
  // else if(!sequenceMode){
  //   if (millis() > timeStamp_LSR + reactionLength_LSR) {
  //     digitalWrite(LASER, LOW);
  //     bangFlag_LSR = false;
  //   }
  // }
  

  buffer[index] = raw;
  index = (index + 1) % BUFFER_LENGTH;
  // if(index % BUFFER_LENGTH == 0){
  //   Serial.println(millis());
  // }

  // Serial.print(raw);
  // Serial.print(' ');
  // Serial.println(smoothedByMeanFilter);

  if(rotateReactFlag && !playFlag){
    // truning around while laser is on
    if (bangFlag_LSR) {
      // turn
      if (turnDirection) {
        digitalWrite(MTR_A_F, HIGH);
        digitalWrite(MTR_A_B, LOW);
        digitalWrite(MTR_B_F, LOW);
        digitalWrite(MTR_B_B, HIGH);
        analogWrite(MTR_A_PWM, motorSpeed);
        analogWrite(MTR_B_PWM, motorSpeed);
      }
      else if (!turnDirection) {
        digitalWrite(MTR_A_F, LOW);
        digitalWrite(MTR_A_B, HIGH);
        digitalWrite(MTR_B_F, HIGH);
        digitalWrite(MTR_B_B, LOW);
        analogWrite(MTR_A_PWM, motorSpeed);
        analogWrite(MTR_B_PWM, motorSpeed);
      }
    }
    // finish turning when laser blink has finished
    if (millis() > timeStamp_LSR + reactionLength_LSR && turnFlag == true) {
      // digitalWrite(LASER, LOW);
      if (turnDirection) {
        analogWrite(MTR_A_PWM, 0);
        analogWrite(MTR_B_PWM, 0);
        // digitalWrite(MTR_B_F, HIGH);
        // digitalWrite(MTR_B_B, LOW);
        turnFlag = false;
      }
      else if (!turnDirection) {
        analogWrite(MTR_A_PWM, 0);
        analogWrite(MTR_B_PWM, 0);
        // digitalWrite(MTR_A_F, HIGH);
        // digitalWrite(MTR_A_B, LOW);
        turnFlag = false;
      }
      // Serial.println("turn finished");
      if (int(random(50)) % 2 == 1) turnDirection = true;
      else turnDirection = false;
      // Serial.println(turnDirection);
    }
  }
}

int smoothByMeanFilter() {
  long sum = 0;
  for (int i = 0; i < BUFFER_LENGTH; i++) {
    sum += buffer[i];
  }
  return (int)(sum / BUFFER_LENGTH);
}

void dump(decode_results *results) {
  if (results->bits == 12) irFunction = sonyRemote;
  else if (results->bits == 29) irFunction = irCommander;
}

void irCommand() {
  // receiving signal
  if (irrecv.decode(&results)) {
    // Serial.println(results.value);
    dump(&results);
    irrecv.resume(); // Receive the next value
    irCommandFlag = true;
    timeStamp_MTR = millis();
  }

  if (irFunction == sonyRemote) { // react to sony remote
    if (results.value == 1724) { // turn right
      if (timeStamp_MTR + reactionLength_MTR > millis() && irCommandFlag) {
        digitalWrite(MTR_B_F, LOW);
        digitalWrite(MTR_B_B, HIGH);
        analogWrite(MTR_B_PWM, 100);
      }
      else {
        analogWrite(MTR_B_PWM, 0);
        irCommandFlag = false;
        irNotification = true;
      }
    }
    if (results.value == 3772) { // turn left
      if (timeStamp_MTR + reactionLength_MTR > millis() && irCommandFlag) {
        digitalWrite(MTR_A_F, LOW);
        digitalWrite(MTR_A_B, HIGH);
        analogWrite(MTR_A_PWM, 100);
      }
      else {
        analogWrite(MTR_A_PWM, 0);
        irCommandFlag = false;
        irNotification = true;
      }
    }
    if (results.value == 1436 && irCommandFlag) { // play the behavior
      playFlag = true;
      irCommandFlag = false;
      irNotification = true;
    }
    if (results.value == 412 && irCommandFlag) { // stop the behavior
      playFlag = false;
      irCommandFlag = false;
      irNotification = true;
    }
  }
  else if (irFunction == irCommander && irCommandFlag) {
    // Serial.println(results.value);
    if (results.value != ignoreThis) {
      id = results.value >> 21;
      op = (results.value >> 19) - (id << 2);
      address = (results.value >> 16) - (op << 3) - (id << 5);
      value = results.value - (address << 16) - (op << 19) - (id << 21);
    }
    // id = results.value >> 24;
    // op = (results.value >> 20) - (id << 4);
    // address = (results.value >> 16) - (op << 4) - (id << 8);
    // value = results.value - (address << 16) - (op << 20) - (id << 24);

    Serial.print(id);
    Serial.print(' ');
    Serial.print(op);
    Serial.print(' ');
    Serial.print(address);
    Serial.print(' ');
    Serial.println(value);

    // id is 8bit(0~255)
    // op is 2bit(0~4) [==, !=, <, >]
    // address is 3bit(0~7)
    // value is 16bit(0~65535)
    if (id >= 0 && id < 256 && 
        op >= 0 && op < 5 &&
        address >= 0 && address < 8 &&
        value >= 0 && value < 65536) {
      if (value > 4096 && address == 0 || // cycleLength
          value > 100 && address == 1 ||  // laserReaction_ratio
          value > 255 && address == 2 ||  // motorSpeed
          value > 127 && address == 3 ||  // threshold
          value < 3   && address == 3 ||
          value > 127 && address == 4 ||  // solenoid on time
          value > 16  && address == 6 ||  // loopDigit
          value > 11 && address == 7       // functions
            ){
        Serial.println("irCommand Error");
        irCommandFlag = false;
      }
      else if(id == 0 && op == 0 && address == 0){
        Serial.println("irCommand Error");
        irCommandFlag = false;
      }
      // those are in case of error, prevent values replacing
      else if (id == 0) {
        correspond = true;
      }
      else {
        switch (op) {
          case 0: // ==
            if (id == myId) correspond = true;
            else {
              correspond = false;
              irCommandFlag = false;
            }
            break;
          case 1: // !=
            if (id != myId) correspond = true;
            else {
              correspond = false;
              irCommandFlag = false;
            }
            break;
          case 2: // <
            if (id < myId) correspond = true;
            else {
              correspond = false;
              irCommandFlag = false;
            }
            break;
          case 3: // >
            if (id > myId) correspond = true;
            else {
              correspond = false;
              irCommandFlag = false;
            }
            break;
            // default:
        }
      }
      if (correspond) {
        switch (address) {
          case 0:
            if (value < 4096) cycleLength = value;
            if(!sequenceMode) reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio);
            else if(sequenceMode) reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio/2);
            irCommandFlag = false;
            irNotification = true;
            break;
          case 1:
            laser_reactionLength_ratio = value / 100.0;
            if(!sequenceMode) reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio);
            else if(sequenceMode) reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio/2);
            irCommandFlag = false;
            irNotification = true;
            break;
          case 2:
            // #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
            //             motorSpeed = int(value * 0.5); //
            // #endif
            // #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
            //             motorSpeed = value;
            // #endif
            motorSpeed = value;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 3:
            if (value > 0 && value < 1024) threshold = value;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 4:
            if(value > (cycleLength/4)) value = cycleLength/4;
            reactionLength_SLND = value;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 5:
            sequence = value;
            //converting value to array
            for (int i = 0; i < 16; i++) {
              beat[15 - i] = (value >> i) % 2;
            }
            irCommandFlag = false;
            irNotification = true;
            break;
          case 6:
            loopDigit = value;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 7:
            switch (value) {
              case 1: // play
                playFlag = true;
                followLightFlag = false;
                irCommandFlag = false;
                irNotification = true;
                break;
              case 2: // stop
                playFlag = false;
                followLightFlag = false;
                irCommandFlag = false;
                irNotification = true;
                break;
              case 3: // go to charge
                followLightFlag = true;
                playFlag = false;
                irCommandFlag = false;
                irNotification = true;
                break;
              case 4: // save
                //EEPROM saving function
                /*
                int cycleLength_add_0 = 0;
                int cycleLength_add_1 = 1;
                int laser_reactionLength_ratio_add = 2;
                int motorSpeed_add = 3;
                int threshold_add = 4;
                int reactionLength_SLND_add = 5;
                */

                // EEPROM.write(cycleLength_add_0, cycleLength >> 8);
                // EEPROM.write(cycleLength_add_1, cycleLength - ((cycleLength >> 8) << 8));
                // EEPROM.write(laser_reactionLength_ratio_add, int(laser_reactionLength_ratio * 100));
                // EEPROM.write(motorSpeed_add, motorSpeed);
                // EEPROM.write(threshold_add, threshold);
                // EEPROM.write(reactionLength_SLND_add, reactionLength_SLND);

                irCommandFlag = false;
                irNotification = true;
                break;
              case 5: // sequence mode on
                sequenceMode = true;
                reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio / 2);
                bangFlag = false;
                irCommandFlag = false;
                irNotification = true;
                break;
              case 6: // sequence mode off
                sequenceMode = false;
                reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio);
                bangFlag = false;
                irCommandFlag = false;
                irNotification = true;
                break;
              case 7: // sequence count reset
                seqCount = 0;
                irCommandFlag = false;
                irNotification = true;
                break;
              case 8: // sequence mode off
                timeStamp_LSR = millis();
                bangFlag_LSR = true;
                bangFlag_SLND = true;
                shiftAmount = millis() % cycleLength;
                bangFlag = true;
                irCommandFlag = false;
                irNotification = true;
                break;
              case 9: // sequence mode off
                Serial.print("cycleLength");
                Serial.print("         ");
                Serial.println(cycleLength);
                Serial.print("reactionLength_LSR");
                Serial.print("  ");
                Serial.println(reactionLength_LSR);
                Serial.print("motorSpeed");
                Serial.print("          ");
                Serial.println(motorSpeed);
                Serial.print("threshold");
                Serial.print("           ");
                Serial.println(threshold);
                Serial.print("reactionLength_SLND");
                Serial.print(' ');
                Serial.println(reactionLength_SLND);
                Serial.print("sequenceMode");
                Serial.print("        ");
                Serial.println(sequenceMode);
                Serial.print("loopDigit");
                Serial.print("           ");
                Serial.println(loopDigit);
                irCommandFlag = false;
                irNotification = true;
                break;
              case 10:
                rotateReactFlag = true;
                irCommandFlag = false;
                irNotification = true;
                break;
              case 11:
                rotateReactFlag = false;
                irCommandFlag = false;
                irNotification = true;
                break;
            }
            break;
        }
      }
    }
    else {
      Serial.println("irCommand Error");
      irCommandFlag = false;
    }
  }
  if (irNotification) {
    if (blinkCount < 5) {
      if (millis() % 100 > 50) {
        // digitalWrite(LED_R, HIGH);
        digitalWrite(LED_R, LOW);
        if (countFlag) {
          blinkCount++;
          countFlag = false;
        }
      }
      else {
        // digitalWrite(LED_R, LOW);
        digitalWrite(LED_R, HIGH);
        if (!countFlag) countFlag = true;
      }
    }
    else {
      // Serial.println("blinkfinished");
      blinkCount = 0;
      irNotification = false;
    }
  }
}

void battery_check() {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  if (analogRead(BAT_CHK) < 200) {
    if (millis() % 500 < 250) digitalWrite(LED_R, HIGH);
    else digitalWrite(LED_R, LOW);
  }
  // Serial.println(analogRead(BAT_CHK));
#endif
}

void bumperFunction() {
  bumpLstate = digitalRead(L_BUMPER);
  bumpRstate = digitalRead(R_BUMPER);

  if (!bumpLstate && lastBumpLstate && !bumpLreactFlag && !bumpRreactFlag) {
    bumpLreactFlag = true;
    timeStamp_BMP = millis();
  }
  if (!bumpRstate && lastBumpRstate && !bumpLreactFlag && !bumpRreactFlag) {
    bumpRreactFlag = true;
    timeStamp_BMP = millis();
  }

  if (bumpLreactFlag) {
    if (millis() < timeStamp_BMP + reactionLength_BMP) {
      digitalWrite(MTR_B_F, LOW);
      digitalWrite(MTR_B_B, HIGH);
      analogWrite(MTR_B_PWM, motorSpeed);
      digitalWrite(MTR_A_F, HIGH);
      digitalWrite(MTR_A_B, LOW);
      analogWrite(MTR_A_PWM, motorSpeed);
      // digitalWrite(LED_R, HIGH);
    }
    else {
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      // analogWrite(MTR_B_PWM, 0);
      // analogWrite(MTR_A_PWM, 0);
      digitalWrite(LED_R, LOW);
      bumpLreactFlag = false;
    }
  }
  if (bumpRreactFlag) {
    if (millis() < timeStamp_BMP + reactionLength_BMP) {
      digitalWrite(MTR_A_F, LOW);
      digitalWrite(MTR_A_B, HIGH);
      analogWrite(MTR_A_PWM, motorSpeed);
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      analogWrite(MTR_B_PWM, motorSpeed);
      // digitalWrite(LED_G, HIGH);
    }
    else {
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

void play() {
  if (playFlag) { // "play" only works when playFlag is true


    if (sequenceMode) {
      // if((millis() % cycleLength) > (shiftAmount % cycleLength) &&
      //    (millis() % cycleLength) < (shiftAmount % cycleLength) + debounce && !bangFlag){
      if ((millis() % (cycleLength/4)) > (shiftAmount % (cycleLength/4)) &&
          (millis() % (cycleLength/4)) < (shiftAmount % (cycleLength/4)) + debounce && !bangFlag) {
        if (beat[seqCount]) {
          bangFlag_LSR = true;
          bangFlag_SLND = true;
        }
        timeStamp_LSR = millis();
        seqCount ++;
        bangFlag = true;
        turnFlag = true;
        // Serial.print(seqCount-1);
        // Serial.print(' ');
        // Serial.println(beat[seqCount]);
        if (seqCount >= loopDigit) seqCount = 0;
      }
    }
    else if (!sequenceMode) {

      //blinking and hitting
      // if(((millis() + shiftAmount) % cycleLength) < reactionLength_LSR && !bangFlag_LSR){
      // if((millis() % cycleLength) == shiftAmount &&  !bangFlag_LSR){
      // if((millis() % cycleLength) > (shiftAmount % cycleLength) &&
      //    (millis() % cycleLength) < (shiftAmount % cycleLength) + debounce && !bangFlag){
      if ((millis() % cycleLength) > shiftAmount &&
          (millis() % cycleLength) < shiftAmount + debounce && !bangFlag) {
        bangFlag_LSR = true;
        bangFlag_SLND = true;
        timeStamp_LSR = millis();
        bangFlag = true;
        turnFlag = true;
        // Serial.print("blinks");
        // Serial.print(' ');
        // Serial.print(cycleLength);
        // Serial.print(' ');
        // Serial.println(shiftAmount);
      }
    }

    // something common in Seq mode and non-seq mode
    if (millis() - timeStamp_LSR > debounce) {
      bangFlag = false;
    }

    // moving forward 
    if (!bumpLreactFlag && !bumpRreactFlag) {
      digitalWrite(MTR_A_F, HIGH);
      digitalWrite(MTR_A_B, LOW);
      analogWrite(MTR_A_PWM, motorSpeed);
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      analogWrite(MTR_B_PWM, motorSpeed);
      // Serial.println("motorB should work");
    }

    // truning around while laser is on
    if (bangFlag_LSR) {
      // turn
      if (turnDirection) {
        digitalWrite(MTR_B_F, LOW);
        digitalWrite(MTR_B_B, HIGH);
      }
      else if (!turnDirection) {
        digitalWrite(MTR_A_F, LOW);
        digitalWrite(MTR_A_B, HIGH);
      }
    }
    // finish turning when laser blink has finished
    if (millis() > timeStamp_LSR + reactionLength_LSR && turnFlag == true) {
      // digitalWrite(LASER, LOW);
      if (turnDirection) {
        digitalWrite(MTR_B_F, HIGH);
        digitalWrite(MTR_B_B, LOW);
        turnFlag = false;
      }
      else if (!turnDirection) {
        digitalWrite(MTR_A_F, HIGH);
        digitalWrite(MTR_A_B, LOW);
        turnFlag = false;
      }
      if (int(random(50)) % 2 == 1) turnDirection = true;
      else turnDirection = false;
      // Serial.println(turnDirection);
    }
  }
  else if (!playFlag && lastPlayFlag){
    analogWrite(MTR_A_PWM, 0);
    analogWrite(MTR_B_PWM, 0);
    Serial.println("motor off");
  }

  // else if (!followLightFlag || !rotateReactFlag) {
    // analogWrite(MTR_A_PWM, 0);
    // analogWrite(MTR_B_PWM, 0);
  // }
  lastPlayFlag = playFlag;
}

void followLight() {
  if (followLightFlag) {
    // "followLight" only works when followLightFlag is true
    // considering charging at night, this function make collecting robots easier.
    // basically it just following light.
    // but need to figure out how to stop it...
    int brghtnsL = analogRead(CDS_L);
    int brghtnsR = analogRead(CDS_R);
    // float brDif = map(brghtnsL - brghtnsR, -1024.0, 1024.0, -1.0, 1.0);
    float brDif = (brghtnsL - brghtnsR) / 1000.0;

    if (brDif > 0) { // means light is on the LEFT. so left wheel should be turn less.
      direction[0] = brDif;  // less power for LEFT
      direction[1] = 1;      // full power for RIGHT
    }
    else if (brDif < 0) {
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
