#pragma once


#include <Common.h>

#include <Entity/Time.h>


class Clock
{
public:
	static uint64_t DEFAULT_DELTA;
	static uint64_t DEFAULT_WARP;
	static uint64_t DEFAULT_MIN_WARP;
	static uint64_t DEFAULT_MAX_WARP;

	static double DEFAULT_DIVISOR;


public:
	Clock(
		uint64_t dt0 = DEFAULT_DELTA
		, uint64_t warp = DEFAULT_WARP
		, uint64_t minWarp = DEFAULT_MIN_WARP
		, uint64_t maxWarp = DEFAULT_MAX_WARP
		, double divisor = DEFAULT_DIVISOR
	);

	Clock(const Clock& clock) = default;
	Clock(Clock&& clock) = default;

	Clock& operator = (const Clock& clock) = default;
	Clock& operator = (Clock&& clock) = default;


public:
	void updateDelta();

	void updateTime();

	void updateDeltaWarped();

	void updateTimeWarped();


	Time elapsed() const
	{
		return m_t;
	}

	Time delta() const
	{
		return m_dt;
	}

	Time elapsedWarped() const
	{
		return m_tWarped;
	}

	Time deltaWarped() const
	{
		return m_dtWarped;
	}


	uint64_t getWarp() const
	{
		return m_warp;
	}

	uint64_t getMinWarp() const
	{
		return m_warpMin;
	}

	uint64_t getMaxWarp() const
	{
		return m_warpMax;
	}

	double getDivisor() const
	{
		return m_divisor;
	}


	void setWarp(uint64_t warp)
	{
		m_warp = std::min(warp, m_warpMax);
		m_warp = std::max(warp, m_warpMin);
	}

	void setWarpMin(uint64_t warpMin)
	{
		m_warpMin = warpMin;
	}

	void setWarpMax(uint64_t warpMax)
	{
		m_warpMax = warpMax;
	}

	void setDivisor(double divisor)
	{
		m_divisor = divisor;
	}


private:
	uint64_t m_start; // start point
	uint64_t m_tRaw;  // current point(t0)
	uint64_t m_dtRaw; // delta t1 - t0

	uint64_t m_dt0;  // limit of delta

	Time m_t;
	Time m_dt;

	Time m_tWarped;
	Time m_dtWarped;

	uint64_t m_warp;
	uint64_t m_warpMin;
	uint64_t m_warpMax;

	double m_divisor;
};