#pragma once

#include <Common.h>

#include "IObject.h"

#include <Physics/PhysicsComponent.h>


class View : public IObject
{
public:
	static const double DEFAULT_SENSITIVITY;

	static const double DELTA_THRESHOLD;

	static const double PITCH_MAX;
	static const double PITCH_MIN;

	static const double ROLL_MAX;
	static const double ROLL_MIN;


	enum Axis : int
	{
		  X = 0
		, Y = 1
		, Z = 2
	};


public:
	View(const View& view) = default;
	View(View&& view)      = default;

	View(
		  const Mat4& axes = Mat4(1.0f)
		, const Mat4& proj = Mat4(1.0f)
		, const Vec3& pos = Vec3(0.0f)
		, double sensitivity = DEFAULT_SENSITIVITY
		, const PhysicsComponentWeak& track = PhysicsComponentShared(nullptr)
	);


	View& operator = (const View& view) = default;
	View& operator = (View&& view)      = default;


	virtual ~View() = default;


	void lookAround(double pitch, double roll);

	void rotateAxes(double delta, Axis axis);

	void rotateView(double delta, Axis axis);

	void travelView(double distance, Axis axis);


	void setTrack(const PhysicsComponentWeak& tracká, const Vec3& relativePos);

	void setPos(const Vec3& pos);

	void setView(const Mat4& mat);

	void setProj(const Mat4& mat);


	const PhysicsComponentWeak& track() const;

	const Mat4& axes() const;

	const Mat4& view() const;

	const Mat4& proj() const;

	const Vec3& pos() const;

	const Vec3& lastPos() const;


	double& sensivity();

	const double& sensivity() const;


public:
	void update(const Time& t, const Time& dt) override;

	const Type& componentType() const override;


private:
	void updateView();

	void updateAxes();


private:
	double mPitch; // y
	double mRoll;  // x

	double mSensivity;

	Mat4 mAxes;
	Mat4 mView;
	Mat4 mProj;

	Vec3 mPos;
	Vec3 mLastPos;

	PhysicsComponentWeak mTrack;

	bool lastTrackNull;
	bool shouldUpdateAxes;
};