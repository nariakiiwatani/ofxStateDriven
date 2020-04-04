#pragma once

#include "ofxStateDriven.h"

namespace ofxStateDriven {
template<typename CounterType>
class Counter : public Condition
{
public:
	Counter(CounterType &counter, CounterType limit)
	:Condition([&counter,limit] {
		return ++counter >= limit;
	}){};
};
}
