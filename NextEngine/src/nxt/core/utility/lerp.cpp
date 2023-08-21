
#include "lerp.h"

namespace nxt
{

	// condense this into one equation that just uses modifiers
	float Lerp(float start, float stop, float time, nxtEasingStyle style)
	{
		switch (style)
		{
			case(nxtEasingStyle_Linear):
			{
				return start + (stop - start) * time;
			}
			case(nxtEasingStyle_Exponential):
			{
				return start + (stop - start) * (time * time);
			}
		}
		return 0.f;
	}

}
