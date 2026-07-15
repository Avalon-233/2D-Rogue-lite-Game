#pragma once
#include <cmath>

//designed with AI assistance

inline float ResolveFacingScaleX(float directionX, float currentScaleX, bool textureFacesRight)
{
	if (directionX == 0.f)
		return currentScaleX;

	const float scaleMagnitude = std::abs(currentScaleX);
	const bool shouldFaceRight = directionX > 0.f;
	return shouldFaceRight == textureFacesRight ? scaleMagnitude : -scaleMagnitude;
}
