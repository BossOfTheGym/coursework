#pragma once


#include <Common.h>

#include <Objects/Satellite/Satellite.h>
#include <Objects/Planet/Planet.h>

#include <Entity/ITimeVarying.h>

#include "Method.h"


class RendezvousControl : public ITimeVarying
{
public:
	RendezvousControl(
		  MethodUnique&& method = nullptr
		, const Time& t = Time()
	);

	RendezvousControl(const RendezvousControl& rc) = delete;
	RendezvousControl(RendezvousControl&& rc)      = default;
	
	~RendezvousControl() = default;

	RendezvousControl& operator = (const RendezvousControl& rc) = delete;
	RendezvousControl& operator = (RendezvousControl&& rc)      = default;


public:
	virtual void update(const Time& t, const Time& dt) override;


public:
	void start();

	void stop();


public:
	void setTarget(const SatelliteWeak& target);

	void setChaser(const SatelliteWeak& chaser);

	void setTime(const Time& t);


	SatelliteWeak getTarget() const;

	SatelliteWeak getChaser() const;

	Time getTime() const;


private:
	std::unique_ptr<Method> mMethod;
};


//aliases
using RendezvousControlShared = std::shared_ptr<RendezvousControl>;
using RendezvousControlWeak   = std::weak_ptr<RendezvousControl>;