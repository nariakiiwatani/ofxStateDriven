#include "ofApp.h"

using namespace ofxStateDriven;
using StateIdType = std::string;
ofxStateDriven::Component<StateIdType> comp;
//--------------------------------------------------------------
void ofApp::setup(){
	StateIdType a_id="a",b_id="b",c_id="c";
	Behavior<StateIdType>
	a2b(
		Condition([this](){return prev_key_=='1';}) && Condition([this](){return key_=='2';}),
		[=](){ofLog()<<"a2b";return b_id;}
	),
	a2c(
		Condition([this](){return prev_key_=='1';}) && Condition([this](){return key_=='3';}),
		[=](){ofLog()<<"a2c";return c_id;}
	),
	b2a(
		Condition([this](){return prev_key_=='2';}) && Condition([this](){return key_=='1';}),
		[=](){ofLog()<<"b2a";return a_id;}
	),
	b2c(
		Condition([this](){return prev_key_=='2';}) && Condition([this](){return key_=='3';}),
		[=](){ofLog()<<"b2c";return c_id;}
	),
	c2a(
		Condition([this](){return prev_key_=='3';}) && Condition([this](){return key_=='1';}),
		[=](){ofLog()<<"c2a";return a_id;}
	),
	c2b(
		Condition([this](){return prev_key_=='3';}) && Condition([this](){return key_=='2';}),
		[=](){ofLog()<<"c2b";return b_id;}
	);
	State<StateIdType> a{a2b,a2c},b{b2a,b2c},c{c2a,c2b};
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
	prev_key_ = key_;
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
