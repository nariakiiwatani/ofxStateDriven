#pragma once

#include "ofxStateDriven.h"

namespace ofxStateDriven {
class Condition
{
public:
	Condition(std::function<bool()> condition):condition_(condition){}
	bool operator()() const { return condition_(); }
	Condition operator&&(const Condition &c) {
		Condition me(condition_);
		return Condition([me,c](){return me()&&c();});
	}
	Condition operator||(const Condition &c) {
		Condition me(condition_);
		return Condition([me,c](){return me()||c();});
	}
protected:
	std::function<bool()> condition_;
};
template<typename CounterType>
class CountUp : public Condition
{
public:
	CountUp(CounterType &counter, CounterType limit)
	:Condition([&counter,limit] {
		return ++counter >= limit;
	}){};
};
template<typename CounterType>
class CountDown : public Condition
{
public:
	CountDown(CounterType &counter, CounterType limit)
	:Condition([&counter,limit] {
		return --counter <= limit;
	}){};
};
}
