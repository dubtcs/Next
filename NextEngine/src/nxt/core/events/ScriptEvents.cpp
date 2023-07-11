
#include "ScriptEvents.h"

#include <nxt/core/log/Log.h>
#include <vector>

namespace nxt::exp
{

	void test1()
	{
		NXT_LOG_CRIT("Called Test1");
	}

	class testClass
	{
	public:
		testClass(int32_t n) : number{ n } {}
		void addTo(int32_t add)
		{
			NXT_LOG_DEBUG("I chose the  wrogn week to quit drinking.");
			this->number += add;
		}
	protected:
		int32_t number;
	};

	void TEST_SIGNALS()
	{

		

		//Event2<void()> ev2{};
		//ev2.Connect({ std::bind<void()>(&test1) });

		//StaticSubcall<void()> s11{ &test1 };
		////s11.Fire();

		//testClass c1{ 7 };
		//MethodSubcall<testClass, void()> m1{ &c1, &testClass::empty };
		////m1.Fire();

		//std::vector<ASubcall<void()>> subs{};
		//subs.push_back(s11);
		//subs.push_back(m1);

		//for (ASubcall<void()>& call : subs)
		//{
		//	call.Fire();
		//}

		/*exp::Event<void()> ev{};
		ev.Add(&test1);
		exp::Event<void(const testClass&)> ev2{};
		testClass j{ 56 };
		ev2.Add(&testClass::bruh);
		ev2.Fire(j);
		ev.Fire();*/

		/*exp::Action<int(int)> sig{ &t1 };
		NXT_LOG_TRACE(sig.Fire(param));

		exp::Action<void()> sig2{ &t2 };
		sig2.Fire();

		exp::Action<void(int, float, std::string)> sig3{ &t3 };
		sig3.Fire(1, 1.624f, "Does this work? IDK Halo 5: Guardians");

		exp::Sender<void(float)> updateSystem{};
		updateSystem.Add(&nxt::whatthe);
		updateSystem.Add(&nxt::gang);
		updateSystem.Fire(0.56f);*/

	}

}
