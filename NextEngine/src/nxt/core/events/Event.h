#pragma once

#include <nxt/EngineCore.h>

#include "EventEnums.h"

#include <nxt/core/input/InputEnums.h>

#include <vector>
#include <functional>

#include <iostream>

namespace nxt
{

	namespace events
	{

		// listener
		class NXT_API Event
		{
		public:
			virtual EVENT_TYPE_ GetType() const = 0;
			virtual const char* GetName() const = 0;
			EVENT_TYPE_ Type;
			bool Handled{ false };
			static const EVENT_TYPE_ StaticType;
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
			EVENT_TYPE_ Type{ EVENT_TYPE_KEYBOARD };
			input::KEYCODE_ Keycode;

			virtual EVENT_TYPE_ GetType() const override { return Type; }
			virtual const char* GetName() const override { return "KEYBOARD"; }

			static const EVENT_TYPE_ StaticType{ EVENT_TYPE_KEYBOARD };
			KeyboardPressed(input::KEYCODE_ keycode) : Keycode{ keycode } {}
		};

		class NXT_API MouseButtonPressed : public Event
		{
		public:
			EVENT_TYPE_ Type{ EVENT_TYPE_MOUSE_PRESSED };
			uint32_t Keycode;
			bool IsDoubleClick;

			virtual EVENT_TYPE_ GetType() const override { return Type; }
			virtual const char* GetName() const override { return "MOUSE"; }

			static const EVENT_TYPE_ StaticType{ EVENT_TYPE_MOUSE_PRESSED };
			MouseButtonPressed(input::KEYCODE_ keycode, bool isDoubleClick) : Keycode{ keycode }, IsDoubleClick{ isDoubleClick } {}
		};

		class NXT_API MouseButtonReleased : public Event
		{
		public:
			EVENT_TYPE_ Type{ EVENT_TYPE_MOUSE_RELEASED };
			uint32_t Keycode;
			//bool IsDoubleClick;

			virtual EVENT_TYPE_ GetType() const override { return Type; }
			virtual const char* GetName() const override { return "MOUSE"; }

			static const EVENT_TYPE_ StaticType{ EVENT_TYPE_MOUSE_RELEASED };
			MouseButtonReleased(input::KEYCODE_ keycode) : Keycode{ keycode } {}
		};

		class NXT_API WindowResized : public Event
		{
		public:
			EVENT_TYPE_ Type{ EVENT_TYPE_WINDOW_RESIZE };
			uint32_t Width, Height;

			virtual EVENT_TYPE_ GetType() const override { return Type; }
			virtual const char* GetName() const override { return "WINDOW_RESIZE"; }

			static const EVENT_TYPE_ StaticType{ EVENT_TYPE_WINDOW_RESIZE };
			WindowResized(uint32_t x, uint32_t y) : Width{ x }, Height{ y } {};
		};

		class NXT_API WindowClosed : public Event
		{
		public:
			EVENT_TYPE_ Type{ EVENT_TYPE_WINDOW_CLOSE };

			virtual EVENT_TYPE_ GetType() const override { return Type; }
			virtual const char* GetName() const override { return "WINDOW_CLOSED"; }

			static const EVENT_TYPE_ StaticType{ EVENT_TYPE_WINDOW_CLOSE };
		};

	}

}