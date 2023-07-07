
#include "ScriptEvents.h"

#include <nxt/core/log/Log.h>

namespace nxt
{

	void test1()
	{
		NXT_LOG_CRIT("Called Test1");
	}

	void exp::TEST_SIGNALS(int param)
	{

		exp::Event<void()> ev{};
		ev.Add(&test1);
		ev.Fire();

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
