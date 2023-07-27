#pragma once

#include <nxt/EngineCore.h>

#include "EventEnums.h"

#include <nxt/core/input/InputEnums.h>

#include <vector>
#include <functional>
#include <filesystem>

#include <iostream>

namespace nxt
{

	namespace events
	{

		// listener
		class NXT_API Event
		{
		public:
			virtual nxtEventType GetType() const = 0;
			virtual const char* GetName() const = 0;
			nxtEventType Type;
			bool Handled{ false };
			static const nxtEventType StaticType;
		};

		/*template<typename T>
		concept EnumDerivation = std::is_enum_v<nxtEventType>;

		template<EnumDerivation E>
		class NXT_API Event2
		{
		public:
			nxtEventType GetType() { return Type; }
		public:
			nxtEventType Type{ E };
			bool Handled{ false };
		};*/

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
			nxtEventType Type{ nxtEventType_Keyboard };
			nxtKeycode Keycode;

			virtual nxtEventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "KEYBOARD"; }

			static const nxtEventType StaticType{ nxtEventType_Keyboard };
			KeyboardPressed(nxtKeycode keycode) : Keycode{ keycode } {}
		};

		class NXT_API MouseButtonPressed : public Event
		{
		public:
			nxtEventType Type{ nxtEventType_MousePressed };
			uint32_t Keycode;
			bool IsDoubleClick;

			virtual nxtEventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "MOUSE"; }

			static const nxtEventType StaticType{ nxtEventType_MousePressed };
			MouseButtonPressed(nxtKeycode keycode, bool isDoubleClick) : Keycode{ keycode }, IsDoubleClick{ isDoubleClick } {}
		};

		class NXT_API MouseButtonReleased : public Event
		{
		public:
			nxtEventType Type{ nxtEventType_MouseReleased };
			uint32_t Keycode;
			//bool IsDoubleClick;

			virtual nxtEventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "MOUSE"; }

			static const nxtEventType StaticType{ nxtEventType_MouseReleased };
			MouseButtonReleased(nxtKeycode keycode) : Keycode{ keycode } {}
		};

		class NXT_API MouseScroll : public Event
		{
		public:
			static const nxtEventType StaticType{ nxtEventType_MouseScroll };
			nxtEventType Type{ StaticType };

			float delta;

			virtual nxtEventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "MOUSE"; }

			MouseScroll(float delta) : delta{ delta } {}
		};

		class NXT_API WindowResized : public Event
		{
		public:
			nxtEventType Type{ nxtEventType_WindowResize };
			uint32_t Width, Height;

			virtual nxtEventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "WINDOW_RESIZE"; }

			static const nxtEventType StaticType{ nxtEventType_WindowResize };
			WindowResized(uint32_t x, uint32_t y) : Width{ x }, Height{ y } {};
		};

		class NXT_API WindowClosed : public Event
		{
		public:
			nxtEventType Type{ nxtEventType_WindowClose };

			virtual nxtEventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "WINDOW_CLOSED"; }

			static const nxtEventType StaticType{ nxtEventType_WindowClose };
		};

		class NXT_API DragFileReceived : public Event
		{
		public:
			nxtEventType Type{ nxtEventType_DragReceived };
			std::filesystem::path Path;

			virtual nxtEventType GetType() const override { return Type; }
			virtual const char* GetName() const override { return "DRAG_FILE_RECEIVED"; }

			static const nxtEventType StaticType{ nxtEventType_DragReceived };
			DragFileReceived(const std::filesystem::path& filepath) : Path{ filepath } {}
		};

	}

}