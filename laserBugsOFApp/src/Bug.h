#pragma once

#include "ofMain.h"
#include "SharedData.h"

namespace laserbugs{

class Bug{
	public:
		Bug(
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
	
	private:
		shared_ptr<SharedData> _sharedData;
	
		string _lightMode;
	
		ofPoint _position;
		ofPoint _location;
	
		float _rotationSpeed;
		float _movementSpeed;
		float _rotSize;
		float _angle;
	
		int _eachLoopTime;
		int _shiftTime;
		int _blinkTime;
		int _laserRange;
		int _detectCount;
	
};

} // namespace laserbugs
