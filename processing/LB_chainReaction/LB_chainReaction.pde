/* 
 LASERBUG SIMULATOR by SoKANNO
 05.11.2014
 
 this is a simulation of new idea of installation named "laserbug".
 inspired by swarm intelligence.
 
 algorhithm of each instance is based in mechanical structures.
 */

//making sound with SuperCollider
//this is setup for OSC communication of P5 and SC 
import oscP5.*;
import netP5.*;
OscP5 osc;
NetAddress supercollider;

//for control parameters
import controlP5.*;
ControlP5 cp5;
int blinkTime = 50;
int loopTime = 2000;
float range = 1.0;
boolean startFlag = false;

//to make a report file in csv
PrintWriter output;
long reportTime = 1 * 60 * 1000;
int detectCount = 0;
int totalLT = 0;
String date;

ArrayList<Rotator> rots;
//number and size of modules
int row = 7;
int column = 7;
int moduleSize = 5; //half diamiter

// turnAmount is angle that rotater turns when hit other module or wall.
// from science of leaf angle of "5/13".
float turnAmount = 2.416; 

void setup() {
  //size(1200, 800);
  size(1000, 500);
  // frameRate(30);

  osc = new OscP5(this, 12000);
  supercollider = new NetAddress("127.0.0.1", 57120);

  // rots = new ArrayList<Rotator>();
  // // calculate the interval
  // float xInterval = width / column + 1;
  // float yInterval = height / row + 1;
  // // make instanses in glid
  // // Rotator(xpos, ypos, rotationSpeed, movingSpeed)
  // for (int i=0; i<row; i++) {
  //   for (int j=0; j<column; j++) {
  //     float xpos = (j+0.5)*xInterval;
  //     float ypos = (i+0.5)*yInterval;
  //     float rotateSpeed = 0.1;
  //     float moveSpeed = 1;
  //     float r = random(0, 1.0);
  //     // println(r);
  //     // int eachloopTime = int(random(loopTime*range, loopTime));
  //     int eachloopTime;
  //     if(r < 0.95) {
  //       eachloopTime = loopTime;
  //     }
  //     else {
  //       // eachloopTime = loopTime/4;
  //       eachloopTime = loopTime;
  //     }
  //     // int loopTime = 1000;
  //     int shiftTime = int(random(0, loopTime));
  //     float blinkTime = 50; // in milli seconds.
  //     rots.add(new Rotator(xpos, ypos, rotateSpeed, moveSpeed, eachloopTime, shiftTime, blinkTime));
  //     // 0));
  //   }
  // }

  // cp5
  cp5 = new ControlP5(this);
  cp5.addSlider("blinkTimeSlider")
    .setPosition(20, 20)
      .setSize(100, 20)
        .setRange(1, 100)
          .setValue(blinkTime);
  cp5.addSlider("loopTimeSlider")
    .setPosition(20, 50)
      .setSize(100, 20)
        .setRange(1000, 8000)
          .setValue(loopTime);
  // cp5.addSlider("blinkTimeRangeSlider")
  //   .setPosition(20, 80)
  //     .setSize(100, 20)
  //       .setRange(0.5, 1.0)
  //         .setValue(range);

  cp5.addSlider("row")
    .setPosition(20, 110)
      .setSize(100, 20)
        .setRange(1, 10)
          .setValue(row);
  cp5.addSlider("column")
    .setPosition(20, 140)
      .setSize(100, 20)
        .setRange(1, 10)
          .setValue(column);
  cp5.addButton("Start")
     .setPosition(20, height-50)
     .setSize(100,20)
     ;

  date = year() + nf(month(),2) + nf(day(),2) + "-"  + nf(hour(),2) + nf(minute(),2) + ".csv";
  println(startFlag);
  //output = createWriter(date); 
}

void draw() {
  smooth();
  ellipseMode(RADIUS);
  background(30);

  if(startFlag){
    for (Rotator r : rots) {
      r.react(rots);
      r.collision(rots);
      r.selfblinking();
      r.edgeDetection();
      r.movement();
      r.update();
    }
  }else{
    rots = new ArrayList<Rotator>();
    // calculate the interval
    float xInterval = width / column + 1;
    float yInterval = height / row + 1;
    for (int i=0; i<row; i++) {
      for (int j=0; j<column; j++) {
        float xpos = (j+0.5)*xInterval;
        float ypos = (i+0.5)*yInterval;
        float rotateSpeed = 0.04;
        float moveSpeed = 0.4;
        float r = random(0, 1.0);
        // println(r);
        // int eachloopTime = int(random(loopTime*range, loopTime));
        int eachloopTime;
        if(r < 0.95) {
          eachloopTime = loopTime;
        }
        else {
          // eachloopTime = loopTime/4;
          eachloopTime = loopTime;
        }
        // int loopTime = 1000;
        int shiftTime = int(random(0, loopTime));
        float blinkTime = 50; // in milli seconds.
        rots.add(new Rotator(xpos, ypos, rotateSpeed, moveSpeed, eachloopTime, shiftTime, blinkTime));
        // 0));
      }
    }
    for (Rotator r : rots) {
      r.update();
    }
  }
  fill(200);
  text(frameRate, 20, height-40);

  if(millis() > reportTime){
//    report();
  }

}

// for debug
void keyPressed() {
  // println("new loop");
  if (key == 'm') {
    background(30);
    for (Rotator r : rots) {
      r.react(rots);
      r.collision(rots);
      r.selfblinking();
      r.edgeDetection();
      r.movement();
      r.update();
    }
  }
}

public void Start(int theValue) {
  startFlag = !startFlag;
}

void blinkTimeSlider(int blinkTime){
  for (Rotator r : rots) {
    r.blinkTime = blinkTime;
  }
}

// void loopTimeSlider(int loopTime){
//   for (Rotator r : rots) {
//     r.loopTime = int(random(loopTime*range, loopTime));
//   }
// }

// void rangeSlider(int range){
//   for (Rotator r : rots) {
//     r.loopTime = int(random(loopTime*range, loopTime));
//   }
// }

/*
Hao Hao の真似してみたいな
 とりあえずまだちゃんと動いてない。
 ちゃんと動くところまでまず作る・
 試したいアルゴリズム。
   - ヒットされてる時だけ光るんじゃなくて、ヒットされたら一定期間光って回るとかにすれば
     連鎖が起きうる気がする。
   - ただ、光を受けない限り動き始めないとすると、最初のきっかけがけっこう重要になる。
 
 */