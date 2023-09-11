
#include "lerp.h"

namespace nxt
{

	// condense this into one equation that just uses modifiers
	float Lerp(float start, float stop, float time, nxtInterpolation style)
	{
		switch (style)
		{
			case(nxtInterpolation_Linear):
			{
				return start + (stop - start) * time;
			}
			case(nxtInterpolation_Exponential):
			{
				return start + (stop - start) * (time * time);
			}
		}
		return 0.f;
	}

}
