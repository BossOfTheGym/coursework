#pragma once

#include <Common.h>

#include <Entity/ITimeVarying.h>

#include <Objects/Satellite/Satellite.h>
#include <Objects/Planet/Planet.h>


class Method : public ITimeVarying
{
public:
	Method(
		  const SatelliteWeak& target = SatelliteShared(nullptr)
		, const SatelliteWeak& chaser = SatelliteShared(nullptr)
	);

	Method(const Method& method) = delete;
	Method(Method&& method)      = default;

	virtual ~Method() = default;

	Method& operator = (const Method& method) = delete;
	Method& operator = (Method&& method)      = default;


public:
	virtual void start();

	virtual void stop();

	virtual void update(const Time& t, const Time& dt) override;

	virtual bool finished() const;


public:
	void setTarget(const SatelliteWeak& target);

	void setChaser(const SatelliteWeak& chaser);

	void setTime(const Time& t);


	SatelliteWeak getTarget() const;

	SatelliteWeak getChaser() const;

	Time getTime() const;


protected:
	SatelliteWeak mTarget;
	SatelliteWeak mChaser;

	Time mTime;
};


//aliases
using MethodShared = std::shared_ptr<Method>;
using MethodWeak   = std::weak_ptr<Method>;
using MethodUnique = std::unique_ptr<Method>;