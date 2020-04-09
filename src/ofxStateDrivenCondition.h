#pragma once

#include "ofxStateDriven.h"

namespace ofxStateDriven {
class Condition
{
public:
	Condition(std::function<bool()> condition=[]{return true;}):condition_(condition){}
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
class Counter : public Condition
{
public:
	template<typename StepType=CounterType>
	Counter(CounterType limit, CounterType count_from=0, StepType step=1) {
		auto counter = count_from;
		condition_ = [=]() mutable {
			counter += step;
			if(step > 0 ? counter >= limit : counter <= limit) {
				counter = count_from;
				return true;
			}
			return false;
		};
	};
};
}
