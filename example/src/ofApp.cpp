#include "ofApp.h"

// for convenience
using namespace ofxStateDriven;
// you can use any type as state ID unless it can used as key_type of std::unordered_map
using StateIdType = std::string;
// Component is a type that has multiple states.
ofxStateDriven::Component<StateIdType> comp;
//--------------------------------------------------------------
void ofApp::setup(){
	// state IDs
	StateIdType a_id="a",b_id="b",c_id="c";
	// you can describe everything in constructor. 
	comp = Component<StateIdType> {
		{
			// here we code what to do while the state is "a"
			a_id,
			{
				// in a state, you can set one or more behaviors.
				// behaviors are defined as a pair of "Condition" and "Action"
				// they are checked in order.
				{
					// if the condition was true, the action goes.
					[this]{return key_=='b';},
					// if the action returned StateIdType, the state changes and following behaviors are skipped.
					[=]{
						ofLog() << "move from a to b";
						return b_id;
					}
				},
				// here is the second behavior.
				{
					[this]{return key_=='c';},
					// if the action returned nothing (or anything that is not convertible to StateIdType)
					// the action is executed but the state will not changes.
					// so latter behaviors will run as usual.
					[this]{
						ofLog() << "tryed to move from a to c but it's not implemented";
						key_ = 0;
					}
				}
			}
		},
		{
			// when the state is "b"
			b_id,
			{
				{
					// you can combine conditions using "||" or "&&" by wrapping each in ofxStateDriven::Condition
					Condition([this]{return key_=='a';}) || Condition([this]{return key_==OF_KEY_LEFT;}),
					[=]{
						ofLog() << "move from b to a";
						return a_id;
					}
				},
				{
					// yes of course you can do this. I know.
					[this]{return key_=='c' || key_==OF_KEY_RIGHT;},
					[]{
						ofLog() << "move from b to c";
						// "c" is not a string (it's actually const char[1])
						// but it is convertible to StateIdType(std::string in this example) so you can do this to move to state "c".
						return "c";
					}
				}
			}
		},
		{
			// state c
			c_id,
			{
				{
					// you can define behaviors by only action.
					[=]{
						if(key_=='a') {
							ofLog() << "move from c to a";
							return a_id;
						}
						if(key_=='b') {
							ofLog() << "move from c to b";
							return b_id;
						}
						// return this special ID to stay in current state.
						return StateID<StateIdType>::NO_CHANGE();
					}
				}
			}
		}
	};
	// you can set these callbacks for each state.
	comp.setEnterStateCallback(a_id, [](StateIdType from_id) {
		ofLog() << "enter a from " << from_id;
	});
	comp.setLeaveStateCallback(b_id, [](StateIdType to_id) {
		ofLog() << "leave b to " << to_id;
	});
	comp.setBeforeUpdateCallback(b_id, []() {
		ofLog() << "before update in b";
	});
	comp.setAfterUpdateCallback(b_id, []() {
		ofLog() << "after update in b";
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
	key_ = key;
}
