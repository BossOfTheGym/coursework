#pragma once

#include "Action.h"

#include <variant>


//visitor
template<class ... Callable>
struct Visitor : public Callable...
{
public:
	Visitor(Callable&&... f) : Callable(std::forward<Callable>(f))... 
	{}

	using Callable::operator()...;
};

template<class ... Callable>
auto make_visitor(Callable&&... args)
{
	return Visitor<Callable...>(std::forward<Callable>(args)...);
}


//Action variant
template<class ... Types>
class ActionVariant : public ActionBase
{
public:
	using Base = std::variant<Types...>;


public:
	ActionVariant() : ActionBase()
	{}

	template<class T>
	ActionVariant(T&& action) 
		: ActionBase()
		, mAction(action)
	{}


	ActionVariant(const ActionVariant& a) = default;
	ActionVariant(ActionVariant&& a)      = default;

	~ActionVariant()
	{}

	ActionVariant& operator = (const ActionVariant& a) = default;
	ActionVariant& operator = (ActionVariant&& a)      = default;


public:
	void update(const Time& t, const Time& dt)
	{
		auto visitor = make_visitor(
			[&t, &dt] (auto& arg)
			{
				arg.update(t, dt);

				return arg.state();
			}
		);

		mState = std::visit(visitor, mAction);
	}


private:
	std::variant<Types...> mAction;
};