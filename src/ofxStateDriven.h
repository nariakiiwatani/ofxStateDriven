#pragma once

namespace ofxStateDriven {

using StateIdType = std::size_t;
namespace {
	static StateIdType NO_CHANGE=0;
	static StateIdType INVALID=0;
}

class Behavior
{
public:
	void setup(std::function<bool()> condition, std::function<StateIdType()> action) {
		condition_ = condition;
		action_ = action;
	}
	bool check() {
		return condition_();
	}
	StateIdType force() {
		return action_();
	}
	StateIdType run() {
		if(!check()) {
			return NO_CHANGE;
		}
		return force();
	}
private:
	std::function<bool()> condition_;
	std::function<StateIdType()> action_;
};
class State
{
public:
	void addBehavior(Behavior behavior) {
		behavior_.push_back(behavior);
	}
	StateIdType update() {
		for(auto &&b : behavior_) {
			auto next_state_id = b.run();
			if(next_state_id != NO_CHANGE) return next_state_id;
		}
		return NO_CHANGE;
	}
protected:
	std::vector<Behavior> behavior_;
};
class Component
{
public:
	void addState(StateIdType identifier, State state) {
		state_[identifier] = state;
		if(current_state_id_ == INVALID) {
			current_state_id_ = identifier;
		}
	}
	void update() {
		if(current_state_id_ == INVALID) return;
		auto next_state_id = state_[current_state_id_].update();
		if(next_state_id != NO_CHANGE) {
			auto found = state_.find(next_state_id);
			current_state_id_ = (found != std::end(state_) ? found->first : INVALID);
		}
	}
protected:
	std::map<StateIdType, State> state_;
	StateIdType current_state_id_=INVALID;
};
}
