//#pragma once
//
//
//#include <Common.h>
//
//#include <Entity/Time.h>
//
//
//class Clock
//{
//public:
//	static uint64_t DEFAULT_DELTA;
//	static uint64_t DEFAULT_WARP;
//	static uint64_t DEFAULT_MIN_WARP;
//	static uint64_t DEFAULT_MAX_WARP;
//
//	static double DEFAULT_DIVISOR;
//
//
//public:
//	Clock(
//		  uint64_t delta   = DEFAULT_DELTA
//		, uint64_t warp    = DEFAULT_WARP
//		, uint64_t minWarp = DEFAULT_MIN_WARP
//		, uint64_t maxWarp = DEFAULT_MAX_WARP
//		, double divisor   = DEFAULT_DIVISOR
//	);
//
//	Clock(const Clock& clock) = default;
//	Clock(Clock&& clock)      = default;
//
//	Clock& operator = (const Clock& clock) = default;
//	Clock& operator = (Clock&& clock)      = default;
//
//
//public:
//	Time time() const;
//
//	Time accumulated() const;
//
//	Time delta() const;
//
//	Time delta0() const;
//
//
//	double divisor() const;
//
//
//	uint64_t warp() const;
//
//	uint64_t minWarp() const;
//
//	uint64_t maxWarp() const;
//
//
//	void setWarp(uint64_t warp);
//
//
//	void tick();
//
//	void reset();
//
//
//private:
//	Time mCurrentTime;
//	Time mAccumulated;
//	Time mDelta0;
//	Time mDelta;
//
//	uint64_t mT0;
//	uint64_t mT1;
//
//	uint64_t mWarp;
//	uint64_t mMinWarp;
//	uint64_t mMaxWarp;
//
//	double mDivisor;
//};