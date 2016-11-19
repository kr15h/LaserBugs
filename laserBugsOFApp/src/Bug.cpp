#include "Bug.h"

namespace laserbugs{

Bug::Bug(
	ofPoint location,
	float rotationSpeed,
	float movementSpeed,
	int loopTime,
	int shiftTime,
	int blinkTime,
	int laserRange,
	shared_ptr<SharedData> sd){
	
	_soundFlag = true;
	_location = location;
	_rSpeed = rotationSpeed;
	_mSpeed = movementSpeed;
	_loopTime = loopTime;
	_shiftTime = shiftTime;
	_blinkTime = blinkTime;
	_laserRange = laserRange;
	_sharedData = sd;
	
	// TODO: use enum's here
	_turnMode = "OFF";
	
	_rotSize = _sharedData->bugSize;
	
	_pulseOffset = ofRandom(2.0f);
}

void Bug::setup(){
	
}

void Bug::update(){
	
}

void Bug::draw(){
	ofPushStyle();
	
	ofSetColor(0, 255, 255, 255);
	ofSetLineWidth(4);
	//ofDrawCircle(_location.x, _location.y,
	//	_rotSize + 4.0f + (cos(ofGetElapsedTimef() * (4.0f + _pulseOffset))));
	ofDrawRectangle(
		_location.x - ((_rotSize + 4.0f) / 2.0f),
		_location.y - ((_rotSize + 4.0f) / 2.0f),
		_rotSize + 4.0f + (cos(ofGetElapsedTimef() * (4.0f + _pulseOffset))),
		_rotSize + 4.0f + (cos(ofGetElapsedTimef() * (4.0f + _pulseOffset))));
	
    // drawing Modules
    ofSetLineWidth(2);
	
	// Line color
	ofSetColor(255, 0, 255, 255);
	ofFill();
    //ofDrawEllipse(_location.x, _location.y, _rotSize, _rotSize);
	ofDrawRectangle(
		_location.x - (_rotSize / 2.0f),
		_location.y - (_rotSize / 2.0f),
		_rotSize,
		_rotSize);
	
    ofDrawLine(
		_location.x,
		_location.y,
		_location.x + (cos(_angle) * _rotSize * 2.5f),
		_location.y + (sin(_angle) * _rotSize * 2.5f));
	

    //drawing lasers
	ofSetColor(0, 255, 0, 255);
	
    if(_lightMode == "ON"){ // when it's on the corner, light is on
		for(int i = 0; i < _laserRange; ++i){
			ofSetColor(0, 255, 0,
				255 - (int)((float)i * 255.0f / (float)_laserRange));
			
			//ofDrawCircle(
			//	_location.x + cos(_angle)*i,
			//	_location.y + sin(_angle)*i, 2);
			ofDrawRectangle(
				_location.x + cos(_angle) * i,
				_location.y + sin(_angle) * i,
				2, 2);
		}
    }
	
	// reset detect count when finish to draw
    _detectCount = 0;
	
	ofPopStyle();
}

void Bug::selfblinking(){
	if((ofGetElapsedTimeMillis() + _shiftTime) % _loopTime < _blinkTime){
		_lightMode = "ON";
		if(_r > 0.5){
			_angle += _rSpeed;
		}else{
			_angle -= _rSpeed;
		}
		
		if(_soundFlag){
			ofxOscMessage msg;
			msg.setAddress("/collision");
			float xpos = ofMap(_location.x, 0.0f, (float)_sharedData->appWidth, -1.0f, 1.0f);
			//float xpos = map(location.x, 0, width, -1, 1);
			msg.addFloatArg(xpos);
			msg.addIntArg(440);
			//OscMessage msg = new OscMessage("/collision");
			
			//msg.add(xpos);
			// if(loopTime == 2000) msg.add(800);
			// else msg.add(1000);
			// msg.add(int(random(700,740)));
			//msg.add(440);
			//_sharedData->getOscSender()->sendMessage(msg);
			//osc.send(msg, supercollider);
			_soundFlag = false;
			_r = ofRandom(0.0f, 1.0f);
			//r = random(0, 1);
		}
		// println(lightMode);
	}else{
		_lightMode = "OFF";
		_soundFlag = true;
	}
}

void Bug::react(vector<Bug *> & bugs){

///for (Rotator other : rots) {
	for(unsigned int i = 0; i < bugs.size(); ++i){
		Bug * other = bugs[i];

		// calculate the distance
		if(_location != other->getLocation()){
			float distance = other->getLocation().distance(_location);
			// float distance = PVector.dist(other.location, location);
			// calculate the tolerance from distance and rotSize
			float tolerance = atan2(_rotSize, distance);
			//float tolerance = atan2(rotSize, distance);
			// calculate the angle of two rotators
			float collideAngle = atan2(
				other->getLocation().y - _location.y,
				other->getLocation().x - _location.x);
			//float collideAngle = atan2(other.location.y - location.y,
					//other.location.x - location.x);
			_capAngle = collideAngle;
			collideAngle = collideAngle + PI;
			//collideAngle = collideAngle + PI;  // because atan2 value is -PI ~ PI
			// compare the angle of other's laser and itself
			float diff = abs(collideAngle - other->getAngle());
			//float diff = abs(collideAngle - other.angle);
        // if(other.detectCount > 1 || other.turnMode != "OFF"){ 
			if(other->getLightMode() == "ON"){
          // when it's on the corner, light is on
          // if(other.detectCount > 1){ 
          // if (diff < tolerance || abs(diff - TWO_PI) < tolerance) {
				
				if(
				(diff < tolerance) ||
				((abs(diff - TWO_PI) < tolerance) &&
				(distance < (float)_laserRange))){
					_detectCount++;
					_shift = _shift + 5;
					_detectFlag = true;
            // if (detectFlag && !lastDetectFlag && lightMode == "OFF") {
					if(_detectFlag && !_lastDetectFlag){
						//most important line of code for SYNC
						_shiftTime =_shiftTime + (_loopTime - ((
							ofGetElapsedTimeMillis() + _shiftTime) % _loopTime));
						//shiftTime = shiftTime + (loopTime - ((millis() + shiftTime) % loopTime));
						_lastDetectFlag = true;
					}
				}else{
					_detectFlag = false;
					if(!_detectFlag && _lastDetectFlag){
						_lastDetectFlag = false;
					}
				}
			}
		}
	}
}

void Bug::collision(vector<Bug *> & bugs){
	for(unsigned int i = 0; i < bugs.size(); ++i){
	//for (Rotator other : rots) {
		Bug * other = bugs[i];
		if(_location != other->getLocation()){
		//if (location != other.location) { // to ignore itself
			float d = other->getLocation().distance(_location);
			//float d = PVector.dist(other.location, location); // calculate the distance to other modules
			if(d < _rotSize * 2.0f){
			//if (d < rotSize*2) {
				// calculate the angle of two mods
				float collideAngle = atan2(_location.y - other->getLocation().y,
					_location.x - other->getLocation().x);
				//float collideAngle = atan2(location.y - other.location.y,
				//	location.x - other.location.x);
				collideAngle = collideAngle + PI;
				if(_turnMode == "OFF"){
					if(_angle > PI){
						if((_angle - collideAngle) < PI && (_angle - collideAngle) > 0){
							_turnMode = "CW";
							_targetAngle = _angle + _sharedData->turnAmount;
						}else{
							_turnMode = "CCW";
							_targetAngle = _angle - _sharedData->turnAmount;
						}
					}else if(_angle < PI){
						if((collideAngle - _angle) < PI && (collideAngle - _angle) > 0){
							_turnMode = "CCW";
							_targetAngle = _angle - _sharedData->turnAmount;
						}else{
							_turnMode = "CW";
							_targetAngle = _angle + _sharedData->turnAmount;
						}
					}
				}
			}
		}
	}
}

void Bug::edgeDetection(){
	// reflection detect for X axis end
	if(_turnMode == "OFF"){
	//if (turnMode == "OFF") {
		// for right end
		if(_location.x > (900.0f - _rotSize)){
		//if (location.x > (width-(rotSize))) {
			if(_angle < TWO_PI && _angle > PI * 3.0f / 2.0f){
			//if (angle < TWO_PI && angle > PI*3/2) { // angle is down
				_turnMode = "CCW";
				_targetAngle = _angle - _sharedData->turnAmount;
			}else if(_angle > 0.0f && _angle < HALF_PI){
			//}else if(angle > 0 && angle < HALF_PI) { // angle is up
				_turnMode = "CW";
				_targetAngle = _angle + _sharedData->turnAmount;
			}
		}
		//for left end
		else if(_location.x < _rotSize){
		//else if (location.x < (rotSize)) {
			if(_angle > PI){ // angle is up
				_turnMode = "CW";
				_targetAngle = _angle + _sharedData->turnAmount;
			}else if(_angle < PI){ // angle is down
				_turnMode = "CCW";
				_targetAngle = _angle - _sharedData->turnAmount;
			}
		}
		// reflection detect for Y axis end
		// for bottom end
		if(_location.y > (600.0f - _rotSize)){
		//if (location.y > (height-(rotSize))) {
			// print("bot", angle, ' ');
			if(_angle > (HALF_PI)){ // angle is right
				_turnMode = "CW";
				_targetAngle = _angle + _sharedData->turnAmount;
          // println(turnMode);
			}else if(_angle < (HALF_PI)){ // angle is left
				_turnMode = "CCW";
				_targetAngle = _angle - _sharedData->turnAmount;
				// println(turnMode);
			}
		}
		//for top end
		else if(_location.y < _rotSize){
			// print("top", angle, ' ');
			if(_angle > PI + HALF_PI){ // angle is left
				_turnMode = "CW";
				_targetAngle = _angle + _sharedData->turnAmount;
				// println(turnMode);
			}else if(_angle < PI + HALF_PI) { // angle is right
				_turnMode = "CCW";
				_targetAngle = _angle - _sharedData->turnAmount;
				// println(turnMode);
			}
		}
      // println(angle, " ", targetAngle);
	  //ofLogNotice("Bug::edgeDetection") << "angle: " << _targetAngle;
    }
}

void Bug::movement(){

	//ofLogNotice("Bug::movement") << "_turnMode: " << _turnMode;

	// rotate and change direction
	if(_turnMode != "OFF"){
		if(_turnMode == "CW"){
			_angle += _rSpeed;
			// rotation finish
			if(_angle > _targetAngle){
				if (_angle < 0.0f){
					_angle = _angle + TWO_PI;
				}
				
				if(_angle > TWO_PI){
					_angle = _angle - TWO_PI;
				}

				// move a little to prevent detect end again
				_location.x = _location.x + cos(_angle) * _mSpeed * 2.0f;
				_location.y = _location.y + sin(_angle) * _mSpeed * 2.0f;
				_turnMode = "OFF";
			}
		}else if(_turnMode == "CCW"){
			_angle -= _rSpeed;
			//rotation finish
			if(_angle < _targetAngle){
				if(_angle < 0.0f){
					_angle = _angle + TWO_PI;
				}
				
				if(_angle > TWO_PI){
					_angle = _angle - TWO_PI;
				}

				// move a little to prevent detect end again
				_location.x = _location.x + cos(_angle) * _mSpeed * 2.0f;
				_location.y = _location.y + sin(_angle) * _mSpeed * 2.0f;
				_turnMode = "OFF";
			}
		}
    }else{
		_location.x = _location.x + cos(_angle) * _mSpeed;
		_location.y = _location.y + sin(_angle) * _mSpeed;
    }
}

void Bug::setBlinkTime(int time){
	_blinkTime = time;
}

} // namespace laserbugs
