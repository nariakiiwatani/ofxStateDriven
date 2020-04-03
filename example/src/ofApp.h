#pragma once

#include "ofMain.h"

#include "ofxStateDriven.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
private:
	int key_ = 0;
};
