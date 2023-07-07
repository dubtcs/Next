#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/log/Log.h>

#include <vector>

// exp == experimental
namespace nxt::exp
{

	// need to accept both class member method calls and global function calls

	void TEST_SIGNALS(int num);

	template<typename>
	class Event;

	template<typename RV, typename... param_types>
	class Event<RV(param_types...)>
	{
		using fn_ptr = RV(*)(param_types...);
	public:
		Event() = default;
		void Add(fn_ptr callback) { mCallbacks.push_back(callback); }
		void Fire(param_types... params) { for (fn_ptr ptr : mCallbacks) { ptr(params...); } }
	protected:
		std::vector<fn_ptr> mCallbacks;
	};

	/*template<typename T, typename... param_types>
	class Action<T(param_types...)>
	{
	public:
		Action(T(*function)(param_types...)) : mFunctionPointer{ function } {}
		T Fire(param_types... params) { return mFunctionPointer(params...); }
	protected:
		T(*mFunctionPointer)(param_types...);
	};

	template<typename>
	class Sender;

	template<typename T, typename... param_types>
	class Sender<T(param_types...)>
	{
		using fn_ptr = T(*)(param_types...);
	public:
		Sender() = default;
		void Add(fn_ptr fn) { mListeners.push_back(fn); };
		void Fire(param_types... params) { for (fn_ptr& ptr : mListeners) { ptr(params...); } }
	protected:
		std::vector<fn_ptr> mListeners;
	};

	template<typename>
	class Subcall;

	template<typename C, typename RV, typename... param_types>
	class Subcall<RV(C::*)(param_types...)>
	{
		using fn_ptr = RV(C::*)(param_types...);
	public:
		Subcall(C* obj, fn_ptr function) : mObject{ obj }, mFunction{ function } {}
	protected:
		C* mObject;
		fn_ptr mFunction;
	};*/

}
