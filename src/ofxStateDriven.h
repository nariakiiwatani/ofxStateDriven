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
	Behavior()=default;
	template<typename Action>
	Behavior(std::function<bool()> condition, Action &&action)
	:condition_(condition)
	,action_(wrapper<decltype(declval<Action>()())>::wrap(action))
	{}
	inline bool check() { return condition_(); }
	inline StateIdType force() { return action_(); }
	inline StateIdType run() { return check() ? force() : StateID<StateIdType>::NO_CHANGE(); }
private:
	std::function<bool()> condition_;
	std::function<StateIdType()> action_;

	template<typename T, typename SFINAE=void>
	struct wrapper {
		static std::function<StateIdType()> wrap(std::function<T()> f) {
			return [f]{f();return StateID<StateIdType>::NO_CHANGE();};
		}
	};
	template<typename T>
	struct wrapper<T, typename std::enable_if<std::is_convertible<T, StateIdType>::value>::type> {
		static std::function<StateIdType()> wrap(std::function<T()> f) {
			return f;
		}
	};
};

template<typename StateIdType=std::size_t>
class State
{
using BehaviorType = Behavior<StateIdType>;
public:
	State()=default;
	State(std::initializer_list<BehaviorType> behaviors)
	:behavior_(behaviors.begin(), behaviors.end())
	{
	}
	inline void addBehavior(BehaviorType behavior) { behavior_.push_back(behavior); }
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
	Component()=default;
	Component(std::initializer_list<std::pair<StateIdType, StateType>> init)
	:state_(init.begin(), init.end())
	,current_state_id_(state_.begin()->first)
	{
	}
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
		auto from_id = current_state_id_;
		auto callback = callback_[from_id];
		callback.before_update();
		auto next_id = found->second.update();
		callback.after_update();
		if(next_id != StateID<StateIdType>::NO_CHANGE()) {
			auto found = state_.find(next_id);
			if(found == std::end(state_)) {
				return;
			}
			callback.leave_to(next_id);
			current_state_id_ = next_id;
			callback_[next_id].enter_from(from_id);
		}
	}
	void setBeforeUpdateCallback(StateIdType identifier, std::function<void()> callback) { callback_[identifier].before_update = callback; }
	void setAfterUpdateCallback(StateIdType identifier, std::function<void()> callback) { callback_[identifier].after_update = callback; }
	void setEnterStateCallback(StateIdType identifier, std::function<void(StateIdType)> callback) { callback_[identifier].enter_from = callback; }
	void setLeaveStateCallback(StateIdType identifier, std::function<void(StateIdType)> callback) { callback_[identifier].leave_to = callback; }
protected:
	std::map<StateIdType, StateType> state_;
	StateIdType current_state_id_=StateID<StateIdType>::INVALID();
	struct Callback {
		std::function<void()> before_update=[]{};
		std::function<void()> after_update=[]{};
		std::function<void(StateIdType)> enter_from=[](StateIdType){};
		std::function<void(StateIdType)> leave_to=[](StateIdType){};
	};
	std::map<StateIdType, Callback> callback_;
};
}
