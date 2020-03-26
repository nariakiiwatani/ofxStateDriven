#include "ofApp.h"

using namespace ofxStateDriven;
ofxStateDriven::Component comp;
//--------------------------------------------------------------
void ofApp::setup(){
	StateIdType a_id=1,b_id=2,c_id=3;
	Behavior
	a2b([this](){return key_=='1';},[=](){ofLog()<<"a2b";return b_id;}),
	a2c([this](){return key_=='2';},[=](){ofLog()<<"a2c";return c_id;}),
	b2a([this](){return key_=='3';},[=](){ofLog()<<"b2a";return a_id;}),
	b2c([this](){return key_=='4';},[=](){ofLog()<<"b2c";return c_id;}),
	c2a([this](){return key_=='5';},[=](){ofLog()<<"c2a";return a_id;}),
	c2b([this](){return key_=='6';},[=](){ofLog()<<"c2b";return b_id;});
	State a{a2b,a2c},b{b2a,b2c},c{c2a,c2b};
	comp.addState(a_id,a);
	comp.addState(b_id,b);
	comp.addState(c_id,c);
}

//--------------------------------------------------------------
void ofApp::update(){
	comp.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	key_ = key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
