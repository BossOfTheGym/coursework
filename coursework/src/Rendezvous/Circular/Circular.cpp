#include "Circular.h"

#include "ClohessyWitshire.h"


Circular::Circular(const SatelliteWeak& target, const SatelliteWeak& chaser)
	: Method(target, chaser)
	, mState(State::FINISHED)
	, mActions()
{
	mActions.reserve(DEFAULT_ACTIONS_CAPACITY);
}


void Circular::update(const Time& t, const Time& dt)
{
	if (mState == State::EXECUTING)
	{
		//necessary checks
		if (mTime > dt)
		{
			mTime -= dt;
		}

		while(true)
		{
			if (mActions.empty())
			{
				mState = State::FINISHED;
				break;
			}

			auto& action = mActions.back();

			action.update(t, dt);
			if (action.state() == ActionBase::State::FINISHED)
			{
				std::cout << "Finished action at time: " << t.asFloat() << std::endl;

				mActions.pop_back();
			}
			else
			{
				break;
			}
		}
	}
}

void Circular::start()
{
	//clear actions stack
	mState = State::EXECUTING;

	mActions.clear();

	//calculations
	using namespace ClohessyWitshire;
	using glm::inverse;
	using glm::dot;
	using glm::length;
	using glm::cross;
	using glm::transpose;


	auto target = mTarget.lock();
	auto chaser = mChaser.lock();
	if (!target || !chaser)
	{
		return;
	}

	//mean motion & time
	double n = target->mOrbit->meanMotion();
	double t = mTime.asFloat();

	//satellites parameters
	Vec3& rChaser = chaser->mPhysics->mPosition;
	Vec3& rTarget = target->mPhysics->mPosition;

	Vec3& vChaser = chaser->mPhysics->mVelocity;
	Vec3& vTarget = target->mPhysics->mVelocity;

	//energy
	Vec3 h = cross(rTarget, vTarget);

	//angular velocity
	Vec3 omega = h / dot(rTarget, rTarget);

	//transformation matrix
	Vec3 i = rTarget / length(rTarget);
	Vec3 k = h / length(h);
	Vec3 j = cross(k, i);
	Mat3 qi = Mat3(i, j, k);
	Mat3 q  = transpose(qi);

	//absolute
	Vec3 dr = rChaser - rTarget;
	Vec3 dv = vChaser - vTarget - cross(omega, dr);

	//relative
	Vec3 dr0 = q * dr;
	Vec3 dv0 = q * dv;

	Vec3 dvNew0 = -inverse(phi_rv(t, n)) * phi_rr(t, n) * dr0;
	Vec3 dvNew1 = phi_vr(t, n) * dr0 + phi_vv(t, n) * dvNew0;

	//actions
	Vec3 dvFirstBefore = (dvNew0 - dv0);
	Vec3 dvFirst  = qi * dvFirstBefore;
	

	Vec3 dvSecond = qi * (-dvNew1);

	std::cout << "First : x:" << dvFirst.x  << " y: " << dvFirst.y  << " z: " << dvFirst.z  << std::endl;
	std::cout << "Second: x:" << dvSecond.x << " y: " << dvSecond.y << " z: " << dvSecond.z << std::endl;

	mActions.push_back(Impuls(mChaser, -dvFirst));
	mActions.push_back(Wait(mTime));
	mActions.push_back(Impuls(mChaser, dvSecond));
}

void Circular::stop()
{
	mState = State::FINISHED;

	mActions.clear();
}

bool Circular::finished() const
{
	return mState == State::FINISHED;
}