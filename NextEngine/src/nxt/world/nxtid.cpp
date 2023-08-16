
#include "nxtid.h"

#include <nxt/core/utility/random.h>

namespace nxt
{

	/*static std::random_device gRandomDevice;
	static std::mt19937_64 gMtEngine{ gRandomDevice() };
	static std::uniform_int_distribution<nxtid> gDistribution;*/

	nxtid GenerateID()
	{
		return random::GetInt<nxtid>();
	}

}
