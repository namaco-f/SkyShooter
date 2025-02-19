#pragma once

/// <summary>
/// •ª•bƒ~ƒŠ•b—p‚Ìî•ñ
/// </summary>
struct Time_Unit
{
	int minutes;
	int seconds;
	int milliSeconds;

	Time_Unit(void)
		:
		minutes(0),
		seconds(0),
		milliSeconds(0)
	{};

	Time_Unit(const float min, const float time, const float milliSecond)
		:
		minutes(static_cast<int>(min)),
		seconds(static_cast<int>(time)),
		milliSeconds(static_cast<int>(milliSecond * 100.0f))
	{};
};
