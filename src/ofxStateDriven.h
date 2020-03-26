#pragma once

namespace ofxStateDriven {

template<typename ValueType>
struct StateID {
	using value_type = ValueType;
	static constexpr value_type NO_CHANGE() { return value_type(); }
	static constexpr value_type INVALID() { return value_type(); }
};

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
template<typename StateIdType=std::size_t>
class Behavior
{
public:
	Behavior(std::function<bool()> condition, std::function<StateIdType()> action)
	:condition_(condition)
	,action_(action)
	{
	}
	bool check() {
		return condition_();
	}
	StateIdType force() {
		return action_();
	}
	StateIdType run() {
		if(!check()) {
			return StateID<StateIdType>::NO_CHANGE();
		}
		return force();
	}
private:
	std::function<bool()> condition_;
	std::function<StateIdType()> action_;
};

template<typename StateIdType=std::size_t>
class State
{
using BehaviorType = Behavior<StateIdType>;
public:
	State(std::initializer_list<BehaviorType> behaviors)
	:behavior_(behaviors.begin(), behaviors.end())
	{
	}
	void addBehavior(BehaviorType behavior) {
		behavior_.push_back(behavior);
	}
	StateIdType update() {
		for(auto &&b : behavior_) {
			auto next_state_id = b.run();
			if(next_state_id != StateID<StateIdType>::NO_CHANGE()) return next_state_id;
		}
		return StateID<StateIdType>::NO_CHANGE();
	}
protected:
	std::vector<BehaviorType> behavior_;
};

template<typename StateIdType=std::size_t>
class Component
{
using StateType = State<StateIdType>;
public:
	void addState(StateIdType identifier, StateType state) {
		state_.insert(make_pair(identifier, state));
		if(current_state_id_ == StateID<StateIdType>::INVALID()) {
			current_state_id_ = identifier;
		}
	}
	void update() {
		if(current_state_id_ == StateID<StateIdType>::INVALID()) return;
		auto found = state_.find(current_state_id_);
		if(found == std::end(state_)) {
			return;
		}
		auto next_state_id = found->second.update();
		if(next_state_id != StateID<StateIdType>::NO_CHANGE()) {
			auto found = state_.find(next_state_id);
			current_state_id_ = (found != std::end(state_) ? found->first : StateID<StateIdType>::INVALID());
		}
	}
protected:
	std::map<StateIdType, StateType> state_;
	StateIdType current_state_id_=StateID<StateIdType>::INVALID();
};
}
