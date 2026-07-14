#include "../LightRogue/Facing.h"

#include <cassert>
#include <cmath>

int main()
{
	assert(ResolveFacingScaleX(10.f, 1.f, true) == 1.f);
	assert(ResolveFacingScaleX(-10.f, 1.f, true) == -1.f);
	assert(ResolveFacingScaleX(-10.f, 1.5f, false) == 1.5f);
	assert(ResolveFacingScaleX(10.f, 1.5f, false) == -1.5f);
	assert(ResolveFacingScaleX(0.f, -1.1f, true) == -1.1f);
	assert(std::abs(ResolveFacingScaleX(-10.f, -1.5f, true)) == 1.5f);
}
