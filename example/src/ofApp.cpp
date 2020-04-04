#include "ofApp.h"
#include "ofxStateDrivenCondition.h"

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
				// they are checked in order of addition.
				// a behavior is defined as a pair of "Condition" and "Action"
				{
					// if the condition was true, the action goes.
					[this]{return key_=='b';},
					// the simplest action is only a StateIdType.
					// the state will change and following behaviors are skipped.
					b_id
				},
				// here is the second behavior.
				{
					[this]{return key_=='c';},
					// an action can also be defined as a function.
					// if the action returned nothing (or anything that is "not" convertible to StateIdType)
					// the action is executed but the state will not change.
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
						// "c" is not a string (it's actually const char[2])
						// but it is convertible to StateIdType(std::string in this example) so you can do this to change the state to "c".
						return "c";
					}
				}
			}
		},
		{
			// state "c"
			c_id,
			{
				{
					// you can define behaviors by only an action.
					[=]{
						if(key_=='a') {
							ofLog() << "move from c to a";
							return a_id;
						}
						if(key_=='b') {
							ofLog() << "move from c to b";
							return b_id;
						}
						// return this special value to stay in current state.
						return StateID<StateIdType>::NO_CHANGE();
					}
				},
				{
					// there are some pre-defined conditions (of course you can make your own)
					// this will increment "key_" every frame and check it is more than 200 or not
					Counter<int>(key_, 200),
					"a"
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
