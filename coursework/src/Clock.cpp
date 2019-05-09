#include "Clock.h"

#include <chrono>


uint64_t Clock::DEFAULT_DELTA = 100;
uint64_t Clock::DEFAULT_WARP = 1;
uint64_t Clock::DEFAULT_MIN_WARP = 1;
uint64_t Clock::DEFAULT_MAX_WARP = 20000;

double Clock::DEFAULT_DIVISOR = 1e6;


uint64_t now()
{
	return std::chrono::steady_clock::now().time_since_epoch().count();
}

Clock::Clock(
	  uint64_t dt0
	, uint64_t warp
	, uint64_t minWarp
	, uint64_t maxWarp
	, double divisor
)
	: m_start(now())
	, m_tRaw(m_start)
	, m_dtRaw(0)

	, m_dt0(dt0)

	, m_t()
	, m_dt()

	, m_tWarped()
	, m_dtWarped()

	, m_warp(warp)
	, m_warpMin(minWarp)
	, m_warpMax(maxWarp)

	, m_divisor(divisor)
{
}

void Clock::updateDelta()
{
	m_dtRaw = std::min(now() - m_tRaw, m_dt0);

	m_dt = Time(m_dtRaw, m_dtRaw / m_divisor);
}

void Clock::updateTime()
{
	m_tRaw += m_dtRaw;

	m_t = Time(m_tRaw, m_tRaw / m_divisor);
}

void Clock::updateDeltaWarped()
{
	uint64_t delta = m_dtRaw * m_warp;

	m_dtWarped = Time(delta, delta/ m_divisor);
}

void Clock::updateTimeWarped()
{
	uint64_t warped = m_tWarped.asU64() + m_dtWarped.asU64();

	m_tWarped = Time(warped, warped / m_divisor);
}
