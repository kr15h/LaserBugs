#include "Background.h"

namespace laserbugs{

Background::Background(){
	_numCols = 0;
	_numRows = 0;
	_colWidth = 0.0f;
	_rowHeight = 0.0f;
}

void Background::setup(){
	ofBackground(0);
	_numCols = 10;
	_numRows = 8;
	calcGrid();
}

void Background::update(){
	calcGrid();
}

void Background::draw(){
	ofPushStyle();
	for(float y = _rowHeight; y < ofGetHeight(); y += _rowHeight){
		ofSetColor(BACKGROUND_LINE_BRIGHTNESS);
		ofDrawLine(0, y, ofGetWidth(), y);
		for(float x = _colWidth; x < ofGetWidth(); x += _colWidth){
			if(y == _rowHeight){
				ofSetColor(BACKGROUND_LINE_BRIGHTNESS);
				ofDrawLine(x, 0, x, ofGetHeight());
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
	_colWidth = (float)ofGetWidth() / (float)_numCols;
	_rowHeight = (float)ofGetHeight() / (float)_numRows;
}

} // namespace laserbugs
