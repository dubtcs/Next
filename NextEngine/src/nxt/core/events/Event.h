#pragma once

#include <nxt/EngineCore.h>

#include <vector>
#include <functional>

#include <iostream>

namespace nxt
{

	namespace types
	{
		enum EventType
		{
			None = BIT(0),
			Mouse = BIT(1),
			MouseMove = BIT(2),
			Keyboard = BIT(3),
			WindowResize = BIT(4),
			WindowClose = BIT(5)
		};
	}

	namespace events
	{

		// listener
		class NXT_API Event
		{
		public:
			virtual types::EventType GetType() const = 0;
			virtual const char* GetName() const = 0;
			types::EventType Type;
			bool Handled{ false };
			static const types::EventType StaticType;
		};

		// dispatcher
		class NXT_API Handler
		{
		public:
			template<typename T, typename F> // Event Type, Function Lambda Type
			bool Fire(const F& fn)
			{
				static_assert(std::is_base_of<Event, T>::value, "T must derive from Event");
				if (mEvent.GetType() & T::StaticType)
				{
					mEvent.Handled = fn(static_cast<T&>(mEvent));
					return true;
				}
				return false;
			}
		public:
			Handler(Event& newEvent) : mEvent{ newEvent } {};
			~Handler() {};
		private:
			Event& mEvent;
		};

		// specific types

		class NXT_API KeyboardPressed : public Event
		{
		public:
			types::EventType Type{ types::EventType::Keyboard };
			uint32_t Keycode;

			virtual types::EventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "KEYBOARD"; }

			static const types::EventType StaticType{ types::EventType::Keyboard };
		};

		class NXT_API MouseButtonPressed : public Event
		{
		public:
			types::EventType Type{ types::EventType::Mouse };
			uint32_t Keycode;

			virtual types::EventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "MOUSE"; }

			static const types::EventType StaticType{ types::EventType::Mouse };
		};

		class NXT_API WindowResized : public Event
		{
		public:
			types::EventType Type{ types::EventType::WindowResize };
			uint32_t Width, Height;

			virtual types::EventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "WINDOW_RESIZE"; }

			static const types::EventType StaticType{ types::EventType::WindowResize };
			WindowResized(uint32_t x, uint32_t y) : Width{ x }, Height{ y } {};
		};

		class NXT_API WindowClosed : public Event
		{
		public:
			types::EventType Type{ types::EventType::WindowClose };

			virtual types::EventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "WINDOW_CLOSED"; }

			static const types::EventType StaticType{ types::EventType::WindowClose };
		};

	}

}