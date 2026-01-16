#pragma once

namespace MathUtils
{
	float Lerp(float start, float end, float delta)
	{
		return start * (1.0f - delta) + end * delta;
	}
}
