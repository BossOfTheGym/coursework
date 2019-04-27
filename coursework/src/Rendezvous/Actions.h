//#pragma once
//
//#include <Common.h>
//
//#include <variant>
//
//#include "../Satellite.h"
//
//
////forward decl
//struct Satellite;
//using SatelliteShared = std::shared_ptr<Satellite>;
//using SatelliteWeak   = std::weak_ptr<Satellite>;
//
//
//class ActionBase
//{
//public:
//	enum class State : int
//	{
//		  INITIALIZING = -1
//		, EXECUTING    = +0
//		, FINISHED     = +1
//	};
//
//
//protected:
//	ActionBase();
//
//	ActionBase(const ActionBase& ab) = default;
//	ActionBase(ActionBase&& ab)      = default;
//
//	~ActionBase() = default;
//
//	ActionBase& operator = (const ActionBase& ab) = default;
//	ActionBase& operator = (ActionBase&& ab)      = default;
//
//
//public:
//	void update(const Time& t, const Time& dt);
//
//	State state() const;
//
//
//protected:
//	State mState;
//};
//
//class Impuls : public ActionBase
//{
//public:
//	Impuls(
//		Satellite* chaser = nullptr
//		, const Vec3& impuls = Vec3()
//	);
//
//	Impuls(const Impuls& ab) = default;
//	Impuls(Impuls&& ab)      = default;
//
//	~Impuls() = default;
//
//	Impuls& operator = (const Impuls& ab) = default;
//	Impuls& operator = (Impuls&& ab)      = default;
//
//
//public:
//	void update(const Time& t, const Time& dt);
//
//
//private:
//	Satellite* mChaser;
//	Vec3 mImpuls;
//};
//
//class Wait : public ActionBase
//{
//public:
//	Wait(const Time& wait = Time());
//
//	Wait(const Wait& ab) = default;
//	Wait(Wait&& ab)      = default;
//
//	~Wait() = default;
//
//	Wait& operator = (const Wait& ab) = default;
//	Wait& operator = (Wait&& ab)      = default;
//
//
//public:
//	void update(const Time& t, const Time& dt);
//
//
//private:
//	Time mWait;
//};
//
//
////visitor
//template<class ... Callable>
//struct Visitor : public Callable...
//{
//public:
//	Visitor(Callable&&... f) : Callable(std::forward<Callable>(f))... 
//	{}
//
//	using Callable::operator()...;
//};
//
//template<class ... Callable>
//auto make_visitor(Callable&&... args)
//{
//	return Visitor<Callable...>(std::forward<Callable>(args)...);
//}
//
//
////Action variant
//template<class ... Types>
//class ActionVariant :  public ActionBase
//{
//public:
//	using Base = std::variant<Types...>;
//
//
//public:
//	ActionVariant() : ActionBase()
//	{}
//
//	template<class T>
//	ActionVariant(T&& action) 
//		: ActionBase()
//		, mAction(action)
//	{}
//
//
//	ActionVariant(const ActionVariant& a) = default;
//	ActionVariant(ActionVariant&& a)      = default;
//
//	~ActionVariant()
//	{}
//
//	ActionVariant& operator = (const ActionVariant& a) = default;
//	ActionVariant& operator = (ActionVariant&& a)      = default;
//
//
//public:
//	void update(const Time& t, const Time& dt)
//	{
//		auto visitor = make_visitor(
//			[&t, &dt] (auto& arg)
//			{
//				arg.update(t, dt);
//
//				return arg.state();
//			}
//		);
//
//		mState = std::visit(visitor, mAction);
//	}
//
//
//private:
//	std::variant<Types...> mAction;
//};
