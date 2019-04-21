#include "RendezvousComponent.h"


//constructors
RendezvousComponent::RendezvousComponent(
	IComponent* parent
	, const SatelliteWeak& target
)
	: IComponent(parent)
	, mChaser(dynamic_cast<Satellite*>(parent))
	, mTarget(target)
	, mActions()
{
	mActions.reserve(DEFAULT_CAPACITY);
}


//overrides
const IComponent::Type& RendezvousComponent::componentType() const
{
	static const Type type = "Rendezvous";

	return type;
}

void RendezvousComponent::update(const Time& t, const Time& dt)
{
	if (!mActions.empty())
	{
		auto& action = mActions.back();

		action.update(t, dt);
		if (action.state() == ActionBase::State::FINISHED)
		{
			mActions.pop_back();
			std::cout << "Success" << std::endl;
		}
	}
}


//methods
void RendezvousComponent::start()
{
	mActions.clear();

	//TODO(it's just test)
	mActions.push_back(Impuls(mChaser, Vec3(0.0f, 0.0f, 5.0f)));
	mActions.push_back(Wait(Time(100000)));
	mActions.push_back(Impuls(mChaser, Vec3(0.0f, 0.0f, 5.0f)));
	mActions.push_back(Wait(Time(100000)));
}

void RendezvousComponent::reset()
{
	mActions.clear();
}

bool RendezvousComponent::finished() const
{
	return mActions.empty();
}

const SatelliteWeak& RendezvousComponent::target() const
{
	return mTarget;
}
