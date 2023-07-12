
#include "random.h"

namespace nxt::random
{

	std::random_device nxtgRandomDevice;
	std::mt19937_64 nxtgMtEngine{ nxtgRandomDevice() };

}
