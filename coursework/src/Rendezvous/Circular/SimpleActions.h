#pragma once

#include <Common.h>
#include <Objects/Satellite/Satellite.h>

#include "Action.h"

//forward decl
struct Satellite;
using SatelliteWeak   = std::weak_ptr<Satellite>;
using SatelliteShared = std::shared_ptr<Satellite>;


class Impuls : public ActionBase
{
public:
	Impuls(
		  const SatelliteWeak& chaser = SatelliteShared(nullptr)
		, const Vec3& impuls = Vec3()
	);

	Impuls(const Impuls& ab) = default;
	Impuls(Impuls&& ab)      = default;

	~Impuls() = default;

	Impuls& operator = (const Impuls& ab) = default;
	Impuls& operator = (Impuls&& ab)      = default;


public:
	void update(const Time& t, const Time& dt);


private:
	SatelliteWeak mChaser;
	Vec3          mImpuls;
};


class Wait : public ActionBase
{
public:
	Wait(const Time& wait = Time());

	Wait(const Wait& ab) = default;
	Wait(Wait&& ab)      = default;

	~Wait() = default;

	Wait& operator = (const Wait& ab) = default;
	Wait& operator = (Wait&& ab)      = default;


public:
	void update(const Time& t, const Time& dt);


private:
	Time mWait;
};



