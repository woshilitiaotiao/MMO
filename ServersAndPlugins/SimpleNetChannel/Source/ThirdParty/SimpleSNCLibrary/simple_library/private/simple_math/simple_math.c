#include "../../public/simple_math/simple_math.h"

unsigned int get_uint32_random(unsigned int in_max)
{
	if (in_max > 0)
	{
		return rand() / (double)RAND_MAX * (in_max - 0) + 0;
	}

	return 0;
}
