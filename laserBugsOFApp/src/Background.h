#pragma once

#include "ofMain.h"

#define BACKGROUND_DOT_SIZE 2.0f
#define BACKGROUND_DOT_BRIGHTNESS 75
#define BACKGROUND_LINE_WIDTH 2.0f
#define BACKGROUND_LINE_BRIGHTNESS 75
#define BACKGROUND_DOT_MARGIN 2.0f // Black dots behind the light ones

namespace laserbugs{

class Background{
	public:
		Background();
	
		void setup();
		void update();
		void draw();
	
		void calcGrid();
	
	private:
		unsigned int _numCols;
		unsigned int _numRows;
	
		float _colWidth;
		float _rowHeight;
};

} // namespace laserbugs