//#include "RendezvousComponent.h"
//
//
////constructors
//RendezvousComponent::RendezvousComponent(
//	IComponent* parent
//	, const SatelliteWeak& target
//)
//	: IComponent(parent)
//	, mChaser(dynamic_cast<Satellite*>(parent))
//	, mTarget(target)
//	, mActions()
//	, mState(State::FINISHED)
//{
//	mActions.reserve(DEFAULT_CAPACITY);
//}
//
//
////overrides
//const IComponent::Type& RendezvousComponent::componentType() const
//{
//	static const Type type = "Rendezvous";
//
//	return type;
//}
//
//void RendezvousComponent::update(const Time& t, const Time& dt)
//{
//	if (mState != State::FINISHED)
//	{
//		if (!mActions.empty())
//		{
//			auto& action = mActions.back();
//
//			action.update(t, dt);
//			if (action.state() == ActionBase::State::FINISHED)
//			{
//				mActions.pop_back();
//				std::cout << "Success" << std::endl;
//			}
//		}
//		else
//		{
//			mState = State::FINISHED;
//		}
//	}
//}
//
//
////methods
//void RendezvousComponent::start()
//{
//	mState = State::STARTED;
//}
//
//void RendezvousComponent::stop()
//{
//	mActions.clear();
//	mState = State::FINISHED;
//}
//
//
//RendezvousComponent::State RendezvousComponent::state() const
//{
//	return mState;
//}
//
//void RendezvousComponent::setTarget(const SatelliteWeak& target)
//{
//	mTarget = target;
//}
//
//const SatelliteWeak& RendezvousComponent::target() const
//{
//	return mTarget;
//}
