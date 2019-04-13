#pragma once

#include <Common.h>


//TODO : should be object-like
class View
{
public:
	static const float DEFAULT_SENSITIVITY;

	static const float DELTA_THRESHOLD;

	static const float PITCH_MAX;
	static const float PITCH_MIN;

	static const float ROLL_MAX;
	static const float ROLL_MIN;


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
		, float sensitivity = DEFAULT_SENSITIVITY
	);


	View& operator = (const View& view) = default;

	View& operator = (View&& view) = default;


	void lookAround(float pitch, float roll);

	void rotateAxes(float delta, Axis axis);

	void rotateView(float delta, Axis axis);

	void travelView(float distance, Axis axis);



	void setPos(const Vec3& pos);

	void setView(const Mat4& mat);

	void setProj(const Mat4& mat);


	const Mat4& axes() const;

	const Mat4& view() const;

	const Mat4& proj() const;

	const Vec3& pos() const;


	float& sensivity();

	const float& sensivity() const;


private:
	void updateView();

	void updateAxes();


private:
	float mPitch; // y
	float mRoll;  // x

	Mat4 mAxes;
	Mat4 mView;
	Mat4 mProj;

	Vec3 mPos;

	float mSensivity;
};