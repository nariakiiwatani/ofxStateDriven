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
template<typename CounterType>
using Increment = Counter<CounterType>;
template<typename CounterType>
class Decrement : public Condition
{
public:
	Decrement(CounterType &counter, CounterType limit)
	:Condition([&counter,limit] {
		return --counter <= limit;
	}){};
};
}
