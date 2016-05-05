import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import oscP5.*; 
import netP5.*; 
import controlP5.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class LB_chainReaction extends PApplet {

/* 
 LASERBUG SIMULATOR by SoKANNO
 05.11.2014
 
 this is a simulation of new idea of installation named "laserbug".
 inspired by swarm intelligence.
 
 algorhithm of each instance is based in mechanical structures.
 */

//making sound with SuperCollider
//this is setup for OSC communication of P5 and SC 


OscP5 osc;
NetAddress supercollider;

//for control parameters

ControlP5 cp5;
int blinkTime = 200;
int loopTime = 3000;
float range = 1.0f;
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
float turnAmount = 2.416f; 

int laserRange = 300;

public void setup() {
  println(loopTime);
  // size(1366, 730); //MBA11inch Fullscreen
  
  // size(360, 240);
  // size(1920, 1160); //MBP15inch Fullscreen
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
  cp5.addSlider("blinkTime")
    .setPosition(20, 20)
      .setSize(100, 20)
        .setRange(1, 1000)
          // .setValue(blinkTime);
          .setValue(200);


  cp5.addSlider("loopTime")
    .setPosition(20, 50)
      .setSize(100, 20)
        .setRange(100, 5000)
          // .setValue(loopTime);
          .setValue(2000);
  cp5.addSlider("laserRange")
    .setPosition(20, 80)
      .setSize(100, 20)
        .setRange(100, 1000)
          .setValue(laserRange);

  cp5.addSlider("row")
    .setPosition(20, 110)
      .setSize(100, 20)
        .setRange(1, 15)
          .setValue(row);
  cp5.addSlider("column")
    .setPosition(20, 140)
      .setSize(100, 20)
        .setRange(1, 15)
          .setValue(column);
  cp5.addButton("Start")
     .setPosition(20, height-50)
     .setSize(100,20)
     ;

  date = year() + nf(month(),2) + nf(day(),2) + "-"  + nf(hour(),2) + nf(minute(),2) + ".csv";
  println(startFlag);
  //output = createWriter(date); 
    println(loopTime);

}

public void draw() {
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
        float xpos = (j+0.5f)*xInterval;
        float ypos = (i+0.5f)*yInterval;
        float rotateSpeed = 0.04f;
        float moveSpeed = 0.4f;
        float r = random(0, 1.0f);
        // println(r);
        // int eachloopTime = int(random(loopTime*range, loopTime));
        int eachloopTime;
        if(r < 0.95f) {
          eachloopTime = loopTime;
        }
        else {
          // eachloopTime = loopTime/4;
          eachloopTime = loopTime;
        }
        // int loopTime = 1000;
        int shiftTime = PApplet.parseInt(random(0, loopTime));
        // float blinkTime = 50; // in milli seconds.
        rots.add(new Rotator(xpos, ypos, rotateSpeed, moveSpeed, eachloopTime, shiftTime, blinkTime, laserRange));
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
public void keyPressed() {
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

public void blinkTimeSlider(int blinkTime){
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
Hao Hao \u306e\u771f\u4f3c\u3057\u3066\u307f\u305f\u3044\u306a
 \u3068\u308a\u3042\u3048\u305a\u307e\u3060\u3061\u3083\u3093\u3068\u52d5\u3044\u3066\u306a\u3044\u3002
 \u3061\u3083\u3093\u3068\u52d5\u304f\u3068\u3053\u308d\u307e\u3067\u307e\u305a\u4f5c\u308b\u30fb
 \u8a66\u3057\u305f\u3044\u30a2\u30eb\u30b4\u30ea\u30ba\u30e0\u3002
   - \u30d2\u30c3\u30c8\u3055\u308c\u3066\u308b\u6642\u3060\u3051\u5149\u308b\u3093\u3058\u3083\u306a\u304f\u3066\u3001\u30d2\u30c3\u30c8\u3055\u308c\u305f\u3089\u4e00\u5b9a\u671f\u9593\u5149\u3063\u3066\u56de\u308b\u3068\u304b\u306b\u3059\u308c\u3070
     \u9023\u9396\u304c\u8d77\u304d\u3046\u308b\u6c17\u304c\u3059\u308b\u3002
   - \u305f\u3060\u3001\u5149\u3092\u53d7\u3051\u306a\u3044\u9650\u308a\u52d5\u304d\u59cb\u3081\u306a\u3044\u3068\u3059\u308b\u3068\u3001\u6700\u521d\u306e\u304d\u3063\u304b\u3051\u304c\u3051\u3063\u3053\u3046\u91cd\u8981\u306b\u306a\u308b\u3002
 
 */
  class Rotator {
  PVector location;
  float rSpeed, mSpeed, rotSize, angle, targetAngle, capAngle, blinkTime, r;
  int detectCount, shift, type, loopTime, defaultTime, laserRange;
  long shiftTime;
  boolean pairing, angleMatch, detectFlag, lastDetectFlag, soundFlag;
  String turnMode, lightMode;
  Rotator (float x, float y, float rs, float ms, int lt, int dt, float bt, int rr) {
    location = new PVector(x, y);
    rSpeed = rs;
    mSpeed = ms;
    rotSize = moduleSize;
    angle = random(0, TWO_PI);
    detectCount = 0;
    shift = 0;
    angleMatch = false;
    turnMode = "OFF";
    loopTime = lt;
    shiftTime = dt;
    soundFlag = true;
    blinkTime = bt;
    laserRange = rr;
  }

  // detecting other rotator's laser
  public void selfblinking() {
    if ((millis() + shiftTime) % loopTime < blinkTime) {
      lightMode = "ON";
      if(r > 0.5f) angle += rSpeed;
      else angle -= rSpeed;
      if (soundFlag) {
        OscMessage msg = new OscMessage("/collision");
        float xpos = map(location.x, 0, width, -1, 1);
        msg.add(xpos);
        // if(loopTime == 2000) msg.add(800);
        // else msg.add(1000);
        // msg.add(int(random(700,740)));
        msg.add(440);
        osc.send(msg, supercollider); 
        soundFlag = false;
        r = random(0, 1);
      }
      // println(lightMode);
    } else {
      lightMode = "OFF";
      soundFlag = true;
    }
  }


  public void react(ArrayList<Rotator> rots) {
    for (Rotator other : rots) {
      // calculate the distance
      if (location != other.location) { // this line is important to ignore itself
        float distance = PVector.dist(other.location, location);
        // calculate the tolerance from distance and rotSize
        float tolerance = atan2(rotSize, distance);
        // calculate the angle of two rotators
        float collideAngle = atan2(other.location.y - location.y, 
        other.location.x - location.x);
        capAngle = collideAngle;
        collideAngle = collideAngle + PI;  // because atan2 value is -PI ~ PI      
        // compare the angle of other's laser and itself
        float diff = abs(collideAngle - other.angle);
        // if(other.detectCount > 1 || other.turnMode != "OFF"){ 
        if (other.lightMode == "ON") { 
          // when it's on the corner, light is on
          // if(other.detectCount > 1){ 
          // if (diff < tolerance || abs(diff - TWO_PI) < tolerance) {
          if (diff < tolerance || abs(diff - TWO_PI) < tolerance && distance < laserRange) {
            detectCount ++;
            shift = shift + 5; 
            detectFlag = true;
            // if (detectFlag && !lastDetectFlag && lightMode == "OFF") {
            if (detectFlag && !lastDetectFlag) {
              //most important line of code for SYNC
              shiftTime = shiftTime + (loopTime - ((millis() + shiftTime) % loopTime));
              lastDetectFlag = true;
            }
          } else {
            detectFlag = false;
            if (!detectFlag && lastDetectFlag) {
              lastDetectFlag = false;
            }
          }
        }
      }
    }
  }

  // detecting collision
  public void collision(ArrayList<Rotator> rots) {
    for (Rotator other : rots) {
      if (location != other.location) { // to ignore itself
        float d = PVector.dist(other.location, location); // calculate the distance to other modules
        if (d < rotSize*2) {
          // calculate the angle of two mods
          float collideAngle = atan2(location.y - other.location.y, 
          location.x - other.location.x);
          collideAngle = collideAngle + PI;
          if (turnMode == "OFF") {
            if (angle > PI) {
              if (angle - collideAngle < PI && angle - collideAngle > 0) {
                turnMode = "CW";
                targetAngle = angle + turnAmount;
              } else {
                turnMode = "CCW";
                targetAngle = angle - turnAmount;
              }
            } else if (angle < PI) {
              if (collideAngle - angle < PI && collideAngle - angle > 0) {
                turnMode = "CCW";
                targetAngle = angle - turnAmount;
              } else {
                turnMode = "CW";
                targetAngle = angle + turnAmount;
              }
            }
          }
        }
      }
    }
  }

  public void edgeDetection() {
    // reflection detect for X axis end
    if (turnMode == "OFF") {
      // for right end
      if (location.x > (width-(rotSize))) {
        if (angle < TWO_PI && angle > PI*3/2) { // angle is down
          turnMode = "CCW";
          targetAngle = angle - turnAmount;
        } else if (angle > 0 && angle < HALF_PI) { // angle is up
          turnMode = "CW";
          targetAngle = angle + turnAmount;
        }
      }
      //for left end
      else if (location.x < (rotSize)) {
        if (angle > PI) { // angle is up
          turnMode = "CW";
          targetAngle = angle + turnAmount;
        } else if (angle < PI) { // angle is down
          turnMode = "CCW";
          targetAngle = angle - turnAmount;
        }
      }
      // reflection detect for Y axis end
      // for bottom end
      if (location.y > (height-(rotSize))) {
        // print("bot", angle, ' ');
        if (angle > (HALF_PI)) { // angle is right
          turnMode = "CW";
          targetAngle = angle + turnAmount;
          // println(turnMode);
        } else if (angle < (HALF_PI)) { // angle is left
          turnMode = "CCW";
          targetAngle = angle - turnAmount;
          // println(turnMode);
        }
      }
      //for top end
      else if (location.y < (rotSize)) {
        // print("top", angle, ' ');
        if (angle > PI+HALF_PI) { // angle is left
          turnMode = "CW";
          targetAngle = angle + turnAmount;
          // println(turnMode);
        } else if (angle < PI+HALF_PI) { // angle is right
          turnMode = "CCW";
          targetAngle = angle - turnAmount;
          // println(turnMode);
        }
      }
      // println(angle, " ", targetAngle);
    }
  }

  public void movement() { // modules turn or rotate
    // rotate and change direction
    if (turnMode != "OFF") {
      if (turnMode == "CW") {
        angle += rSpeed;
        // rotation finish
        if (angle > targetAngle) {
          if (angle < 0) angle = angle + TWO_PI;
          if (angle > TWO_PI) angle = angle - TWO_PI;

          // move a little to prevent detect end again
          location.x = location.x + cos(angle)*mSpeed*2;
          location.y = location.y + sin(angle)*mSpeed*2;
          turnMode = "OFF";
        }
      } else if (turnMode == "CCW") {
        angle -= rSpeed;
        //rotation finish
        if (angle < targetAngle) {
          if (angle < 0) angle = angle + TWO_PI;
          if (angle > TWO_PI) angle = angle - TWO_PI;

          // move a little to prevent detect end again
          location.x = location.x + cos(angle)*mSpeed*2;
          location.y = location.y + sin(angle)*mSpeed*2;
          turnMode = "OFF";
        }
      }
    }else{
      location.x = location.x + cos(angle)*mSpeed;
      location.y = location.y + sin(angle)*mSpeed;
    }
  }

  // drawing things
  public void update() {
    // change color to fill depend on it's detecting or not.
    if (lightMode == "ON")  fill(0, 200, 0);  
    else fill(150);
    // else fill(loopTime / 10);

    //drawing Modules
    strokeWeight(1);
    ellipseMode(RADIUS);
    stroke(100, 100, 100);
    ellipse(location.x, location.y, rotSize, rotSize);
    line(location.x, location.y, location.x + cos(angle)*rotSize, location.y + sin(angle)*rotSize);

    //drawing lasers
    stroke(0, 255, 0);
    strokeWeight(2);
    // if(detectCount != 0 || turnMode != "OFF" || lightMode == "ON"){ // when it's on the corner, light is on
    if (lightMode == "ON") { // when it's on the corner, light is on
      // if(detectCount != 0){

      // line(location.x + cos(angle)*rotSize, location.y + sin(angle)*rotSize, 
      // location.x + cos(angle)*rotSize*300, location.y + sin(angle)*rotSize*300);

      for(int i = 0; i<laserRange; i++){
        stroke(0, 255, 0, 255-i*255/laserRange);
        point(location.x + cos(angle)*i, location.y + sin(angle)*i);
      }
    }

    // reset detect count when finish to draw
    detectCount = 0;
  }
}
/*
this class is for reporting the result of code with conditions
 content of csv is 
 - size of one module / screen size
 - number of module
 - population density
 - counts of detection
 - loop timing range
 - blinking time length
 
 what I really need:
 - population density
 - filled area (%)
 - detect count / average loop time
 - average of loop time
 - blink time


 another csv (will be seond try)
 - timing of blinking
 (gonna be heavy...?)
 
 */
public void report() {
  //number of module
  float nom = row*column;
  //screen size
  float ss = height*width;
  //population density (how many mods on 100*100)
  float pd = nom / (ss/10000);
  //filled area (%)
  float fa =  (moduleSize*moduleSize*3.14f)*nom / ss * 100;
  //counts of detection
  for (Rotator r : rots) {
      detectCount = detectCount + r.detectCount;
    }
  int dt = detectCount;
  //average of loop time
  for (Rotator r : rots) {
      totalLT = totalLT + r.loopTime;
    }
  float avlt = totalLT / nom;
  //detect count / average loop time
  float dtpavlt = dt / (reportTime / avlt);
  //blinking time length
  int btl = blinkTime;

  output.println(pd + "," + fa + "," + dtpavlt + "," + avlt + "," + btl);
  // output.println(somss);
  // output.println(nom);
  // output.println(pd);
  // output.println(dt);
  // output.println(dtpm);
  // output.println(ltrh);
  // output.println(ltrl);
  // output.println(btl);

  output.flush();
  output.close();
  exit();
}
  public void settings() {  size(1000, 500); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "LB_chainReaction" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
