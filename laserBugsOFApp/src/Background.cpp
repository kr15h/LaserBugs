#include "Background.h"

namespace laserbugs{

Background::Background(){
	_numCols = 0;
	_numRows = 0;
	_colWidth = 0.0f;
	_rowHeight = 0.0f;
}

void Background::setup(){
	allocate(BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	ofBackground(0);
	_numCols = 16;
	_numRows = 9;
	calcGrid();
}

void Background::update(){
	calcGrid();
}

void Background::draw(){
	ofClear(0);

	ofPushStyle();
	for(float y = _rowHeight; y < getHeight(); y += _rowHeight){
		ofSetColor(BACKGROUND_LINE_BRIGHTNESS);
		ofDrawLine(0, y, getWidth(), y);
		for(float x = _colWidth; x < getWidth(); x += _colWidth){
			if(y == _rowHeight){
				ofSetColor(BACKGROUND_LINE_BRIGHTNESS);
				ofDrawLine(x, 0, x, getHeight());
			}
			ofSetColor(0);
			ofDrawCircle(x, y, BACKGROUND_DOT_SIZE + BACKGROUND_DOT_MARGIN);
			ofSetColor(BACKGROUND_DOT_BRIGHTNESS);
			ofDrawCircle(x, y, BACKGROUND_DOT_SIZE);
		}
	}
	ofPopStyle();
}

void Background::calcGrid(){
	_colWidth = (float)getWidth() / (float)_numCols;
	_rowHeight = (float)getHeight() / (float)_numRows;
}

void Background::setNumCols(unsigned int cols){
	_numCols = cols;
	calcGrid();
}

void Background::setNumRows(unsigned int rows){
	_numRows = rows;
	calcGrid();
}

} // namespace laserbugs
