#pragma once

#include "ofMain.h"
#include "FboSource.h"
#include "SharedData.h"

#define BACKGROUND_BRIGHTNESS 0
#define BACKGROUND_INIT_COLS 16
#define BACKGROUND_INIT_ROWS 9
#define BACKGROUND_DOT_SIZE 2.0f
#define BACKGROUND_DOT_BRIGHTNESS 75
#define BACKGROUND_LINE_WIDTH 2.0f
#define BACKGROUND_LINE_BRIGHTNESS 75
#define BACKGROUND_DOT_MARGIN 2.0f // Black dots behind the light ones

namespace laserbugs{

class Background : public ofx::piMapper::FboSource{
	public:
		Background();
	
		void setup();
		void update();
		void draw();
	
		void calcGrid();
		void drawGrid();
		void drawCellNumbers();
	
		void setSharedData(shared_ptr<SharedData> sd);
		void setNumCols(unsigned int cols);
		void setNumRows(unsigned int rows);
	
	private:
		unsigned int _numCols;
		unsigned int _numRows;
	
		float _colWidth;
		float _rowHeight;
	
		shared_ptr<SharedData> _sharedData;
	
		ofTexture _gridTexture;
		ofTexture _cellNumbersTexture;
};

} // namespace laserbugs
