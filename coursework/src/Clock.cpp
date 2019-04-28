//#include "Clock.h"
//
//#include "Context.h"
//
//
//uint64_t Clock::DEFAULT_DELTA = 100;
//uint64_t Clock::DEFAULT_WARP = 1;
//uint64_t Clock::DEFAULT_MIN_WARP = 1;
//uint64_t Clock::DEFAULT_MAX_WARP = 1000;
//
//double Clock::DEFAULT_DIVISOR = 1e6;
//
//
//Clock::Clock(
//	  uint64_t delta   
//	, uint64_t warp    
//	, uint64_t minWarp 
//	, uint64_t maxWarp 
//	, double divisor   
//)
//	: mDelta0(delta, delta / divisor)
//	, mDelta(delta * warp, delta * warp / divisor)
//	, mCurrentTime(0, 0.0)
//	, mAccumulated(0, 0.0)
//	, mWarp(warp)
//	, mMinWarp(minWarp)
//	, mMaxWarp(maxWarp)
//	, mDivisor(divisor)
//{
//	reset();
//}
//
//
//Time Clock::time() const
//{
//	return mCurrentTime;
//}
//
//Time Clock::accumulated() const
//{
//	return mAccumulated;
//}
//
//Time Clock::delta0() const
//{
//	return mDelta0;
//}
//
//Time Clock::delta() const
//{
//	return mDelta;
//}
//
//
//double Clock::divisor() const
//{
//	return mDivisor;
//}
//
//
//uint64_t Clock::warp() const
//{
//	return mWarp;
//}
//
//uint64_t Clock::minWarp() const
//{
//	return mMinWarp;
//}
//
//uint64_t Clock::maxWarp() const
//{
//	return mMaxWarp;
//}
//
//
//void Clock::setWarp(uint64_t warp)
//{
//	mWarp  = std::max(std::min(warp, mMaxWarp), mMinWarp);
//	mDelta = Time(mDelta0.asU64() * mWarp, mDelta0.asU64() * mWarp / divisor);
//}
//
//void Clock::tick()
//{
//	mT1 = glfwGetTimerValue();
//
//
//	uint64_t deltaRaw = (mT1 - mT0) * warp;
//	uint64_t accum = mAccumulated.asU64() + deltaRaw;
//	accum = std::min(accum, mAccumLimit);
//	accum = std::min(accum, mDelta0.asU64() * warp)
//
//
//
//
//	mT0 = mT1;
//}
//
//void Clock::reset()
//{
//	mT0 = glfwGetTimerValue();
//	mT1 = mT0;
//}