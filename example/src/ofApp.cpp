#include "ofApp.h"

using namespace ofxStateDriven;
using StateIdType = std::string;
ofxStateDriven::Component<StateIdType> comp;
//--------------------------------------------------------------
void ofApp::setup(){
	StateIdType a_id="a",b_id="b",c_id="c";
	comp = Component<StateIdType> {
		{
			a_id,
			{
				{
					Condition([this](){return prev_key_=='1';}) && Condition([this](){return key_=='2';}),
					[](){}
				},
				{
					[=]{
						if(prev_key_=='1' && key_=='3') {
							ofLog()<<"a2c";
							return c_id;
						}
						return StateID<StateIdType>::NO_CHANGE();
					}
				}
			}
		},
		{
			b_id,
			{
				{
					Condition([this](){return prev_key_=='2';}) && Condition([this](){return key_=='1';}),
					[=](){ofLog()<<"b2a";return a_id;}
				},
				{
					Condition([this](){return prev_key_=='2';}) && Condition([this](){return key_=='3';}),
					[=](){ofLog()<<"b2c";return c_id;}
				}
			}
		},
		{
			c_id,
			{
				{
					Condition([this](){return prev_key_=='3';}) && Condition([this](){return key_=='1';}),
					[=](){ofLog()<<"c2a";return a_id;}
				},
				{
					Condition([this](){return prev_key_=='3';}) && Condition([this](){return key_=='2';}),
					[=](){ofLog()<<"c2b";return b_id;}
				}
			}
		}
	};
	comp.setEnterStateCallback(a_id, [](StateIdType from_id) {
		ofLog() << "from " << from_id << " to a";
	});
	comp.setBeforeUpdateCallback(b_id, []() {
		ofLog() << "now in b";
	});
	comp.setLeaveStateCallback(b_id, [](StateIdType to_id) {
		ofLog() << "leave b to " << to_id;
	});
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
