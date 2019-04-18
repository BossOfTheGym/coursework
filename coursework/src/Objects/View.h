#pragma once

#include <Common.h>


//TODO : should be object-like
class View
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

	View(View&& view) = default;

	View(
		const Mat4& axes = Mat4(1.0f)
		, const Mat4& proj = Mat4(1.0f)
		, const Vec3& pos = Vec3(0.0f)
		, double sensitivity = DEFAULT_SENSITIVITY
	);


	View& operator = (const View& view) = default;

	View& operator = (View&& view) = default;


	void lookAround(double pitch, double roll);

	void rotateAxes(double delta, Axis axis);

	void rotateView(double delta, Axis axis);

	void travelView(double distance, Axis axis);



	void setPos(const Vec3& pos);

	void setView(const Mat4& mat);

	void setProj(const Mat4& mat);


	const Mat4& axes() const;

	const Mat4& view() const;

	const Mat4& proj() const;

	const Vec3& pos() const;


	double& sensivity();

	const double& sensivity() const;


private:
	void updateView();

	void updateAxes();


private:
	double mPitch; // y
	double mRoll;  // x

	Mat4 mAxes;
	Mat4 mView;
	Mat4 mProj;

	Vec3 mPos;

	double mSensivity;
};