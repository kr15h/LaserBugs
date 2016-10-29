#include "Bug.h"

namespace laserbugs{

Bug::Bug(
	ofPoint position,
	float rotationSpeed,
	float movementSpeed,
	int eachLoopTime,
	int shiftTime,
	int blinkTime,
	int laserRange){
	
	_sharedData = 0;
	
	_position = position;
	_rotationSpeed = rotationSpeed;
	_movementSpeed = movementSpeed;
	_eachLoopTime = eachLoopTime;
	_shiftTime = shiftTime;
	_blinkTime = blinkTime;
	_laserRange = laserRange;
}

void Bug::setup(){
	if(_sharedData == 0){
		ofLogError("Bug::setup", "Please provide SharedData");
		return;
	}
}

void Bug::update(){
	if(_sharedData == 0){
		ofLogError("Bug::setup", "Please provide SharedData");
		return;
	}
}

void Bug::draw(){
	if(_sharedData == 0){
		ofLogError("Bug::setup", "Please provide SharedData");
		return;
	}
	
	ofPushStyle();
	ofSetColor(0, 255, 255);
	ofDrawCircle(_position.x, _position.y, 5);
	ofPopStyle();
	
	/*
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
	*/
}

void Bug::selfblinking(){
 /*
 if ((millis() + shiftTime) % loopTime < blinkTime) {
      lightMode = "ON";
      if(r > 0.5) angle += rSpeed;
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
 */
}

void Bug::react(vector<Bug *> & bugs){
/*
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
*/
}

void Bug::collision(vector<Bug *> & bugs){
/*
 void collision(ArrayList<Rotator> rots) {
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
*/
}

void Bug::edgeDetection(){
/*
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
*/
}

void Bug::movement(){
/*
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
*/
}

void Bug::setSharedData(shared_ptr<SharedData> sd){
	_sharedData = sd;
}

} // namespace laserbugs
