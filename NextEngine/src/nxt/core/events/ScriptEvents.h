#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/log/Log.h>

#include <map>
#include <functional>
#include <vector>

// exp == experimental
namespace nxt::exp
{

	template<typename>
	class NXT_API Connection;

	template<typename>
	class NXT_API Event2;

	template<typename RV, typename... args>
	class NXT_API Connection<RV(args...)>
	{
	public:
		Connection(const std::function<RV(args...)>& function) : mFunction{ function } {}
		Event2<RV(args...)> mEvent;
		std::function<RV(args...)> mFunction;
		void Disconnect()
		{
			mEvent.BreakConnection(this);
		}
	};

	template<typename RV, typename... args>
	class NXT_API Event2<RV(args...)>
	{
	public:
		Event2() = default;
		Connection<RV(args...)>& Connect(const std::initializer_list<Connection<RV(args...)>>& newConnection)
		{
			mConnections.push_back(newConnection);
			return mConnections.back();
		}
		bool BreakConnection(Connection<RV(args...)>& removal)
		{

		}
	protected:
		std::vector<Connection<RV(args...)>> mConnections;
		//std::unordered_map<Connection<RV(args...)>> mConnections;
	};

	// need to accept both class member method calls and global function calls

	/*void TEST_SIGNALS(int num);

	template<typename>
	class Subcall;

	template<typename RV, typename... args>
	class Subcall<RV(args...)>
	{
		Subcall();
	};

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
	};*/

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


/*

// Worth a shot lol
	// CRTP

	template<typename>
	class ASubcall;

	template<typename RV, typename... args>
	class ASubcall<RV(args...)>
	{
	public:
		virtual void Fire(args... params)
		{
			NXT_LOG_WARN("DEFAULT METHOD CALL");
		}
	};

	template<typename>
	class StaticSubcall;

	template<typename RV, typename... args>
	class StaticSubcall<RV(args...)> : public ASubcall<RV(args...)>
	{
	public:
		StaticSubcall(RV(*function)(args...)) : mFunction{ function } { }
		virtual void Fire(args... params) override { mFunction(params...); }
	protected:
		RV(*mFunction)(args...);
	};

	template<typename, typename>
	class MethodSubcall;

	template<typename C, typename RV, typename... args>
	class MethodSubcall<C, RV(args...)> : public ASubcall<RV(args...)>
	{
	public:
		MethodSubcall(C* object, RV(C::*function)(args...)) : mObject{ object }, mFunction{ function } {}
		virtual void Fire(args... params) override
		{
			NXT_LOG_CRIT("I chose the wrong week to quit smoking.");
			return (mObject->*mFunction)(params...);
		}
	protected:
		C* mObject;
		RV(C::*mFunction)(args...);
	};

*/