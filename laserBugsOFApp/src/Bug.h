#pragma once

#include "ofMain.h"
#include "SharedData.h"
#include "ofxOsc.h"

namespace laserbugs{

class Bug{
	public:
		Bug(
			ofPoint location,
			float rotationSpeed,
			float movementSpeed,
			int loopTime,
			int shiftTime,
			int blinkTime,
			int laserRange){
			
			_sharedData = 0;
			_soundFlag = true;
			_location = location;
			_rotationSpeed = rotationSpeed;
			_movementSpeed = movementSpeed;
			_loopTime = loopTime;
			_shiftTime = shiftTime;
			_blinkTime = blinkTime;
			_laserRange = laserRange;
			
			// TODO: use enum's here
			_turnMode = "OFF";
		}
	
		//(new Rotator(xpos, ypos, rotateSpeed, moveSpeed, eachloopTime, shiftTime, blinkTime, laserRange)
	
		void setup();
		void update();
		void draw();
	
		void selfblinking();
		void react(vector<Bug *> & bugs);
		void collision(vector<Bug *> & bugs);
		void edgeDetection();
		void movement();
	
		void setSharedData(shared_ptr<SharedData> sd);
		void setBlinkTime(int time);

		string getLightMode(){ return _lightMode; }

		ofPoint getLocation(){ return _location; }
	
		float getAngle(){ return _angle; }
	
	private:
		shared_ptr<SharedData> _sharedData;
	
		string _lightMode;
		string _turnMode;
	
		ofPoint _location;
	
		bool _soundFlag;
		bool _detectFlag;
		bool _lastDetectFlag;
	
		float _rotationSpeed;
		float _movementSpeed;
		float _rotSize;
		float _angle;
		float _rSpeed; // Rotation speed?
		float _mSpeed; // Movement speed?
		float _r;
		float _capAngle;
		float _targetAngle;
	
		int _loopTime;
		int _shiftTime;
		int _blinkTime;
		int _laserRange;
		int _detectCount;
		int _shift;
	
};

} // namespace laserbugs
